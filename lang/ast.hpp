#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "visitor.hpp"

// ------------------------------- AST Base Class --------------------------------

static uint32_t global_id=0;

class ASTNode {
public:
    ASTNode( uint32_t id = 0,
        const std::string& tag = "AST",
        const std::string& type = "Type",
        const std::string& value = "Value",
        const std::string& comment = "AST Node"
    )
    : id(global_id++), tag(tag), type(type), value(value), print_comment(comment)
    {

    }

    virtual ~ASTNode() = default;


    virtual void Accept(ASTNodeVisitor* visitor) =0;

    void print(int indent = 0) const 
    {
        std::string indentation(indent, ' ');
        std::cout << indentation << print_comment << std::endl;
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }



    void Diagram(std::ofstream & outStream)
    {
        outStream << "node" 
            << id << " [" 
            << " label = \"" 
            << tag << "\n" 
            // << type << "\n"
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
    std::string print_comment;

};


// ------------------------------- Module AST Class --------------------------------


class ModuleASTNode : public ASTNode {
public:
    ModuleASTNode(std::string compilation_unit) 
    : ASTNode( global_id++, "MDL",  "Module", compilation_unit, "Module AST Node" )
    {
    }

    virtual void Accept(ASTNodeVisitor* visitor) override
    {
        visitor->Visit(this);
    }

    virtual ~ModuleASTNode() override = default;
};

// ------------------------------- Scope AST Class --------------------------------


class ScopeASTNode : public ASTNode {
public:
    ScopeASTNode() 
    : ASTNode( global_id++, "SCOP",  "Scope", "Scope", "Scope AST Node" ) 
    {
    }

    virtual void Accept(class ASTNodeVisitor* visitor) override
    {
        visitor->Visit(this);
    }

    virtual ~ScopeASTNode() override = default; 
        
};


// ------------------------------- Keyword AST Class --------------------------------


class KeywordStatementASTNode : public ASTNode {
public:
    KeywordStatementASTNode()
        : ASTNode( global_id++, "KWRD", "KeywordStatement", "KeywordStatement", "Keyword Statement AST Node")
        {

        }

    virtual ~KeywordStatementASTNode() override = default;

};


// ------------------------------- Declaration AST Class --------------------------------


class DeclarationASTNode : public ASTNode {
public:
    DeclarationASTNode()
        : ASTNode( global_id++, "DECL", "Declaration", "Declaration", "Declaration AST Node" ) 
        {

        }

    virtual void Accept(ASTNodeVisitor* visitor) override
    {
        visitor->Visit(this);
    }

    virtual ~DeclarationASTNode() override = default;

};


// ------------------------------- Declaration AST Class --------------------------------


class FunctionASTNode : public ASTNode {
public:
    FunctionASTNode()
        : ASTNode( global_id++, "FUNC", "Function", "Function", "Function AST Node" ) 
        {

        }

    virtual void Accept(ASTNodeVisitor* visitor) override
    {
        visitor->Visit(this);
    }   


    virtual ~FunctionASTNode() override = default;

};


// ------------------------------- Conditional AST Class --------------------------------


class ConditionalASTNode : public ASTNode {
public:
    ConditionalASTNode()
        : ASTNode( global_id++, "COND", "Conditional", "Conditional", "Conditional AST Node" ) 
        {

        }


        virtual void Accept(ASTNodeVisitor* visitor) override
        {
            visitor->Visit(this);
        }

    virtual ~ConditionalASTNode() override = default;

};


// ------------------------------- Loop AST Class --------------------------------


class LoopASTNode : public ASTNode {
public:
    LoopASTNode()
        : ASTNode( global_id++, "LOOP", "Loop", "Loop", "Loop AST Node" ) 
        {

        }

        virtual void Accept(ASTNodeVisitor* visitor) override
        {
            visitor->Visit(this);
        }

    virtual ~LoopASTNode() override = default;


};


// ------------------------------- ExitLoop AST Class --------------------------------


class ExitLoopASTNode : public ASTNode {
public:
    ExitLoopASTNode()
        : ASTNode( global_id++, "EXIT", "ExitLoop", "ExitLoop", "ExitLoop AST Node" ) 
        {

        }

        virtual void Accept(ASTNodeVisitor* visitor) override
        {
            visitor->Visit(this);
        }


    virtual ~ExitLoopASTNode() override = default;

};

// ------------------------------- Return AST Class --------------------------------


class ReturnASTNode : public ASTNode {
public:
    ReturnASTNode()
        : ASTNode( global_id++, "RET", "Return", "Return", "Return AST Node" ) 
        {

        }

        virtual void Accept(ASTNodeVisitor* visitor) override
        {
            visitor->Visit(this);
        }

    virtual ~ReturnASTNode() override = default;

};


// ------------------------------- Assignment AST Class --------------------------------

class AssignmentASTNode : public ASTNode {
public:
    AssignmentASTNode()
        : ASTNode( global_id++, "ASGN", "Assignment", "Assignment", "Assignment AST Node" ) 
        {

        }

    virtual void Accept(ASTNodeVisitor* visitor) override
    {
        visitor->Visit(this);
    }


    virtual ~AssignmentASTNode() override = default;
};


// ------------------------------- Expression AST Class --------------------------------


class ExpressionASTNode : public ASTNode {
public:
    ExpressionASTNode()
        : ASTNode( global_id++, "EXPR", "Expression", "Expression", "Expression AST Node" ) 
        {

        }

    virtual void Accept(ASTNodeVisitor* visitor) override
    {
        visitor->Visit(this);
    }   


    virtual ~ExpressionASTNode() override = default;

};


// ------------------------------- Integer AST Class --------------------------------

class ValueASTNode : public ASTNode {
public:
    ValueASTNode(std::string type, std::string value)
        : ASTNode( global_id++, "VAL", type,  value, "Value AST Node" ) 
        {

        }

    virtual void Accept(ASTNodeVisitor* visitor) override
    {
        visitor->Visit(this);
    }


    virtual ~ValueASTNode() override = default;


};

// ------------------------------- Identifier AST Class --------------------------------

class IdentifierASTNode : public ASTNode {
public:
    IdentifierASTNode(const std::string& name)
            : ASTNode( global_id++, "IDNT", "Identifier",  name, "Identifier AST Node" ) 
        {

        }

        virtual void Accept(ASTNodeVisitor* visitor) override
        {
            visitor->Visit(this);
        }

    virtual ~IdentifierASTNode() override = default;


};



// ------------------------------- Parameter List AST Class --------------------------------

class ParameterListASTNode : public ASTNode {
public:
    ParameterListASTNode()
            : ASTNode( global_id++, "PLST", "ParameterList", "ParameterList", "Parameter List AST Node" ) 
        {

        }

        virtual void Accept(ASTNodeVisitor* visitor) override
        {
            visitor->Visit(this);
        }
        
    virtual ~ParameterListASTNode() override = default;


};

// ------------------------------- End of AST Classes --------------------------------
