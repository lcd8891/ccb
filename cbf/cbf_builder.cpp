#include "cbf_builder.hpp"
#include "sstream"
#include "string"
#include "cbf_file.hpp"
#include <iostream>
#include <gtk/gtk.h>
#include <filesystem>
#include "../te.hpp"

#ifdef _WIN32
#define EXECUTABLE_FORMAT ".exe"
#define PATH_DIVIDER "\\"
#elif __linux__
#define EXECUTABLE_FORMAT ".elf"
#define PATH_DIVIDER "/"
#endif

namespace{
    
}

namespace BP{
    std::string getfilenamewithoutmime(std::string path){
        std::string filename = std::filesystem::path(path).filename().string();
        size_t point = filename.find_last_of('.');
        return (point == 0) ? filename : filename.substr(0,filename.size()-(filename.size()-point));
    }
    bool isPath(std::string str){
        if(str.empty()){
            return false;
        }
        #ifdef _WIN32 
        if(str.substr(1,2)==":\\"){
            return true;
        }
        #elif __linux__
        if(str[0]=='/' || str[1]=='/'){
            return true;
        }
        #endif
        return false;
    }
    std::string getInputComponent(CBF_FILE *cbf,Component c){
        std::string out;
        out+=c.filename;
        return out;
    }
    std::string getOutputComponent(std::string alt_path, Component c,std::string wokspace_folder = ""){
        std::string out;
        out+=alt_path;
        out+=getfilenamewithoutmime(c.filename);
        out+=".o";
        return out;
    }
    std::string parseLibrariesC(CBF_FILE *cbf,Component c){
        std::string out;
        for(LID lid : c.libraries){
            Library l = cbf->libraries[lid];
            for(std::string include : l.include){
                out+="-I"+include+' ';
            }
            for(std::string link : l.links){
                if(isPath(link)){
                    out+="-L"+link+' ';
                }else{
                    out+="-l"+link+' ';
                }
            }
        }
        return out;
    }
    std::string parseLibraries(CBF_FILE *cbf){
        std::string out;
        if(cbf->build_info.use_all_libraries){
            for(Library l : cbf->libraries){
                for(std::string include : l.include){
                    out+="-I"+include+' ';
                }
                for(std::string link : l.links){
                    if(isPath(link)){
                        out+="-L"+link+' ';
                    }else{
                        out+="-l"+link+' ';
                    }
                }
            }
        }else{
            for(LID lid : cbf->build_info.using_libraries){
                Library l = cbf->libraries[lid];
                for(std::string include : l.include){
                    out+="-I"+include+' ';
                }
                for(std::string link : l.links){
                    if(isPath(link)){
                        out+="-L"+link+' ';
                    }else{
                        out+="-l"+link+' ';
                    }
                }
            }
        }
        return out;
    }
    std::string getOuputProgram(CBF_FILE *cbf){
        std::string s;
        s+=cbf->build_info.output_file;
        if(cbf->build_info.add_format){
            s+=EXECUTABLE_FORMAT;
        }
        return s;
    }
    void checkForAltpath(std::string workspace_folder,std::string altpath){
        std::string s;
        s+=workspace_folder+"/"+altpath;
    }
    std::string getAdditionalSourceFiles(BUILD_INFO &bf){
        std::string s;
        for(std::string a : bf.additional_files){
            s+=a+' ';
        }
        return s;
    }
    std::string getBuiledComponent(CBF_FILE *cbf,const Component &c){
        std::string s;
        s = cbf->build_info.workspace_folder+PATH_DIVIDER+cbf->alt_path+getfilenamewithoutmime(c.filename)+".o";
        return s;
    }
}

namespace ccb{
    namespace builder{    
        void build_component(CBF_FILE *cbf, CID id){
            
            std::stringstream ss;
            ss << "cd " << cbf->build_info.workspace_folder << " && ";
            ss << "g++ " << cbf->build_info.additional_arguments << ' ';
            if(!std::filesystem::exists(cbf->compontents[id].filename)){
                throw std::string("component source file doesn't exists");
            }
            ss << "-c " << BP::getInputComponent(cbf,cbf->compontents[id]) << ' ';
            ss << "-o " << BP::getOutputComponent(cbf->alt_path,cbf->compontents[id],cbf->build_info.workspace_folder) << ' ';
            ss << BP::parseLibrariesC(cbf,cbf->compontents[id]);

            executeInTerminal(ss.str());
        }
        void build(CBF_FILE* cbf){
            std::stringstream ss;
            ss << "cd " << cbf->build_info.workspace_folder << " && ";
            ss << "g++ " << cbf->build_info.main_file << " ";
            ss << BP::getAdditionalSourceFiles(cbf->build_info);
            for(Component& c : cbf->compontents){
                if(!std::filesystem::exists(BP::getBuiledComponent(cbf,c))){
                    throw std::string("builded component "+c.filename+" doesn't builded!");
                }
                ss << BP::getOutputComponent(cbf->alt_path, c) << " ";
            }
            ss << "-o " << BP::getOuputProgram(cbf) + " ";
            ss << BP::parseLibraries(cbf);

            executeInTerminal(ss.str());
        }
    }
}