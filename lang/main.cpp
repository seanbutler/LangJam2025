
#include <iostream>
#include <fstream>

#include "tokeniser.hpp"
#include "parser.hpp"
// #include "ast.hpp"

int main(int argc, char** argv) {

    std::printf("Starting lang.exe\n");

    if ( argc > 1 ) {

        Tokeniser tokeniser;

        if (false == tokeniser.tokenise(argv[1], "main"))
        {
            std::cerr << "Failed to read file: " << argv[1] << "\n";

            return 1;
        }

        // tokeniser.printTokens();


        // mylang::Parser parser(tokeniser.tokens());

        // ModuleASTNode * ast = nullptr;
        // ast = parser.ParseModule();
    }

    return 0;
}