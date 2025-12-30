#pragma once

#include <iostream>
#include <vector>

#include "ast.hpp"
#include "tokens.hpp"

namespace mylang {

    class Parser {

        public:
            Parser(const std::vector<Token>& tokens , std::string compilation_unit);
            ~Parser() = default;

            ModuleASTNode * ParseModule();

            void ParseStatement(ASTNode * parent);
            void ParseDeclaration(ASTNode * parent); 
            void ParseAssignment(ASTNode* parent); 
            void ParseFunction(ASTNode * parent);
            void ParseCondition(ASTNode* parent); 

            void ParseAssignmentStatement(ASTNode * parent);
            void ParseKeywordStatement(ASTNode * parent);
            void ParseScope(ASTNode * parent);
            void ParseExpression(ASTNode * parent);


            IdentifierASTNode * ParseIdentifier(); 

        private:        
            std::vector<Token> tokens_;
            std::vector<Token>::iterator tokenItor;
            std::shared_ptr<ASTNode> abstractSyntaxTree;
            std::string compilation_unit_;
        };
}





