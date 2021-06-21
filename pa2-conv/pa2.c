#include <stdlib_checked.h>
#include <stdio_checked.h>
#include <string_checked.h>
#include <stdint.h>
#include "hcomp.h"


int main(int argc, _Nt_array_ptr<char> argv[] : count(argc))
{
	int i;
/*	if(argc != 6)
	{
		_Unchecked{
			fprintf(stderr, "not correct number of args\n");
		}
	}*/

	size_t count = getCharCount(argv[1]);

	_Array_ptr<_Ptr<Tree>> forest : byte_count(count * sizeof(_Ptr<Tree>)) = calloc(count, sizeof(*forest)); //allocates space for the list of trees

	readFromFile(argv[1], forest, count); //reads the original file into the forest
	
	sortForest(forest, count);

	if(count == 0) //check if empty file
	{
		_Unchecked{
			fprintf(stderr, "file is empty\n");
		}
		free(forest);
		//create empty output files
		_Ptr<FILE> fh = NULL;
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
	
	size_t preBuildCount = count;
	
	int writCheck;

	writCheck = freqOutput(argv[2], forest, count); //gets the frequency output
	if(writCheck != EXIT_SUCCESS)
	{
		_Unchecked{
			fprintf(stderr, "err in freqOutput.\n");
		}
		return EXIT_FAILURE;
	}
	
	_Ptr<Tree> root = buildTree(forest, count); //gets the forest built

	free<_Ptr<Tree>>(_Dynamic_bounds_cast<_Array_ptr<_Ptr<Tree>>>(forest, byte_count(0)));

	_Array_ptr<_Ptr<Code>> codeList : count(preBuildCount) = calloc<_Ptr<Code>>(preBuildCount, sizeof(*codeList)); //allocates memory for the code list

	printCode(argv[3], root, codeList, count); //prints the code and adds them to the code list
	
	_Ptr<FILE> fh = fopen(argv[4], "w"); //opens the output file for the header

	printHeader(root, fh); //prints the header to the output


	fclose(fh);

	_Ptr<FILE> readFile = fopen(argv[1], "r"); //opens the original file

	readToCompress(readFile, argv[5], codeList, root, (long)preBuildCount); //reads and compresses the original file

	
	fclose(readFile); //closes the original file

	destroyCodeList(codeList, preBuildCount); //destroys the code list

	destroyTree(root); //destroys the tree
	
	return EXIT_SUCCESS; //return exit success upon completion
}
