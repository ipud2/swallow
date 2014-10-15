#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H
#include "Statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class ReturnStatement : public Statement
{
public:
    ReturnStatement();
    ~ReturnStatement();
public:
    void setExpression(const ExpressionPtr& expr);
    ExpressionPtr getExpression();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    ExpressionPtr expression;
};

SWIFT_NS_END

#endif//RETURN_STATEMENT_H