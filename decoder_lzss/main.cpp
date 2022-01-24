#include <iostream>
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include "Decoder.hpp"

using namespace std;
using namespace chrono;

void appendDecodeStats(
    string outFileName,
    int decodeTime
);


int main(int argc, char** argv)
{
    /* decoder CODED_FILE DECODED_FILE */

    string sFileInPath = "out_data.bin";
    string sFileOutPath = "decoded_data.bin";

    if (argc > 1)
        sFileInPath = argv[1];
    
    if (argc > 2)
        sFileOutPath = argv[2];


    ifstream inFile(sFileInPath.data(), ios::in | ios::binary);

    if( !inFile.is_open() ) 
    {
        cout << "ERROR, given coded file does not exist!" << endl;
        cout << "Usage: " << argv[0] << " CODED_FILE DECODED_FILE" << endl;
        return 1;
    }

    ofstream outFile(sFileOutPath.data(), ios::out | ios::binary);

    if( !outFile.is_open() ) 
    {
        cout << "ERROR, cannot write specified decoded file!" << endl;
        cout << "Usage: " << argv[0] << " CODED_FILE DECODED_FILE" << endl;
        return 1;
    }

    auto start = high_resolution_clock::now();

    /* initialize dictionary */
    auto decoder = Decoder(&inFile, &outFile);

    while (!decoder.finished())
    {
        decoder.decodeNext();
    }

    inFile.close();
    outFile.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    if (argc > 3)
	appendDecodeStats(argv[3], duration.count());

    return 0;
}


void appendDecodeStats(
    string outFileName,
    int decodeTime
) {
    ofstream outFile(outFileName.data(), ios::out | ios::app);
    if (!outFile.is_open()) {
        return;
    }
    outFile << "Decode time: " << decodeTime << " ms" << endl;
    outFile << endl;

    outFile.close();
}
