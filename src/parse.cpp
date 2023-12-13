#include "parse.hpp"

Parser::Parser(std::vector<Token> tokens)
    :m_tokens(std::move(tokens))
{}

ExpressionNode Parser::parse_expression(const Token& token) const{
    ExpressionNode node = {};
    switch(token.type) {
        case TokenType::int_lit:
            node.var = IntExpressionNode{.value = std::stoi(*token.value)};
            break;
        case TokenType::variable:
            node.var = StringExpressionNode{.value = token.value.value()};
            break;
        case TokenType::_string:
            node.var = StringExpressionNode{.value = token.value.value()};
            break;
        default:
            std::cerr << "Error in expression" << std::endl;
            exit(EXIT_FAILURE);
    }

    return node;
}


NodeRoot Parser::parse() {
    while(peek().has_value()) {

        // return creation
        if(peek().value().type == TokenType::_return) {
            if(peek(1).has_value() && peek(1).value().type == TokenType::int_lit) {
                if(peek(2).has_value() && peek(2).value().type == TokenType::semicolon) {
                    m_root.statements.push_back({.statement = ReturnStatementNode{.identifier = consume(),
                        .expression = ExpressionNode{.var = IntExpressionNode{.value = std::stoi(*consume().value)}}}});
                    consume();
                } else {
                    std::cerr << "Missing `;`" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "No return value" << std::endl;
                exit(EXIT_FAILURE);
            }
            // print creation
        } else if (peek().value().type == TokenType::_print) {
            if(peek(1).has_value() && peek(1).value().type == TokenType::_string) {
                if(peek(2).has_value() && peek(2).value().type == TokenType::semicolon) {
                    m_root.statements.push_back({.statement = PrintStatementNode{.identifier = consume(),
                        .expression = ExpressionNode{.var = StringExpressionNode{.value = consume().value.value()}}}});
                    consume();
                } else {
                    std::cerr << "Missing ';'" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Missing string" << std::endl;
                exit(EXIT_FAILURE);
            }
            // let creation
        } else if (peek().value().type == TokenType::let) {
            LetStatementNode stmt {.identifier = consume()};
            std::vector<ExpressionNode> expressions = {};
            while (peek().has_value() && peek().value().type != TokenType::semicolon) {
                expressions.push_back(parse_expression(consume()));
            }
            consume();
            stmt.expressions = expressions;

            /*
            if (peek(1).has_value() && peek(1).value().type == TokenType::variable) {
                if(peek(2).has_value() && peek(2).value().type == TokenType::int_lit) {
                    if (peek(3).has_value() && peek(3).value().type == TokenType::semicolon) {
                        LetStatementNode stmt {.identifier = consume()};
                        std::vector<ExpressionNode> expressions {ExpressionNode{StringExpressionNode{.value = consume().value.value()}},
                            ExpressionNode{IntExpressionNode{.value = std::stoi(*consume().value)}}};
                        stmt.expressions = expressions;
                        m_root.statements.push_back({.statement = stmt});
                        consume();
                    } else {
                        std::cerr << "Missing ';'" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                } else {
                    std::cerr << "No int given" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Give variable a name" << std::endl;
                exit(EXIT_FAILURE);
            }
            */
        } else {
            std::cerr << "No instruction find" << std::endl;
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
