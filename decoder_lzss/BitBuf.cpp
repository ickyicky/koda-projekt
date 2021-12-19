#include "BitBuf.hpp"


BitBuf::BitBuf(ifstream* buffer) : buffer(buffer), finished(false), reachedFileEnd(false)
{
    readWord();
}

unsigned BitBuf::read(unsigned short len) 
{
    unsigned result = 0;
    /* read bit by bit for easier implementation */
    for (unsigned short i = 0; i < len; i++)
    {
        result |= ((0x01 & currWord) << i);
        currWord = (currWord >> 1);
        currByte++;

        /* if we reached last bit, read new bytes */
        if (currByte == totalBytes)
            if (reachedFileEnd)
                finished = true;
            else
                readWord();
    }

    return result;
}

void BitBuf::readWord() 
{
    currByte = 0;
    buffer->read(&currWord, 1);
    if (buffer->peek() == EOF)
        reachedFileEnd = true;
}
