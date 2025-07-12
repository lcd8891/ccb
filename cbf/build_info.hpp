#pragma once
#include <string>
#include <vector>
#include "cb_types.hpp"

struct BUILD_INFO{
    std::string output_file;
    std::string main_file;
    std::string workspace_folder;
    std::string additional_arguments;
    s_vector additional_files;
    library_list using_libraries;
    bool use_all_libraries;
    bool add_format;
};