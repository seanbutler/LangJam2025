#pragma once

#include "visitor.hpp"
#include <iostream>

// ----------------------------------------------------------------------

class Diagram_Visitor : public ASTNodeVisitor 
{

public:

    virtual void Visit(ASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting ASTNode" << std::endl;

    }

    virtual void Visit(ModuleASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting ModuleASTNode" << std::endl;

        for ( auto child : A->children )
        {
            child->Accept(this);
        }
    }

    virtual void Visit(ScopeASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting ScopeASTNode" << std::endl;


        for ( auto child : A->children )
        {
            child->Accept(this);
        }        
    }

    virtual void Visit(KeywordStatementASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting KeywordStatementASTNode" << std::endl;
    }

    virtual void Visit(DeclarationASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting DeclarationASTNode" << std::endl;
    }

    virtual void Visit(FunctionASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting FunctionASTNode" << std::endl;
    }

    virtual void Visit(IdentifierASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting IdentifierASTNode" << std::endl;
    }

    virtual void Visit(AssignmentASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting AssignmentASTNode" << std::endl;
    }

    virtual void Visit(ExpressionASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting ExpressionASTNode" << std::endl;
    }

    virtual void Visit(ValueASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting ValueASTNode" << std::endl;
    }

    virtual void Visit(ReturnASTNode * A) override 
    {
        std::cout << "Diagram_Visitor Visiting ReturnASTNode" << std::endl;
    }
};


// ----------------------------------------------------------------------