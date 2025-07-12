#include "cbf_file.hpp"
#include "build_info.hpp"
#include "cp.hpp"
#include "lib.hpp"
CBF_FILE::CBF_FILE(){
    compontents.clear();
    libraries.clear();
    build_info.output_file = "Program";
    alt_path = "";
}