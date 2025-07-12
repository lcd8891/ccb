#pragma once
#include "cbf_file.hpp"
#include "cb_types.hpp"

namespace ccb{
    namespace builder{
        void build_component(CBF_FILE *cbf, CID id);
        void build(CBF_FILE* cbf);
    }
}