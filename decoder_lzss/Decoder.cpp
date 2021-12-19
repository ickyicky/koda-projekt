#include "Decoder.hpp"
#include <math.h>


Decoder::Decoder(ifstream* ibuffer, ofstream* output): output(output)
{
	/* initialize bit buffer */
	inputBuffer = new BitBuf(ibuffer);
	/* read dict size and look size */
	size = inputBuffer->read(16);
	lookSize = inputBuffer->read(16);
	dictStart = size;

	/* calculate number of bits for position and matching length */
	_iBitsForPos = (unsigned short)ceil(log2(size));
	_iBitsForMatchingLength = (unsigned short)ceil(log2(lookSize));

	/* create empty dictionary */
	dictionary = new char[size];
	for (unsigned short i = 0; i < size; i++)
		dictionary[i] = 0;
}

Decoder::~Decoder() 
{
	delete inputBuffer;
	delete[] dictionary;
}

bool Decoder::finished() 
{
	return inputBuffer->finished;
}

void Decoder::decodeNext()
{
	/* read record type */
	unsigned recordType = inputBuffer->read(1);
	char* toWrite;
	unsigned length;

	if (recordType == 1)
	{
		/* read position and count of chars from dictionary */
		unsigned position = inputBuffer->read(_iBitsForPos);
		length = inputBuffer->read(_iBitsForMatchingLength);
		if (length == 0)
			length = lookSize;

		cout << "( bit:1, offset:" << position << ", len:" << length << " )\n";
		toWrite = new char[length];

		for (unsigned short i = 0; i < length; i++)
			toWrite[i] = dictionary[position + i + dictStart];
	}
	else if (recordType == 0)
	{
		/* read new char */
		toWrite = new char[1];
		toWrite[0] = (char)inputBuffer->read(8);
		length = 1;
		cout << "( bit:0, chars:" << +toWrite[0] << " )\n";
	}

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
		dictStart -= length;
		if (dictStart < 0)
			dictStart = 0;
	}
}