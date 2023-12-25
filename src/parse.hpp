#pragma once

#include <set>
#include <variant>
#include <cassert>

#include "arena.hpp"
#include "tokenize.hpp"

struct nodeExpression;

struct nodeTerminationIntLit {
    Token int_lit;
};

struct nodeTerminationIdentifier {
    Token identifier;
};

struct nodeBinaryExpressionAdd {
    nodeExpression* left_expression;
    nodeExpression* right_expression;
};

struct nodeTermination {
    std::variant<nodeTerminationIdentifier*, nodeTerminationIntLit*> var;
};

struct nodeBinaryExpression {
    std::variant<nodeBinaryExpressionAdd*> var;
};

struct nodeExpression {
    std::variant<nodeBinaryExpression*, nodeTermination*> var;
};

struct nodeReturnStatement {
    nodeExpression* expression;
};

struct nodeLetStatement {
    Token idetifier;
    nodeExpression* expression;
};

struct nodeStatement {
    std::variant<nodeReturnStatement*, nodeLetStatement*> var;
};

struct nodeRoot {
    std::vector<nodeStatement*> statements;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::optional<nodeRoot> parse();
private:
    [[nodiscard]] std::optional<Token> peek(int offset = 0) const;
    std::optional<nodeExpression*> parse_expression();
    std::optional<nodeStatement*> parse_statement();
    std::optional<nodeTermination*> parse_termination();
    std::optional<Token> try_consume(TokenType type);
    std::optional<Token> try_consume(TokenType type, const std::string& err);

    //[[nodiscard]] ExpressionNode parse_binary_expression(const Token& token) const;

    Token consume(); 
    const std::vector<Token> m_tokens;
    int m_index = 0;
    ArenaAllocator m_allocator;
};