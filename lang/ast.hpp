#pragma once

#include <fstream>
#include <iostream>
#include <vector>

// ------------------------------- AST Base Class --------------------------------

static uint32_t global_id=0;

class ASTNode {
public:
    ASTNode( uint32_t id = 0,
        const std::string& tag = "AST",
        const std::string& type = "Type",
        const std::string& value = "Value" )
    : id(global_id++), tag(tag), type(type), value(value)
    {
    }

    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;

    void Diagram(std::ofstream & outStream)
    {
        outStream << "node" 
            << id << " [" 
            << " label = \"" 
            << tag << "\n" 
            << type << "\n"
            ;

        if (value == ">")  { outStream << "\\>"; }
        else if (value == "<")  { outStream << "\\<"; }
        else if (value == ">=") { outStream << "\\>="; }
        else if (value == "<=") { outStream << "\\<="; }
        else { outStream << value; }
        outStream << "\""; 

        outStream << " ];" << std::endl;

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

    std::vector<ASTNode*> children;
};


// ------------------------------- Module AST Class --------------------------------


class ModuleASTNode : public ASTNode {
public:
    ModuleASTNode(std::string compilation_unit) 
    : ASTNode( global_id++, "MDL",  "Module", compilation_unit ) 
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

// ------------------------------- Scope AST Class --------------------------------


class ScopeASTNode : public ASTNode {
public:
    ScopeASTNode() 
    : ASTNode( global_id++, "SCP",  "Scope", "Scope" ) 
    {
    }

    virtual ~ScopeASTNode() override = default; 
        
    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Scope AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};


// ------------------------------- Keyword AST Class --------------------------------


class KeywordStatementASTNode : public ASTNode {
public:
    KeywordStatementASTNode()
        : ASTNode( global_id++, "KWD", "KeywordStatement", "KeywordStatement" ) 
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


class DeclarationASTNode : public ASTNode {
public:
    DeclarationASTNode()
        : ASTNode( global_id++, "DEC", "Declaration", "Declaration" ) 
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


class FunctionASTNode : public ASTNode {
public:
    FunctionASTNode()
        : ASTNode( global_id++, "FUN", "Function", "Function" ) 
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


class AssignmentStatementASTNode : public ASTNode {
public:
    AssignmentStatementASTNode()
        : ASTNode( global_id++, "ASST", "AssignmentStatement", "AssignmentStatement" ) 
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


class AssignmentASTNode : public ASTNode {
public:
    AssignmentASTNode()
            : ASTNode( global_id++, "ASS", "Assignment", "Assignment" ) 
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


class ExpressionASTNode : public ASTNode {
public:
    ExpressionASTNode()
        : ASTNode( global_id++, "EXP", "Expression", "Expression" ) 
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

class ValueASTNode : public ASTNode {
public:
    ValueASTNode(std::string type, std::string value)
        : ASTNode( global_id++, "VAL", type,  value ) 
        {

        }

    virtual ~ValueASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "Value AST Node\n";

        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }

};

// ------------------------------- Identifier AST Class --------------------------------

class IdentifierASTNode : public ASTNode {
public:
    IdentifierASTNode(const std::string& name)
            : ASTNode( global_id++, "ID", "Identifier",  name) 
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



// ------------------------------- Parameter List AST Class --------------------------------

class ParameterListASTNode : public ASTNode {
public:
    ParameterListASTNode()
            : ASTNode( global_id++, "PLST", "ParameterList", "ParameterList" ) 
        {

        }

    virtual ~ParameterListASTNode() override = default;

    void print(int indent = 0) const override {
        std::string indentation(indent, ' ');
        std::cout << indentation << "ParameterList AST Node\n";
        for (const auto& child : children) {
            child->print(indent + 2);
        }        
    }

};

// ------------------------------- End of AST Classes --------------------------------
