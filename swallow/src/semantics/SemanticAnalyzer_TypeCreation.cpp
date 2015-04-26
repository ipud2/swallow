/* SemanticAnalyzer_TypeCreation.cpp --
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
#include "semantics/SemanticAnalyzer.h"
#include "ast/ast.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "common/Errors.h"
#include "semantics/TypeBuilder.h"
#include "semantics/GlobalScope.h"
#include <cassert>
#include "ast/NodeFactory.h"
#include "common/ScopedValue.h"
#include "semantics/DeclarationAnalyzer.h"

USE_SWALLOW_NS
using namespace std;


void SemanticAnalyzer::visitTypeAlias(const TypeAliasPtr& node)
{
    node->accept(declarationAnalyzer);
}
void SemanticAnalyzer::visitEnum(const EnumDefPtr& node)
{
    if(lazyDeclaration && isGlobal(node))
    {
        //mark it as lazy declaration
        delayDeclare(node);
        return;
    }
    node->accept(declarationAnalyzer);
}
void SemanticAnalyzer::visitClass(const ClassDefPtr& node)
{
    if(lazyDeclaration && isGlobal(node))
    {
        //mark it as lazy declaration
        delayDeclare(node);
        return;
    }
    node->accept(declarationAnalyzer);
}
void SemanticAnalyzer::visitStruct(const StructDefPtr& node)
{
    if(lazyDeclaration && isGlobal(node))
    {
        //mark it as lazy declaration
        delayDeclare(node);
        return;
    }
    node->accept(declarationAnalyzer);
}
void SemanticAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    if(lazyDeclaration && isGlobal(node))
    {
        //mark it as lazy declaration
        delayDeclare(node);
        return;
    }
    node->accept(declarationAnalyzer);
}
void SemanticAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    if(ctx.currentFunction || ctx.currentType)
    {
        error(node, Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, node->getIdentifier()->getName());
        return;
    }
    //check if this type is already registered
    if(lazyDeclaration)
    {
        SymbolPtr sym = symbolRegistry->lookupSymbol(node->getIdentifier()->getName());
        if(!sym)
        {
            delayDeclare(node);
            return;
        }
    }
    node->accept(declarationAnalyzer);
}
void SemanticAnalyzer::visitOptionalType(const OptionalTypePtr& node)
{
    TypePtr innerType = lookupType(node->getInnerType());
    TypePtr type = symbolRegistry->getGlobalScope()->makeOptional(innerType);
    node->setType(type);
}
