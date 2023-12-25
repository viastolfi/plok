#include "parse.hpp"

Parser::Parser(std::vector<Token> tokens)
    :m_tokens(std::move(tokens))
    , m_allocator(1024 * 1024 * 4) // 4 mb
{}

std::optional<Token> Parser::try_consume(TokenType type, const std::string& err) {
    if(peek().has_value() && peek().value().type == type) {
        return consume();
    }
    else {
        std::cerr << err << std::endl;
        exit(EXIT_FAILURE);
    }
}


std::optional<Token> Parser::try_consume(TokenType type) {
    if(peek().has_value() && peek().value().type == type) {
        return consume();
    } else {
        return {};
    }
}


std::optional<nodeTermination *> Parser::parse_termination() {
    if(auto int_lit = try_consume(TokenType::int_lit)) {
        auto term_int_lit = m_allocator.alloc<nodeTerminationIntLit>();
        term_int_lit->int_lit = int_lit.value();
        auto term = m_allocator.alloc<nodeTermination>();
        term->var = term_int_lit;
        return term;
    }
    else if (auto identifier = try_consume(TokenType::variable)) {
        auto term_indentifier = m_allocator.alloc<nodeTerminationIdentifier>();
        term_indentifier->identifier = identifier.value();
        auto term = m_allocator.alloc<nodeTermination>();
        term->var = term_indentifier;
        return term;
    } else {
        return {};
    }
}


std::optional<nodeExpression*> Parser::parse_expression(){
    std::optional<nodeTermination*> left_term = parse_termination();
    if(!left_term.has_value()) {
        return {};
    }
    auto left_expression = m_allocator.alloc<nodeExpression>();
    left_expression->var = left_term.value();

    while(true) {
        std::optional<Token> current_token = peek();

        if(!current_token.has_value() || current_token.value().type == TokenType::semicolon) {
            break;
        }

        Token _operator = consume();

        auto right_term = parse_expression();
        if(!right_term.has_value()) {
            std::cerr << "Invalid expression" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto expression = m_allocator.alloc<nodeBinaryExpression>();
        auto left_term_2 = m_allocator.alloc<nodeExpression>();

        if(_operator.type == TokenType::plus) {
            auto add = m_allocator.alloc<nodeBinaryExpressionAdd>();
            left_term_2->var = left_expression->var;
            add->left_expression = left_term_2;
            add->right_expression = right_term.value();
            expression->var = add;
        }
        else {
            assert(false);
        }

        left_expression->var = expression;
    }
    return left_expression;
}

std::optional<nodeStatement*> Parser::parse_statement() {
    if(peek().value().type == TokenType::_return) {
        auto return_stmt = m_allocator.alloc<nodeReturnStatement>();
        consume();
        if(auto expression = parse_expression()) {
            return_stmt->expression = expression.value();
            auto stmt = m_allocator.alloc<nodeStatement>();
            stmt->var = return_stmt;
            try_consume(TokenType::semicolon, "expected `;`");
            return stmt;
        }
    }
    else if (peek().value().type == TokenType::let) {
        auto let_stmt = m_allocator.alloc<nodeLetStatement>();
        consume();
        let_stmt->idetifier = consume();
        try_consume(TokenType::equal);
        if(auto expression = parse_expression()) {
            let_stmt->expression = expression.value();
            auto stmt = m_allocator.alloc<nodeStatement>();
            stmt->var = let_stmt;
            try_consume(TokenType::semicolon, "expected `;`");
            return stmt;
        }
    }
    else {
        std::cerr << "Invalid Expression" << std::endl;
        exit(EXIT_FAILURE);
    }
}


std::optional<nodeRoot> Parser::parse() {
    nodeRoot root;
    while(peek().has_value()) {
        if(auto stmt = parse_statement()) {
            root.statements.push_back(stmt.value());
        } else {
            std::cerr << "Invalide statement" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return root;
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
