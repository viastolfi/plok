#include "generation.hpp"

Generator::Generator(nodeRoot root)
    :m_root(std::move(root))
{}

// TODO :
// Reimplement the stack usage
// Allocate space depending on how many bytes I need
// Store the data depending on the number of bytes
// Keep in mind the place of every variable
// Get them with their place
// Place value in the stack depending on the number of bytes they take

void Generator::push(const size_t& stack_place) {
    m_output << "    str w0, [sp, #" << 32 - stack_place * 4 << "] \n";
    m_stack_place++;
}

void Generator::pop(const size_t& stack_place) {
    m_output << "    ldr w0, [sp, #" << 32 - stack_place * 4 << "]\n";
}

void Generator::pop(std::string reg,const size_t& stack_place) {
    m_output << "    ldr " << reg << ", [sp, #" << 32 - stack_place * 4 << "]\n";
}

void Generator::pop(std::string reg, std::pair<TokenType, std::string> value) {
    switch (value.first) {
        case TokenType::int_lit:
            m_output << "    mov " << reg << ", #" << value.second << std::endl;
            break;
        case TokenType::variable:
            m_output << "    ldr " << reg << ", [sp, #" << 32 - (std::stoi(value.second) * 4) << "]\n";
            break;
        default:
            // TODO
    }
}



std::optional<std::pair<TokenType, std::string>> Generator::gen_termination(const nodeTermination *node) {
    struct termination_visitor {
        Generator* generator;
        std::optional<std::pair<TokenType, std::string>> operator()(const nodeTerminationIdentifier* node) const{
            auto it = std::find_if(generator->m_idents.cbegin(), generator->m_idents.cend(), [&](const Var& var){
                return var.name == node->identifier.value.value();
            });
            if(it == generator->m_idents.cend()) {
                std::cerr << "unknown identifier : " << node->identifier.value.value() << std::endl;
            }
            return std::make_pair(node->identifier.type, std::to_string(static_cast<Var>((*it)).stack_place));
        }
        std::optional<std::pair<TokenType, std::string>> operator()(const nodeTerminationIntLit* node) {
            return std::make_pair(node->int_lit.type, node->int_lit.value.value());
        }
    };

    termination_visitor visitor {.generator = this};
    return std::visit(visitor, node->var);
}

void Generator::gen_binary_expression(const nodeBinaryExpression* node) {
    struct binary_expression_visitor {
        Generator* generator;
        void operator()(const nodeBinaryExpressionAdd* node) const{
            std::optional<std::pair<TokenType, std::string>> right_value = generator->gen_expression(node->right_expression);
            std::optional<std::pair<TokenType, std::string>> left_value = generator->gen_expression(node->left_expression);
            // generator->m_output << "    add w0, w0, #" << right_value.value() << "\n";
        }
        void operator()(const nodeBinaryExpressionSub* node) const{
            std::optional<std::pair<TokenType, std::string>> right_value = generator->gen_expression(node->right_expression);
            std::optional<std::pair<TokenType, std::string>> left_value = generator->gen_expression(node->left_expression);

            generator->pop("w0", left_value.value());
            generator->pop("w1", right_value.value());
            generator->m_output << "    sub w0, w0, w1\n";
        }
    };

    binary_expression_visitor visitor {.generator = this};
    std::visit(visitor, node->var);
}

std::optional<std::pair<TokenType, std::string>> Generator::gen_expression(const nodeExpression* node) {
    struct expression_visitor {
        Generator *generator;
        std::optional<std::pair<TokenType, std::string>> operator()(const nodeBinaryExpression* node) const {
            generator->gen_binary_expression(node);
            return {};
        }
        std::optional<std::pair<TokenType, std::string>> operator()(const nodeTermination* node) const {
            return generator->gen_termination(node);
        }
    };

    expression_visitor visitor {.generator = this};
    return std::visit(visitor, node->var);
}

void Generator::gen_statement(const nodeStatement* stmt) {
    struct statement_visitor {
        Generator* generator;
        void operator()(const nodeReturnStatement* return_statement) const{
            std::optional<std::pair<TokenType, std::string>> value = generator->gen_expression(return_statement->expression);
            generator->pop("w0", value.value());
            generator->m_output << "    ret\n";
        }
        void operator()(const nodeLetStatement* let_statement) const{
            generator->m_idents.push_back({.name = let_statement->idetifier.value.value(), .stack_place = generator->m_stack_place});
            std::optional<std::pair<TokenType, std::string>> value = generator->gen_expression(let_statement->expression);
            if(value.has_value()) { generator->pop("w0" , value.value()); }
            generator->push(generator->m_stack_place);
        }
    };

    statement_visitor visitor {.generator = this};
    std::visit(visitor, stmt->var);
}

std::string Generator::generate(){
    m_output << ".global _main\n.align 2\n_main:\n    sub sp, sp, 32\n";

    for (const nodeStatement* stmt: m_root.statements) {
        gen_statement(stmt);
    }

    m_output << "    add sp, sp, 32\n";

    return m_output.str();
}