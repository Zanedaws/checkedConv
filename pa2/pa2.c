#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "hcomp.h"

int main(int argc, char** argv)
{
    if(argc != 6) //checks if correct number of arguments are given
    {
        fprintf(stderr,"not correct number of args\n");
        return EXIT_FAILURE;
    }

    Tree** forest = malloc(sizeof(*forest) * STARTSIZE); //allocates space for the list of trees
    int count = 0;

    forest = readFromFile(argv[1], forest, &count); //reads the original file into the forest

    if(count == 0) //checks for empty file
    {
        fprintf(stderr, "file is empty\n");
        free(forest);
        //create empty output files
        int i;
        FILE* fh;
        for(i = 2; i < argc; i++)
        {
            fh = fopen(argv[i], "w");

            if(argv[2])
            {
                long zero = 0;
                int j;
                for(j = 0; j < 256; j++)
                {
                    fwrite(&zero, sizeof(long), 1, fh);
                }
            }

            fclose(fh);
        }
        return EXIT_SUCCESS;
    }

    int preBuildCount = count;

    int writCheck;

    writCheck = freqOutput(argv[2], forest, count); //gets the frequency output
    if(writCheck != EXIT_SUCCESS)
    {
        fprintf(stderr, "err in freqOutput.\n");
        return EXIT_FAILURE;
    }

    Tree** rootForest = buildTree(forest, &count); //gets the forest built

    Code** codeList = malloc(sizeof(*codeList) * preBuildCount); //allocates memory for the code list

    printCode(argv[3], rootForest[0], codeList); //prints the code and adds them to the code list

    FILE* fh = fopen(argv[4], "w"); //opens the output file for the header

    printHeader(rootForest[0], fh); //prints the header to the output

    fclose(fh); //closes the output file

    FILE* readFile = fopen(argv[1], "r"); //opens the original file

    readToCompress(readFile, argv[5], codeList, rootForest[0], (long)preBuildCount); //reads and compresses the original file

    fclose(readFile); //closes the original file

    // print2DUtil(rootForest[0], 0);

    destroyCodeList(codeList, preBuildCount); //destroys the code list

    destroyTree(rootForest[0]); //destroys the tree

    free(rootForest); //frees the Tree** array

    return EXIT_SUCCESS; //returns exit success upon completion
}