#pragma once

#include <iostream>
#include <vector>

#include "ast.hpp"
#include "tokens.hpp"


namespace mylang {


    class Parser {
    public:

        std::vector<Token> tokens_;
        std::vector<Token>::iterator tokenItor;
        std::shared_ptr<AST> abstractSyntaxTree;
        size_t position_;



        Parser(const std::vector<Token>& tokens)
            : tokens_(tokens)
            , position_(0) 
            {

            }


        ExpressionASTNode* ParseExpression() 
        {
            std::cout << "Parser::ParseExpression()" << std::endl;

            // Placeholder for expression parsing logic
            auto exprNode = new ExpressionASTNode();
            return exprNode;
        }


        DeclarationASTNode * ParseDeclaration(ModuleASTNode* parent) 
        {
            std::cout << "Parser::ParseDeclaration()" << std::endl;

            DeclarationASTNode* declNode = new DeclarationASTNode();
            parent->children.push_back(declNode);

            // Advance past 'let' keyword
            ++tokenItor;

            // Expect an identifier
            if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
            {
                std::cerr << "Error: Expected identifier after 'let'" << std::endl;
                return nullptr;
            }

            declNode->children.emplace_back(new Token(*tokenItor));
            std::cout << "Parsed identifier: " << declNode->children.back() << std::endl;

            // Advance past identifier
            ++tokenItor;

            // Expect ':=' operator
            if (tokenItor->token_id != mylang::TokenIDs::DIGRAPH_ASSIGN) 
            {
                std::cerr << "Error: Expected ':=' after identifier" << std::endl;
                return nullptr;
            }

            // Advance past ':='
            ++tokenItor;

            // Next Parse an expression
            // declNode->expression = ParseExpression(); // Placeholder

            return declNode;
        }




    
        ModuleASTNode* ParseModule() 
        {
            ModuleASTNode* moduleNode = new ModuleASTNode();

            tokenItor = tokens_.begin();

            while (tokenItor != tokens_.end()) 
            {

                switch (tokenItor->token_id) 
                {
                    case mylang::TokenIDs::KEYWORD_LET:
                        std::cout << "Parser::ParseModule() TokenIDs::KEYWORD_LET" << std::endl;                    
                        moduleNode->children.push_back(ParseDeclaration(moduleNode));
                        break;

                    // case mylang::TokenIDs::KEYWORD_FUNC:
                    //     std::cout << "Parser::ParseModule() TokenIDs::KEYWORD_FUNC" << std::endl;                    
                    //     moduleNode->children.push_back(ParseDeclaration(moduleNode));
                    //     break;

                    default:
                        std::cerr << "Error: Unexpected token '" << tokenItor->token << "' in module" << std::endl;
                        return nullptr;
                ++tokenItor;
            }

            
            return moduleNode;
    
        }



    }

};



}
