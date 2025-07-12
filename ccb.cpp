#include "ccb.hpp"
#include "window.hpp"
#include "cbf/cbf_fstream.hpp"
#include "cbf/cbf_builder.hpp"
#include "exception"
#include <limits>
#include <sstream>

namespace{
    CBF_FILE *current;
    std::string current_path = "";
    bool saved = true;

    void update_l_libraries(library_list used,LID deleted){
        for(LID& l : used){
            if(l>=deleted){
                l--;
            }
        }
    }
}

namespace ccb{
    CID c_current;
    LID l_current;

    #ifdef _WIN32
    const std::string OUTPUT_FILE_FORMAT = ".exe";
    #else
    const std::string OUTPUT_FILE_FORMAT = ".elf";
    #endif

    void new_file(){
        if(saved){
            Window::updateInfo("new file created");
            current = new CBF_FILE;
            current_path=""; 
            Window::win1_updateMainInfo(current);
            Window::win1_updateComponents(current);
            Window::win1_updateLibraries(current);
            saved = false;
        }else{
            unsigned char dialog = Window::not_saved_dialog("Do you want to save file?");
            switch(dialog){
                case 1:
                Window::updateInfo("new file created");
                current = new CBF_FILE;
                current_path=""; 
                Window::win1_updateMainInfo(current);
                Window::win1_updateComponents(current); 
                Window::win1_updateLibraries(current);
                break;
                saved = false;
                case 2:
                save_file();
                Window::updateInfo("new file created");
                current = new CBF_FILE;
                current_path=""; 
                Window::win1_updateMainInfo(current);
                Window::win1_updateComponents(current);
                Window::win1_updateLibraries(current);
                saved = false;
                break;
            }
        }
    }
    void open_main_settings(){
        Window::win2_loadSettings(current);
        Window::win2_open();
    }
    void save_main_settings(){
        Window::win2_saveSettings(current);
        Window::win1_updateMainInfo(current);
        saved = false;
    }

    void save_file(){
        std::string save_path;
        if(current_path == ""){
            save_path = Window::save_dialog();
            saved = ccb::save(current,save_path);
        }else{
            saved = ccb::save(current,current_path);
        }
        if(saved){
            if(current_path!=save_path){current_path = save_path;}
            Window::updateInfo("file saved");
        }
    }
    void save_as_file(){
        std::string save_path = Window::save_dialog();
        saved = ccb::save(current,save_path);
        if(saved){
            if(current_path!=save_path){current_path = save_path;}
            Window::updateInfo("file saved");
        }
    }
    void open_file(){
        current = new CBF_FILE;
        Window::setLockedControl(true);
        try{
            std::string open_path = Window::open_dialog();
            ccb::open(current,open_path);
            current_path = open_path;
            saved = true;
            Window::setLockedControl(false);
            Window::win1_updateMainInfo(current);
            Window::win1_updateComponents(current);
            Window::win1_updateLibraries(current);
            Window::updateInfo("file opened");
        }catch(const std::string &e){
            Window::error_dialog("CBF open error: "+e);
        }catch(const char* e){
            Window::error_dialog("CBF open error: "+std::string(e));
        }
    }

    void build_app(){
        try{
            ccb::builder::build(current);
        }catch(const std::string &e){
            Window::updateInfo("Compile terminated");
            Window::error_dialog("Terminated: \n"+e);
        }
    }

    void new_component(){        
        CID max = std::numeric_limits<CID>::max();
        if(current->compontents.size()!=max){
            CID index = current->compontents.size();
            Component c;
            current->compontents.push_back(c);

            Window::updateInfo("new component created");
            open_component(index);
        }else{
            Window::error_dialog("Error: max component count reached (" + std::to_string(max) + ")!");
        }
    }
    void open_component(CID cid){
        c_current = cid;
        Window::c_edit_open();
        Window::updateInfo("component opened");
    }
    void build_component(CID cid){
        try{
            ccb::builder::build_component(current,cid);
        }catch(const std::string &e){
            Window::updateInfo("Compile terminated");
            Window::error_dialog("Terminated: \n"+e);
        }
    }
    void delete_component(CID cid){
        current->compontents.erase(current->compontents.begin()+cid);
        Window::win1_updateComponents(current);
        Window::updateInfo("component deleted");
    }
    void component_close(){
        Window::win1_updateComponents(current);
        saved = false;
    }


    void new_library(){
        LID max = std::numeric_limits<LID>::max();
        if(current->libraries.size()!=max){
            LID index = current->libraries.size();
            Library l;
            current->libraries.push_back(l);

            open_library(index);
            Window::updateInfo("new library created");
        }else{
            Window::error_dialog("Error: max libraries count reached (" + std::to_string(max) + ")!");
        }
    }
    void open_library(LID lid){
        l_current = lid;
        Window::l_edit_open();
        Window::updateInfo("library opened");
    }
    void delete_library(LID lid){
        current->libraries.erase(current->libraries.begin()+lid);
        Window::win1_updateLibraries(current);
        update_l_libraries(current->build_info.using_libraries,lid);
        for(Component& c : current->compontents){
            update_l_libraries(c.libraries,lid);
        }
    }
    void library_close(){
        Window::win1_updateLibraries(current);
        saved = false;
    }


    bool c_exist(CID cid){
        return cid < current->compontents.size();
    }
    bool l_exist(LID lid){
        return lid < current->libraries.size();
    }

    bool shutdown(){
        if(saved){
            Window::win1_close();
            delete current;
            return true;
        }else{
            unsigned char dialog = Window::not_saved_dialog("Do you want to save file");
            switch(dialog){
                case 1:
                Window::win1_close();
                delete current;
                return true;
                break;
                case 2:
                ccb::save_file();
                Window::win1_close();
                delete current;
                return true;
                break;
            }
        }
        return false;
    }

    std::string getComponentFilename(CID cid){
        return clear_filename(current->compontents[cid].filename);
    }
    std::string getLibraryName(LID lid){
        return current->libraries[lid].name;
    }
    
    Component& getCurrentC(){
        return current->compontents[c_current];
    }
    Library& getCurrentL(){
        return current->libraries[l_current];
    }

    CBF_FILE *getCurrent(){
        return current;
    }
}

s_vector parse_text(std::string str){
    std::string tmp;
    s_vector out;
    for(char ch : str){
        if(ch=='\n' || ch ==' ' || ch=='\t'){
            if(tmp!=""){out.push_back(tmp);}
            tmp.clear();
        }else{
            tmp+=ch;
        }
    }
    if(tmp!=""){out.push_back(tmp);}
    return out;
}

std::string clear_filename(std::string filename){
    size_t slash_pos = filename.find_last_of("/\\");
    return (slash_pos == std::string::npos) ? filename : filename.substr(slash_pos+1);
}