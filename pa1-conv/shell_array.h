#include <stdio_checked.h>
#include <string_checked.h>
#include <stdlib_checked.h>
#include <time_checked.h>


int getSize(_Nt_array_ptr<char> filename);

_Array_ptr<long> Array_Load_From_File(_Nt_array_ptr<char> filename, int size) : count(size);

int Array_Save_To_File(_Nt_array_ptr<char> filename, _Array_ptr<long> array : count(size), int size);

//void Array_shellsort(_Array_ptr<long> array : count(size), int size, _Array_ptr<long> n_comp);


