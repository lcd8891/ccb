#include "window.hpp"
#include "cbf/cbf_file.hpp"
#include "builder.hpp"
#include "ccb.hpp"
#include <map>
#include <sstream>
#include "time.hpp"
#include <algorithm>

namespace{
    std::map<std::string,GtkWidget*> widgets;
    std::map<std::string,GtkTextBuffer*> text_buffers;

    static void NFF(){
        ccb::new_file();
        Window::setLockedControl(false);
    }
    static void OPEN_WIN2(){
        ccb::open_main_settings();
    }

    void parseTextBuffer(s_vector &list,GtkTextBuffer *buffer){
        std::string output;
        for(std::string s : list){
            output+=s+'\n';
        }
        gtk_text_buffer_set_text(buffer,output.c_str(),output.size());
    }
    std::string getStrFromTextBuffer(std::string text_buffer){
        GtkTextIter start,end;
        gtk_text_buffer_get_start_iter(text_buffers[text_buffer],&start);
        gtk_text_buffer_get_end_iter(text_buffers[text_buffer],&end);
        return gtk_text_buffer_get_text(text_buffers[text_buffer],&start,&end,false);
    }
    template<typename T>
    void constaint(std::vector<T> arr,T value){
        
    }

}

namespace Window{
    void setupWidgets(){
        //Save dialog
        widgets["sv_d"] = Builder::getWidget("sv_d");
        widgets["btt_d_ok"] = Builder::getWidget("btt_d_ok");
        widgets["btt_d_no"] = Builder::getWidget("btt_d_no");
        widgets["btt_d_cn"] = Builder::getWidget("btt_d_cn");
        //Main window
        widgets["win1"] = Builder::getWidget("win1");

        widgets["mi1"] = Builder::getWidget("mi1");
        widgets["mi2"] = Builder::getWidget("mi2");
        widgets["mi3"] = Builder::getWidget("mi3");
        widgets["mi4"] = Builder::getWidget("mi4");
        widgets["mi5"] = Builder::getWidget("mi5");
        widgets["mi6"] = Builder::getWidget("mi6");

        widgets["st1"] = Builder::getWidget("st1");
        
        widgets["lb_ofn"] = Builder::getWidget("lb_ofn");
        widgets["lb_wp"] = Builder::getWidget("lb_wp");
        widgets["btt_st"] = Builder::getWidget("btt_st");
        widgets["btt_b"] = Builder::getWidget("btt_b");
        widgets["btt_ba"] = Builder::getWidget("btt_ba");

        widgets["lb_cp"] = Builder::getWidget("lb_cp");
        widgets["cp_template"] = Builder::getWidget("cp_template");
        widgets["cp_t_lb_fn"] = Builder::getWidget("cp_t_lb_fn");
        widgets["cp_t_btt_ed"] = Builder::getWidget("cp_t_btt_ed");
        widgets["cp_t_btt_b"] = Builder::getWidget("cp_t_btt_b");
        widgets["btt_cn"] = Builder::getWidget("btt_cn");

        widgets["lb_l"] = Builder::getWidget("lb_l");
        widgets["l_template"] = Builder::getWidget("l_template");
        widgets["l_t_lb_ln"] = Builder::getWidget("l_t_lb_ln");
        widgets["l_t_btt_ed"] = Builder::getWidget("l_t_btt_ed");
        widgets["btt_ln"] = Builder::getWidget("btt_ln");

        widgets["lb_i"] = Builder::getWidget("lb_i");

        //CBF Main settings
        widgets["win2"] = Builder::getWidget("win2");
        widgets["st2"] = Builder::getWidget("st2");

        widgets["en_fn"] = Builder::getWidget("en_fn");
        widgets["cb_ft"] = Builder::getWidget("cb_ft");
        widgets["fc_mf"] = Builder::getWidget("fc_mf");
        widgets["fc_wf"] = Builder::getWidget("fc_wf");
        widgets["tv_af"] = Builder::getWidget("tv_af");

        widgets["cb_ua"] = Builder::getWidget("cb_ua");
        widgets["l_ls1"] = Builder::getWidget("l_ls1");
        widgets["ls_template"] = Builder::getWidget("ls_template");
        widgets["ls_t_cb_u"] = Builder::getWidget("ls_t_cb_u");

        widgets["cb_ap"] = Builder::getWidget("cb_ap");
        widgets["en_ap"] = Builder::getWidget("en_ap");
        widgets["en_arg"] = Builder::getWidget("en_arg");

        //Build Window
        widgets["win3"] = Builder::getWidget("win3");
        widgets["lb_bsi"] = Builder::getWidget("lb_bsi");
        widgets["sp1"] = Builder::getWidget("sp1");

        //Component editor win5dow
        widgets["win4"] = Builder::getWidget("win4");
        widgets["fc_cf"] = Builder::getWidget("fc_cf");
        widgets["l_ls2"] = Builder::getWidget("l_ls2");
        widgets["btt_cr"] = Builder::getWidget("btt_cr");

        //Library editor window
        widgets["win5"] = Builder::getWidget("win5");
        widgets["en_ln"] = Builder::getWidget("en_ln");
        widgets["btt_lr"] = Builder::getWidget("btt_lr");

        //Text buffers
        text_buffers["terminal"] = Builder::getTextBuffer("tb1");
        text_buffers["ms-al"] = Builder::getTextBuffer("tb2");
        text_buffers["le-if"] = Builder::getTextBuffer("tb3");
        text_buffers["le-ll"] = Builder::getTextBuffer("tb4");

        g_signal_connect(widgets["mi1"],"activate",G_CALLBACK(NFF),nullptr);
        g_signal_connect(widgets["mi2"],"activate",G_CALLBACK(+[](){
            ccb::open_file();
        }),nullptr);
        g_signal_connect(widgets["mi3"],"activate",G_CALLBACK(+[](){
            ccb::save_file();
        }),nullptr);
        g_signal_connect(widgets["mi4"],"activate",G_CALLBACK(+[](){
            ccb::save_as_file();
        }),nullptr);
        g_signal_connect(widgets["btt_b"],"clicked",G_CALLBACK(+[](){
            ccb::build_app();
        }),nullptr);
        g_signal_connect(widgets["btt_st"],"clicked",G_CALLBACK(OPEN_WIN2),nullptr);
        g_signal_connect(widgets["win1"],"delete-event",G_CALLBACK(+[](GtkWidget* widget, GdkEvent* event, gpointer data) -> gboolean {
                return !ccb::shutdown();
            }), 
            nullptr);
        g_signal_connect(widgets["win2"],"delete-event",G_CALLBACK(+[](GtkWidget* widget, GdkEvent* event, gpointer data) -> gboolean {
                gtk_widget_hide(widget);
                ccb::save_main_settings();
                return TRUE;
            }), 
            nullptr);
        g_signal_connect(widgets["win3"],"delete-event",G_CALLBACK(+[](GtkWidget* widget, GdkEvent* event, gpointer data) -> gboolean {
                gtk_widget_hide(widget);
                return TRUE;
            }), 
            nullptr);
        g_signal_connect(widgets["win4"],"delete-event",G_CALLBACK(+[](GtkWidget* widget, GdkEvent* event, gpointer data) -> gboolean {
                Window::c_edit_close();
                return TRUE;
            }), 
            nullptr);
        g_signal_connect(widgets["win5"],"delete-event",G_CALLBACK(+[](GtkWidget* widget, GdkEvent* event, gpointer data) -> gboolean {
                Window::l_edit_close();
                return TRUE;
            }), 
            nullptr);
        g_signal_connect(widgets["cb_ap"],"toggled",G_CALLBACK(+[](GtkWidget* widget){
            bool active = gtk_toggle_button_get_active(&GTK_CHECK_BUTTON(widget)->toggle_button);
            gtk_widget_set_sensitive(widgets["en_ap"],active);
        }),nullptr);
        g_signal_connect(widgets["cb_ua"],"toggled",G_CALLBACK(+[](GtkWidget* widget){
            bool active = gtk_toggle_button_get_active(&GTK_CHECK_BUTTON(widget)->toggle_button);
            gtk_widget_set_sensitive(widgets["l_ls1"],!active);
        }),nullptr);
        g_signal_connect(widgets["btt_cn"],"clicked",G_CALLBACK(+[](){
            ccb::new_component();
        }),nullptr);
        g_signal_connect(widgets["btt_ln"],"clicked",G_CALLBACK(+[](){
            ccb::new_library();
        }),nullptr);
        g_signal_connect(widgets["btt_cr"],"clicked",G_CALLBACK(+[](){
            ccb::delete_component(ccb::c_current);
            ccb::component_close();
            gtk_widget_hide(widgets["win4"]);
            Window::setLockedControl(false);
        }),nullptr);
        g_signal_connect(widgets["btt_lr"],"clicked",G_CALLBACK(+[](){
            ccb::delete_library(ccb::l_current);
            ccb::library_close();
            gtk_widget_hide(widgets["win5"]);
            Window::setLockedControl(false);
        }),nullptr);
        g_signal_connect(widgets["cb_ua"],"toggled",G_CALLBACK(+[](GtkWidget *widget){
            bool active = gtk_toggle_button_get_active(&GTK_CHECK_BUTTON(widget)->toggle_button);
            CBF_FILE *cbf = ccb::getCurrent();
            if(active){
                for(LID id = 0;id < cbf->libraries.size();id++){
                    cbf->build_info.using_libraries.push_back(id);
                }
            }else{
                cbf->build_info.using_libraries.clear();
            }
        }),nullptr);
    }

    GtkWidget* create_new_row(unsigned char i,CID id, bool toggled = false){
        GtkWidget *row = gtk_list_box_row_new();

        switch(i){
            case 0:{
                GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,15);
                        
                gtk_widget_set_margin_start(box , 15);
                gtk_widget_set_margin_end(box , 15);
                gtk_widget_set_margin_top(box , 15);
                gtk_widget_set_margin_bottom(box , 15);

                gtk_container_add(GTK_CONTAINER(row),box);

                GtkWidget *lb = gtk_label_new(ccb::getComponentFilename(id).c_str());

                gtk_widget_set_hexpand(lb,true);
                
                GtkWidget *edit_btt = gtk_button_new_with_label("Edit");
                GtkWidget *build_btt = gtk_button_new_with_label("Build");

                g_signal_connect(edit_btt,"clicked",G_CALLBACK(+[](GtkButton* btt, gpointer data){
                    ccb::open_component(*static_cast<CID*>(data));
                }),new CID(id));

                g_signal_connect(build_btt,"clicked",G_CALLBACK(+[](GtkButton* btt, gpointer data){
                    ccb::build_component(*static_cast<CID*>(data));
                }),new CID(id));
                gtk_container_add(GTK_CONTAINER(box),lb);
                gtk_container_add(GTK_CONTAINER(box),edit_btt);
                gtk_container_add(GTK_CONTAINER(box),build_btt);
            }
            break;
            case 1:{
                GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,15);
                        
                gtk_widget_set_margin_start(box , 15);
                gtk_widget_set_margin_end(box , 15);
                gtk_widget_set_margin_top(box , 15);
                gtk_widget_set_margin_bottom(box , 15);

                gtk_container_add(GTK_CONTAINER(row),box);

                GtkWidget *lb = gtk_label_new(ccb::getLibraryName(id).c_str());
                gtk_widget_set_hexpand(lb,true);
                GtkWidget *edit_btt = gtk_button_new_with_label("Edit");

                g_signal_connect(edit_btt,"clicked",G_CALLBACK(+[](GtkButton* btt, gpointer data){
                    ccb::open_library(*static_cast<LID*>(data));
                }),new LID(id));
                gtk_container_add(GTK_CONTAINER(box),lb);
                gtk_container_add(GTK_CONTAINER(box),edit_btt);
            }
            break;
            case 2:{
                GtkWidget* cb = gtk_check_button_new_with_label(ccb::getLibraryName(id).c_str());
                gtk_toggle_button_set_active(&GTK_CHECK_BUTTON(cb)->toggle_button,toggled);
                gtk_widget_set_hexpand(cb,true);
                gtk_container_add(GTK_CONTAINER(row),cb);
            }
            break;
        }
        gtk_widget_show_all(row);
        return row;
    }

    void gtk_list_box_clear_full(GtkListBox* list){
        GList *children = gtk_container_get_children(GTK_CONTAINER(list));
        const int count = g_list_length(children);
        for(int i = count - 1;i>=0;i--){
            GtkWidget* row = GTK_WIDGET(g_list_nth_data(children,i));
            gtk_container_remove(GTK_CONTAINER(list),row);
        }

        g_list_free(children);
    }

    void gtk_list_box_clear(GtkListBox* list){
        GList *children = gtk_container_get_children(GTK_CONTAINER(list));
        const int count = g_list_length(children);
        for(int i = count - 1;i>=1;i--){
            GtkWidget* row = GTK_WIDGET(g_list_nth_data(children,i));
            gtk_container_remove(GTK_CONTAINER(list),row);
        }

        g_list_free(children);
    }

    void setLockedControl(bool lock){
        gtk_widget_set_sensitive(widgets["st1"],!lock);
    }
    void updateInfo(std::string str){
        std::stringstream ss;
        Time time = getTime();
        ss << "[" << time.hour << "." << time.minute << "." << time.second << "] : " << str;
        gtk_label_set_label(GTK_LABEL(widgets["lb_i"]),ss.str().c_str());
    }

    void win1_updateMainInfo(CBF_FILE *cbf){
        std::string output = cbf->build_info.output_file;
        if(cbf->build_info.add_format){
            output+=ccb::OUTPUT_FILE_FORMAT;
        }
        gtk_label_set_text(GTK_LABEL(widgets["lb_ofn"]),output.c_str());
        gtk_label_set_text(GTK_LABEL(widgets["lb_wp"]),cbf->build_info.workspace_folder.c_str());
    }
    void win1_updateComponents(CBF_FILE *cbf){
        gtk_list_box_clear(GTK_LIST_BOX(widgets["lb_cp"]));
        CID index = 0;
        for(Component c : cbf->compontents){
            GtkWidget* row = create_new_row(0,index);
            gtk_list_box_insert(GTK_LIST_BOX(widgets["lb_cp"]),row,-1);
            index++;
        }
    }
    void win1_updateLibraries(CBF_FILE *cbf){
        gtk_list_box_clear(GTK_LIST_BOX(widgets["lb_l"]));
        LID index = 0;
        for(Library l : cbf->libraries){
            GtkWidget* row = create_new_row(1,index);
            gtk_list_box_insert(GTK_LIST_BOX(widgets["lb_l"]),row,-1);
            index++;
        }
    }
    void win1_close(){
        gtk_widget_destroy(widgets["win1"]);
        gtk_main_quit();
    }

    void win2_loadSettings(CBF_FILE *cbf){
        //Checking alt_path
        if(cbf->alt_path == ""){
            gtk_toggle_button_set_active(&GTK_CHECK_BUTTON(Builder::getObject("cb_ap"))->toggle_button,false);
            gtk_widget_set_sensitive(widgets["en_ap"],false);
            gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_ap"])),"",0);
        }else{
            gtk_toggle_button_set_active(&GTK_CHECK_BUTTON(Builder::getObject("cb_ap"))->toggle_button,true);
            gtk_widget_set_sensitive(widgets["en_ap"],true);
            gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_ap"])),cbf->alt_path.c_str(),cbf->alt_path.size());
        }
        //Update file choosers
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets["fc_wf"]),cbf->build_info.workspace_folder.c_str());
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets["fc_mf"]),cbf->build_info.main_file.c_str());
        //Update Entry
        gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_fn"])),cbf->build_info.output_file.c_str(),cbf->build_info.output_file.size());
        gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_arg"])),cbf->build_info.additional_arguments.c_str(),cbf->build_info.additional_arguments.size());

        gtk_toggle_button_set_active(&GTK_CHECK_BUTTON(widgets["cb_ua"])->toggle_button,cbf->build_info.use_all_libraries);
        #ifdef _WIN32
        gtk_widget_set_sensitive(widgets["cb_ft"],false);
        gtk_button_set_label(&GTK_CHECK_BUTTON(widgets["cb_ft"])->toggle_button.button,"Add .exe at filename end.");
        gtk_toggle_button_set_active(&GTK_CHECK_BUTTON(widgets["cb_ft"])->toggle_button,true);
        #else
        gtk_toggle_button_set_active(&GTK_CHECK_BUTTON(widgets["cb_ft"])->toggle_button,cbf->build_info.add_format);
        #endif
        //Update text buffer
        parseTextBuffer(cbf->build_info.additional_files,text_buffers["ms-al"]);
        parseLTo(widgets["l_ls1"],cbf->build_info.using_libraries);
    }
    void win2_saveSettings(CBF_FILE *cbf){
        bool active_ap = gtk_toggle_button_get_active(&GTK_CHECK_BUTTON(widgets["cb_ap"])->toggle_button);
        if(active_ap){
            std::string altpath = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_ap"])));
            if(altpath[altpath.length()-1]!='/'){
                altpath+='/';
            }
            cbf->alt_path = altpath;
        }else{
            cbf->alt_path = "";
        }

        BUILD_INFO *build_info = &cbf->build_info;

        build_info->output_file = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_fn"])));
        build_info->additional_arguments = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_arg"])));
        bool active_ft = gtk_toggle_button_get_active(&GTK_CHECK_BUTTON(widgets["cb_ft"])->toggle_button);
        build_info->add_format=active_ft;
        active_ft = gtk_toggle_button_get_active(&GTK_CHECK_BUTTON(widgets["cb_ua"])->toggle_button);
        build_info->use_all_libraries=active_ft;

        gchar *text;
        text = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets["fc_wf"]));
        if(text != nullptr){
            build_info->workspace_folder = text;
        }
        text = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets["fc_mf"]));
        if(text != nullptr){
            build_info->main_file = text;
        }
        //build_info->main_file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets["fc_mf"]));
        build_info->additional_files = parse_text(getStrFromTextBuffer("ms-al"));
        build_info->using_libraries = parseLFrom(widgets["l_ls1"]);
        g_free(text);
        Window::updateInfo("changed build settings");
    }

    void win2_open(){
        gtk_widget_set_visible(widgets["win2"],true);
    }

    std::string save_dialog(){
        GtkWidget* dialog = gtk_file_chooser_dialog_new("Sava CBF file...",GTK_WINDOW(widgets["win1"]),GTK_FILE_CHOOSER_ACTION_SAVE,"_Cancel",GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_ACCEPT,NULL);
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        GtkFileFilter *filter = gtk_file_filter_new();

        gtk_file_chooser_set_current_name(chooser, "filename.cbf");
        gtk_file_filter_set_name(filter,"CBF file");
        gtk_file_filter_add_pattern(filter,"*.cbf");
        gtk_file_chooser_add_filter(chooser,filter);

        int response;
        response = gtk_dialog_run(GTK_DIALOG(dialog));
        std::string path;
        if(response == GTK_RESPONSE_ACCEPT){
            path = gtk_file_chooser_get_filename(chooser);
        }else{
            path = "";
        }
        gtk_widget_destroy(dialog);
        return path;
    }
    std::string open_dialog(){
        GtkWidget* dialog = gtk_file_chooser_dialog_new("Sava CBF file...",GTK_WINDOW(widgets["win1"]),GTK_FILE_CHOOSER_ACTION_OPEN,"_Cancel",GTK_RESPONSE_CANCEL,"_Open",GTK_RESPONSE_ACCEPT,NULL);
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        GtkFileFilter *filter = gtk_file_filter_new();

        gtk_file_chooser_set_current_name(chooser, "filename.cbf");
        gtk_file_filter_set_name(filter,"CBF file");
        gtk_file_filter_add_pattern(filter,"*.cbf");
        gtk_file_chooser_add_filter(chooser,filter);

        int response;
        response = gtk_dialog_run(GTK_DIALOG(dialog));
        std::string path;
        if(response == GTK_RESPONSE_ACCEPT){
            path = gtk_file_chooser_get_filename(chooser);
        }else{
            path = "";
        }
        gtk_widget_destroy(dialog);
        return path;
    }

    void c_edit_open(){
        Component &c = ccb::getCurrentC();
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets["fc_cf"]),c.filename.c_str());

        parseLTo(widgets["l_ls2"],c.libraries);

        setLockedControl(true);
        gtk_widget_set_visible(widgets["win4"],true);
    }
    void c_edit_close(){
        Component &c = ccb::getCurrentC();

        char* text = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets["fc_cf"]));
        if(text!=nullptr){
            c.filename = text;

            c.libraries = parseLFrom(widgets["l_ls2"]);

            delete text;
            gtk_widget_hide(widgets["win4"]);
            setLockedControl(false);
            ccb::component_close();
        }else{
            if(yes_no_dialog("Warning: component file is empty!\nComponent has been deleted.\nDo you want to exit?",widgets["win4"])){
                ccb::delete_component(ccb::c_current);
                ccb::component_close();
                gtk_widget_hide(widgets["win4"]);
                Window::setLockedControl(false);
            }
        }
    }
    void l_edit_open(){
        Library &l = ccb::getCurrentL();
        gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_ln"])),l.name.c_str(),l.name.size());

        parseTextBuffer(l.include,text_buffers["le-if"]);
        parseTextBuffer(l.links,text_buffers["le-ll"]);

        setLockedControl(true);
        gtk_widget_set_visible(widgets["win5"],true);
    }
    void l_edit_close(){
        Library &l = ccb::getCurrentL();
        l.name = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets["en_ln"])));

        if(l.name != std::string("")){

            l.include = parse_text(getStrFromTextBuffer("le-if"));
            l.links = parse_text(getStrFromTextBuffer("le-ll"));

            gtk_widget_hide(widgets["win5"]);
            setLockedControl(false);
            ccb::library_close();
        }else{
            if(yes_no_dialog("Warning: library name is empty!\nLibrary has been deleted.\nDo you want to exit?",widgets["win5"])){
                ccb::delete_library(ccb::l_current);
                ccb::library_close();
                gtk_widget_hide(widgets["win5"]);
                Window::setLockedControl(false);
            }
        }
    }

    void error_dialog(std::string text){
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widgets["win1"]),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"%s",text.c_str());
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        if(response == GTK_RESPONSE_OK){
            gtk_widget_destroy(dialog);
        }
    }
    void info_dialog(std::string text){
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widgets["win1"]),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"%s",text.c_str());
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        if(response == GTK_RESPONSE_OK){
            gtk_widget_destroy(dialog);
        }
    }
    void warning_dialog(std::string text){
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widgets["win1"]),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"%s",text.c_str());
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        if(response == GTK_RESPONSE_OK){
            gtk_widget_destroy(dialog);
        }
    }
    bool yes_no_dialog(std::string str, GtkWidget *parent){
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",str.c_str());
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        if(response==GTK_RESPONSE_YES){
            gtk_widget_destroy(dialog);
            return true;
        }else if(response==GTK_RESPONSE_NO){
            gtk_widget_destroy(dialog);
            return false;
        }
        return false;
    }
    unsigned char not_saved_dialog(std::string str){
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widgets["win1"]),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_NONE,"%s",str.c_str());
        gtk_dialog_add_button(GTK_DIALOG(dialog),"Yes",GTK_RESPONSE_YES);
        gtk_dialog_add_button(GTK_DIALOG(dialog),"No",GTK_RESPONSE_NO);
        gtk_dialog_add_button(GTK_DIALOG(dialog),"Cancel",GTK_RESPONSE_CANCEL);
        int response = gtk_dialog_run(GTK_DIALOG(dialog));
        unsigned char output;
        if(response==GTK_RESPONSE_YES){
            output = 2;
        }else if(response==GTK_RESPONSE_NO){
            output = 1;
        }else if(response==GTK_RESPONSE_CANCEL){
            output = 0;
        }
        gtk_widget_destroy(dialog);
        return output;
    }

    library_list parseLFrom(GtkWidget *list){
        GList *childrens = gtk_container_get_children(GTK_CONTAINER(list));
        library_list out;
        LID max = g_list_length(childrens);
        for(int i = 0;i<max;i++){
            GtkWidget* row = GTK_WIDGET(g_list_nth_data(childrens,i));
            GtkWidget* cb = gtk_bin_get_child(GTK_BIN(row));
            if(gtk_toggle_button_get_active(&GTK_CHECK_BUTTON(cb)->toggle_button)){
                out.push_back(i);
            }
        }
        g_list_free(childrens);
        return out;
    }

    void parseLTo(GtkWidget *list, library_list used){
        gtk_list_box_clear_full(GTK_LIST_BOX(list));
        std::vector<Library> &libraries = ccb::getCurrent()->libraries;
        for(LID i = 0;i<libraries.size();i++){
            bool active = std::find(used.begin(),used.end(),i) !=  used.end();
            GtkWidget* row = create_new_row(2,i,active);
            gtk_list_box_insert(GTK_LIST_BOX(list),row,-1);
        }
        
    }

    void bi_open(){
        gtk_widget_show(widgets["win3"]);
    }
    void bi_status(std::string status, bool spinner){
        gtk_label_set_text(GTK_LABEL(widgets["lb_bsi"]),status.c_str());
        if(spinner){
            gtk_spinner_start(GTK_SPINNER(widgets["sp1"]));
        }else{
            gtk_spinner_stop(GTK_SPINNER(widgets["sp1"]));
        }
    }
    void bi_updateTerminalInfo(std::string str){
        gtk_text_buffer_set_text(text_buffers["terminal"],str.c_str(),str.size());
    }
}