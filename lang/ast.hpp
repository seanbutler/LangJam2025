#pragma once

#include <fstream>
#include <iostream>
#include <vector>

// ------------------------------- AST Base Class --------------------------------
static uint32_t global_id=0;


class AST {
public:
    AST( uint32_t id = 0,
        const std::string& tag = "AST",
        const std::string& type = "Type",
        const std::string& value = "Value" )
    : id(global_id++), tag(tag), type(type), value(value)
    {
    }

    virtual ~AST() = default;
    virtual void print(int indent = 0) const = 0;

    void Diagram(std::ofstream & outStream)
    {
        outStream << "node" << id << " [" 
                // << " uuid = \"" << id << "\""
                // << " shape = \"record\""
                << " label = \"" << tag << "\n" << type << "\n";

        if (value == ">")  { outStream << "\\>"; }
        else if (value == "<")  { outStream << "\\<"; }
        else if (value == ">=") { outStream << "\\>="; }
        else if (value == "<=") { outStream << "\\<="; }
        else { outStream << value; }

        outStream << "\"" << " ];" << std::endl;

        for(auto N : children) {
            if (N != nullptr) {
                N->Diagram(outStream);
                outStream << "node" << this->id << " -> " << "node" << N->id << ";" << std::endl;
            }
        }
    }

    
    uint32_t id;
    std::string tag;
    std::string type;
    std::string value;

    std::vector<AST*> children;
};


// ------------------------------- Module AST Class --------------------------------


class ModuleASTNode : public AST {
public:
    ModuleASTNode(std::string compilation_unit) 
    : AST( global_id++, "MDL", compilation_unit, "Module" ) 
    {
    }

    virtual ~ModuleASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Module AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};


// ------------------------------- Keyword AST Class --------------------------------


class KeywordStatementASTNode : public AST {
public:
    KeywordStatementASTNode()
        : AST( global_id++, "KWD", "KeywordStatement", "KeywordStatement" ) 
        {

        }

    virtual ~KeywordStatementASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Keyword Statement AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};


// ------------------------------- Declaration AST Class --------------------------------


class DeclarationASTNode : public AST {
public:
    DeclarationASTNode()
        : AST( global_id++, "DEC", "Declaration", "Declaration" ) 
        {

        }



    virtual ~DeclarationASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Declaration AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};


// ------------------------------- Declaration AST Class --------------------------------


class FunctionASTNode : public AST {
public:
    FunctionASTNode()
        : AST( global_id++, "FUN", "Function", "Function" ) 
        {

        }





    virtual ~FunctionASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Function AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};



// ------------------------------- Assignment Statement AST Class --------------------------------


class AssignmentStatementASTNode : public AST {
public:
    AssignmentStatementASTNode()
        : AST( global_id++, "ASST", "AssignmentStatement", "AssignmentStatement" ) 
        {

        }


    virtual ~AssignmentStatementASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Assignment Statement AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};


// ------------------------------- Assignment AST Class --------------------------------


class AssignmentASTNode : public AST {
public:
    AssignmentASTNode()
            : AST( global_id++, "ASS", "Assignment", "Assignment" ) 
        {

        }


    virtual ~AssignmentASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Assignment AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};


// ------------------------------- Expression AST Class --------------------------------


class ExpressionASTNode : public AST {
public:
    ExpressionASTNode()
        : AST( global_id++, "EXP", "Expression", "Expression" ) 
        {

        }


    virtual ~ExpressionASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Expression AST Node\n";

        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }
};


// ------------------------------- Integer AST Class --------------------------------

class IntegerASTNode : public AST {
public:
    IntegerASTNode(std::string value)
        : AST( global_id++, "INT", value, "Integer" ) 
        {

        }

    virtual ~IntegerASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Integer AST Node\n";

        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }

};

// ------------------------------- Identifier AST Class --------------------------------

class IdentifierASTNode : public AST {
public:
    IdentifierASTNode(const std::string& name)
            : AST( global_id++, "ID", name, "Identifier" ) 
        {

        }


    virtual ~IdentifierASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Identifier AST Node\n";

        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }

};

// ------------------------------- End of AST Classes --------------------------------
