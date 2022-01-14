#include "BitBuf.hpp"
#include <iostream>
#pragma once

using namespace std;


class Decoder
{
    public:
        Decoder(ifstream* ibuffer, ofstream* output);
        void decodeNext();
        ~Decoder();
		bool finished();
    private:
        unsigned short dictStart;
        unsigned short size;
        unsigned short lookSize;
        unsigned short _iBitsForPos;
        unsigned short _iBitsForMatchingLength;
        char* dictionary;
	char* toWrite;
        BitBuf* inputBuffer;
        ofstream* output;
};
