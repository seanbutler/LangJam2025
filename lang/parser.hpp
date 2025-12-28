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

            void ParseDeclaration(ModuleASTNode* parent); 
            void ParseFunction(ModuleASTNode* parent);

            void ParseAssignmentStatement(ModuleASTNode* parent);
            void ParseKeywordStatement(ModuleASTNode* moduleNode);

            AssignmentASTNode * ParseAssignment(ModuleASTNode* parent);

            IdentifierASTNode * ParseIdentifier(); 
            ExpressionASTNode * ParseExpression(); 

        private:        
            std::vector<Token> tokens_;
            std::vector<Token>::iterator tokenItor;
            std::shared_ptr<AST> abstractSyntaxTree;
            std::string compilation_unit_;
        };
}





