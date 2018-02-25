#pragma once
#include<iostream>
#include<string>
using namespace std;


//////////Container/Data_Structure to Hold the information///////////////////
////////// regarding Data scanned on run-time form source////////////////////

class key
{
private:
	// To prioritize objects on the basis of their 
	// birth/creation sequence
	static unsigned int birth;
public:
	char CHARACTER; // Original ASCII Data Scanned from file
	unsigned int priority; // Depends on frequency
	unsigned int birth_time;
	string Huffman_code; // Compressed code from Huffman tree
 	key *Left;
	key *Right;
	key(char A, unsigned int frequency, key* left = NULL, key* right = NULL);
	key();
	~key();
	void operator=(const key &A);
};
///////////////////////////////////////////////////////////////////////////////


///////////////////////Class for STL'S Priority Queue/////////////////////////
class compare
{
public:
	bool operator()(key *A, key *B);
};
//////////////////////////////////////////////////////////////////////////////


////////////////////////// Huffman Compression ////////////////////////////////
class Huffman_Compression
{
	string FILE_NAME; // File to be compressed
	key *ASCII = new key[128]; // array indices refered as the ASCII characters

public:
	key *Code_Tree; // Huffman Tree root
	Huffman_Compression(string FILE_NAME);
	void Tree_formation();
	void Code_Extraction(key *top, string code);
	void Compress();
};
///////////////////////////////////////////////////////////////////////////////

///////////////////// Huffman Decompression Function //////////////////////////
void Huffman_Decompress(string FILE_Name);
///////////////////////////////////////////////////////////////////////////////