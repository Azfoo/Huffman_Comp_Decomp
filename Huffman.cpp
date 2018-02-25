#include<iostream>
#include<string>
#include<bitset>
#include<fstream>
#include<queue>
#include<sstream>
#include"Huffman.h"
using namespace std;

///////////////////// "key" CONSTRUCTORS AND DESTRUCTORS //////////////////////////

/////Static Data Member/////
unsigned int key::birth = 0;
////////////////////////////
key::key()
{
	CHARACTER = NULL;
	priority = 0;
	birth++;
	Left = NULL; Right = NULL;
	birth_time = birth;
}
key::key(char A, unsigned int frequency, key* left, key* right)
{
	CHARACTER = A; priority = frequency;
	birth++;
	Left = left; Right = right;
	birth_time = birth;
}
key::~key()
{
	delete Left; delete Right;
	Left = NULL; Right = NULL;
}
void key::operator=(const key &A)
{
	CHARACTER = A.CHARACTER;
	priority = A.priority;
	birth_time = A.birth_time;
	Huffman_code = A.Huffman_code;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////  Class Compare //////////////////////////////////
bool compare::operator()(key *A, key *B)
{
	if (A->priority == B->priority)return(A->birth_time > B->birth_time);
	else return(A->priority > B->priority);
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////////// Huffman Compression ////////////////////////////////
Huffman_Compression::Huffman_Compression(string FILE_NAME)
{
	char CHAR;
	this->FILE_NAME = FILE_NAME;
	Code_Tree = NULL;
	fstream FILE(FILE_NAME, ios::in);
	if (FILE.is_open())
		while (FILE >> noskipws >> CHAR)ASCII[int(CHAR)].priority++;
	FILE.close();
}
void Huffman_Compression::Tree_formation()
{
	priority_queue< key*, vector<key*>, compare> QUEUE;
	for (int i = 0; i < 128; ++i)if (ASCII[i].priority != 0)QUEUE.push(new key(char(i), ASCII[i].priority));
	while (QUEUE.size() != 1)
	{
		key* first = QUEUE.top(); QUEUE.pop();
		key*second = QUEUE.top(); QUEUE.pop();
		QUEUE.push(new key(NULL, (first->priority + second->priority), first, second));
	}
	Code_Tree = QUEUE.top(); QUEUE.pop();
}
void Huffman_Compression::Code_Extraction(key *top, string code)
{
	if (top->CHARACTER == NULL)
	{
		Code_Extraction(top->Left, code + "0");
		Code_Extraction(top->Right, code + "1");
		top->Left = NULL; top->Right = NULL;
		delete top;
	}
	else
	{
		top->Huffman_code = code; cout << top->CHARACTER << " " << code << endl;
		ASCII[int(top->CHARACTER)] = *top;
		delete top;
	}
}
void Huffman_Compression::Compress()
{
	stringstream binary;
	string bin;
	char f_char;
	unsigned long long int nmbr = 0;
	fstream FILE(FILE_NAME, ios::in);
	fstream Compressed("Compressed.bin", ios::out | ios::binary);
	if (FILE.is_open() && Compressed.is_open())
	{
		while (FILE >> noskipws >> f_char)
		{
			nmbr++;
			binary << ASCII[int(f_char)].Huffman_code;
		}
		while (binary.str().length() % 32 != 0)binary << "0";
		bin = binary.str();
		for (int i = 0; i < 128; ++i)
		{
			Compressed.write(&ASCII[i].CHARACTER, sizeof(ASCII[i].CHARACTER));
			Compressed.write(reinterpret_cast<const char *>(&ASCII[i].priority), sizeof(ASCII[i].priority));
		}
		Compressed.write(reinterpret_cast<const char *>(&nmbr), sizeof(nmbr));
		for (int i = 0; i < bin.length() / 32; i++)
		{
			string sub = bin.substr(i * 32, 32);
			bitset<32> bits_chunk(sub);
			Compressed.write(reinterpret_cast<const char *>(&bits_chunk), sizeof(bits_chunk));
		}
		Compressed.close();
		FILE.close();
	}
}
///////////////////////////////////////////////////////////////////////////////


////////////////////// Huffman Decompression Function /////////////////////////
void Huffman_Decompress(string FILE_Name)
{
	char CHARACTER;
	unsigned int priority;
	unsigned long long int nmbr;
	fstream FILE(FILE_Name, ios::in | ios::binary);
	fstream Decompressed("Decompress.txt", ios::out);
	if (FILE.is_open() && Decompressed.is_open())
	{
		priority_queue<key*, vector<key*>, compare> QUEUE;
		for (int i = 0; i < 128; ++i)
		{
			FILE.read(&CHARACTER, sizeof(CHARACTER)); FILE.read(reinterpret_cast< char *>(&priority), sizeof(priority));
			if (priority != 0)QUEUE.push(new key(CHARACTER, priority));
		}
		FILE.read(reinterpret_cast< char *>(&nmbr), sizeof(nmbr));
		while (QUEUE.size() != 1)
		{
			key* first = QUEUE.top(); QUEUE.pop();
			key*second = QUEUE.top(); QUEUE.pop();
			QUEUE.push(new key(NULL, (first->priority + second->priority), first, second));
		}

		key *traverse;
		string code;
		stringstream wow;
		bitset<32> f_char;
		traverse = QUEUE.top();
		while (true)
		{
			if (nmbr == 0)break;
			FILE.read(reinterpret_cast<char*> (&f_char), sizeof(f_char));
			code = f_char.to_string();
			for (int i = 0; i < code.length(); ++i)
			{
				if (nmbr == 0)break;
				if (traverse->CHARACTER == NULL)
				{
					if (code[i] == '0')traverse = traverse->Left;
					else if (code[i] == '1')traverse = traverse->Right;
				}
				if (traverse->CHARACTER != NULL)
				{
					nmbr--;
					Decompressed << traverse->CHARACTER;
					traverse = QUEUE.top();
				}
			}
		}
	}
	FILE.close();
	Decompressed.close();
}
///////////////////////////////////////////////////////////////////////////////