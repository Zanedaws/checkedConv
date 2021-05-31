#include "pack.h"

int main(int argc, char** argv)
{
    if(argc != 5)
    {
        fprintf(stderr, "Not correct number of input files.\n");
        return EXIT_FAILURE;
    }

    int size = 0;
    node** nodes = readNodes(argv[1], &size);

    if(size == 0)
	{
		free(nodes);
		fprintf(stderr, "input file is empty\n");
		return EXIT_FAILURE;
	}

    if(nodes == NULL)
        return EXIT_FAILURE;

    int udx = size - 1;
    node* root = constTree(nodes, &udx);
    free(nodes);
    
    FILE* fh = fopen(argv[2], "w");
    printPre(root, fh);
    fclose(fh);

    fh = fopen(argv[3], "w");
    printDim(root, fh);
    fclose(fh);

    fh = fopen(argv[4], "w");
    printPack(root, fh, 0, 0);
    fclose(fh);

    root = destroyTree(root);
    free(root);

    return EXIT_SUCCESS;
}
