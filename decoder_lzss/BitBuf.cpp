#include "BitBuf.hpp"


BitBuf::BitBuf(ifstream* buffer) : buffer(buffer), finished(false), reachedFileEnd(false)
{
    readWord();
}

unsigned BitBuf::read(unsigned len, unsigned& read)
{
    unsigned result = 0, i;
    /* read bit by bit for easier implementation */
    for (i = 0; i < len; i++)
    {
        result |= ((0x01 & currWord) << i);
        currWord = (currWord >> 1);
        currByte++;

        /* if we reached last bit, read new bytes */
        if (currByte == totalBytes)
        {
            if (reachedFileEnd) {
                finished = true; i++;
                break;
            }
            readWord();
        }
    }
    read = i;
    return result;
}

bool BitBuf::hasAtLeastWord() 
{
    return !reachedFileEnd || !currByte;
}

void BitBuf::readWord() 
{
    currByte = 0;
    buffer->read(&currWord, 1);
    if (buffer->peek() == EOF)
        reachedFileEnd = true;
}
