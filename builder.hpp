#pragma once
#include <gtk/gtk.h>

namespace Builder{
    void Start();
    GObject* getObject(const gchar* name);
    GtkWidget* getWidget(const gchar* name);
    GtkTextBuffer* getTextBuffer(const gchar* name);
    GType getType(const gchar* name);
    void stop();
}