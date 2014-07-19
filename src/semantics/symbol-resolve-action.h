#ifndef SYMBOL_RESOLVE_ACTION_H
#define SYMBOL_RESOLVE_ACTION_H
#include "swift_conf.h"
#include "ast/node-visitor.h"
#include "semantics/type.h"
SWIFT_NS_BEGIN

class SymbolRegistry;
class CompilerResults;
class TypeDeclaration;
class Expression;
class Pattern;
class SymbolResolveAction : public NodeVisitor
{
public:
    SymbolResolveAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitAssignment(Assignment* node);
    virtual void visitVariables(Variables* node);
    virtual void visitConstants(Constant* node);
    virtual void visitClass(ClassDef* node);
    virtual void visitStruct(StructDef* node);
    virtual void visitEnum(EnumDef* node);
    virtual void visitProtocol(ProtocolDef* node);
    virtual void visitExtension(ExtensionDef* node);
    virtual void visitFunction(FunctionDef* node);
private:
    void defineType(TypeDeclaration* node, Type::Category category);
    //Register all symbols in the pattern
    void registerPattern(Pattern* pattern);
    //Verify each symbol in the tuple is initialized
    void verifyTuplePattern(Pattern* pattern);

    //Verify every symbol in the expression is initialized
    void verifyExpression(Expression* expr);
    //set or reset flag in all identifiers in given pattern
    void setFlag(Pattern* pattern, bool add, int flag);
private:
    SymbolRegistry* symbolRegistry;
    CompilerResults* compilerResults;
};

SWIFT_NS_END


#endif//SYMBOL_RESOLVE_ACTION_H