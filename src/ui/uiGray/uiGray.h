
#pragma once

#include <gtk/gtk.h>
#include <imgStruct.h>

void setup_ui_Gray(GtkWidget *stack);
static void set_img_to_pixbuf_gray(GdkPixbuf *pixbuf);
static void on_flip_horizontal_gray_button_clicked(GtkWidget *widget, gpointer data);
static void on_flip_vertical_gray_button_clicked(GtkWidget *widget, gpointer data);
