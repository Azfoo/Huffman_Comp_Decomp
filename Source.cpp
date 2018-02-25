
#include<iostream>
#include<string>
#include"Huffman.h"
using namespace std;


int main()
{
	string code, File_name;
	cout << "Enter File/Path to be compressed: "; getline(cin, File_name);
	Huffman_Compression alpha(File_name);
	alpha.Tree_formation();
	alpha.Code_Extraction(alpha.Code_Tree,code);
	alpha.Compress();
	cout << "Enter File/Path to be Decompressed: "; getline(cin, File_name);
	Huffman_Decompress(File_name);
}