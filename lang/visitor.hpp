#pragma once

// ----------------------------------------------------------------------
// AST Visitor Base Class Declaration
// ----------------------------------------------------------------------


class ASTNode;
class ModuleASTNode;
class ScopeASTNode;
class KeywordStatementASTNode;
class DeclarationASTNode;

class FunctionASTNode;

class ConditionalASTNode;

class LoopASTNode;
class ExitLoopASTNode;

class ReturnASTNode;
class AssignmentASTNode;
class ExpressionASTNode;
class ValueASTNode;
class IdentifierASTNode;
class ParameterListASTNode;

// ----------------------------------------------------------------------

class ASTNodeVisitor{

public:
    virtual void Visit(ASTNode * A)=0;
    virtual void Visit(ModuleASTNode * A)=0;
    virtual void Visit(ScopeASTNode * A)=0;
    virtual void Visit(KeywordStatementASTNode * A)=0;
    virtual void Visit(DeclarationASTNode * A)=0;
    virtual void Visit(FunctionASTNode * A)=0;

    virtual void Visit(IdentifierASTNode * A)=0;

    virtual void Visit(AssignmentASTNode * A)=0;
    virtual void Visit(ExpressionASTNode * A)=0;

    virtual void Visit(ValueASTNode * A)=0;
    virtual void Visit(ReturnASTNode * A)=0;
    // virtual void Visit(FunctionASTNode * A)=0;


    // virtual void Visit(ProcedureASTNode * A)=0;
    // virtual void Visit(OperatorASTNode * A)=0;

};