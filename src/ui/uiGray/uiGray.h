
#pragma once

#include <gtk/gtk.h>
#include <imgStruct.h>

extern GtkWidget *imageWidgetGray;

void setup_ui_Gray(GtkWidget *stack);
static void on_clahe_gray_button_clicked(GtkWidget *widget, gpointer data);
static void on_median_blur_gray_button_clicked(GtkWidget *widget, gpointer data);
static void on_transpose_gray_button_clicked(GtkWidget *widget, gpointer data);
static void on_flip_vertical_gray_button_clicked(GtkWidget *widget, gpointer data);
static void on_flip_horizontal_gray_button_clicked(GtkWidget *widget, gpointer data);
