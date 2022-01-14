#include "BitBuf.h"

CBitBuf::CBitBuf(unsigned uiSize)
{
	_puiBuf = new unsigned[_uiSize = uiSize];
	_uiPos = 0;
    _uiCurr= 0;
    _uiLeft= 32;
    _uiBytes = 0;
}

CBitBuf::~CBitBuf()
{
	delete _puiBuf;
}

unsigned CBitBuf::WriteBits(unsigned uiVal, unsigned uiLen)
{
    if( uiLen < _uiLeft )
    {
        _uiCurr |= uiVal << (32 - _uiLeft);
        _uiLeft -= uiLen;
    }
    else
    {
        unsigned full_bits = uiLen - _uiLeft;                       // number of bits we need to write to another 32 bits word
        unsigned towrite = _uiCurr | (uiVal << (32 - _uiLeft));     // write bits to space left in current word
        _uiCurr = full_bits ? (uiVal >> _uiLeft) : 0;               // set new word because previous go to buffer
        _puiBuf[_uiPos++] =  towrite;                               // push to buufer 32 bit word
        _uiLeft = 32 - full_bits;
        if( _uiPos == _uiSize ) {
            FlushBuffer();
        }
    }
    
    return 0;
}

int CBitBuf::FlushBuffer()
{
    _uiBytes += _uiPos * 4;
    _uiPos = 0;
    return 0;
}

unsigned* CBitBuf::GetBuf()
{
    return _puiBuf;
}

unsigned CBitBuf::GetSize() 
{
    return _uiSize;
}

const unsigned& CBitBuf::GetCurr()
{
    return _uiCurr;
}

unsigned CBitBuf::GetLeft()
{
    return _uiLeft;
}

unsigned CBitBuf::GetPos()
{
    return _uiPos;
}