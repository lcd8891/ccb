#include "te.hpp"
#include "window.hpp"
#include <cstdio>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <glib.h>

namespace {
    unsigned int counter = 0;
}

namespace sbi{
    void updateInfo(std::string text,bool spinner = false){
        g_idle_add([](gpointer data) -> gboolean {
            auto* params = static_cast<std::pair<std::string, bool>*>(data);
            Window::bi_status(params->first,params->second);
            delete params;
            return false;
        },new std::pair<std::string,bool>(text,spinner));
    }
    void updateTerminal(std::string text){
        g_idle_add([](gpointer data) -> gboolean {
            auto* str = static_cast<std::string*>(data);
            Window::bi_updateTerminalInfo(*str);
            delete str;
            return false;
        },new std::string(text));
    }
    void MAINWINDOWsetlockControl(bool locked){
        g_idle_add([](gpointer data) -> gboolean {
            bool locked = *static_cast<bool*>(data);
            Window::setLockedControl(locked);
            delete static_cast<bool*>(data);
            return false;
        }, new bool(locked));
        }
}

void executeInTerminal(std::string cmd){
    Window::setLockedControl(true);
    Window::bi_open();

    std::thread([cmd](){
        const unsigned int locked = counter; 
        FILE* pipe = popen((cmd+" 2>&1").c_str(),"r");
        if(!pipe){
            sbi::updateInfo("Couldn't create process!");
            sbi::MAINWINDOWsetlockControl(false);
        }
        sbi::updateInfo("Compiling...",true);
        char buffer[256];
        std::string ss;
        ss+=" ("+std::to_string(locked)+") > "+cmd+'\n';
        sbi::updateTerminal(ss);
        while(fgets(buffer,sizeof(buffer),pipe)!=nullptr){
            ss+=buffer;
            sbi::updateTerminal(ss);
        }
        int status = pclose(pipe);
        int exit_code = WEXITSTATUS(status);
        ss+="\nProgram returned: "+std::to_string(exit_code);
        sbi::updateTerminal(ss);
        if(exit_code == 0){
            sbi::updateInfo("Compilation successfully");
        }else{
            sbi::updateInfo("Compilation error");
        }
        sbi::MAINWINDOWsetlockControl(false);
    }).detach();
    counter++;
}