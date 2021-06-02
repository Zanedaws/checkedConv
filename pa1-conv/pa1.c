//pa1.c contains the main function

#include "shell_array.h"

int main(int argc, _Nt_array_ptr<char> argv[] : count(argc))
{

	if(argc < 3)
	{
	  fprintf(stderr, "arguments missing");
		return 0;
	}

	if(!strcmp(argv[1], "-a"))
	{
		size_t size = getSize(argv[2]);

		_Array_ptr<long> array : count(size) = Array_Load_From_File(argv[2], size);
		
		long n_comp = 0;

		clock_t begin = clock();
		Array_Shellsort(array, size, &n_comp);
		clock_t end = clock();
		double timeSpend = (double)(end - begin) / CLOCKS_PER_SEC;
	  fprintf(stderr, "list sorted in: %fs\n", timeSpend);
		int writ = 0;

		writ = Array_Save_To_File(argv[3], array, size);
	 

		//Don't understand how to get free to work without errors on compilation
		//free<long>(array);
	}

}
