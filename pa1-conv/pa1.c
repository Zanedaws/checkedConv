#include "shell_array.h"

int main(int argc, _Nt_array_ptr<char> argv[] : count(argc))
{

	if(argc < 3)
	{
	  fprintf(stderr, "arguments missing");
		return 0;
	}

	int i = 0;
	
	if(!strcmp(argv[1], "-a"))
	{
		int size = 0;
		_Array_ptr<long> array = _Dynamic_bounds_cast<_Array_ptr<long>>(Array_Load_From_File(argv[2], &size), count(size));
		
		//long n_comp = 0;

		clock_t begin = clock();
		// Array_Shellsort(array, size, &n_comp);
		clock_t end = clock();
		double timeSpend = (double)(end - begin) / CLOCKS_PER_SEC;
	  fprintf(stderr, "list sorted in: %fs\n", timeSpend);
		int writ = 0;


		writ = Array_Save_To_File(argv[3], array, size);
	  
	}

}
