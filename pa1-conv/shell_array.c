#include "shell_array.h"

//This is unchecked because reading of file causes the bounds of the array to be undetermined
_Array_ptr<long> Array_Load_From_File(_Nt_array_ptr<char> filename,  _Ptr<int> size) : count(*size)
{

	_Ptr<FILE> fh = fopen(filename, "r");


	if (fh == NULL)
	{
	  fprintf(stderr, "failed to open file properly\n");
		return NULL;
	}

	//get size of ifle in bytes
	fseek(fh, 0, SEEK_END);
	*size = ftell(fh) / sizeof(long);
	rewind(fh);

  int locSize = *size;

	//allocate array space
 	//_Array_ptr<long> arr = _Dynamic_bounds_cast<_Array_ptr<long>>(calloc<long>((locSize), sizeof(long)), count(sizeof(long) * (*size)));
	
	long* arr = calloc<long>(locSize, sizeof(long));

	//read file
	int ver;
  
  ver = fread(arr, sizeof(long), (locSize), fh);
	
	if(ver != locSize)
	{
		fclose(fh);
		free<long>(arr);
		exit(EXIT_FAILURE);
		return NULL;
	}

	int i;
	for(i = 0; i < locSize; i++)
	{
		printf("%ld\n", arr[i]);
	}

	//close file and return
	fclose(fh);
	return arr;
}



int Array_Save_To_File(_Nt_array_ptr<char> filename, _Array_ptr<long> array : count(size), int size)
{

	//open file
	_Ptr<FILE> fh = NULL;

	fh = fopen(filename, "wb");

	//check if opened successfully and if there is an array to write
	if (fh == NULL || size == 0)
	{
		fclose(fh);
		free<long>(array);
		return 0;
	}

	//number of written items
	int numWrit;

	//write to file
	//numWrit = fwrite(array, sizeof(long), size, fh); //don't know how to get fwrite to cooperate at the moment
  

	//Thus i use fputs to just print the values as strings into a file
  int i;
	for(i = 0; i < size; i++)
	{
		fprintf(fh, "%ld\n", array[i]);
  }

  i++;

  //check number written to see if successful
	if (i != size)
	{
	  fclose(fh);
		free<long>(array);
    return 0;
	}

	//close file
	fclose(fh);

	//free array
	free<long>(array);

	return i;

}
