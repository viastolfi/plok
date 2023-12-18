#include "parse.hpp"

Parser::Parser(std::vector<Token> tokens)
    :m_tokens(std::move(tokens))
{}

std::optional<nodeExpression> Parser::parse_expression(){
   // TODO
}

std::optional<nodeStatement> Parser::parse_statement() {
    if(peek().value().type == TokenType::_return) {
        auto stmt = nodeReturnStatement{.identifier = consume()};
        if(auto expression = parse_expression()) {
            stmt.expression = expression.value();
        } else {
            std::cerr << "Invalid Expression" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}


NodeRoot Parser::parse() {
    while(peek().has_value()) {
        if(auto stmt = parse_statement()) {
            m_root.statements.push_back(stmt.value);
        } else {
            std::cerr << "Invalide statement" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return m_root;
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
