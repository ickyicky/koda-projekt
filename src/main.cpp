#include <iostream>

#include "lzss.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
        {
            std::cout << "Usage: koda compress|decompress FILE\n";
            return 1;
        }

    return 0;
}
