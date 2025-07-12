#pragma once
#include "cbf_file.hpp"
#include <string>

namespace ccb{
    bool save(CBF_FILE* cbf, std::string path);
    void open(CBF_FILE* cbf, std::string path);
}