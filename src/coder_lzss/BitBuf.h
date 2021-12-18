#pragma once

class CBitBuf
{
public:
	CBitBuf(unsigned uiSize);
	~CBitBuf();

	unsigned			WriteBits(unsigned uiVal, unsigned uiLen);
	
protected:
	virtual int			FlushBuffer();
	unsigned*			GetBuf();
	unsigned			GetSize();
	const unsigned&		GetCurr();
	unsigned			GetLeft();
	unsigned			GetPos();

private:
	unsigned*			_puiBuf;			// mem buffer
	unsigned			_uiPos;				// buffer position to place word
	unsigned			_uiSize;			// buffer size in uints

	unsigned			_uiCurr;			// current 32 bits word to write to buffer 
	unsigned			_uiLeft;			// number of bits left in current 32 bits word
};

