#include "builder.hpp"
#include "gtk_app.hpp"
#include "ccb.hpp"
#include "window.hpp"
#include "res.hpp"

namespace{
    GtkBuilder *builder;
    GtkBuilder *t_builder;

    static void quit(){
        ccb::shutdown();
    }

}

namespace Builder{
    GObject* getObject(const gchar* name){
        return gtk_builder_get_object(builder,name);
    }
    GtkWidget* getWidget(const gchar* name){
        return GTK_WIDGET(gtk_builder_get_object(builder,name));
    }
    GtkTextBuffer* getTextBuffer(const gchar* name){
        return GTK_TEXT_BUFFER(gtk_builder_get_object(builder,name));
    }
    GType getType(const gchar* name){
        return gtk_builder_get_type_from_name(t_builder,name);
    }
    
    void bindActions(){
        GtkWidget *quit_menu_item = getWidget("mi5");
        g_signal_connect(quit_menu_item, "activate", G_CALLBACK(quit), nullptr);
    }

    void Start(){
        builder = gtk_builder_new_from_string(window_ui_xml.c_str(),window_ui_xml.size());
        //builder = gtk_builder_new_from_file("window.glade");
        //t_builder = gtk_builder_new_from_file("template.ui");
        
        GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder,"win1"));

        bindActions();
        Window::setupWidgets();
        
        gtk_widget_show_all(window);
    }
    void stop(){
        g_object_unref(builder);
    }
}