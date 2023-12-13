#pragma once

#include "tokenize.hpp"

struct IntExpressionNode {
    int value;
};

struct StringExpressionNode {
    std::string value;
};

struct ExpressionNode {
    std::variant<StringExpressionNode, IntExpressionNode> var;
};

struct ReturnStatementNode {
    Token identifier;
    ExpressionNode expression;
};

struct PrintStatementNode {
    Token identifier;
    ExpressionNode expression;
};

struct LetStatementNode {
    Token identifier;
    std::vector<ExpressionNode> expressions;
};

struct StatementNode {
    std::variant<ReturnStatementNode, PrintStatementNode, LetStatementNode> statement;
};

struct NodeRoot {
    std::vector<StatementNode> statements;
};


class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    NodeRoot parse();
private:
    std::optional<Token> peek(int offset = 0) const;
    ExpressionNode parse_expression(const Token& token) const;
    Token consume();
    const std::vector<Token> m_tokens;
    int m_index = 0;
    NodeRoot m_root;
};
