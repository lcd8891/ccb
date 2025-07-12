#include "cbf_fstream.hpp"
#include <fstream>
#include "../ccb.hpp"

#define _STR_CHAR_SIZE 4

#ifdef _WIN32
    #define OS_CODE 'w'
#elif __linux__
    #define OS_CODE 'l'
#else
    #define OS_CODE 'u'
#endif

void write_string(std::ofstream &file, std::string &str){
    size_t size = str.size();
    file.write(reinterpret_cast<char*>(&size),_STR_CHAR_SIZE);
    file.write(str.data(),size);
}

void read_string(std::ifstream &file, std::string &str){
    size_t size = 0;
    file.read(reinterpret_cast<char*>(&size),_STR_CHAR_SIZE);
    str.resize(size);
    file.read(&str[0],size);
}

template<typename T>
void write_data(std::ofstream &file,T &t){
    file.write(reinterpret_cast<char*>(&t),sizeof(T));
}

template<typename T>
void read_data(std::ifstream &file,T &t){
    file.read(reinterpret_cast<char*>(&t),sizeof(T));
}

void write_s_vector(std::ofstream &file,s_vector &vec){
    size_t size = vec.size();
    write_data<size_t>(file,size);
    for(std::string s : vec){
        write_string(file,s);
    }
}
void read_s_vector(std::ifstream &file,s_vector &vec){
    size_t size = 0;
    read_data<size_t>(file,size);
    for(int i = 0;i<size;i++){
        std::string s;read_string(file,s);
        vec.push_back(s);
    }
}

void write_library_list(std::ofstream &file,library_list &list){
    size_t size = list.size();
    write_data(file,size);
    for(LID lid : list){
        write_data<LID>(file,lid);
    }
}
void read_library_list(std::ifstream &file,library_list &list){
    size_t size = 0;
    read_data(file,size);
    for(int i = 0;i<size;i++){
        LID lid;read_data<LID>(file,lid);
        list.push_back(lid);
    }
}

namespace ccb{
    bool save(CBF_FILE* cbf, std::string path){
        std::ofstream file(path,std::ios::binary);
        if(!file.is_open()){
            return false;
        }
        //Signature
        file.write("cbfv_",5);
        char* version = new char[2]{CBF_VERSION,OS_CODE};
        file.write(version,2);
        delete version;
        //alt path
        write_string(file,cbf->alt_path);
        //Build info
        write_string(file,cbf->build_info.additional_arguments);
        write_string(file,cbf->build_info.main_file);
        write_string(file,cbf->build_info.output_file);
        write_string(file,cbf->build_info.workspace_folder);

        write_data<bool>(file,cbf->build_info.use_all_libraries);
        write_data<bool>(file,cbf->build_info.add_format);
        
        write_library_list(file,cbf->build_info.using_libraries);
        write_s_vector(file,cbf->build_info.additional_files);

        //Components
        size_t size = cbf->compontents.size();
        write_data<size_t>(file,size);
        for(Component c : cbf->compontents){
            write_string(file,c.filename);
            write_library_list(file,c.libraries);
        }

        //Libraries
        size = cbf->libraries.size();
        write_data<size_t>(file,size);
        for(Library l : cbf->libraries){
            write_string(file,l.name);
            write_s_vector(file,l.include);
            write_s_vector(file,l.links);
        }

        file.close();
        return true;
    }
    void open(CBF_FILE* cbf, std::string path){
        std::ifstream file(path,std::ios::binary);
        //Signature check
        if(!file.is_open()){
            throw "Couldn't open file: " + path;
        }
        std::string str = "-------";
        {
            file.read(&str[0],7);
            std::string signature = "-----";str.copy(&signature[0],5,0);
            if(signature!="cbfv_"){
                throw "Bad signature!";
            }
            if(str[5]!=CBF_VERSION){
                throw "This version of file is not compatible with app version ( file: " + std::to_string(str[5]) + ", app: " + std::to_string(CBF_VERSION) + ")!"; 
            }
            if(str[6]!=OS_CODE){
                throw "This file is not compatible for this OS";
            }
        }
        //alt path
        read_string(file,cbf->alt_path);
        //Build info
        read_string(file,cbf->build_info.additional_arguments);
        read_string(file,cbf->build_info.main_file);
        read_string(file,cbf->build_info.output_file);
        read_string(file,cbf->build_info.workspace_folder);
        
        read_data<bool>(file,cbf->build_info.use_all_libraries);
        read_data<bool>(file,cbf->build_info.add_format);

        read_library_list(file,cbf->build_info.using_libraries);
        read_s_vector(file,cbf->build_info.additional_files);
        
        //Components
        size_t size;
        read_data<size_t>(file,size);
        for(int i = 0;i<size;i++){
            Component c;
            read_string(file,c.filename);
            read_library_list(file,c.libraries);
            cbf->compontents.push_back(c);
        }

        //Libraries
        read_data<size_t>(file,size);
        for(int i = 0;i<size;i++){
            Library l;
            read_string(file,l.name);
            read_s_vector(file,l.include);
            read_s_vector(file,l.links);
            cbf->libraries.push_back(l);
        }

        file.close();
    }
}