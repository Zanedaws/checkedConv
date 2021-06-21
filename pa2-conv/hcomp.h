#include <stdio_checked.h>
#include <stdint.h>
#include <stdlib_checked.h>
#include <string_checked.h>
#define STARTSIZE 4
#define GROWFACTOR 1.5
#define COUNT 10

typedef struct CodeHolder{ //struct to hold the code of a character
	char chr; //character that has the code
	long code; //code of said character
	int depth; //depth of the character in the huffman tree
} Code;

typedef struct HTree{ //huffman tree
	char chr; //character for the node on the tree
	long freq; //frequency or weight of character
	_Ptr<struct HTree> left; //left pointer
	_Ptr<struct HTree> right; //right pointer
} Tree;

size_t getCharCount(_Nt_array_ptr<char> filename);

void readFromFile(_Nt_array_ptr<char> filename, _Array_ptr<_Ptr<Tree>> forest : count(count), int count);

void addTree(_Array_ptr<_Ptr<Tree>> forest : count(count), char tmp, int count);

_Array_ptr<_Ptr<Tree>> destroyForest(_Array_ptr<_Ptr<Tree>> forest : count(count), int count) : byte_count(0);

void destroyTree(_Ptr<Tree> root);

void sortForest(_Array_ptr<_Ptr<Tree>> forest : count(size), int size);

int freqOutput(_Nt_array_ptr<char> filename, _Array_ptr<_Ptr<Tree>> forest : count(size), int size);

_Ptr<Tree> buildTree(_Array_ptr<_Ptr<Tree>> forest : count(inSize), int inSize);

void insert(_Ptr<Tree> root, _Array_ptr<_Ptr<Tree>> forest : count(inSize), int inSize);

void shift(_Array_ptr<_Ptr<Tree>> forest : count(size), int size, int ammount);

void printCode(_Nt_array_ptr<char> filename, _Ptr<Tree> root, _Array_ptr<_Ptr<Code>> codeList  : count(size), int size);

void getSequence(_Ptr<Tree> root, long seq, int depth, _Ptr<FILE> fh, _Array_ptr<_Ptr<Code>> codeList : count(size), _Ptr<int> index, int size);

void printSequence(long seq, int depth, _Ptr<FILE> fh, _Array_ptr<_Ptr<Code>> codeList : count(size), _Ptr<int> index, char chr, int size);

void destroyCodeList(_Array_ptr<_Ptr<Code>> codeList : count(size), int size);

static _Nt_array_ptr<char> stringMalloc(size_t sz) : count(sz);

void readToCompress(_Ptr<FILE> readFile, _Nt_array_ptr<char> writeName, _Array_ptr<_Ptr<Code>> codeList : count(size), _Ptr<Tree> root, int size);

void printCompCode(_Array_ptr<_Ptr<Code>> codeList : count(size), char key, _Ptr<FILE> writeFile, _Ptr<int> totalBits, _Ptr<int> bits, long size, _Ptr<long> totalNumChar);

long printNumChar(_Ptr<FILE> readFile);

void printHuffNum(_Ptr<FILE> writeFile, long size);

void printHeaderInfo(_Ptr<FILE> writeFile, _Ptr<Tree> root, _Ptr<int> bits, _Ptr<int> totalBits, _Ptr<int> totalBytesAdded);

int reverseByte(int bin);

void printHeader(_Ptr<Tree> root, _Ptr<FILE> fh);

void printTree(_Ptr<Tree> root);

void print2DUtil(_Ptr<Tree> root, int space);

void printBinary(int bin, int lentht);
