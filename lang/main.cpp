
#include <iostream>
#include <fstream>

#include "tokeniser.hpp"
#include "parser.hpp"
// #include "ast.hpp"

int main(int argc, char** argv) {

    std::printf("Starting lang.exe\n");

    if ( argc > 1 ) 
    {
        Tokeniser tokeniser;

        if (false == tokeniser.tokenise(argv[1], "main"))
        {
            std::cerr << "Failed to read file: \'" << argv[1] << "\'\n";
            return 1;
        }

        tokeniser.printTokens();

        mylang::Parser parser(tokeniser.tokens(), argv[1]);

        ModuleASTNode * ast = nullptr;
        ast = parser.ParseModule();
        

        std::ofstream diagramFile("ast_diagram.gv");
        diagramFile << "digraph G {" <<  std::endl;
        diagramFile << "node [shape = rectangle];" << std::endl;
        ast->Diagram(diagramFile);
        diagramFile << "}" << std::endl;
        diagramFile.close();


        ast->print();


    }

    return 0;
}