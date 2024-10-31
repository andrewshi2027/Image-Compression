#include "qtree.h"

QTNode *create_quadtree(Image *image, double max_rmse) {
    int width = image->width;
    int height = image->height;

    //Create a new QTNode
    QTNode *node = (QTNode *)malloc(sizeof(QTNode));
    if (!node) {
        return NULL;
    }

    //Base Case: Area is 1x1
    if (width == 1 && height == 1) {
        node->intensity = image->pixels[0][0];
        node->child1 = node->child2 = node->child3 = node->child4 = NULL;
        return node;
    }

    //Calculate Average Intensity of the current region
    unsigned char sum = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width * 3; j++) {
            sum += image->pixels[i][j];
        }
    }
    unsigned char average = sum / (width * height);

    //Calculate RMSE
    double rmse;

    //Check if RMSE is within threshold
    if (rmse < max_rmse) {
        node->intensity = average;
        node->child1 = node->child2 = node->child3 = node->child4 = NULL;
        return node;
    }




    // (void)image;
    // (void)max_rmse;
    // return NULL;
}

QTNode *get_child1(QTNode *node) {
    (void)node;
    return NULL;
}

QTNode *get_child2(QTNode *node) {
    (void)node;
    return NULL;
}

QTNode *get_child3(QTNode *node) {
    (void)node;
    return NULL;
}

QTNode *get_child4(QTNode *node) {
    (void)node;
    return NULL;
}

unsigned char get_node_intensity(QTNode *node) {
    return node->intensity;
    // (void)node;
    // return 0;
}

void delete_quadtree(QTNode *root) {
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

