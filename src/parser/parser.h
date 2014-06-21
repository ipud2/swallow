#ifndef PARSER_H
#define PARSER_H
#include "swift_conf.h"
#include "tokenizer/tokens.h"
#include <string>

SWIFT_NS_BEGIN

class Tokenizer;
class Node;
class ExpressionNode;
class SymbolRegistry;
class Identifier;
class NodeFactory;
class Parser
{
public:
    Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry);
    ~Parser();
public:
    Node* parse(const wchar_t* code);
    void setFileName(const wchar_t* fileName);
    void setFunctionName(const wchar_t* function);
private:
    ExpressionNode* parseFloat();
    ExpressionNode* parseInteger();
    ExpressionNode* parseString();
    ExpressionNode* parseExpression();
    ExpressionNode* parsePrefixExpression();
    ExpressionNode* parsePostfixExpression();
    ExpressionNode* parsePrimaryExpression();
    ExpressionNode* parseLiteralExpression();
    ExpressionNode* parseLiteral();
    
    ExpressionNode* parseSelfExpression();
    ExpressionNode* parseSuperExpression();
    Identifier* parseIdentifier();
    
    std::pair<ExpressionNode*, ExpressionNode*> parseDictionaryLiteralItem();
private:
    /**
     * Check if the following token is an identifier, throw exception if not matched
     */
    void expect_identifier(Token& token);
    /**
     * Check if the following token is the specified one, throw exception if not matched
     */
    void expect(const wchar_t* token);

    /**
     * Throw an exception with the unexpected token
     */
    void unexpected(const Token& token);

    /**
     * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
     */
    bool match(const wchar_t* token);
    /**
     * Return true if the next token is the specified one, no token will be consumed
     */
    bool predicate(const wchar_t* token);

    void tassert(Token& token, bool cond);
private:
    Tokenizer* tokenizer;
    NodeFactory* nodeFactory;
    SymbolRegistry* symbolRegistry;
    std::wstring fileName;
    std::wstring functionName;
};



SWIFT_NS_END

#endif//PARSER_H