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
    Image *image = load_image(input_filename);
    if (!image) {
        return 0;
    }

    //Variables
    unsigned int width = image->width;
    unsigned int height = image->height;
    unsigned int total_pixels = image->width * image->height;
    unsigned int message_length = strlen(message);
    unsigned int printable_count = 0;
    
    unsigned int max_chars = total_pixels / 8;

    if (message_length < max_chars) {
        printable_count = message_length;
    }
    else {
        printable_count = max_chars - 1;
    }

    //Move intensities from 2D Array to 1D Array
    unsigned int pixels[total_pixels];
    int index = 0;
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            pixels[index++] = image->pixels[i][j];
        }
    }

    //Encode each character into the pixels array
    unsigned int pixel_index = 0;
    for (unsigned int i = 0; i < message_length && pixel_index + 8 < total_pixels; i++) {
        unsigned char current_char = message[i];
        for (int bit = 7; bit >= 0; bit--) {
            unsigned current_bit = (current_char >> bit) & 1;

            //Clear the LSB of the current pixel
            pixels[pixel_index] &= ~1;

            //Set the LSB to the current bit
            pixels[pixel_index] |= current_bit;
            pixel_index++;
        }
    }

    //Encode Null Terminator
    if (pixel_index + 8 <= total_pixels){
        for (int bit = 7; bit >= 0; bit--) {
            pixels[pixel_index] &= ~1;
            pixel_index++;
        }
    }

    //Put 1D Array back into 2D Array
    index = 0;
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            image->pixels[i][j] = pixels[index++];
        }
    }

    //Save the modified image
    FILE *fp = fopen(output_filename, "w");
    if(!fp) {
        delete_image(image);
        return 0;
    }

    //Write PPM header
    fprintf(fp, "%s\n%d %d\n%d\n", "P3", image->width, image->height, image->max_intensity);

    //Write RGB to PPM File
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            fprintf(fp, "%d %d %d ", image->pixels[i][j], image->pixels[i][j], image->pixels[i][j]);
        }
    }

    fclose(fp);
    delete_image(image);
    //Don't include null
    return printable_count;

    // (void)message;
    // (void)input_filename;
    // (void)output_filename;
    // return 0;
}

char *reveal_message(char *input_filename) {
    Image *image = load_image(input_filename);

    unsigned int width = image->width;
    unsigned int height = image->height;
    unsigned int total_pixels = width * height;

    char* message = (char *)malloc((total_pixels / 8) * sizeof(char));

    //Move intensities from 2D Array to 1D Array
    char pixels[total_pixels];
    unsigned int index = 0;
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            pixels[index++] = (char) image->pixels[i][j];
        }
    }

    unsigned message_index = 0;

    index = 0;
    while (index < total_pixels) {
        char character = 0;

        for (int bit = 7; bit >= 0; bit--) {
            //LSB of Pixel
            char char_bit = pixels[index] & 1;
            //Set corresponding bit in character
            character |= (char_bit << bit);
            index++;
        }

        message[message_index++] = character;

        if (character == '\0') {
            delete_image(image);
            return message;
        }
    }

    delete_image(image);
    return message;

    // (void)input_filename;
    // return NULL;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) {
    Image *secret_image = load_image(secret_image_filename);
    Image *image = load_image(input_filename);

    //Variables
    unsigned int secret_width = secret_image->width;
    unsigned int secret_height = secret_image->height;
    unsigned int image_width = image->width;
    unsigned int image_height = image->height;

    unsigned int total_pixels = 16 + (8 * secret_width * secret_height);
    unsigned int total_image_pixels = image_width * image_height;
    unsigned int total_secret_pixels = secret_width * secret_height;

    if (total_pixels > image_width * image_height) {
        delete_image(secret_image);
        delete_image(image);
        return 0;
    }

    //2D Array to 1D Array (secret_image)
    char secret_pixels[total_secret_pixels];
    int secret_index = 0;
    for (unsigned int i = 0; i < secret_height; i++) {
        for (unsigned int j = 0; j < secret_width; j++) {
            secret_pixels[secret_index++] = secret_image->pixels[i][j];
        }
    }
    //2D Array to 1D Array (image)
    char image_pixels[total_image_pixels];
    int image_index = 0;
    for (unsigned int i = 0; i < image_height; i++) {
        for (unsigned int j = 0; j < image_width; j++) {
            image_pixels[image_index++] = image->pixels[i][j];
        }
    }

    int pixel_index = 0;

    //Encode Secret Width and Height
    //Width
    for (int bit = 7; bit >= 0; bit--) {
        image_pixels[pixel_index] &= ~1;
        image_pixels[pixel_index] |= (secret_width >> bit) & 1;
        pixel_index++;
    }
    //Height
    for (int bit = 7; bit >= 0; bit--) {
        image_pixels[pixel_index] &= ~1;
        image_pixels[pixel_index] |= (secret_height >> bit) & 1;
        pixel_index++;
    }

    //Encode Secret Image Pixels
    for (unsigned int i = 0; i < secret_width * secret_height; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            image_pixels[pixel_index] &= ~1;
            image_pixels[pixel_index] |= (secret_pixels[i] >> bit) & 1;
            pixel_index++;
        }
    }
    
    FILE *fp = fopen(output_filename, "w");
    fprintf(fp, "%s\n%d %d\n%d\n", "P3", image_width, image_height, 255);
    for (unsigned int i = 0; i < total_image_pixels; i++) {
        fprintf(fp, "%d %d %d ", image_pixels[i], image_pixels[i], image_pixels[i]);
    }

    fclose(fp);
    delete_image(secret_image);
    delete_image(image);
    return 1;

    // (void)secret_image_filename;
    // (void)input_filename;
    // (void)output_filename;
    // return 10;
}

void reveal_image(char *input_filename, char *output_filename) {
    Image *image = load_image(input_filename);
    unsigned int width = image->width;
    unsigned int height = image->height;
    unsigned int total_pixels = width * height;

    //2D Array to 1D Array 
    char pixels[total_pixels];
    int index = 0;
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            pixels[index++] = image->pixels[i][j];
        }
    }

    index = 0;

    width = 0;
    for (int bit = 7; bit >= 0; bit--) {
        char current_bit = pixels[index] & 1;
        width |= current_bit << bit;
        index++;
    }

    height = 0;
    for (int bit = 7; bit >= 0; bit--) {
        char current_bit = pixels[index] & 1;
        height |= current_bit << bit;
        index++;
    }

    char secret_pixels[width * height];
    for (unsigned int i = 0; i < width * height; i++) {
        char pixel_value = 0;
        for (int bit = 7; bit >= 0; bit--) {
            char bit_value = pixels[index] & 1;
            pixel_value |= bit_value << bit;
            index++; 
        }
        secret_pixels[i] = pixel_value;
    }

    FILE *fp = fopen(output_filename, "w");
    if (!fp) {
        delete_image(image);
        return;
    }
    fprintf(fp, "%s\n%d %d\n%d\n", "P3", width, height, 255);
    for (unsigned int i = 0; i < width * height; i++) {
        fprintf(fp, "%d %d %d ", secret_pixels[i], secret_pixels[i], secret_pixels[i]);
    }

    fclose(fp);
    delete_image(image);
    // (void)input_filename;
    // (void)output_filename;
}
