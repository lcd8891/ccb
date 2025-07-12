#pragma once

#include <gtk/gtk.h>
#include <string>
#include <vector>   
#include "cb_types.hpp"

struct Library{
    std::string name;
    s_vector include;
    s_vector links;
};