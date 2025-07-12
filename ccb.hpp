#pragma once
#include "cbf/cb_types.hpp"
#include <string>
#include <vector>
#include <gtk/gtk.h>
#include "./cbf/cbf_file.hpp"

#define CBF_VERSION 1

namespace ccb{
    extern const std::string OUTPUT_FILE_FORMAT;
    extern CID c_current;
    extern LID l_current;

    void new_file();
    void open_main_settings();
    void save_main_settings();

    void new_component();
    void open_component(CID cid);
    void build_component(CID cid);
    void delete_component(CID cid);
    void component_close();

    void new_library();
    void open_library(LID lid);
    void delete_library(LID lid);
    void library_close();

    void build_app();
    void build_all();

    void save_file();
    void save_as_file();
    void open_file();

    bool c_exist(CID cid);
    bool l_exist(LID lid);

    bool shutdown();

    std::string getComponentFilename(CID cid);
    std::string getLibraryName(LID lid);

    Component& getCurrentC();
    Library& getCurrentL();

    CBF_FILE *getCurrent();
}

s_vector parse_text(std::string str);
std::string clear_filename(std::string filename);
bool isPath(std::string str);