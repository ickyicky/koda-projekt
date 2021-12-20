#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "Decoder.hpp"

using namespace std;


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

    /* initialize dictionary */
    auto decoder = Decoder(&inFile, &outFile);

    while (!decoder.finished())
    {
        decoder.decodeNext();
    }

    inFile.close();
    outFile.close();

    return 0;
}
