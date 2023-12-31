#include "tokenize.hpp"

Tokenizer::Tokenizer(std::string content)
    :m_content(std::move(content))
{}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens = {};
    std::string buf;
    while(peek().has_value()) {
       if(std::isalpha(peek().value())) {
           buf.push_back(consume());
           while(peek().has_value() && std::isalpha(peek().value())) {
               buf.push_back(consume());
           }
           if(buf == "return") {
               tokens.push_back({.type = TokenType::_return});
               buf.clear();
           } else if (buf == "let") {
               tokens.push_back({.type = TokenType::let});
               buf.clear();
           } else {
                tokens.push_back({.type = TokenType::variable, .value = buf});
                buf.clear();
           }
       } else if (std::isdigit(peek().value())) {
           buf.push_back(consume());
           while(peek().has_value() && std::isdigit(peek().value())) {
               buf.push_back(consume());
           }
           tokens.push_back({.type = TokenType::int_lit, .value = buf});
           buf.clear();
       }
       else if (peek().value() == ';') {
           consume();
           tokens.push_back({.type = TokenType::semicolon});
       }
       else if (peek().value() == '=') {
           tokens.push_back({.type = TokenType::equal});
           consume();
       }
       else if (peek().value() == '+') {
           tokens.push_back({.type = TokenType::plus});
           consume();
       }
       else if (peek().value() == '-') {
           tokens.push_back({.type = TokenType::sub});
           consume();
       }
       else if (std::isspace(peek().value())) {
           consume();
       }
    }
    return tokens;
}

std::optional<char> Tokenizer::peek(int offset) const{
    if(m_index + offset >= m_content.length()) {
        return {};
    } else {
        return m_content.at(m_index + offset);
    }
}

char Tokenizer::consume() {
    return m_content.at(m_index++);
}
