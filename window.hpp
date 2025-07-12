#pragma once
#include <string>
#include <vector>
#include <gtk/gtk.h>
#include "cbf/cb_types.hpp"

using LID = unsigned char;
struct CBF_FILE;

namespace Window{
    void setupWidgets();
    
    void setLockedControl(bool lock);
    void updateInfo(std::string str);

    void win1_updateMainInfo(CBF_FILE *cbf);
    void win1_updateComponents(CBF_FILE *cbf);
    void win1_updateLibraries(CBF_FILE *cbf);
    void win1_close();

    void win2_loadSettings(CBF_FILE *cbf);
    void win2_saveSettings(CBF_FILE *cbf);
    void win2_open();

    void c_edit_open();
    void c_edit_close();

    void l_edit_open();
    void l_edit_close();

    void bi_open();
    void bi_status(std::string status, bool spinner = false);
    void bi_updateTerminalInfo(std::string str);

    library_list parseLFrom(GtkWidget *list);
    void parseLTo(GtkWidget *list, library_list used);

    GtkListBoxRow* getTemplateComponentRow();

    void error_dialog(std::string text);
    void info_dialog(std::string text);
    void warning_dialog(std::string text);
    bool yes_no_dialog(std::string str, GtkWidget *parent);
    unsigned char not_saved_dialog(std::string str);

    std::string save_dialog();
    std::string open_dialog();
}