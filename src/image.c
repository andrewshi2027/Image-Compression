#include "image.h"
#include <string.h>

//Loads a file off disk with the given filename and returns a pointer 
//to a correctly initialized Image structure that stores the image.
Image *load_image(char *filename) {   

    Image *image = (Image *)malloc(sizeof(Image));
    unsigned int width, height, max_intensity;

    //**Open the PPM file for reading. If the file can't be opened, then return NULL**
    FILE *file = fopen(filename, "r");
    if (!file) {
        free(image);
        return NULL; //failed to open file
    }

    //**Read the PPM file header**
    char format[3];
    fscanf(file, "%s", format);
    format[2] = '\n';
    //not a P3 PPM file
    if (strcmp(format, "P3") != 0) {
        fclose(file); //close the file
        free(image);
        return NULL; 
    }

    //**Skip over any comments in the file header**
    while (1) {
        unsigned int x;
        //if is an integer
        if (fscanf(file, "%u", &x) == 1) {
            image->width = x;
            width = x;
            break;
        }
        //if is NOT an integer
        else {
            char ch;
            fscanf(file, "%c", &ch);
        }
    }

    //**Read the image width, height and max intensity**
    fscanf(file, "%u, %u", &height, &max_intensity); //read unsigned short values

    //only handle max intensity of 255
    if (max_intensity != 255) {
        fclose(file); //close file
        return NULL; 
    }

    //**Read pixel data from the file**
    int* pixels = (int*)malloc(width * height * sizeof(int));

    for (unsigned int i = 0; i < width * height; i++) {
        fscanf(file, "%d", &pixels[i]);
    }

    image->header[0] = format[0]; 
    image->header[1] = format[1]; 
    image->width = width;
    image->height = height;
    image->pixels = pixels;

    fclose(file);
    return image;

    //(void)filename;
    //return NULL;
}

void delete_image(Image *image) {
    if (image) {
        free(image);
    }
    //(void)image;
}

unsigned short get_image_width(Image *image) {
    return image->width;
    //(void)image;
    //return 0;
}

unsigned short get_image_height(Image *image) {
    return image->height;
    // (void)image;
    // return 0;
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
