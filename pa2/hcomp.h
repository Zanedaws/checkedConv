#include <stdio.h>
#include <stdint.h>
#define STARTSIZE 4
#define GROWFACTOR 1.5
#define COUNT 10

typedef struct CodeHolder{ //struct to hold the code of a character
    char chr; //character that has the code
    long code; //code of said character
    int depth; //depth of the character in the huffman tree
}Code;

typedef struct HTree{ //huffman tree
    char chr; //character for the node on the tree
    long freq; //frequency or weight of character
    struct HTree * left; //left pointer
    struct HTree * right; //right pointer
} Tree;

//opens the file and reads it to an array of trees
Tree** readFromFile(char* filename, Tree** forest, int* size);

//adds a new tree to the forest that is passed to it and returns the new forest
Tree** addTree(Tree** forest, char tmp, int* count, int* size);

//destroys the forest passed to it of size count
Tree** destroyForest(Tree** forest, int count);

//destroys the tree using recursion
void destroyTree(Tree* root);

//outputs the frequency of the letters in the forest to the ouput file passed to it.
int freqOutput(char* filename, Tree** forest, int size);

//sorts the forest passed to it using shell sort with values
void sortForest(Tree** forest, int size);

//buids a huffman tree from the forest passed to it and returns the new forest where forest[0] is the full tree
Tree** buildTree(Tree** forest, int* size);

//inserts a tree into a sorted forest for huffman compression
Tree** insert(Tree* root, Tree** forest, int* size);

//shifts the indexes within a forest of size size by ammount to the left
Tree** shift(Tree** forest, int* size, int ammount);

//prints the code of each character in the huffman tree
void printCode(char* filename, Tree* root, Code** codeList);

//gets the header of the huffman tree
void getSequence(Tree* root, long seq, int depth, FILE* fh, Code** codeList, int* index);

//prints the header of the huffman tree
void printSequence(long seq, int depth, FILE* fh, Code** codeList, int* index, char chr);

//prints the character header to the given output file
void printHeader(Tree* root, FILE* fh);

//prints the binary header to the compressed file
void printHeaderInfo(FILE* writeFile, Tree* root, int* bits, int* totalBits, int* totalBytesAdded);

//reads a file to compress it to an output writeFile
void readToCompress(FILE* readFile, char* writeFile, Code** codeList, Tree* root, long size);

//prints the compressed code of the file
void printCompCode(Code** codeList, char key, FILE* writeFile, int* totalBits, int* bits, long size, long* totalNumChar);

//prints the number of characters in the original file
long printNumChar(FILE* readFile);

//prints the number of characters that the huffman tree shows
void printHuffNum(FILE* writeFIle, long size);

//reverses the first byte in the given integer
int reverseByte(int bin);

//destroys the Code** structure
void destroyCodeList(Code** codeList, int size);

//testing code
void printTree(Tree* root);

//prints the tree in 2D
void print2DUtil(Tree* root, int space);

//prints the binary code of a given integer for length number of bits
void printBinary(int bin, int length);