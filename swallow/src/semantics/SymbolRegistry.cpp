/* SymbolRegistry.cpp --
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
#include "semantics/SymbolRegistry.h"
#include "semantics/SymbolScope.h"
#include "semantics/Type.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GlobalScope.h"
#include <cassert>

using namespace Swallow;

SymbolRegistry::SymbolRegistry()
:currentScope(nullptr), fileScope(nullptr)
{
    globalScope = new GlobalScope();
    globalScope->initRuntime(this);
    enterScope(globalScope);
    //?:  Right associative, precedence level 100

    //Register built-in type

}
SymbolRegistry::~SymbolRegistry()
{
    delete globalScope;
}

bool SymbolRegistry::registerOperator(const std::wstring& name, OperatorType::T type, Associativity::T associativity, int precedence, bool assignment)
{
    assert(fileScope != nullptr);
    return registerOperator(fileScope, name, type, associativity, precedence, assignment);
}
bool SymbolRegistry::registerOperator(SymbolScope* scope, const std::wstring& name, OperatorType::T type, Associativity::T associativity, int precedence, bool assignment)
{
    assert(scope != nullptr && "Operator cannot be registered in an invalid scope");
    SymbolScope::OperatorMap::iterator iter = scope->operators.find(name);
    if(iter == scope->operators.end())
    {
        auto ret = scope->operators.insert(std::make_pair(name, OperatorInfo(name, associativity, assignment)));
        iter = ret.first;
    }
    else
    {
        if((iter->second.type & type) != 0)
            return false;//operator redeclared
    }

    iter->second.type = (OperatorType::T)(iter->second.type | type);
    iter->second.assignment = iter->second.assignment || assignment;
    
    if(type & OperatorType::PrefixUnary)
        iter->second.precedence.prefix = precedence;
    if(type & OperatorType::InfixBinary)
        iter->second.precedence.infix = precedence;
    if(type & OperatorType::PostfixUnary)
        iter->second.precedence.postfix = precedence;
    return true;
}
OperatorInfo* SymbolRegistry::getOperator(const std::wstring& name, int typeMask)
{
    OperatorInfo* ret = NULL;
    if(fileScope)
    {
        ret = getOperator(fileScope, name, typeMask);
    }
    if(!ret)
        ret = getOperator(globalScope, name, typeMask);
    return ret;
}
OperatorInfo* SymbolRegistry::getOperator(SymbolScope* scope, const std::wstring& name, int typeMask)
{
    SymbolScope::OperatorMap::iterator iter = scope->operators.find(name);
    if(iter == scope->operators.end())
        return nullptr;
    if((iter->second.type & typeMask) == 0)
        return nullptr;
    return &iter->second;
}
bool SymbolRegistry::isPrefixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name, OperatorType::PrefixUnary);
    return op != nullptr;
}
bool SymbolRegistry::isPostfixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name, OperatorType::PostfixUnary);
    return op != nullptr;
}
bool SymbolRegistry::isInfixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name, OperatorType::InfixBinary);
    return op != nullptr;
}

GlobalScope* SymbolRegistry::getGlobalScope()
{
    return globalScope;
}

SymbolScope* SymbolRegistry::getFileScope()
{
    return fileScope;
}
void SymbolRegistry::setFileScope(SymbolScope* scope)
{
    fileScope = scope;
}

SymbolScope* SymbolRegistry::getCurrentScope()
{
    return currentScope;
}

/*!
 * Not recommended to use, it will interrupt the internal scope state, use it wisely.
 */
void SymbolRegistry::setCurrentScope(SymbolScope* scope)
{
    this->currentScope = scope;
}
void SymbolRegistry::enterScope(SymbolScope* scope)
{
    scopes.push(currentScope);
    scope->parent = currentScope;
    if(currentScope)
        scope->depth = currentScope->depth + 1;
    else
        scope->depth = 0;
    currentScope = scope;
}
void SymbolRegistry::leaveScope()
{
    if(scopes.empty())
    {
        currentScope = nullptr;
        return;
    }
    currentScope = scopes.top();
    scopes.pop();
}

SymbolPtr SymbolRegistry::lookupSymbol(const std::wstring& name)
{
    SymbolPtr ret = NULL;
    lookupSymbol(name, NULL, &ret);
    return ret;
}
bool SymbolRegistry::lookupSymbol(const std::wstring& name, SymbolScope** container, SymbolPtr* ret, bool lazyResolve)
{
    return lookupSymbol(currentScope, name, container, ret, lazyResolve);
}
bool SymbolRegistry::lookupSymbol(SymbolScope* scope, const std::wstring& name, SymbolScope** container, SymbolPtr* ret, bool lazyResolve)
{
    SymbolScope* s = scope;
    for(; s; s = s->parent)
    {
        SymbolPtr symbol = s->lookup(name, lazyResolve);
        if(symbol)
        {
            if(container)
                *container = s;
            if(ret)
                *ret = symbol;
            return true;
        }
    }
    return false;
}
TypePtr SymbolRegistry::lookupType(const std::wstring& name)
{
    TypePtr ret;
    if(lookupType(name, NULL, &ret))
        return ret;
    return NULL;
}
/*!
    * Check if a symbol is defined. This will not use LazySymbolResolver to resolve it if it's undefined
    */
bool SymbolRegistry::isSymbolDefined(const std::wstring& name) const
{
    SymbolScope* s = currentScope;
    for(; s; s = s->parent)
    {
        if(s->isSymbolDefined(name))
            return true;
    }
    return false;
}
bool SymbolRegistry::lookupType(const std::wstring& name, SymbolScope** scope, TypePtr* ret, bool lazyResolve)
{
    SymbolPtr symbol = NULL;
    bool r = lookupSymbol(name, scope, &symbol, lazyResolve);
    if(ret)
        *ret = std::dynamic_pointer_cast<Type>(symbol);
    return r;
}
