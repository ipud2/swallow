#ifndef TYPE_INFERENCE_ACTION_H
#define TYPE_INFERENCE_ACTION_H
#include "swift_conf.h"
#include "ast/node-visitor.h"
#include "semantic-node-visitor.h"
#include "type.h"
#include "ast/parenthesized-expression.h"
SWIFT_NS_BEGIN


class Type;
class Expression;
class TypeInferenceAction : public SemanticNodeVisitor
{
public:
    TypeInferenceAction(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    void visitFunctionCall(const IdentifierPtr& name, const FunctionCallPtr& node);
    virtual void visitFunctionCall(const FunctionCallPtr& node) override;
    virtual void visitOperator(const OperatorDefPtr& node) override;
    virtual void visitVariable(const VariablePtr& node) override;
    virtual void visitConstant(const ConstantPtr& node) override;
    virtual void visitString(const StringLiteralPtr& node) override;
    virtual void visitInteger(const IntegerLiteralPtr& node) override;
    virtual void visitFloat(const FloatLiteralPtr& node) override;
    virtual void visitConditionalOperator(const ConditionalOperatorPtr& node) override;
    virtual void visitBinaryOperator(const BinaryOperatorPtr& node) override;
    virtual void visitUnaryOperator(const UnaryOperatorPtr& node) override;
    virtual void visitTuple(const TuplePtr& node) override;
    virtual void visitIdentifier(const IdentifierPtr& node) override;
    virtual void visitCompileConstant(const CompileConstantPtr& node) override;
    virtual void visitMemberAccess(const MemberAccessPtr& node) override;

private:
    void checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer);
    TypePtr evaluateType(const ExpressionPtr& expr);

    /**
     * Calculates the fit score of arguments on given function
     * @return -1 if the type is not matched
     */
    float calculateFitScore(const FunctionSymbolPtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors);
    bool checkArgument(const Type::Parameter& parameter, const ParenthesizedExpression::Term& argument, bool variadic, float& score, bool supressErrors);
    TypePtr getExpressionType(const ExpressionPtr& expr, const TypePtr& hint, float& score);
private:
    TypePtr t_int, t_uint, t_int8, t_uint8, t_int16, t_uint16, t_int32, t_uint32, t_int64, t_uint64;
    TypePtr t_float, t_double;
    std::vector<TypePtr> t_numbers;
    std::vector<TypePtr> t_ints;
    TypePtr t_string;
    TypePtr t_bool;

};


SWIFT_NS_END


#endif//TYPE_INFERENCE_ACTION_H