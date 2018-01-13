//
// Created by fmatica on 1/13/18.
//
//  See http://www.libpng.org
//  See http://www.libpng.org/pub/png/libpng-manual.txt
//

#include <cstdlib>
#include <cstdio>
#include "my_png.h"

MyPng::MyPng() {
    this->height = this->width = 0;
}

MyPng::MyPng(const char *fileName) {
    this->read_png(fileName);
}

MyPng::~MyPng() {
    int y;
    for(y = 0; y < this->height; y++) {
        free(this->copy[y]);
        free(this->row_pointers[y]);
    }
    free(this->row_pointers);
    free(this->copy);
}

void MyPng::read_png(const char *fileName) {
    FILE *fp = fopen(fileName, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    this->width      = static_cast<int>(png_get_image_width(png, info));
    this->height     = static_cast<int>(png_get_image_height(png, info));
    this->color_type = png_get_color_type(png, info);
    this->bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    if(this->bit_depth == 16)
        png_set_strip_16(png);

    if(this->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(this->color_type == PNG_COLOR_TYPE_GRAY && this->bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(this->color_type == PNG_COLOR_TYPE_RGB ||
       this->color_type == PNG_COLOR_TYPE_GRAY ||
       this->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(this->color_type == PNG_COLOR_TYPE_GRAY ||
       this->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    this->row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * this->height);
    this->copy = (png_bytep*)malloc(sizeof(png_bytep) * this->height);
    for(int y = 0; y < this->height; y++) {
        this->row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
        this->copy[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, this->row_pointers);

    fclose(fp);
}

void MyPng::write_png(const char *filename) {
    printf("writing to file %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    if(!fp) abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
            png,
            info,
            static_cast<png_uint_32>(this->width), static_cast<png_uint_32>(this->height),
            8,
            PNG_COLOR_TYPE_RGBA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    printf("done writing headers\n");

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    png_write_image(png, this->copy);
    png_write_end(png, NULL);

    printf("Finished writing png!\n");

    fclose(fp);
}