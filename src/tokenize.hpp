#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <optional>

enum class TokenType {
    _return,
    int_lit,
    semicolon,
    variable,
    let,
    equal,
    plus,
    sub,
};

struct Token {
    TokenType type;
    std::optional<std::string> value = {};
};

class Tokenizer {
public:
    explicit Tokenizer(std::string content);
    std::vector<Token> tokenize();
private:
    [[nodiscard]] std::optional<char> peek(int offset = 0) const;
    char consume();
    int m_index = 0;
    const std::string m_content;
};