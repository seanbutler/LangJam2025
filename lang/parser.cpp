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
            ParseStatement(moduleNode);
        }

        return moduleNode;
    }


    void Parser::ParseStatement(ASTNode* parent) 
    {
        switch(tokenItor->type_id) 
        {
            case mylang::TypeIDs::KEYWORD:
                ParseKeywordStatement(parent);
                break;

            case mylang::TypeIDs::IDENTIFIER:
                ParseAssignmentStatement(parent);
                break;

            case mylang::TypeIDs::SYMBOL:
                if ( tokenItor->token_id == mylang::TokenIDs::SYM_BEGIN_BLOCK )
                {
                    ParseScope(parent);
                }
                break;

            default:
                std::cout << __FILE__ << ":" << __LINE__ << " Error: Unexpected token type in module token = \'" << tokenItor->token << "\'" << std::endl;
                tokenItor++;
        }
    }


    void Parser::ParseScope(ASTNode* parent) 
    {
        ScopeASTNode* scopeNode = new ScopeASTNode();
        parent->children.push_back(scopeNode);
        
        while (tokenItor != tokens_.end() 
                && tokenItor->token_id != mylang::TokenIDs::SYM_END_BLOCK ) 
        {
            ParseStatement(scopeNode);
        }

    }


    void Parser::ParseKeywordStatement(ASTNode* parent)
    {
        switch (tokenItor->token_id) 
        {
            case mylang::TokenIDs::KEYWORD_DECL:
                std::cout << __FILE__<<":" << __LINE__ << " Parser::ParseModule() TokenIDs::KEYWORD_DECL" << std::endl;                    
                ParseDeclaration(parent);
            break;

            case mylang::TokenIDs::KEYWORD_FUNC:
                std::cout << __FILE__<<":" << __LINE__ << " Parser::ParseModule() TokenIDs::KEYWORD_FUNC" << std::endl;
                ParseFunction(parent);
            break;

            case mylang::TokenIDs::KEYWORD_IF:
                std::cout << __FILE__<<":" << __LINE__ << " Parser::ParseModule() TokenIDs::KEYWORD_IF" << std::endl;
                ParseCondition(parent);
            break;

            default:
                std::cout << __FILE__ << ":" << __LINE__ << " Error: Unexpected token '" << tokenItor->token << "' in module" << std::endl;
        }    

        ++tokenItor;
    }


    void Parser::ParseAssignmentStatement(ASTNode* parent) 
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

        ParseExpression(assNode);
    }    


    void Parser::ParseDeclaration(ASTNode* parent) 
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

    
    void Parser::ParseExpression(ASTNode* parent) 
    {
        std::cout << __FILE__ << ":" << __LINE__ << " Parser::ParseExpression()" << std::endl;

        auto exprNode = new ExpressionASTNode();
        parent->children.emplace_back(exprNode);

        // WORKING HERE
        // FOR NOW AN EXPRESSION IS JUST AN INTEGER LITERAL

        if (tokenItor->type_id != mylang::TypeIDs::INTEGER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected integer literal in expression" << std::endl;
        }

        ValueASTNode* intNode = new ValueASTNode("Integer", tokenItor->token.c_str());
        exprNode->children.emplace_back(intNode);

        std::cout << __FILE__ << ":" << __LINE__ << " Parsed integer literal: \'" << tokenItor->token.c_str() << "\' at " << exprNode->children.back() << std::endl;

        ++tokenItor;
    }


    void Parser::ParseFunction(ASTNode* parent) 
    {
        std::cout << __FILE__ << ":" << __LINE__ << " Parser::ParseFunction()" << std::endl;

        if ( tokenItor->token_id != mylang::TokenIDs::KEYWORD_FUNC) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected 'func' keyword to start function declaration" << std::endl;
            return;
        }

        ++tokenItor;

        // Expect an identifier
        if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected identifier after 'func' got \"" << tokenItor->token << "\"" << std::endl;
            return;
        }

        FunctionASTNode* funDeclNode = new FunctionASTNode();
        parent->children.push_back(funDeclNode);

        IdentifierASTNode* identifierNode = new IdentifierASTNode(tokenItor->token.c_str());
        funDeclNode->children.emplace_back(identifierNode);

        std::cout << __FILE__ << ":" << __LINE__ << " Parsed identifier: \'" << tokenItor->token.c_str() << "\' at " << funDeclNode->children.back() << std::endl;
        ++tokenItor;

        if (tokenItor->token_id != mylang::TokenIDs::SYM_LEFT_PAREN) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \"(\" after Function Identifier" << std::endl;
        }

        ++tokenItor;

        ParameterListASTNode * paramListNode = new ParameterListASTNode ();
        uint8_t params = 0;

        while ( tokenItor->token_id != mylang::TokenIDs::SYM_RIGHT_PAREN )
        {
            // Expect an identifier
            if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
            {
                std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected Identifier in Parameter List got \"" << tokenItor->token << "\"" << std::endl;
                return;
            }

            IdentifierASTNode* paramIdentifierNode = new IdentifierASTNode(tokenItor->token.c_str());
            paramListNode->children.emplace_back(paramIdentifierNode);
            params++;  

            std::cout << __FILE__ << ":" << __LINE__ << " Parsed Parameter identifier: \'" << tokenItor->token.c_str() << "\' at " << funDeclNode->children.back() << std::endl;

            ++tokenItor;

            if ( tokenItor->token_id == mylang::TokenIDs::SYM_COMMA ) 
            {
                ++tokenItor;    // skip the comma
            }
        }

        if (params>0) 
        {
            funDeclNode->children.push_back(paramListNode);
        }

        if (tokenItor->token_id != mylang::TokenIDs::SYM_RIGHT_PAREN) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \")\" after Parameter Identifier List" << std::endl;
        }

        ++tokenItor;        // Skip the ')'

        if (tokenItor->token_id != mylang::TokenIDs::SYM_BEGIN_BLOCK) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \"{\" to begin Function Body" << std::endl;
        }
        ++tokenItor;        // Skip the '{'

        ParseScope(funDeclNode);
        
        if (tokenItor->token_id != mylang::TokenIDs::SYM_END_BLOCK) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \"}\" to end Function Body" << std::endl;
        }
    }


    void Parser::ParseCondition(ASTNode* parent) 
    {
        std::cout << __FILE__ << ":" << __LINE__ << " Parser::ParseCondition()" << std::endl;

        if ( tokenItor->token_id != mylang::TokenIDs::KEYWORD_IF) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected 'if' keyword to start condition" << std::endl;
        }

        ++tokenItor;

        ConditionalASTNode* condNode = new ConditionalASTNode();
        parent->children.push_back(condNode);

        if (tokenItor->token_id != mylang::TokenIDs::SYM_LEFT_PAREN) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \"(\" after Conditional Keyword" << std::endl;
        }
        ++tokenItor;


        // Parse the condition expression
        ParseExpression(condNode);


        if (tokenItor->token_id != mylang::TokenIDs::SYM_RIGHT_PAREN) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \")\" after Conditional Expression" << std::endl;
        }
        ++tokenItor;


        if (tokenItor->token_id != mylang::TokenIDs::SYM_BEGIN_BLOCK) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \"{\" to begin Conditional Body" << std::endl;
        }
        ++tokenItor;        // Skip the '{'

        ParseScope(condNode);
        
        if (tokenItor->token_id != mylang::TokenIDs::SYM_END_BLOCK) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected \"}\" to end Conditional Body" << std::endl;
        }
    }


    void Parser::ParseAssignment(ASTNode* parent) 
    {
        std::cout << "Parser::ParseAssignment()" << std::endl;

        AssignmentASTNode* assNode = new AssignmentASTNode();
        parent->children.push_back(assNode);

        // Expect an identifier
        if (tokenItor->type_id != mylang::TypeIDs::IDENTIFIER) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected identifier after 'let'" << std::endl;
        }

        IdentifierASTNode* identifierNode = new IdentifierASTNode(  tokenItor->token.c_str());
        assNode->children.emplace_back(identifierNode);
        std::cout << __FILE__ << ":" << __LINE__ << " Parsed identifier: " << assNode->children.back() << std::endl;

        ++tokenItor;

        // Expect an assignment operator
        if (tokenItor->token_id != mylang::TokenIDs::DIGRAPH_ASSIGN) 
        {
            std::cout << __FILE__ << ":" << __LINE__ << " Error: Expected Assignment ':=' after identifier" << std::endl;
        }
    }    

}
