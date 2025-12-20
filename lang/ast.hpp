#pragma once


#include <iostream>
#include <vector>

class AST {
public:
    AST() = default;
    virtual ~AST() = default;
    virtual void print(int indent = 0) const = 0;

    std::vector<AST*> children;
};

class ModuleASTNode : public AST {
public:
    ModuleASTNode() = default;
    virtual ~ModuleASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Module AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};

class   DeclarationASTNode : public AST {
public:
    DeclarationASTNode() = default;
    virtual ~DeclarationASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Declaration AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};



class ExpressionASTNode : public AST {
public:
    ExpressionASTNode() = default;
    virtual ~ExpressionASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Expression AST Node\n";

        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }
};


class IntegerASTNode : public AST {
public:
    IntegerASTNode() = default;
    virtual ~IntegerASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Integer AST Node\n";

        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }

};

class IdentifierASTNode : public AST {
public:
    IdentifierASTNode() = default;
    virtual ~IdentifierASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Identifier AST Node\n";

        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }

};



