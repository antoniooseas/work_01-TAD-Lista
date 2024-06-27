#pragma once

#include <imgStruct.h>

ImageRGB *create_image_rgb(int largura, int altura);
void free_image_rgb(ImageRGB *image);

// Operações para ImageRGB
ImageRGB *flip_vertical_rgb(const ImageRGB *image);
ImageRGB *flip_horizontal_rgb(const ImageRGB *image);
ImageRGB *transpose_rgb(const ImageRGB *image);

// rotate img
ImageRGB *add90_rotation_RGB(const ImageRGB *image);
ImageRGB *neq90_rotation_RGB(const ImageRGB *image);

// Manipulação por pixel para ImageRGB
ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height);
ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size);

ImageRGB *read_imageRGB(const char *filename);
void mostra_imageRGB(const ImageRGB *image);