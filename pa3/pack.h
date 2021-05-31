#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TNode{
    char type;
    int width;
    int height;
    int label;
    struct TNode* left;
    struct TNode* right;
}node;

node** readNodes(char* filename, int* size);
node** addNode(node** nodes, node* newNode, int* size, int* maxSize);

node* constTree(node** nodes, int* udx);

void printPre(node* root, FILE* writeFile);

void printDim(node* root, FILE* writeFile);

void printPack(node* root, FILE* writeFile, int xpos, int ypos);

node* destroyTree(node* root);