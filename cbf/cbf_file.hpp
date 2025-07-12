#pragma once
#include <string>
#include "build_info.hpp"
#include <vector>
#include "cp.hpp"
#include "lib.hpp"

struct CBF_FILE{
    std::string alt_path;
    BUILD_INFO build_info;
    std::vector<Component> compontents; 
    std::vector<Library> libraries; 

    CBF_FILE();
};