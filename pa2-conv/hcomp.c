#include "hcomp.h"

#pragma CHECKED_SCOPE on

//gets the size of the initial forest

//REWORK: This function was added to obtain the size of the array needed for the program before the time of initialization of said array.
//This just makes it easier to deal with the bounds checking of the array instead of using reallocation.
size_t getCharCount(_Nt_array_ptr<char> filename)
{
  _Ptr<FILE> fh = fopen(filename, "rb");

  size_t count = 0;

  int charTrack _Checked[32] : count(32);

  int i;

  for(i = 0; i < 32; i++)
  {
    charTrack[i] = 0;
  }

  if(fh == NULL)
  {
    _Unchecked{
      fprintf(stderr, "file failed to open\n");
    }
    return count;
  }

  unsigned int tmp = 0;
  int read;

  while(!feof(fh))
  {
    read = fread(&tmp, 1, 1, fh);
    if(read == 0 && !feof(fh))
    {
      _Unchecked{
        fprintf(stderr, "freed messed up\n");
      }
			break;
    }
    if((charTrack[tmp / 32] & (1 << (tmp % 32))) == 0 && !feof(fh))
    {
      charTrack[tmp / 32] |= (1 << (tmp % 32));
      count++;
    }
  }

  fclose(fh);

  return count;
}


//opens the file and reads it to an array of trees

//REWORK: Converted to checked pointer types.
void readFromFile(_Nt_array_ptr<char> filename, _Array_ptr<_Ptr<Tree>> forest : count(count), int count)
{
    _Ptr<FILE> fh = fopen(filename, "rb"); //opens file to be read

    if(fh == NULL) //checks if file opens correctly
    {
        _Unchecked {fprintf(stderr, "file failed to open\n");}
        return;
    }

    unsigned int tmp;
    int read = 0;

    while(!feof(fh)) //read through the file character by character
    {
        read = fread(&tmp,1, 1, fh);
        if(read == 0 && !feof(fh))
            _Unchecked {fprintf(stderr, "fread messed up\n");}
        if(!feof(fh))
        {
            addTree(forest, tmp, count); //add the read letter to the forest
        }
    }

    fclose(fh);

    sortForest(forest, count); //call to sort the forest

    return;
}


//adds a new tree to the forest that is passed to it and returns the new forest

//REWORK: The checked version takes in a count (count of all characters possible in the array) and checks the array for the given tmp char and if found
//increments the frequency value of the character and then returns. There is no need to return an array as there is no new array being allocated or
//any reallocation of the passed in array that may cause a new pointer location to be made for the array pointer passed in.
void addTree(_Array_ptr<_Ptr<Tree>> forest : count(count), char tmp, int count)
{
    _Ptr<Tree> newTree = malloc<Tree>(sizeof(*newTree)); //allocate space for new tree root
   
    //initialize the new tree
    newTree ->  chr = tmp;
    newTree -> freq = 1;
    newTree -> left = NULL;
    newTree -> right = NULL;

    int i;
    //check if character is already in forest
    for(i = 0; i < count; i++)
    {
				if(forest[i] == NULL)
				{
					forest[i] = newTree;
					return;
				}
        else if(forest[i] -> chr == tmp)
        {
        	forest[i] -> freq += 1;
					free<Tree>(newTree);
        	return;
        }
    }

    return;
}

//destroys the forest passed to it of size count

//REWORK: Just changed the types and added the type to the free statement.
_Array_ptr<_Ptr<Tree>> destroyForest(_Array_ptr<_Ptr<Tree>> forest : count(count), int count) : byte_count(0)
{
    int i;
    for(i = count - 1; i >= 0; i--) // loops through the forest from the end to the beginning
    {
        destroyTree(forest[i]); //calls the destroy tree function on each tree
    }
    free<_Ptr<Tree>>(_Dynamic_bounds_cast<_Array_ptr<_Ptr<Tree>>>(forest, byte_count(0))); //frees the array pointer
    return NULL;
}

//destroys the tree using recursion

//REWORK: changed the types
void destroyTree(_Ptr<Tree> root)
{
    if(root != NULL)
    {
        destroyTree(root -> left);
        destroyTree(root -> right);
        free<Tree>(root);
    }
		root = NULL;
}

//sorts the forest passed to it using shell sort with values

//REWORK: changed the types
void sortForest(_Array_ptr<_Ptr<Tree>> forest : count(size), int size)
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

//REWORK: changed types
int freqOutput(_Nt_array_ptr<char> filename, _Array_ptr<_Ptr<Tree>> forest : count(size), int size)
{

    _Ptr<FILE> fh = fopen(filename, "w"); //open the output file
    
    if(fh == NULL) //check if opened correctly
    {
        fclose(fh);
        _Unchecked {fprintf(stderr,"failed to open freqOutput file.\n");}
        return EXIT_FAILURE;
    }

    //indexing and temp variables
    int written = 0;
    int i;
    int j;
    char current;
    long zero = 0;

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
                    _Unchecked {fprintf(stderr, "error occurred when writing a frequency\n");}
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
                _Unchecked {fprintf(stderr, "error occurred when writing a zero\n");}
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

//REWORK: changed types and made the size pointer into a non-pointer variable. Made int a void function as no new array is being created and thus,
//no new array needs to be assigned through the return statement.
_Ptr<Tree> buildTree(_Array_ptr<_Ptr<Tree>> forest : count(inSize), int inSize)
{
		int size = inSize; //needed due to changing size variable throughout funciton

		int i;

		//for(i = 0; i < size; i++)
		//	_Unchecked {fprintf(stderr, "build chr = %c\n", forest[i] -> chr);}


    //go through tree and pull lowest two nodes and make a new root with them as the left and right nodes
    while(size > 1)
    {
				
        //initialize new root for tree
        _Ptr<Tree> newRoot = malloc<Tree>(sizeof(*newRoot));
        newRoot -> freq = forest[0] -> freq + forest[1] -> freq;
        newRoot -> chr = '\0';
        newRoot -> left = malloc<Tree>(sizeof(*(newRoot -> left)));
        newRoot -> right = malloc<Tree>(sizeof(*(newRoot -> right)));
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

        shift(forest, size, 2);
				size = size - 1;
        insert(newRoot, forest, size);
				free<Tree>(newRoot);
    }

    return forest[0];
}

//inserts a tree into a sorted forest for huffman compression

//REWORK: changed to checked types. Made the function void as there is no new array made that needs to be returned. Changed size to non-pointer variable
void insert(_Ptr<Tree> root, _Array_ptr<_Ptr<Tree>> forest : count(inSize), int inSize)
{

		int size = inSize; //needed due to changing of size throughout function

    int i = 0;
    while(root -> freq >= forest[i] -> freq && i < size - 1)
    {
        i++;
    }
    int j = size - 2;
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


    return;
}

//shifts the indexes within a forest of size size by ammount to the left

//REWORK: fixed functionality and made it much easier to read and far simpler. Made void funciton due to no new array being created.
void shift(_Array_ptr<_Ptr<Tree>> forest : count(size), int size, int ammount)
{
    int i;
    for(i = ammount; i < size; i++)
    {
			forest[i - ammount] -> freq = forest[i] -> freq;
			forest[i - ammount] -> chr = forest[i] -> chr;
			forest[i - ammount] -> left = forest[i] -> left;
			forest[i - ammount] -> right = forest[i] -> right;
		}
		
		free<Tree>(forest[size - 1]);

    return;
}


//CODE OUTPUT START------------------------------------------------------------------------

//prints the code of each character in the huffman tree

//REWORK: Changed to checked pointer types. Added size variable for the codeList array pointer.
void printCode(_Nt_array_ptr<char> filename, _Ptr<Tree> root, _Array_ptr<_Ptr<Code>> codeList : count(size), int size)
{
    _Ptr<FILE> fh = fopen(filename, "w");
    long sequence = 0x00;
    int depth = 0;
    int index = 0;
    getSequence(root, sequence, depth, fh, codeList, &index, size);
    fclose(fh);
}


//gets the header of the huffman tree

//REWORK: chagned to checked poitner types. Added size parameter for codeList array pointer.
void getSequence(_Ptr<Tree> root, long seq, int depth, _Ptr<FILE> fh, _Array_ptr<_Ptr<Code>> codeList : count(size), _Ptr<int> index, int size)
{
    if(root -> left == NULL && root -> right == NULL) //if it is a leaf node
    {
        _Unchecked {fprintf(fh, "%c:", root -> chr);}
        printSequence(seq, depth - 1, fh, codeList, index, root -> chr, size);
        _Unchecked {fprintf(fh, "\n");}
        return;
    }

    seq = seq << 1; //adds a zero to bit sequence
    depth += 1;
    getSequence(root -> left, seq, depth, fh, codeList, index, size);
    seq = seq >> 1;
    depth--;
    depth++;
    seq = (seq << 1) | 1; //adds a 1 to bit sequence
    getSequence(root -> right, seq, depth, fh, codeList, index, size);
    seq = seq >> 1;
    depth--;
}

//prints the sequence of the characters of the huffman tree and makes the code list

//REWORK: changed the checked pointer types. Added size parameter for codeList array pointer.
void printSequence(long seq, int depth, _Ptr<FILE> fh, _Array_ptr<_Ptr<Code>> codeList : count(size), _Ptr<int> index, char chr, int size)
{
    int i;
    long tmp;

    codeList[*index] = malloc<Code>(sizeof(*codeList[*index]));
    codeList[*index] -> chr = chr;
    codeList[*index] -> code = seq;
    codeList[*index] -> depth = depth + 1;
    (*index)++;
    for(i = depth; i >= 0; i--)
    {
        tmp = seq;
        seq = seq >> i;
        _Unchecked {fprintf(fh, "%ld", seq & 0x01);}
        seq = tmp;
    }
    
}

//destroys the Code** structure

//REWORK: changed to checked pointer types. Added size parameter for codeList array pointer.
void destroyCodeList(_Array_ptr<_Ptr<Code>> codeList : count(size), int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        free<Code>(codeList[i]);
    }
    free<_Ptr<Code>>(_Dynamic_bounds_cast<_Array_ptr<_Ptr<Code>>>(codeList, byte_count(0)));
		codeList = NULL;
}

//COMPRESSION CODE---------------------------------------------------------------------------



//code was stolen from parson.c manually converted code
static _Nt_array_ptr<char> stringMalloc(size_t sz) : count(sz) _Unchecked
{
	char* p = (char *)malloc<char>(sz + 1);
	if(p != NULL)
		p[sz] = 0;
	return _Assume_bounds_cast<_Nt_array_ptr<char>>(p, count(sz));
}



//reads a file to compress it to an output writeFile

//REWORK: changed to checked pointer types. Changed size parameter to type int. Implemented use of stringMalloc function to allow for creation of
//_Nt_array_ptr<char> pointers
void readToCompress(_Ptr<FILE> readFile, _Nt_array_ptr<char> writeName, _Array_ptr<_Ptr<Code>> codeList : count(size), _Ptr<Tree> root, int size)
{
    int totalBits = 0;
    int bits = 0;
    long totalNumChar = 0;
    int totalBytesAdded = 0;

    _Ptr<FILE> writeFile = fopen(writeName, "w");

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

    _Nt_array_ptr<char> headerInfo : byte_count(totalBytesAdded) = stringMalloc(sizeof(*headerInfo) * totalBytesAdded);

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

    _Nt_array_ptr<char> compCode : byte_count(totalNumChar) = stringMalloc(sizeof(char) * totalNumChar);

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
    writeFile = NULL;
		writeFile = fopen(writeName, "w");

    // rewind(writeFile); //resets the writeFile pointer

    long finalNumChar = 0;
		finalNumChar = totalNumChar + sizeof(long); //had to be added because totalNumChar is used as a bounds parameter earlier in function.
		fwrite(&finalNumChar, sizeof(long), 1, writeFile); //writes the total bytes in teh compressed file

    fwrite(&headerInfoSize, sizeof(long), 1, writeFile);

    fwrite(&numCharOrig, sizeof(long), 1, writeFile);
    
    fwrite(headerInfo, sizeof(char), headerInfoSize, writeFile);

    fwrite(compCode, sizeof(char), totalNumChar, writeFile);

    fclose(writeFile);

    //free<_Ptr<Tree>>(_Dynamic_bounds_cast<_Array_ptr<_Ptr<Tree>>>(forest, byte_count(0))); //frees the array pointer
    free<char>(_Dynamic_bounds_cast<_Array_ptr<char>>(headerInfo, byte_count(0)));
    free<char>(_Dynamic_bounds_cast<_Array_ptr<char>>(compCode, byte_count(0)));
}

//prints the compressed code of the file

//REWORK: Converted to checked pointer types
void printCompCode(_Array_ptr<_Ptr<Code>> codeList : count(size), char key, _Ptr<FILE> writeFile, _Ptr<int> totalBits, _Ptr<int> bits, long size, _Ptr<long> totalNumChar)
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

//REWORK: Converted to checked pointer types
long printNumChar(_Ptr<FILE> readFile)
{
    fseek(readFile, 0, SEEK_SET);
    fseek(readFile, 0, SEEK_END);
    long numChar = ftell(readFile);
    rewind(readFile);
    return numChar;
}

//prints the number of characters that the huffman tree shows

//REWORK: Converted to checked pointer types
void printHuffNum(_Ptr<FILE> writeFile, long size)
{
    fwrite(&size, sizeof(long), 1, writeFile);
}

//prints the binary header to the compressed file

//REWORK: Converted to checked pointer types
void printHeaderInfo(_Ptr<FILE> writeFile, _Ptr<Tree> root, _Ptr<int> bits, _Ptr<int> totalBits, _Ptr<int> totalBytesAdded)
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

//REWORK: no work needed
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

//REWORK: converted to checked pointer types. Added unchecked fprintfs.
void printHeader(_Ptr<Tree> root, _Ptr<FILE> fh)
{
    if(root -> left != NULL && root -> right != NULL)
    {
        _Unchecked {fprintf(fh, "0");}
        printHeader(root -> left, fh);
        printHeader(root -> right, fh);
    }
    else
    {
        _Unchecked {fprintf(fh,"1%c", root -> chr);}
    }
}


//TESTING CODE-----------------------------------------------------------------------------

//prints the given tree

//REWORK: Converted to checked pointer types. Added unchecked fprintfs.
void printTree(_Ptr<Tree> root)
{
    if(root == NULL)
    {
        _Unchecked {fprintf(stderr, "root NULL\n");}
        return;
    }
    if(root -> left == NULL && root -> right == NULL)
    {
        _Unchecked {fprintf(stderr, "chr = %c, freq = %ld\n", root -> chr, root -> freq);}
        return;
    }
    _Unchecked {fprintf(stderr, "moved left\n");}
    printTree(root -> left);
    _Unchecked {fprintf(stderr, "moved right\n");}
    printTree(root -> right);
}

//prints the tree in 2D

//REWORK: Converted to checked pointer types. Added unchecked fprintfs
void print2DUtil(_Ptr<Tree> root, int space) 
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
    _Unchecked {fprintf(stderr, "\n");}
    for (int i = COUNT; i < space; i++) 
        _Unchecked {fprintf(stderr, " "); }
    _Unchecked {fprintf(stderr, "%ld, %c\n", root->freq, root -> chr); }
  
    // Process left child 
    print2DUtil(root->left, space); 
} 

//prints the binary code of a given integer for length number of bits

//REWORK: added unchecked fprintfs.
void printBinary(int bin, int length)
{
    int i;
    int temp;
    for(i = 1; i <= length; i++)
    {
        temp = bin;
        temp = temp >> (length - i);
        _Unchecked {fprintf(stderr, "%d", (temp & 0x01));}
    }
    _Unchecked {fprintf(stderr, "\n");}
}
