// Coder_LZSS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <chrono>
#include "FileBuf.h"

using namespace std;
using namespace chrono;

bool readFromFile(string sFilePath, char* bufer, int bytesToRead);
void makeStatsToFile(string sFilePath, int exeTime, int inFileSize,
    int outFileSize, int GenCdWrds, int dictS, int lkLen);

int main(int argc, char** argv)
{
    string sFileInPath = "in_data.bin";
    string sFileOutPath = "out_data.bin";
    int iDictLen = 1024, iLookLen = 32; // default symbol sizes of dictonary and look ahead buffer
    // read args from command line
    // 1st arg - InFile, 2nd - outFile, 3rd - Dict len, 4th - look len
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
        // dictonary should be bigger than look ahead buf
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
        //cout << "Size of file: " << iFileSize << " bytes\n";
    }
    else {
        cout << "Failed to get file size\n";
        exit(1);
    }

    CFileBuf  fileBuff(32, sFileOutPath, iDictLen, iLookLen); // creating an object to handle wtriting bits to file
    char* inBuffer = new char[results.st_size]; // buffer to store file byte data

    if( readFromFile(sFileInPath, inBuffer, iFileSize) ) 
    {
        auto start = high_resolution_clock::now();

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
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        // Wrtite coding stats to file
        makeStatsToFile(sFileOutPath, duration.count(), iFileSize, fileBuff.GetBytes(),
            fileBuff.GetNumCdWrds(), iDictLen, iLookLen);
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

void makeStatsToFile(string sFilePath, int exeTime, int inFileSize,
    int outFileSize, int GenCdWrds, int dictS, int lkLen ) 
{
    
    float cr = ((float)(inFileSize) / outFileSize); // compression ratio
    float avgBitLen = 0.0;
    //avgBitLen = 8 * ((float)outFileSize / GenCdWrds);
    avgBitLen = 8 / cr;
    // prepare out file name
    size_t pos = sFilePath.find(".");
    if (pos != std::string::npos)
    {
        sFilePath.erase(pos, sFilePath.length() - pos);
    }
    string sFileNm = sFilePath;
    sFilePath += "_stats.txt";
    // write stats to file
    ofstream outFile(sFilePath.data(), ios::out | ios::app);
    if (!outFile.is_open()) {
        return;
    }
    outFile << "Dictonary size: " << dictS << endl;
    outFile << "Look ahead buffer size: " << lkLen << endl;
    outFile << "Execution time: " << exeTime << " ms" << endl;
    outFile << "Input file size: " << inFileSize << " bytes" << endl;
    outFile << "Output file size: " << outFileSize << " bytes" << endl;
    outFile << "Compression ratio: " << cr << endl;
    outFile << "Number of code words: " << GenCdWrds << endl;
    outFile << "Average code length in bits: " << avgBitLen << endl;
    outFile << endl;

    outFile.close();

    // Csv file for tests
    pos = sFileNm.find("\\");
    if (pos != std::string::npos)
    {
        sFileNm.erase(0, pos+1);
    }
    string sCsvFilePath = "out/Stats.csv";
    outFile.open(sCsvFilePath.data(), ios::out | ios::app);
    if (!outFile.is_open()) {
        return;
    }
    outFile << sFileNm << ";" << dictS << ";" << lkLen << ";" << exeTime << ";"
        << inFileSize << ";" << outFileSize << ";" << cr << ";" << GenCdWrds << ";" << avgBitLen << ";" << "\n";
    outFile.close();
}
