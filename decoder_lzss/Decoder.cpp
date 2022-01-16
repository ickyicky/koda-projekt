#include "Decoder.hpp"
#include <math.h>
#include "stdio.h"


Decoder::Decoder(ifstream* ibuffer, ofstream* output): output(output)
{
	/* initialize bit buffer */
	unsigned read;
	inputBuffer = new BitBuf(ibuffer);
	/* read dict size and look size */
	size = inputBuffer->read(16, read);
	lookSize = inputBuffer->read(16, read);
	dictStart = size;

	/* calculate number of bits for position and matching length */
	_iBitsForPos = (unsigned short)ceil(log2(size));
	_iBitsForMatchingLength = (unsigned short)ceil(log2(lookSize));

	/* create empty dictionary */
	dictionary = new char[size];
	for (unsigned short i = 0; i < size; i++)
		dictionary[i] = 0;

	toWrite = new char[size];
}

Decoder::~Decoder() 
{
	delete inputBuffer;
	delete[] dictionary;
	delete[] toWrite;
}

bool Decoder::finished() 
{
	return inputBuffer->finished;
}
/* DEBUG */
unsigned dbg_uiCnt = 0;

void Decoder::decodeNext()
{
	/* read record type */
	unsigned length{}, read;
	unsigned recordType = inputBuffer->read(1, read);
	//char* toWrite {nullptr};

	if (recordType == 1)
	{
		/* read position and count of chars from dictionary */
		unsigned position = inputBuffer->read(_iBitsForPos, read);
		if (inputBuffer->finished && read < _iBitsForPos)
			return;
		length = inputBuffer->read(_iBitsForMatchingLength, read);
		if (inputBuffer->finished && read < _iBitsForMatchingLength)
			return;
		if (length == 0)
			length = lookSize;

		//DEBUG
		printf("%5d: bit:1, offset: %u, len: %u\n", dbg_uiCnt, position, length);

		//toWrite = new char[length];

		for (unsigned short i = 0; i < length; i++)
			toWrite[i] = dictionary[position + i + dictStart];
	}
	else if (recordType == 0)
	{
		/* read new char */
		//toWrite = new char[1];
		toWrite[0] = (char)inputBuffer->read(8, read);
		length = 1;
		//DEBUG
		printf("%5d: bit:0, chars: %c %02x\n", dbg_uiCnt, toWrite[0], (unsigned) (unsigned char) toWrite[0]);
		if (!inputBuffer->hasAtLeastWord()) {
			output->write(toWrite, length);
			inputBuffer->finished = true;
			return;
		}
	}
	dbg_uiCnt++;
	/* write decoded data */
	output->write(toWrite, length);

	/* shift dictionary */
	for (unsigned short i = 0; i < size - length; i++)
	{
		dictionary[i] = dictionary[i + length];
	}
	
	/* write new chars to dictionary */
	for (unsigned short i = 0; i < length; i++)
		dictionary[size - length + i] = toWrite[i];

	/* update dict start position */
	if (dictStart > 0)
	{
		if( dictStart >= length )
			dictStart -= length;
		else
			dictStart = 0;
	}
}