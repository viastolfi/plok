#pragma once

#include "tokenize.hpp"

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::string parse();
private:
    std::vector<Token> m_tokens;
};
