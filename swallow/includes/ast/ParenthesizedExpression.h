/* ParenthesizedExpression.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PARENTHESIZED_EXPRESSION_H
#define PARENTHESIZED_EXPRESSION_H
#include "Expression.h"
#include <string>

SWALLOW_NS_BEGIN
class ExpressionNode;
class SWALLOW_EXPORT ParenthesizedExpression : public Expression
{
public:
    struct Term
    {
        std::wstring name;
        ExpressionPtr expression;
        ExpressionPtr transformedExpression;
        Term(const std::wstring& name, const ExpressionPtr& expr)
                :name(name), expression(expr)
        {}
    };

public:
    ParenthesizedExpression();
    ~ParenthesizedExpression();
public:
    void append(const std::wstring& name, const ExpressionPtr& expr);
    void append(const ExpressionPtr& expr);
    size_t numExpressions()const;
    std::wstring getName(int idx);
    ExpressionPtr get(int idx);
public:
    virtual void accept(NodeVisitor* visitor);
    std::vector<Term>::iterator begin() {return expressions.begin();}
    std::vector<Term>::iterator end() {return expressions.end();}
public:
    std::vector<Term> expressions;
};

SWALLOW_NS_END



#endif//PARENTHESIZED_EXPRESSION_H
