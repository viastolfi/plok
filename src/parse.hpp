#pragma once

#include <set>
#include <variant>
#include "tokenize.hpp"

struct nodeStatement;
struct NodeRoot {
    std::vector<nodeStatement> statements;
};

struct nodeBinaryExpression;
struct nodeIdentifier;

struct nodeExpression {
    std::variant<nodeBinaryExpression, nodeIdentifier> var;
};

struct nodeReturnStatement {
    Token identifier;
    nodeExpression expression;
};

struct nodeLetStatement {
    Token identifier;
    nodeExpression expression;
};

//struct nodeCalculStatement {
//    Token identifier;
//    nodeExpression expression;
//};

struct nodeStatement {
    std::variant<nodeLetStatement, nodeReturnStatement> var;
};

struct nodeAdditionBinaryExpression;

struct nodeBinaryExpression {
    std::variant<nodeAdditionBinaryExpression> var;
};

struct nodeAdditionBinaryExpression {
    nodeExpression left_expression;
    nodeExpression right_expression;
};

struct nodeIntLitIdentifier {
    Token identifier;
};

struct nodeStringIdentifier {
    Token Identifier;
};

struct nodeVariableIdentifier {
    Token Identifier;
};

struct nodeIdentifier {
    std::variant<nodeIntLitIdentifier, nodeStringIdentifier, nodeVariableIdentifier> var;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    NodeRoot parse();
private:
    [[nodiscard]] std::optional<Token> peek(int offset = 0) const;
    std::optional<nodeExpression> parse_expression();
    [[nodiscard]] std::optional<nodeStatement> parse_statement();
    //[[nodiscard]] ExpressionNode parse_binary_expression(const Token& token) const;

    Token consume(); 
    const std::vector<Token> m_tokens;
    int m_index = 0;
    NodeRoot m_root;
};