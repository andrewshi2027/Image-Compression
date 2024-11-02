#include "qtree.h"
#include <math.h>

double calculate_average(int** pixels, int width, int height) {
    double intensity_sum, average = 0.0;
    int pixel_count = width * height;

    //Calculate sum of intensities
    for (int i = 0; i < height; i++) {
        for (int j = 0; i < width; j++)
            intensity_sum = pixels[i][j];
    }

    //Average of intensities
    average = intensity_sum / pixel_count;

    return average;
}

double calculate_rmse(int** pixels, int width, int height) {
    double average, squared_difference = 0.0;
    int pixel_count = width * height;
    
    average = calculate_average(pixels, width, height);

    //Difference of average with each pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            squared_difference += (average - pixels[i][j]) * (average - pixels[i][j]);
        } 
    }

    return sqrt(squared_difference / pixel_count);
}

QTNode *quadtree_helper(Image *image, double max_rmse, int row_start, int row_end, int column_start, int column_end) {

    //Create a new QTNode
    QTNode *node = (QTNode *)malloc(sizeof(QTNode));
    if (!node) {
        return NULL;
    }

    node->row_start = row_start;
    node->row_end = row_end;
    node->column_start = column_start;
    node->column_end = column_end;

    //Intensity 
    node->intensity = (unsigned char)calculate_average(image->pixels, column_end, row_end);

    //RMSE
    double rmse = calculate_rmse(image->pixels, column_end, row_end);

    int width = column_end - column_start;
    int height = row_end - row_start;

    //Split Node and Create Four Children
    if (rmse > max_rmse) {
        //Case 1: Single Row of Pixels, children 3 and 4 are NULL
        if (height == 1) {
            node->child1 = quadtree_helper(image, max_rmse, row_start, row_end, column_start, (column_start + width) / 2);
            node->child2 = quadtree_helper(image, max_rmse, row_start, row_end, (column_start + width) / 2 , column_end);
        }
        //Case 2: Single Column of Pixels, children 2 and 4 are NULL
        else if (width == 1) {
            node->child1 = quadtree_helper(image, max_rmse, row_start, (row_start + height) / 2, column_start, column_end);
            node->child3 = quadtree_helper(image, max_rmse, (row_start + height) / 2, row_end, column_start, column_end);
        }
        //Case 3: General case, split into 4 quadrants
        else {
            node->child1 = quadtree_helper(image, max_rmse, row_start, (row_start + height) / 2, column_start, (column_start + width) / 2);
            node->child2 = quadtree_helper(image, max_rmse, row_start, (row_start + height) / 2, (column_start + width) / 2, column_end);
            node->child3 = quadtree_helper(image, max_rmse, (row_start + height) / 2, row_end, column_start, (column_start + width) / 2);
            node->child4 = quadtree_helper(image, max_rmse, (row_start + height) / 2, row_end, (column_start + width) / 2, column_end);
        }
    }
    return node;
}

QTNode *create_quadtree(Image *image, double max_rmse) {

    int width = image->width;
    int height = image->height;

    return quadtree_helper(image, max_rmse, 0, height, 0, width);
    // (void)image;
    // (void)max_rmse;
    // return NULL;
}

QTNode *get_child1(QTNode *node) {
    return node->child1;
    // (void)node;
    // return NULL;
}

QTNode *get_child2(QTNode *node) {
    return node->child2;
    // (void)node;
    // return NULL;
}

QTNode *get_child3(QTNode *node) {
    return node->child3;
    // (void)node;
    // return NULL;
}

QTNode *get_child4(QTNode *node) {
    return node->child4;
    // (void)node;
    // return NULL;
}

unsigned char get_node_intensity(QTNode *node) {
    return node->intensity;
    // (void)node;
    // return 0;
}

void delete_quadtree(QTNode *root) {
    if (root) {
        delete_quadtree(root->child1);
        delete_quadtree(root->child2);
        delete_quadtree(root->child3);
        delete_quadtree(root->child4);
        free(root);
    }
    //(void)root;
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

