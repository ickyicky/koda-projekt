#include <iostream>
#include <fstream>
#pragma once

using namespace std;


class BitBuf
{
    public:
        BitBuf(ifstream* buffer);
		unsigned read(unsigned short len);
		bool finished;
		bool hasAtLeastWord();
    private:
		char currWord;
		unsigned short currByte;
		const static unsigned short totalBytes = 8;	
		ifstream* buffer;
		void readWord();
		bool reachedFileEnd;
};
