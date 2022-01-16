#pragma once
#include <iostream>
#include <fstream>
#include "BitBuf.h"
using namespace std;

class CFileBuf : CBitBuf
{
public:
	CFileBuf(unsigned uiBitBufSize, string sOutFile, int iDictLen, int iLookLen);
	~CFileBuf();

	void				makeCodeWord(char* pBuf, int iMtchPos, int iMtchLen);
	unsigned			GetBytes();
	int					GetNumCdWrds();

protected:
	int					FlushBuffer();

private:
	ofstream			_outFileStream;
	int					_iBitsForPos;				// Num of bits in code word to store position where matching occurs in dictonary
	int					_iBitsForMatchingLength;	// Num of bits in code word to store length of the matching
	int					_iGeneratedCodeWords;		// Num of generated code words
	int					_iLookLen;
};

