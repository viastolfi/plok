#pragma once

#include "tokenize.hpp"

struct InstructionNode {
    Token token;
};

struct IntValueNode {
    int value;
};

struct StringValueNode {
    std::string value;
};

struct StatementNode {
    InstructionNode instruction;
    std::variant<IntValueNode, StringValueNode> value;
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
