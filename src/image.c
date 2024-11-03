#include "image.h"
#include <string.h>

//Loads a file off disk with the given filename and returns a pointer 
//to a correctly initialized Image structure that stores the image.
Image *load_image(char *filename) {   

    Image *image = (Image *)malloc(sizeof(Image));
    unsigned int width, height, max_intensity;

    //**Open the PPM file for reading**
    FILE *file = fopen(filename, "r");
    if (!file) {
        free(image);
        return NULL; //failed to open file
    }

    //**Read the PPM file header**
    char format[3];
    fscanf(file, "%s", format);
    format[2] = '\0';
    //not a P3 PPM file
    if (strcmp(format, "P3") != 0) {
        fclose(file); //close the file
        free(image);
        return NULL; 
    }

    //**Skip over any comments in the file header**
    while (1) {
        unsigned int x;
        //if scanned value is an integer
        if (fscanf(file, "%u", &x) == 1) {
            image->width = x;
            width = x;
            break;
        }
        //if scanned value  is NOT an integer
        else {
            char ch;
            fscanf(file, "%c", &ch);
        }
    }

    //**Read the height and max intensity**
    fscanf(file, "%u %u", &height, &max_intensity);

    //only handle max intensity of 255
    if (max_intensity != 255) {
        fclose(file); //close file
        return NULL; 
    }

    //**Read pixel data from the file**
    int** pixels = (int**)malloc(width * height * sizeof(int*));

    for (unsigned int i = 0; i < height; i++) {
        int r, g, b;
        pixels[i] = malloc(width * sizeof(int));
        for (unsigned int j = 0; j < width; j++) {
            fscanf(file, "%d %d %d", &r, &g, &b);
            pixels[i][j] = r;
            //read the first two and assign to garbage
            //read every third element and store it to 2D array (pixels[i][j])
        }
    }

    image->header[0] = format[0]; 
    image->header[1] = format[1]; 
    image->height = height;
    image->max_intensity = 255;
    image->pixels = pixels;

    //Close the file
    fclose(file);
    return image;

    //(void)filename;
    //return NULL;
}

void delete_image(Image *image) {
    if (image) {
        for (unsigned int i = 0; i < image->height; i++) {
            free(image->pixels[i]); //free each row
        }
        free(image->pixels); //free the pixel array itself
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
    return (unsigned char) image->pixels[row][col];
    // (void)image;
    // (void)row;
    // (void)col;
    // return 0;
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    // Image *image = load_image(input_filename);
    // if (!image) {
    //     return 0;
    // }

    // //Variables
    // unsigned int message_length = strlen(message);
    // unsigned int total_pixels = image->width * image->height;
    // unsigned int max_chars = total_pixels / 8;

    // //If message_length exceeds the capacity
    // if (message_length >= max_chars) {
    //     message_length = max_chars - 1;
    // }

    // //Track the current character of the message
    // unsigned int message_index = 0;

    // //Each characters in the message
    // for (unsigned int i = 0; i < message_length; i++) {
    //     char current_char = message[i];
    //     //Each bit in the character
    //     for (int bit = 7; bit >= 0; bit--) {

    //         unsigned int pixel_index = message_index * 8 + (7 - bit);
    //         int row = pixel_index / image->width;
    //         int column = pixel_index % image->width;

    //         //Clear LSB
    //         image->pixels[row][column] &= ~1;
    //         //Set LSB based on character's bit
    //         image->pixels[row][column] |= (current_char >> bit) & 1; 
    //     }
    //     message_index++;
    // }

    // //Encode null terminator to signal end of message
    // for (int bit = 7; bit >= 0; bit--) {
    //     unsigned int pixel_index = message_index * 8 + (7 - bit);
    //     int row = pixel_index / image->width;
    //     int column = pixel_index % image->width;

    //     //Set LSB based on null terminator
    //     image->pixels[row][column] &= ~1;
    // }

    // //Save the modified image
    // FILE *fp = fopen(output_filename, "w");
    // if(!fp) {
    //     delete_image(image);
    //     return 0;
    // }

    // //Write PPM header
    // fprintf(fp, "%s\n%d %d\n%d\n", image->header, image->width, image->height, image->max_intensity);

    // //Write Pixel Data
    // for (unsigned int i = 0; i < image->height; i++) {
    //     for (unsigned int j = 0; j < image->width; j++) {
    //         fprintf(fp, "%d ", image->pixels[i][j]);
    //     }
    // }

    // fclose(fp);
    // delete_image(image);
    // return message_length;

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
