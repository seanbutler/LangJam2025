#pragma once

#include <iostream>
#include <vector>

#include "ast.hpp"
#include "tokens.hpp"

#include "parser.hpp"

namespace mylang {

    Parser::Parser(const std::vector<Token>& tokens, std::string compilation_unit)
        : tokens_(tokens)
        , compilation_unit_(compilation_unit)  
    {

    }


    ModuleASTNode* Parser::ParseModule() 
    {
        ModuleASTNode* moduleNode = new ModuleASTNode(compilation_unit_.c_str());

        tokenItor = tokens_.begin();

        while (tokenItor != tokens_.end()) 
        {
            switch(tokenItor->type_id) 
            {
                case mylang::TypeIDs::KEYWORD:
                    ParseKeywordStatement(moduleNode);
                    break;

                case mylang::TypeIDs::IDENTIFIER:
                    ParseAssignmentStatement(moduleNode);
                    break;

                // case mylang::TypeIDs::FNAME:
                //     ParseFunctionStatement(moduleNode);
                //     break;

                default:
                    std::cout << __FILE__ << ":" << __LINE__ << " Error: Unexpected token type in module token = \'" << tokenItor->token << "\'" << std::endl;
                    tokenItor++;
                    // return nullptr;
            }
        }

        return moduleNode;
    }


    void Parser::ParseKeywordStatement(ModuleASTNode* moduleNode)
    {
        switch (tokenItor->token_id) 
        {
            case mylang::TokenIDs::KEYWORD_DECL:
                std::cout << __FILE__<<":" << __LINE__ << " Parser::ParseModule() TokenIDs::KEYWORD_DECL" << std::endl;                    
                ParseDeclaration(moduleNode);
            break;

            case mylang::TokenIDs::KEYWORD_FUNC:
                std::cout << __FILE__<<":" << __LINE__ << " Parser::ParseModule() TokenIDs::KEYWORD_FUNC" << std::endl;
                ParseFunction(moduleNode);
            break;

            default:
                std::cout << __FILE__ << ":" << __LINE__ << " Error: Unexpected token '" << tokenItor->token << "' in module" << std::endl;
        }    

        ++tokenItor;
    }


    void Parser::ParseAssignmentStatement(ModuleASTNode* parent) 
    {
        std::cout << __FILE__ << ":" << __LINE__ << " Parser::ParseAssignmentStatement()" << std::endl;

        AssignmentASTNode* assNode = new AssignmentASTNode();
        parent->children.push_back(assNode);

        // Expect an identifier
        if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected identifier after 'let'" << std::endl;
            return;
        }

        IdentifierASTNode* identifierNode = new IdentifierASTNode(tokenItor->token.c_str());

        assNode->children.emplace_back(identifierNode);
        std::cout << __FILE__ << ":" << __LINE__ << " Parsed identifier: \'" << tokenItor->token.c_str() << "\' at " << assNode->children.back() << std::endl;

        ++tokenItor;

        // Expect an assignment operator
        if (tokenItor->token_id != mylang::TokenIDs::DIGRAPH_ASSIGN) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected Assignment ':=' after identifier" << std::endl;
        }

        ++tokenItor;

        ExpressionASTNode* exprNode = ParseExpression();
        assNode->children.emplace_back(exprNode);

    }    


    void Parser::ParseDeclaration(ModuleASTNode* parent) 
    {
        std::cout << __FILE__ << ":" << __LINE__ << " Parser::ParseDeclaration()" << std::endl;

        DeclarationASTNode* declNode = new DeclarationASTNode();
        parent->children.push_back(declNode);

        ++tokenItor;

        // Expect an identifier
        if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected identifier after 'let'" << std::endl;
            return;
        }

        IdentifierASTNode* identifierNode = new IdentifierASTNode(tokenItor->token.c_str());
        declNode->children.emplace_back(identifierNode);

        std::cout << __FILE__ << ":" << __LINE__ << " Parsed identifier: \'" << tokenItor->token.c_str() << "\' at " << declNode->children.back() << std::endl;
    }

    
    ExpressionASTNode * Parser::ParseExpression() 
    {
        std::cout << __FILE__ << ":" << __LINE__ << " Parser::ParseExpression()" << std::endl;

        auto exprNode = new ExpressionASTNode();

        // WORKING HERE
        // FOR NOW AN EXPRESSIO IS JUST AN INTEGER LITERAL

        if (tokenItor->type_id != mylang::TypeIDs::INTEGER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected integer literal in expression" << std::endl;
            return nullptr;
        }

        IntegerASTNode* intNode = new IntegerASTNode(tokenItor->token.c_str());
        exprNode->children.emplace_back(intNode);

        std::cout << __FILE__ << ":" << __LINE__ << " Parsed integer literal: \'" << tokenItor->token.c_str() << "\' at " << exprNode->children.back() << std::endl;

        ++tokenItor;
        
        return exprNode;
    }


    void Parser::ParseFunction(ModuleASTNode* parent) 
    {
        return;
        
        std::cout << __FILE__ << ":" << __LINE__ << " Parser::ParseFunction()" << std::endl;

        FunctionASTNode* declNode = new FunctionASTNode();
        parent->children.push_back(declNode);

        ++tokenItor;

        // Expect an identifier
        if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected identifier after 'func'" << std::endl;
            return;
        }

        IdentifierASTNode* identifierNode = new IdentifierASTNode(tokenItor->token.c_str());
        declNode->children.emplace_back(identifierNode);

        std::cout << __FILE__ << ":" << __LINE__ << " Parsed identifier: \'" << tokenItor->token.c_str() << "\' at " << declNode->children.back() << std::endl;

        if (tokenItor->type_id != mylang::TypeIDs::SYMBOL) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \"(\" after Function Name" << std::endl;
        }

        ++tokenItor;

        // Expect Identifier List

        if (tokenItor->type_id != mylang::TypeIDs::SYMBOL) 
        {
            if (tokenItor->token != ")" ) 
            {
                std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \")\" after Parameter Identifier List" << std::endl;
            }
        }
        ++tokenItor;    // skip the begin block
        
    }


    AssignmentASTNode * Parser::ParseAssignment(ModuleASTNode* parent) 
    {
        return nullptr;

        std::cout << "Parser::ParseAssignment()" << std::endl;

        AssignmentASTNode* assNode = new AssignmentASTNode();
        parent->children.push_back(assNode);

        // Expect an identifier
        if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected identifier after 'let'" << std::endl;
            return nullptr;
        }

        IdentifierASTNode* identifierNode = new IdentifierASTNode(  tokenItor->token.c_str());
        assNode->children.emplace_back(identifierNode);
        std::cout << __FILE__ << ":" << __LINE__ << " Parsed identifier: " << assNode->children.back() << std::endl;

        ++tokenItor;

        // Expect an assignment operator
        if (tokenItor->token_id != mylang::TokenIDs::DIGRAPH_ASSIGN) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected Assignment ':=' after identifier" << std::endl;
            return nullptr;
        }
    }    

}
