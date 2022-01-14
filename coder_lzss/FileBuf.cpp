#include "FileBuf.h"
#include <math.h>

CFileBuf::CFileBuf(unsigned uiBitBufSize, string sOutFile, int iDictLen, int iLookLen) 
	: CBitBuf(uiBitBufSize) 
{
	_outFileStream.open(sOutFile, ios::out | ios::binary);
	_iBitsForPos = (int)ceil(log2(iDictLen));
	_iBitsForMatchingLength = (int)ceil(log2(iLookLen));
	_iLookLen = iLookLen;
    // write info about ditionary and look ahead buffer in 16 bits
    // this will be stored at first 32 bits of out file and be used by decoder
    WriteBits(iDictLen, 16);
    WriteBits(iLookLen, 16);
}

CFileBuf::~CFileBuf() 
{
	// write down bits left in bits buffor
	unsigned	uiLeft = GetLeft();
	_outFileStream.write((char*) GetBuf(), GetPos() * sizeof(unsigned));
	uiLeft = uiLeft / 8 + (uiLeft % 8 ? 1 : 0);
	_outFileStream.write((char*) &GetCurr(), sizeof(unsigned) - uiLeft);
	_outFileStream.close();
}

/// <summary>
/// Writes down bits buffer to file when buffer is full
/// </summary>
/// <returns></returns>
int CFileBuf::FlushBuffer()
{
	_outFileStream.write((char*)GetBuf(), GetSize() * sizeof(unsigned));
	CBitBuf::FlushBuffer();
	return 0;
}

//DEBUG
unsigned dbg_uiCnt = 0;

/// <summary>
/// Create code word that is a bits squance stored in bits buffer
/// </summary>
/// <param name="pBuf">Pointer to the data to be encoded</param>
/// <param name="iMtchPos">Position index in dictorany where occurs longest string match</param>
/// <param name="iMtchLen">Number of matching symbols in dictionary</param>
void CFileBuf::makeCodeWord(char* pBuf, int iMtchPos, int iMtchLen)
{
	if( iMtchLen > 1 ) 
	{
		// if we have phrase matching
		WriteBits(1, 1);
		WriteBits(iMtchPos, _iBitsForPos);
		if (iMtchLen >= _iLookLen) {
			iMtchLen = 0;
		}
		WriteBits(iMtchLen, _iBitsForMatchingLength);
		//DEBUG
		printf("%5d: bit:1, offset: %u, len: %u\n", dbg_uiCnt++, iMtchPos, iMtchLen);

	}
	else
	{
		// if we have short word - 1 match or match no found
		WriteBits(0, 1);
		WriteBits((unsigned)(unsigned char) *pBuf, 8);
		//DEBUG
		printf("%5d: bit:0, chars: %c %02x\n", dbg_uiCnt++, *pBuf, (unsigned)(unsigned char) *pBuf);
	}
}