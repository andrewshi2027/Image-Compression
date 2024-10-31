#include "qtree.h"
#include <math.h>

double calculate_rmse (int** pixels, int width, int height) {
    double intensity_sum, average, squared_difference = 0.0;
    int pixel_count = width * height;
    
    //Calculate sum of intensities
    for (int i = 0; i < height; i++) {
        for (int j = 0; i < width * 3; j++)
            intensity_sum = pixels[i][j * 3];
    }

    //Average of intensities
    average = intensity_sum / pixel_count;

    //Difference of average with each pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width * 3; j++) {
            squared_difference += (average - pixels[i][j * 3]) * (average - pixels[i][j * 3]);
        } 
    }

    return sqrt(squared_difference / pixel_count);
}

QTNode *create_quadtree(Image *image, double max_rmse) {
    int width = image->width; 
    int height = image->height;

    //Create a new QTNode
    QTNode *node = (QTNode *)malloc(sizeof(QTNode));
    if (!node) {
        return NULL;
    }

    //Calculate RMSE
    double rmse = calculate_rmse(image->pixels, width, height);

    //Check if RMSE is within threshold
    if (rmse < max_rmse) {
        //node->intensity = average;
        node->child1 = node->child2 = node->child3 = node->child4 = NULL;
        return node;
    }

    //If RMSE > max_rmse, split the node and create 4 children
    //int half_width = width / 2;
    //int half_height = height / 2;

    //Special Cases

    //Case 1: Single Row of Pixels
    //Case 2: Single Column of Pixels





    (void)image;
    (void)max_rmse;
    return NULL;
}

QTNode *get_child1(QTNode *node) {
    //return node->child1;
    (void)node;
    return NULL;
}

QTNode *get_child2(QTNode *node) {
    //return node->child2;
    (void)node;
    return NULL;
}

QTNode *get_child3(QTNode *node) {
    //return node->child3;
    (void)node;
    return NULL;
}

QTNode *get_child4(QTNode *node) {
    //return node->child4;
    (void)node;
    return NULL;
}

unsigned char get_node_intensity(QTNode *node) {
    //return node->intensity;
    (void)node;
    return 0;
}

void delete_quadtree(QTNode *root) {
    // if (root) {
    //     delete_quadtree(root->child1);
    // }
    (void)root;
}

void save_qtree_as_ppm(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}

QTNode *load_preorder_qt(char *filename) {
    (void)filename;
    return NULL;
}

void save_preorder_qt(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}

