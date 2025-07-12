#pragma once
#include <gtk/gtk.h>
#include <string>
#include <vector>
#include "cb_types.hpp"

struct Component{
    std::string filename;
    library_list libraries;
};