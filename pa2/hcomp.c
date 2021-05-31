#include "hcomp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define ZERO 0

//opens the file and reads it to an array of trees
Tree** readFromFile(char* filename, Tree** forest, int* count)
{
    FILE * fh = fopen(filename, "rb"); //opens file to be read

    if(fh == NULL) //checks if file opens correctly
    {
        fprintf(stderr, "file failed to open\n");
        return 0;
    }

    unsigned int tmp;
    int size = STARTSIZE;
    int read = 0;

    while(!feof(fh)) //read through the file character by character
    {
        read = fread(&tmp,1, 1, fh);
        if(read == 0 && !feof(fh))
            fprintf(stderr, "fread messed up\n");
        if(!feof(fh))
        {
            forest = addTree(forest, tmp, count, &size); //add the read letter to the forest
        }
    }

    fclose(fh);

    sortForest(forest, *count); //call to sort the forest

    return forest;
}

//adds a new tree to the forest that is passed to it and returns the new forest
Tree** addTree(Tree** forest, char tmp, int* count, int* size)
{
    Tree* newTree = malloc(sizeof(*newTree)); //allocate space for new tree root
    
    //initialize the new tree
    newTree ->  chr = tmp;
    newTree -> freq = 1;
    newTree -> left = NULL;
    newTree -> right = NULL;

    int i;
    //check if character is already in forest
    for(i = 0; i < *count; i++)
    {
        if(forest[i] -> chr == tmp)
        {
            forest[i] -> freq += 1;
            free(newTree);
            return forest;
        }
    }


    //if it isn't then check if the array is big enough, if not increase the size
    if(*count != 0 && *count < *size)
    {
        forest[*count] = newTree;
        forest[*count] = newTree;
        *count += 1;
        return forest;
    }
    else if(*count == 0)
    {
        forest[*count] = newTree;
        forest[*count] = newTree;
        *count += 1;
        return forest;
    }
    else
    {
        Tree** newForest = realloc(forest, sizeof(*forest) * (*size + *size/4)); //realloc for size by 25% of array size
        *size = *size + *size/4;
        newForest[*count] = newTree;
        newForest[*count] = newTree;
        *count += 1;
        return newForest;
    }

    return forest;
}

//destroys the forest passed to it of size count
Tree** destroyForest(Tree** forest, int count)
{
    int i;
    for(i = count - 1; i >= 0; i--) // loops through the forest from the end to the beginning
    {
        destroyTree(forest[i]); //calls the destroy tree function on each tree
    }
    free(forest); //frees the array pointer
    return NULL;
}

//destroys the tree using recursion
void destroyTree(Tree* root)
{
    if(root != NULL)
    {
        destroyTree(root -> left);
        destroyTree(root -> right);
        free(root);
    }
    root = NULL;
}

//sorts the forest passed to it using shell sort with values
void sortForest(Tree** forest, int size)
{
    //sequence value
    long h = 0;

    //indexing values
    long i = 0;
    long j = 0;

    //temp variables
    long tmp = 0;
    char tmp2 = '\0';

    //getting max sequence value
    do
    {

        h = 3 * h + 1;

    } while (h < size);
    h = (h - 1) / 3;
    
    // h = 1;

    //shell sort --------------------------------------------------
    while (h > 0)
    {
        for (j = h; j < size; j++)
        {
            tmp = forest[j] -> freq;
            tmp2 = forest[j] -> chr;
            i = j;
            while (i >= h && forest[i-h] -> freq > tmp)
            {
                forest[i] -> freq = forest[i-h] -> freq;
                forest[i] -> chr = forest[i-h] -> chr;
                i = i - h;
                forest[i] -> freq = tmp;
                forest[i] -> chr = tmp2;
            }
        }
        //decrement sequence
        h = (h - 1) / 3;
    }
    //shell sort --------------------------------------------------

    return;
}

//outputs the frequency of the letters in the forest to the ouput file passed to it.
int freqOutput(char* filename, Tree** forest, int size)
{

    FILE* fh = fopen(filename, "w"); //open the output file
    
    if(fh == NULL) //check if opened correctly
    {
        fclose(fh);
        fprintf(stderr,"failed to open freqOutput file.\n");
        return EXIT_FAILURE;
    }

    //indexing and temp variables
    int written;
    int i;
    int j;
    char current;
    long zero = ZERO;

    for(i = 0; i < 256; i++)//loops through from 0 to 255 (inclusive) to check if the ascii character is in the tree
    {
        current = i;
        for(j = 0; j < size; j++)//loops through the whole list of characters to see if the current is in the list
        {
            if(forest[j] -> chr == current)//check if character is in the list
            {
                written = fwrite(&(forest[j] -> freq), sizeof(long), 1, fh); //write the frequency of the character to the binary file
                if(written != 1)
                {
                    fprintf(stderr, "error occurred when writing a frequency\n");
                    fclose(fh);
                    return EXIT_FAILURE;
                }
            }
        }
        if(written == 0) //if there is nothing written then write a zero to the output
        {
            written = fwrite(&(zero), sizeof(long), 1, fh);
            if(written != 1)
            {
                fprintf(stderr, "error occurred when writing a zero\n");
                fclose(fh);
                return EXIT_FAILURE;
            }
        }
        written = 0;
    }

    fclose(fh);
    return EXIT_SUCCESS;
}


//TREE BUILDING STARTS HERE---------------------------------------------------------------------------------------------------------------------

//buids a huffman tree from the forest passed to it and returns the new forest where forest[0] is the full tree
Tree** buildTree(Tree** forest, int* size)
{
    //go through tree and pull lowest two nodes and make a new root with them as the left and right nodes
    while(*size > 1)
    {
        //initialize new root for tree
        Tree* newRoot = malloc(sizeof(*newRoot));
        newRoot -> freq = forest[0] -> freq + forest[1] -> freq;
        newRoot -> chr = '\0';
        newRoot -> left = malloc(sizeof(*(newRoot -> left)));
        newRoot -> right = malloc(sizeof(*(newRoot -> right)));
        newRoot -> left -> left = NULL;
        newRoot -> right -> right = NULL;


        //copy all data into a new left and right node
        newRoot -> left -> freq = forest[0] -> freq;
        newRoot -> right -> freq = forest[1] -> freq;
        newRoot -> left -> chr = forest[0] -> chr;
        newRoot -> right -> chr = forest[1] -> chr;
        newRoot -> left -> left = forest[0] -> left;
        newRoot -> right -> right = forest[1] -> right;
        newRoot -> left -> right = forest[0] -> right;
        newRoot -> right -> left = forest[1] -> left;

        forest = shift(forest, size, 2);
        forest = insert(newRoot, forest, size);
        free(newRoot);
    }
    return forest;
}

//inserts a tree into a sorted forest for huffman compression
Tree** insert(Tree* root, Tree** forest, int* size)
{

    int i = 0;
    while(root -> freq >= forest[i] -> freq && i < *size - 1)
    {
        i++;
    }
    int j = *size - 2;
    while(j >= i)
    {
        forest[j + 1] -> freq = forest[j] -> freq;
        forest[j + 1] -> chr = forest[j] -> chr;
        forest[j + 1] -> left = forest[j] -> left;
        forest[j + 1] -> right = forest[j] -> right;
        j--;
    }
    forest[i] -> freq = root -> freq;
    forest[i] -> chr = root -> chr;
    forest[i] -> left = root -> left;
    forest[i] -> right = root -> right;

    return forest;
}

//shifts the indexes within a forest of size size by ammount to the left
Tree** shift(Tree** forest, int* size, int ammount)
{
    int i;
    for(i = ammount; i < *size; i++)
    {   
        forest[i - ammount] -> freq = forest[i] -> freq;
        forest[i - ammount] -> chr = forest[i] -> chr;
        forest[i - ammount] -> left = forest[i] -> left;
        forest[i - ammount] -> right = forest[i] -> right;
    }

    free(forest[i - 1]);

    *size = *size - 1;

    return forest;
}


//CODE OUTPUT START------------------------------------------------------------------------

//prints the code of each character in the huffman tree
void printCode(char* filename, Tree* root, Code** codeList)
{
    FILE* fh = fopen(filename, "w");
    long sequence = 0x00;
    int depth = 0;
    int index = 0;
    getSequence(root, sequence, depth, fh, codeList, &index);
    fclose(fh);
}


//gets the header of the huffman tree
void getSequence(Tree* root, long seq, int depth, FILE* fh, Code** codeList, int* index)
{
    if(root -> left == NULL && root -> right == NULL) //if it is a leaf node
    {
        fprintf(fh, "%c:", root -> chr);
        printSequence(seq, depth - 1, fh, codeList, index, root -> chr);
        fprintf(fh, "\n");
        return;
    }

    seq = seq << 1; //adds a zero to bit sequence
    depth += 1;
    getSequence(root -> left, seq, depth, fh, codeList, index);
    seq = seq >> 1;
    depth--;
    depth++;
    seq = (seq << 1) | 1; //adds a 1 to bit sequence
    getSequence(root -> right, seq, depth, fh, codeList, index);
    seq = seq >> 1;
    depth--;
}

//prints the sequence of the characters of the huffman tree and makes the code list
void printSequence(long seq, int depth, FILE* fh, Code** codeList, int* index, char chr)
{
    int i;
    long tmp;
    codeList[*index] = malloc(sizeof(*codeList[*index]));
    codeList[*index] -> chr = chr;
    codeList[*index] -> code = seq;
    codeList[*index] -> depth = depth + 1;
    (*index)++;
    for(i = depth; i >= 0; i--)
    {
        tmp = seq;
        seq = seq >> i;
        fprintf(fh, "%ld", seq & 0x01);
        seq = tmp;
    }
    
}

//destroys the Code** structure
void destroyCodeList(Code** codeList, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        free(codeList[i]);
    }
    free(codeList);
    codeList = NULL;
}

//COMPRESSION CODE---------------------------------------------------------------------------

//reads a file to compress it to an output writeFile
void readToCompress(FILE* readFile, char* writeName, Code** codeList, Tree* root, long size)
{
    int totalBits = 0;
    int bits = 0;
    long totalNumChar = 0;
    int totalBytesAdded = 0;

    FILE* writeFile = fopen(writeName, "w");

    // fseek(writeFile, sizeof(long) * 3, SEEK_SET);
    printHeaderInfo(writeFile, root, &bits, &totalBits, &totalBytesAdded); //prints the tree topology to the file
   
    if(totalBits > 0)
    {
        while(totalBits < 8)
        {
            bits = bits << 1;
            totalBits++;
        }
        bits = reverseByte(bits);
        fwrite(&bits, 1, 1, writeFile);
        totalBytesAdded++;
    }

    fclose(writeFile);
    writeFile = fopen(writeName, "r");

    char* headerInfo = malloc(sizeof(*headerInfo) * totalBytesAdded);

    int i = 0;
    for(i = 0; i < totalBytesAdded; i++)
    {
        headerInfo[i] = fgetc(writeFile);
    }

    long headerInfoSize = totalBytesAdded;
    
    fclose(writeFile);
    writeFile = fopen(writeName, "w");
    
    // fseek(writeFile, sizeof(long), SEEK_SET);//goes to the 9th byte of the file to save the first ones for a later write
    // fwrite(&(totalBytesAdded), sizeof(long), 1, writeFile);//prints the number of bytes in the header section of the file
    totalNumChar += sizeof(long); //adds the long to the number of bytes
    long numCharOrig = printNumChar(readFile); //prints the number of characters in uncompressed file
    totalNumChar = totalNumChar + sizeof(long); //add the long to the number of bytes

    totalNumChar += totalBytesAdded;
    bits = 0;
    totalBits = 0;

    // fseek(writeFile, 0, SEEK_END);
    // fclose(writeFile);
    // writeFile = fopen(writeName, "a"); //goes to the end of file to pring the compressed version of the file

    char tmp;
    //reads each character of the original file
    while(!feof(readFile))
    {
        tmp = fgetc(readFile);
        
        if(!feof(readFile))
            printCompCode(codeList, tmp, writeFile, &totalBits, &bits, size, &totalNumChar);
    }
    if(totalBits > 0) //checks for any left over bits
    {
        while(totalBits < 8)
        {
            bits = bits << 1;
            totalBits++;
        }
        bits = reverseByte(bits);
        fwrite(&bits, 1, 1, writeFile);
        totalNumChar++;
    }

    fclose(writeFile);
    writeFile = fopen(writeName, "r");

    char* compCode = malloc(sizeof(char) * totalNumChar);

    i = 0;
    char tmpChar;
    while(!feof(writeFile))
    {

        tmpChar = fgetc(writeFile);
        if(!feof(writeFile))
        {
            compCode[i] = tmpChar;
            i++;
        }    
    }

    int compCodeSize = i;

    fclose(writeFile);
    writeFile = fopen(writeName, "w");

    // rewind(writeFile); //resets the writeFile pointer

    totalNumChar += sizeof(long); //adds another long to the total bytes in the compressed file
    fwrite(&totalNumChar, sizeof(long), 1, writeFile); //writes the total bytes in teh compressed file

    fwrite(&headerInfoSize, sizeof(long), 1, writeFile);

    fwrite(&numCharOrig, sizeof(long), 1, writeFile);
    
    fwrite(headerInfo, sizeof(char), headerInfoSize, writeFile);

    fwrite(compCode, sizeof(char), compCodeSize, writeFile);

    fclose(writeFile);

    free(headerInfo);
    free(compCode);
}

//prints the compressed code of the file
void printCompCode(Code** codeList, char key, FILE* writeFile, int* totalBits, int* bits, long size, long* totalNumChar)
{
    
    long i =0;

    //find code in the code list
    while(i < size)
    {
        if(codeList[i] -> chr == key)
        {
            *totalBits = *totalBits + codeList[i] -> depth;
            *bits = *bits << (codeList[i] -> depth);
            *bits = *bits | codeList[i] -> code;
            break;
        }
        i++;
    }
    int tmp;
    //check if the total bits in the bits to print are equal to or greater than 8 then print a byte to the file
    if(*totalBits >= 8)
    {
        tmp = *bits;
        tmp = tmp >> (*totalBits - 8);
        
        tmp = reverseByte(tmp);
        (*totalNumChar)++;
        fwrite(&tmp, 1, 1, writeFile);
        *totalBits = *totalBits - 8;
    }
}


//prints the number of characters in the original file
long printNumChar(FILE* readFile)
{
    fseek(readFile, 0, SEEK_SET);
    fseek(readFile, 0, SEEK_END);
    long numChar = ftell(readFile);
    rewind(readFile);
    return numChar;
}

//prints the number of characters that the huffman tree shows
void printHuffNum(FILE* writeFile, long size)
{
    fwrite(&size, sizeof(long), 1, writeFile);
}

//prints the binary header to the compressed file
void printHeaderInfo(FILE* writeFile, Tree* root, int* bits, int* totalBits, int* totalBytesAdded)
{
    if(root -> left != NULL && root -> right != NULL)
    {
        *bits = *bits << 1; 
        *totalBits = *totalBits + 1;
        
        if(*totalBits >= 8)
        {
            int toWrite = *bits;
            
            toWrite = toWrite >> (*totalBits - 8);
            toWrite = reverseByte(toWrite);
            fwrite(&toWrite, 1, 1, writeFile);
            (*totalBytesAdded)++;
            *totalBits = *totalBits - 8;
        }

        printHeaderInfo(writeFile, root -> left, bits, totalBits, totalBytesAdded);
        printHeaderInfo(writeFile, root -> right, bits, totalBits, totalBytesAdded);
    }
    else
    {
        *bits = *bits << 1;
        *bits = *bits | 0x01;
        *totalBits = *totalBits + 1;
        int j;
        int chr = root -> chr;
        for(j = 0; j < (sizeof(char) * 8); j++)
        {
            chr = root -> chr;
            *bits = *bits << 1;
            chr = chr >> j;
            chr = chr & 0x01;
            *bits = *bits | chr;
            *totalBits = *totalBits + 1;
        }

        if(*totalBits >= 8)
        {
            int toWrite = *bits;
            toWrite = toWrite >> (*totalBits - 8);
            toWrite = reverseByte(toWrite);
            fwrite(&toWrite, 1, 1, writeFile);
            (*totalBytesAdded)++;
            *totalBits = *totalBits - 8;
        }
    }
}

//reverses the first byte in the given integer
int reverseByte(int bin)
{

    int rev = 0;
    int i;
    int tmp;

    for(i = 0; i < 8; i++)
    {
        tmp = (bin & (1 << i));
        if(tmp)
            rev |= (1 << ((8 - 1) - i));
    }

    return rev;
}

//HEADER CODE--------------------------------------------------------------------------------

//prints the character header to the given output file
void printHeader(Tree* root, FILE* fh)
{
    if(root -> left != NULL && root -> right != NULL)
    {
        fprintf(fh, "0");
        printHeader(root -> left, fh);
        printHeader(root -> right, fh);
    }
    else
    {
        fprintf(fh,"1%c", root -> chr);
    }
}


//TESTING CODE-----------------------------------------------------------------------------

//prints the given tree
void printTree(Tree* root)
{
    if(root == NULL)
    {
        fprintf(stderr, "root NULL\n");
        return;
    }
    if(root -> left == NULL && root -> right == NULL)
    {
        fprintf(stderr, "chr = %c, freq = %ld\n", root -> chr, root -> freq);
        return;
    }
    fprintf(stderr, "moved left\n");
    printTree(root -> left);
    fprintf(stderr, "moved right\n");
    printTree(root -> right);
}

//prints the tree in 2D
void print2DUtil(Tree *root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    fprintf(stderr, "\n"); 
    for (int i = COUNT; i < space; i++) 
        fprintf(stderr, " "); 
    fprintf(stderr, "%ld, %c\n", root->freq, root -> chr); 
  
    // Process left child 
    print2DUtil(root->left, space); 
} 

//prints the binary code of a given integer for length number of bits
void printBinary(int bin, int length)
{
    int i;
    int temp;
    for(i = 1; i <= length; i++)
    {
        temp = bin;
        temp = temp >> (length - i);
        fprintf(stderr, "%d", (temp & 0x01));
    }
    fprintf(stderr, "\n");
}