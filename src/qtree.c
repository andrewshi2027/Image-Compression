#include "qtree.h"
#include <math.h>

double calculate_average(int** pixels, int row_start, int row_end, int column_start, int column_end) {

    //Variables
    double intensity_sum = 0.0;
    double average = 0.0;
    double pixel_count = (column_end - column_start) * (row_end - row_start);

    //Calculate sum of intensities
    for (int i = row_start; i < row_end; i++) {
        for (int j = column_start; j < column_end; j++)
            intensity_sum += pixels[i][j];
    }

    //Average of intensities
    average = intensity_sum / pixel_count;

    return average;
}

double calculate_rmse(int** pixels, int row_start, int row_end, int column_start, int column_end, double average) {

    //Variables
    double squared_difference = 0.0;
    double pixel_count = (column_end - column_start) * (row_end - row_start);
    
    //Difference of average with each pixel
    for (int i = row_start; i < row_end; i++) {
        for (int j = column_start; j < column_end; j++) {
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

    //Set Children to NULL
    node->child1 = NULL;
    node->child2 = NULL;
    node->child3 = NULL;
    node->child4 = NULL;

    node->row_start = row_start;
    node->row_end = row_end;
    node->column_start = column_start;
    node->column_end = column_end;

    int width = column_end - column_start;
    int height = row_end - row_start;

    //Intensity  
    double average_intensity = calculate_average(image->pixels, row_start, row_end, column_start, column_end);
    node->intensity = (unsigned char)average_intensity;

    //RMSE
    double rmse = calculate_rmse(image->pixels, row_start, row_end, column_start, column_end, node->intensity);

    //Split Node
    if (rmse > max_rmse) {
        int row_mid = row_start + (height / 2);
        int column_mid = column_start + (width / 2);

        //Case 1: Single Row of Pixels, children 3 and 4 are NULL
        if (height == 1) {
            node->child1 = quadtree_helper(image, max_rmse, row_start, row_end, column_start, column_mid);
            node->child2 = quadtree_helper(image, max_rmse, row_start, row_end, column_mid , column_end);
        }
        //Case 2: Single Column of Pixels, children 2 and 4 are NULL
        else if (width == 1) {
            node->child1 = quadtree_helper(image, max_rmse, row_start, row_mid, column_start, column_end);
            node->child3 = quadtree_helper(image, max_rmse, row_mid, row_end, column_start, column_end);
        }
        //Case 3: Split into 4 Quadrants
        else {
            node->child1 = quadtree_helper(image, max_rmse, row_start, row_mid, column_start, column_mid);
            node->child2 = quadtree_helper(image, max_rmse, row_start, row_mid, column_mid, column_end);
            node->child3 = quadtree_helper(image, max_rmse, row_mid, row_end, column_start, column_mid);
            node->child4 = quadtree_helper(image, max_rmse, row_mid, row_end, column_mid, column_end);
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


void save_ppm_helper (QTNode *root, int width, int height, int pixels[height][width]) {
    if(!root) {
        return;
    }

    //Case 1: Leaf Node, no children
    if(!root->child1 && !root->child2 && !root->child3 && !root->child4) {
        for (int i = root->column_start; i < root->column_end; i++) {
            for (int j = root->row_start; j < root->row_end; j++) {
                pixels[i][j] = root->intensity;
            }
        }
        return;
    }

    //Case 2: Internal Node, continue recursion on each child
    save_ppm_helper(root->child1, width, height, pixels);
    save_ppm_helper(root->child2, width, height, pixels);
    save_ppm_helper(root->child3, width, height, pixels);
    save_ppm_helper(root->child4, width, height, pixels);
}

//Traverses the quadtree rooted at root and saves an image in PPM format using the fields stored in each quadtree node
void save_qtree_as_ppm(QTNode *root, char *filename) {
    FILE *fp = fopen(filename, "w");
    if(!fp) {
        return;
    }

    int width = root->column_end - root->column_start;
    int height = root->row_end -root->row_start;

    //Write PPM File Header
    fprintf(fp, "P3\n");

    //Write Width, Height and Maximum Intensity
    fprintf(fp, "%d %d\n%d\n", width, height, 255);
    
    //Populate pixels array
    int pixels[height][width];
    save_ppm_helper(root, height, width, pixels);

    //Write RGB to PPM File
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(fp, "%d %d %d ", pixels[i][j], pixels[i][j], pixels[i][j]);
        }
    }

    fclose(fp);
    // (void)root;
    // (void)filename;
}

QTNode *load_preorder_helper(FILE *fp) {
    //Variables
    char node_type;
    int average_intensity, row_start, column_start, width, height;

    //Read Data
    fscanf(fp, "%c %d %d %d %d %d\n", &node_type, &average_intensity, &row_start, &height, &column_start, &width);

    //Initialize and assign values to node
    QTNode *node = (QTNode *)malloc(sizeof(QTNode));
    node->intensity = (unsigned char)average_intensity;
    node->row_start = row_start;
    node->row_end = row_start + height;
    node->column_start = column_start;
    node->column_end = column_start + width;

    //Set Children to NULL
    node->child1 = NULL;
    node->child2 = NULL;
    node->child3 = NULL;
    node->child4 = NULL;

    //If node is an internal node
    if (node_type == 'N') {
        //Case 1: Single Row of Pixels, children 3 and 4 are NULL
        if(width == 1) {
            node->child1 = load_preorder_helper(fp);
            node->child3 = load_preorder_helper(fp);
        }
        //Case 2: Single Column of Pixels, children 2 and 4 are NULL
        else if(height == 1) {
            node->child1 = load_preorder_helper(fp);
            node->child2 = load_preorder_helper(fp);
        }
        //Case 3: Split into 4 Quadrants
        else {
            node->child1 = load_preorder_helper(fp);
            node->child2 = load_preorder_helper(fp);
            node->child3 = load_preorder_helper(fp);
            node->child4 = load_preorder_helper(fp);
        }
    }
    return node;
}

QTNode *load_preorder_qt(char *filename) {
    FILE *fp = fopen(filename, "r");

    if(!fp) {
        return NULL;
    }

    QTNode *node = load_preorder_helper(fp);

    fclose(fp);
    return node;
    // (void)filename;
    // return NULL;
}

void save_preorder_helper(QTNode *root, FILE *fp) {
    if(!root) {
        return;
    }

    //Variables
    char node_type;
    int average_intensity, row_start, row_end, column_end, column_start, width, height;

    average_intensity = root->intensity;
    row_start = root->row_start;
    row_end = root->row_end;
    column_start = root->column_start;
    column_end = root->column_end;
    width = column_end - column_start;
    height = row_end - row_start;

    //Leaf Node
    if (!root->child1 && !root->child2 && !root->child3 && !root->child4) {
        node_type = 'L';
        fprintf(fp, "%c %d %d %d %d %d\n", node_type, average_intensity, row_start, height, column_start, width);
    }
    //Internal Node
    else {
        node_type = 'N';
        fprintf(fp, "%c %d %d %d %d %d\n", node_type, average_intensity, row_start, height, column_start, width);
        save_preorder_helper(root->child1, fp);
        save_preorder_helper(root->child2, fp);
        save_preorder_helper(root->child3, fp);
        save_preorder_helper(root->child4, fp);
    }
}

//Saves the preorder traversal of the quadtree rooted at root using the preorder traversal encoding scheme described above.
void save_preorder_qt(QTNode *root, char *filename) {
    FILE *fp = fopen(filename, "w");

    if(!fp) {
        return;
    }

    save_preorder_helper(root, fp);
    fclose(fp);
    // (void)root;
    // (void)filename;
}

