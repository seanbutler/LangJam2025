
#include <iostream>
#include <fstream>

#include "tokeniser.hpp"

int main(int argc, char** argv) {

        std::printf("Starting lang.exe\n");

    if ( argc > 1 ) {

        Tokeniser tokeniser;

        if (!tokeniser.tokenise(argv[1], "main"))
        {
            std::cerr << "Failed to read file: " << argv[1] << "\n";
            return 1;
        }

        tokeniser.printTokens();

        return 0;
    }

    return 0;
}