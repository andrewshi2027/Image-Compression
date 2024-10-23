#include "image.h"

/*
Loads a file off disk with the given filename and returns a pointer 
to a correctly initialized Image structure that stores the image.
*/
Image *load_image(char *filename) {   

    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL; //failed to open file
    }

    //Read the PPM file header
    char format[3];
    fscanf(file, "%s", format);
    if (strcmp(format, "P3") != 0) {
        fclose(file);
        return NULL; //not a P3 PPM file
    }

    //Skip Single Line Comments
    char ch = fgetc(file);
    while (ch == '#') {
        while (fgetc(file) != '\n'); //skip the entire comment line
        ch = fgetc(file);
    }

    //Read the image width, height and max intensity
    unsigned short width, height, max_intensity;
    fscanf(file, "%hu, %hu, %hu", &width, &height, &max_intensity);
    if (!max_intensity != 255) {
        fclose(file);
        return NULL; //only handle max intensity of 255
    }

    //Allocate memory for the Image structure
    

    //(void)filename;
    //return NULL;
}

void delete_image(Image *image) {
    (void)image;
}

unsigned short get_image_width(Image *image) {
    (void)image;
    return 0;
}

unsigned short get_image_height(Image *image) {
    (void)image;
    return 0;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) {
    (void)image;
    (void)row;
    (void)col;
    return 0;
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    (void)message;
    (void)input_filename;
    (void)output_filename;
    return 0;
}

char *reveal_message(char *input_filename) {
    (void)input_filename;
    return NULL;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) {
    (void)secret_image_filename;
    (void)input_filename;
    (void)output_filename;
    return 10;
}

void reveal_image(char *input_filename, char *output_filename) {
    (void)input_filename;
    (void)output_filename;
}
