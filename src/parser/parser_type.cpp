#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include <cstdlib>
#include <stack>
#include <sstream>
#include <iostream>
using namespace Swift;

Node* Parser::parseType(const wchar_t* code)
{
    tokenizer->set(code);
    return parseType();
}
TypeNode* Parser::parseType()
{
    Token token;
    TypeNode* ret = NULL;
    next(token);
    
    if(token == TokenType::OpenParen)
    {
        tokenizer->restore(token);
        ret = parseTupleType();
    }
    else if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::_)
    {
        ret = parseTypeIdentifier();
    }
    else if(token.getKeyword() == Keyword::Protocol)
    {
        tokenizer->restore(token);
        ret = parseProtocolComposition();
    }
    else
    {
        unexpected(token);
    }
    do
    {
        next(token);
        
        //type chaining
        if(token == L"->")
        {
            //function-type → type->type
            TypeNode* retType = parseType();
            ret = nodeFactory->createFunctionType(ret, retType);
            continue;
        }
        if(token == TokenType::OpenBracket)
        {
            expect(L"]");
            ret = nodeFactory->createArrayType(ret);
            continue;
        }
        if(token == L"?")
        {
            //optional-type → type?
            ret = nodeFactory->createOptionalType(ret);
            continue;
        }
        if(token == L"!")
        {
            //implicitly-unwrapped-optional-type → type!
            ret = nodeFactory->createImplicitlyUnwrappedOptional(ret);
            continue;
        }
        // ‌ metatype-type → type.Type  type.Protocol
        //TODO meta type is not supported
        tokenizer->restore(token);
    }while(false);
    return ret;
}

/*
  GRAMMAR OF A TUPLE TYPE
 
 ‌ tuple-type → (tuple-type-bodyopt)
 ‌ tuple-type-body → tuple-type-element-list...opt
 ‌ tuple-type-element-list → tuple-type-element  tuple-type-element,tuple-type-element-list
 ‌ tuple-type-element → attributesoptinoutopttype inoutoptelement-nametype-annotation
 ‌ element-name → identifier
*/
TupleType* Parser::parseTupleType()
{
    TupleType* ret = nodeFactory->createTupleType();
    expect(L"(");
    if(!predicate(L")"))
    {
        do
        {
            //‌ tuple-type-element → attributes opt inout opt type inoutoptelement-nametype-annotation
        
        }while(match(L","));
        if(match(L"..."))
        {
            ret->setVariadicParameters(true);
        }
    }
    
    expect(L")");
    return ret;
}
TypeIdentifier* Parser::parseTypeIdentifier()
{
    Token token;
    match_identifier(token);
    TypeIdentifier* ret = nodeFactory->createTypeIdentifier(token.token);
    if(match(L"<"))
    {
        do
        {
            TypeNode* arg = parseType();
            ret->addGenericArgument(arg);
        }while(match(L","));

        expect(L">");
    }
    return ret;
}
ProtocolComposition* Parser::parseProtocolComposition()
{
    expect(L"protocol");
    expect(L"<");
    ProtocolComposition* ret = nodeFactory->createProtocolComposition();
    if(!predicate(L">"))
    {
        do
        {
            TypeIdentifier* protocol = parseTypeIdentifier();
            ret->addProtocol(protocol);
        }while(match(L","));
    }
    expect(L">");
    return ret;
}

/*
 “GRAMMAR OF A GENERIC PARAMETER CLAUSE
 
 ‌ generic-parameter-clause → <generic-parameter-listrequirement-clauseopt>
 ‌ generic-parameter-list → generic-parameter | generic-parameter,generic-parameter-list
 ‌ generic-parameter → type-name
 ‌ generic-parameter → type-name:type-identifier
 ‌ generic-parameter → type-name:protocol-composition-type
 ‌ requirement-clause → whererequirement-list
 ‌ requirement-list → requirement | requirement,requirement-list
 ‌ requirement → conformance-requirement | same-type-requirement
 ‌ conformance-requirement → type-identifier:type-identifier
 ‌ conformance-requirement → type-identifier:protocol-composition-type
 ‌ same-type-requirement → type-identifier==type-identifier
 GRAMMAR OF A GENERIC ARGUMENT CLAUSE
 
 ‌ generic-argument-clause → <generic-argument-list>
 ‌ generic-argument-list → generic-argument | generic-argument,generic-argument-list
 ‌ generic-argument → type”
 
 
 “GRAMMAR OF A TYPE
 
 ‌ type → array-type  function-type  type-identifier tuple-type  optional-type  implicitly-unwrapped-optional-type  protocol-composition-type  metatype-type
 GRAMMAR OF A TYPE ANNOTATION
 
 ‌ type-annotation → :attributesopttype
 GRAMMAR OF A TYPE IDENTIFIER
 
 ‌ type-identifier → type-namegeneric-argument-clauseopt type-namegeneric-argument-clauseopt.type-identifier
 ‌ type-name → identifier
 GRAMMAR OF A TUPLE TYPE
 
 ‌ tuple-type → (tuple-type-bodyopt)
 ‌ tuple-type-body → tuple-type-element-list...opt
 ‌ tuple-type-element-list → tuple-type-element  tuple-type-element,tuple-type-element-list
 ‌ tuple-type-element → attributesoptinoutopttype inoutoptelement-nametype-annotation
 ‌ element-name → identifier
 GRAMMAR OF A FUNCTION TYPE
 
 ‌ function-type → type->type
 GRAMMAR OF AN ARRAY TYPE
 
 ‌ array-type → type[]  array-type[]”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF AN OPTIONAL TYPE
 
 ‌ optional-type → type?
 GRAMMAR OF AN IMPLICITLY UNWRAPPED OPTIONAL TYPE
 
 ‌ implicitly-unwrapped-optional-type → type!
 GRAMMAR OF A PROTOCOL COMPOSITION TYPE
 
 ‌ protocol-composition-type → protocol<protocol-identifier-listopt>
 ‌ protocol-identifier-list → protocol-identifier  protocol-identifier,protocol-identifier-list
 ‌ protocol-identifier → type-identifier
 GRAMMAR OF A METATYPE TYPE
 
 ‌ metatype-type → type.Type  type.Protocol
 GRAMMAR OF A TYPE INHERITANCE CLAUSE
 
 ‌ type-inheritance-clause → :type-inheritance-list
 ‌ type-inheritance-list → type-identifier  type-identifier,type-inheritance-list”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
*/