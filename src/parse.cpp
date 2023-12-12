#include "parse.hpp"

Parser::Parser(std::vector<Token> tokens)
    :m_tokens(std::move(tokens))
{}

NodeRoot Parser::parse() {
    NodeRoot out;

    while(peek().has_value()) {

        // return creation
        if(peek().value().type == TokenType::_return) {
            if(peek(1).has_value() && peek(1).value().type == TokenType::int_lit) {
                if(peek(2).has_value() && peek(2).value().type == TokenType::semicolon) {
                    out.statements.push_back({.instruction = {.token = consume()}, .value = {.value = stoi(*consume().value)}});
                    consume();
                } else {
                    std::cerr << "Missing `;`" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "No return value" << std::endl;
                exit(EXIT_FAILURE);
            }
        }


    }

    return out;
}

std::optional<Token> Parser::peek(int offset) const {
    if(m_index + offset >= m_tokens.size()) {
        return {};
    } else {
        return m_tokens.at(m_index + offset);
    }
}

Token Parser::consume() {
    return m_tokens.at(m_index++);
}
