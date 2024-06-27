#pragma once

#include <gtk/gtk.h>
#include <imgStruct.h>

extern GtkWidget *aboutDialog;

void on_about_button_clicked(GtkWidget *widget, gpointer data);
void on_window_destroy(GtkWidget *widget, gpointer data);
int start(int argc, char *argv[]);