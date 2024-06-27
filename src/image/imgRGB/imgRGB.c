#include <imgRGB.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImageRGB *create_image_rgb(int largura, int altura)
{
    ImageRGB *image = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (image == NULL)
    {
        return NULL;
    }
    image->dim.largura = largura;
    image->dim.altura = altura;

    image->pixels = (PixelRGB *)malloc(largura * altura * sizeof(PixelRGB));
    if (image->pixels == NULL)
    {
        free(image);
        return NULL;
    }
    return image;
}

void free_image_rgb(ImageRGB *image)
{
    if (image == NULL)
    {
        return;
    }
    else if (image->pixels != NULL)
    {
        free(image->pixels);
    }

    free(image);
}

ImageRGB *flip_vertical_rgb(const ImageRGB *image)
{
    if (image == NULL)
    {
        return NULL;
    }
    int largura = image->dim.largura;
    int altura = image->dim.altura;

    // to criando uma nova imagem e armazenando em nova_image.
    ImageRGB *nova_imageVertical = create_image_rgb(largura, altura);

    if (nova_imageVertical == NULL)
    {
        return NULL;
    }
    // aqui ira copiar os pixels da imagem original para a nova imagem e inverter ela verticalmente.
    for (int i = 0; i < altura; ++i)
    {
        for (int x = 0; x < largura; ++x)
        {
            nova_imageVertical->pixels[(altura - 1 - i) * largura + x] = image->pixels[i * largura + x];
        }
    }
    return nova_imageVertical;
}

ImageRGB *flip_horizontal_rgb(const ImageRGB *image)
{
    if (image == NULL)
    {
        return NULL;
    }

    int largura = image->dim.largura;
    int altura = image->dim.altura;

    ImageRGB *nova_imagem_horizontal = create_image_rgb(largura, altura);

    if (nova_imagem_horizontal == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < altura; ++i)
    {
        for (int y = 0; y < largura; ++y)
        {
            nova_imagem_horizontal->pixels[i * largura + (largura - 1 - y)] = image->pixels[i * largura + y];
        }
    }
    return nova_imagem_horizontal;
}

ImageRGB *transpose_rgb(const ImageRGB *image){
    if(image==NULL){
        return NULL;

    }
    int largura=image->dim.largura;
    int altura=image->dim.altura;

    ImageRGB*transpose_image=create_image_rgb(altura,largura);

    if(transpose_image==NULL){
        return NULL;
    }

    for(int i=0;i<altura;++i){
        for(int y=0;y<largura;++y){
            transpose_image->pixels[y*altura+i]=image->pixels[i*largura+y];
        }
    }
    return  transpose_image;
}


ImageRGB *add90_rotation_RGB(const ImageRGB *image)
{
    if(image==NULL){
        return NULL;
    }
    int largura =image->dim.largura;
    int altura=image->dim.altura;

    ImageRGB *newImg =create_image_rgb(altura,largura);
    if(newImg==NULL){
        return NULL;
    }
    for(int i=0;i<altura;++i){
        for(int y=0;y<largura;++y){
            newImg->pixels[y*altura + (altura-1-i)]=image->pixels[i*largura+y];
        }
    }

    return newImg;
}

ImageRGB *neq90_rotation_RGB(const ImageRGB *image)
{
    if(image==NULL){
        return NULL;
    }
    int largura=image->dim.largura;
    int altura=image->dim.altura;

    ImageRGB *newImg = create_image_rgb(altura,largura);
    if(newImg==NULL){
        return NULL;
    }
    for(int i=0;i<altura;++i){
        for(int y=0;y<largura;++y){
            newImg->pixels[(largura-1-y)*altura+i]=image->pixels[i*largura+y];
        }
    }
    return newImg;
}

void calcula_histograma_rgb(int fim_x, int inicio_x, int fim_y, int inicio_y, int *histograma, const ImageRGB *img, int largura, char canal) {
    for (int y = inicio_y; y < fim_y; ++y) {
        for (int x = inicio_x; x < fim_x; ++x) {
            int valor_pixel;
            if (canal == 'r') {
                valor_pixel = img->pixels[y * largura + x].red;
            } else if (canal == 'g') {
                valor_pixel = img->pixels[y * largura + x].green;
            } else {
                valor_pixel = img->pixels[y * largura + x].blue;
            }
            histograma[valor_pixel]++;
        }
    }
}

void limite_histograma_rgb(int *histograma, int num_bins, int limite_corte) {
    int excesso = 0;

    for (int i = 0; i < num_bins; ++i) {
        if (histograma[i] > limite_corte) {
            excesso += histograma[i] - limite_corte;
            histograma[i] = limite_corte;
        }
    }

    int incremento = excesso / num_bins;
    int limite_superior = limite_corte - incremento;

    for (int i = 0; i < num_bins; ++i) {
        if (histograma[i] > limite_superior) {
            excesso += histograma[i] - limite_superior;
            histograma[i] = limite_superior;
        } else {
            histograma[i] += incremento;
            excesso -= incremento;
        }
    }

    for (int i = 0; i < num_bins && excesso > 0; ++i) {
        if (histograma[i] < limite_corte) {
            histograma[i]++;
            excesso--;
        }
    }
}

void calcular_distribuicao_rgb(const int *histograma, int num_bins, int total_pixels, int *cdf) {
    cdf[0] = histograma[0];

    for (int i = 1; i < num_bins; ++i) {
        cdf[i] = cdf[i - 1] + histograma[i];
    }

    for (int i = 0; i < num_bins; ++i) {
        cdf[i] = (int)(((float)cdf[i] / total_pixels) * 255.0f);
    }
}



ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height)
{
    if (image == NULL || tile_width <= 0 || tile_height <= 0) {
        return NULL;
    }

    int largura = image->dim.largura;
    int altura = image->dim.altura;
    int total_pixels = largura * altura;
    int limite_corte = (total_pixels / 256) * 2;

    ImageRGB *resultado = create_image_rgb(largura, altura);
    if (resultado == NULL) {
        return NULL;
    }

    int num_blocos_horizontal = (largura + tile_width - 1) / tile_width;
    int num_blocos_vertical = (altura + tile_height - 1) / tile_height;

    int num_bins = 256;
    int *histograma = (int *)calloc(num_bins, sizeof(int));
    int *cdf = (int *)calloc(num_bins, sizeof(int));

    if (!histograma || !cdf) {
        free(histograma);
        free(cdf);
        free_image_rgb(resultado);
        return NULL;
    }

    for (int id_vertical = 0; id_vertical < num_blocos_vertical; ++id_vertical) {
        for (int id_horizontal = 0; id_horizontal < num_blocos_horizontal; ++id_horizontal) {
            int inicio_x = id_horizontal * tile_width;
            int inicio_y = id_vertical * tile_height;
            int fim_x = inicio_x + tile_width;
            int fim_y = inicio_y + tile_height;

            if (fim_x > largura) fim_x = largura;
            if (fim_y > altura) fim_y = altura;

            // Processar cada canal separadamente
            char canais[3] = {'r', 'g', 'b'};
            for (int c = 0; c < 3; c++) {
                char canal = canais[c];
                for (int i = 0; i < num_bins; i++) histograma[i] = 0;

                calcula_histograma_rgb(fim_x, inicio_x, fim_y, inicio_y, histograma, image, largura, canal);
                limite_histograma_rgb(histograma, num_bins, limite_corte);
                int regiao_pixels = (fim_x - inicio_x) * (fim_y - inicio_y);
                calcular_distribuicao_rgb(histograma, num_bins, regiao_pixels, cdf);

                for (int y = inicio_y; y < fim_y; ++y) {
                    for (int x = inicio_x; x < fim_x; ++x) {
                        int valor_pixel, novo_valor;
                        if (canal == 'r') {
                            valor_pixel = image->pixels[y * largura + x].red;
                            novo_valor = cdf[valor_pixel];
                            resultado->pixels[y * largura + x].red = novo_valor;
                        } else if (canal == 'g') {
                            valor_pixel = image->pixels[y * largura + x].green;
                            novo_valor = cdf[valor_pixel];
                            resultado->pixels[y * largura + x].green = novo_valor;
                        } else {
                            valor_pixel = image->pixels[y * largura + x].blue;
                            novo_valor = cdf[valor_pixel];
                            resultado->pixels[y * largura + x].blue = novo_valor;
                        }
                    }
                }
            }
        }
    }

    free(histograma);
    free(cdf);
    return resultado;
}

int compare_red(const void *a, const void *b)
{
    PixelRGB *pixel_a = (PixelRGB *)a;
    PixelRGB *pixel_b = (PixelRGB *)b;
    return pixel_a->red - pixel_b->red;
}

int compare_green(const void *a, const void *b)
{
    PixelRGB *pixel_a = (PixelRGB *)a;
    PixelRGB *pixel_b = (PixelRGB *)b;
    return pixel_a->green - pixel_b->green;
}

int compare_blue(const void *a, const void *b)
{
    PixelRGB *pixel_a = (PixelRGB *)a;
    PixelRGB *pixel_b = (PixelRGB *)b;
    return pixel_a->blue - pixel_b->blue;
}

ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size)
{
    if (image == NULL || kernel_size <= 0 || kernel_size % 2 == 0) {
        return NULL;
    }

    int largura = image->dim.largura;
    int altura = image->dim.altura;

    ImageRGB *result = create_image_rgb(largura, altura);
    if (result == NULL) {
        return NULL;
    }

    int pad = kernel_size / 2;
    int window_size = kernel_size * kernel_size;
    PixelRGB *window = (PixelRGB *)malloc(window_size * sizeof(PixelRGB));

    if (window == NULL) {
        free_image_rgb(result);
        return NULL;
    }

    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int count = 0;

            for (int ky = -pad; ky <= pad; ++ky) {
                for (int kx = -pad; kx <= pad; ++kx) {
                    int yy = y + ky;
                    int xx = x + kx;

                    if (yy >= 0 && yy < altura && xx >= 0 && xx < largura) {
                        window[count++] = image->pixels[yy * largura + xx];
                    }
                }
            }

            for (int c = 0; c < 3; ++c) {
                if (c == 0) {
                    qsort(window, count, sizeof(PixelRGB), compare_red);
                } else if (c == 1) {
                    qsort(window, count, sizeof(PixelRGB), compare_green);
                } else {
                    qsort(window, count, sizeof(PixelRGB), compare_blue);
                }

                int median_idx = count / 2;
                if (c == 0) {
                    result->pixels[y * largura + x].red = window[median_idx].red;
                } else if (c == 1) {
                    result->pixels[y * largura + x].green = window[median_idx].green;
                } else {
                    result->pixels[y * largura + x].blue = window[median_idx].blue;
                }
            }
        }
    }

    free(window);
    return result;
}




ImageRGB *read_imageRGB(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        char *suf = "builddir/";
        char *new_filename = malloc(strlen(filename) + strlen(suf) + 1);
        strcpy(new_filename, suf);
        strcat(new_filename, filename);
        file = fopen(new_filename, "r");
        if (!file)
        {
            fprintf(stderr, "Não foi possível abrir o arquivo %s\n", filename);
            return NULL;
        }
        free(new_filename);
    }

    int largura, altura;

    fscanf(file, "%d %d", &largura, &altura);

    ImageRGB *image = create_image_rgb(largura, altura);
    image->dim.altura = altura;
    image->dim.largura = largura;

    if (image == NULL)
    {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < largura * altura; i++)
    {
        fscanf(file, "%d %d %d", &image->pixels[i].red, &image->pixels[i].green, &image->pixels[i].blue);
        fgetc(file);
    }

    fclose(file);
    return image;
}

void mostra_imageRGB(const ImageRGB *image)
{
    if (image == NULL)
    {
        printf("imagem é NULL");
        return;
    }

    int largura = image->dim.largura;
    int altura = image->dim.altura;

    for (int i = 0; i < altura; ++i)
    {
        for (int y = 0; y < largura; ++y)
        {
            PixelRGB pixel = image->pixels[i * largura + y];
            printf("(%d,%d,%d)", pixel.red, pixel.green, pixel.blue);
        }
        printf("\n");
    }
}

