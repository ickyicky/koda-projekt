// Coder_LZSS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "FileBuf.h"

using namespace std;

bool readFromFile(string sFilePath, char* bufer, int bytesToRead);

int main(int argc, char** argv)
{
    string sFileInPath = "in_data.bin";
    string sFileOutPath = "out_data.bin";
    int iDictLen = 1024, iLookLen = 32; // default symbol sizes of dictonary and look ahead buffer
    // read args from command line
    // 1st arg - InDile, 2nd - outFile, 3rd - Dict len, 4th - look len
    if( argc > 4 ) {
        sFileInPath = argv[1];
        sFileOutPath = argv[2];
        iDictLen = atoi(argv[3]);
        iLookLen = atoi(argv[4]);
        // dictonary should be bigger than look ahead buffer
        if( iLookLen > iDictLen )
        {
            int temp = iLookLen;
            iLookLen = iDictLen;
            iDictLen = temp;
        }
    }
    else if( argc > 3)
    {
        sFileInPath = argv[1];
        sFileOutPath = argv[2];
        iDictLen = atoi(argv[3]);
        // dictonary should be biiger than look ahead buf
        if( iLookLen > iDictLen )
        {
            int temp = iLookLen;
            iLookLen = iDictLen;
            iDictLen = temp;
        }
    }
    else if( argc > 2 ) {
        sFileInPath = argv[1];
        sFileOutPath = argv[2];
    }
    else if( argc > 1 ) {
        sFileInPath = argv[1];
    }
    int iFileSize = 0;
    struct stat results;
    // get file size in bytes
    if( stat(sFileInPath.data(), &results) == 0 ) {
        iFileSize = results.st_size;
        cout << "Size of file: " << iFileSize << " bytes\n";
    }
    else {
        cout << "Failed to get file size\n";
        exit(1);
    }
    CFileBuf  fileBuff(32, sFileOutPath, iDictLen, iLookLen); // creating an object to handle wtriting bits to file
    char* inBuffer = new char[results.st_size]; // buffer to store file byte data
    if( readFromFile(sFileInPath, inBuffer, iFileSize) ) 
    {
        // At start code one symbol to init dictionary
        char* pDctStart = inBuffer; // pointer to dictionary start positon.
        int iDictCrtLen = 1; // Current number of elements placed in dictonary
        fileBuff.makeCodeWord(pDctStart, 0, 0);
        
        int iMtchLen, iLenNew, iMtchPos, iShiftPos, iLookAhs;
        while( (pDctStart + iDictCrtLen) != &inBuffer[iFileSize] ) {

            // Search for longest match
            iMtchLen = 0; iLenNew = 0; iMtchPos = 0; iShiftPos = 0;

            for( int i = 0; i < iDictCrtLen; i++ )
            {
                if( pDctStart[i] != pDctStart[iDictCrtLen] ) {
                    continue;
                }
                iLenNew = 1; 
                iLookAhs = min((long)iLookLen, iFileSize - iDictCrtLen - (inBuffer - pDctStart));
                for( int k = i + 1; k < iDictCrtLen && iLenNew < iLookAhs; k++ )
                {
                    if( pDctStart[k] == pDctStart[iDictCrtLen + iLenNew] ) {
                        iLenNew++;
                    }
                    else {
                        break;
                    }
                }
                if( iLenNew > iMtchLen ) {
                    iMtchLen = iLenNew; iMtchPos = i;
                }
            }
            // make code word
            fileBuff.makeCodeWord((pDctStart + iDictCrtLen), iMtchPos, iMtchLen);
           
            // update dictonary params - current lenght and start pointer
            if( iMtchLen > 0 )
            {
                iDictCrtLen += iMtchLen;
            }
            else {
                iDictCrtLen++;
            }
            if( iDictCrtLen > iDictLen ) {
                iShiftPos = iDictCrtLen - iDictLen; // shift dictionary start pointer by the given number of positions
                pDctStart = (pDctStart + iShiftPos);
            }
            iDictCrtLen = min(iDictCrtLen, iDictLen); // compare to dictonary max size

        }
    }
    delete[] inBuffer;
}


bool readFromFile(string sFilePath, char* buffer, int bytesToRead) {  

    ifstream inFile(sFilePath.data(), ios::in | ios::binary);
    if( inFile.is_open() ) 
    {
        inFile.read(buffer, bytesToRead);
        if( !inFile ) 
        {
            cout << "An error occurred during file read!\n";
            cout << "the number of bytes read: " << inFile.gcount() << "\n";
            // calling myFile.clear() will reset the stream state
            // so it is usable again.
            return false;
        }
        else
            return true;
    }
    else 
    {
        cout << "Failed to open specified file\n";
        return false;
    }
    inFile.close();
}