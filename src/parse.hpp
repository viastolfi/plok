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

struct StatementNode {
    std::variant<ReturnStatementNode, PrintStatementNode> statement;
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
    Token consume();
    const std::vector<Token> m_tokens;
    int m_index = 0;
};
