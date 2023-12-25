#include "generation.hpp"

Generator::Generator(nodeRoot root)
    :m_root(std::move(root))
{}

void Generator::add_var(Token ident) {
    std::pair<std::string, int> var {ident.value.value(), m_stack_place};
    idents.emplace(var);
}

void Generator::push(const std::string &reg) {
    m_stack_place++;
    m_output << "    str " << reg << ", [sp, " << m_stack_place << "]\n";
}

void Generator::push(const std::string &reg, int stack_place) {
    m_output << "    str " << reg << ", [sp, " << stack_place << "]\n";
}

void Generator::pop(const std::string &reg, Token ident) {
    for(auto& it : idents) {
        if (it.first == ident.value.value()) {
            m_output << "    ldr " << reg << ", [sp, " << it.second << "]\n";
            m_stack_place--;
            return;
        }
    }
    std::cerr << "Unknown identifier : " << ident.value.value() << std::endl;
    exit(EXIT_FAILURE);
}

void Generator::pop(const std::string& reg) {
    m_output << "    ldr " << reg << ", [sp, " << m_stack_place << "]\n";
    m_stack_place--;
}

void Generator::gen_termination(const nodeTermination *node) {
    struct termination_visitor {
        Generator* generator;
        void operator()(const nodeTerminationIdentifier* node) {
            for(auto& it : generator->idents) {
                if(it.first == node->identifier.value.value()) {
                    generator->push("x0", it.second);
                    return;
                }
            }
            generator->push("x0");
            generator->add_var(node->identifier);
        }
        void operator()(const nodeTerminationIntLit* node) {
            generator->m_output << "    mov x0, " << node->int_lit.value.value() << "\n";
            generator->push("x0");
        }
    };

    termination_visitor visitor {.generator = this};
    std::visit(visitor, node->var);
}

void Generator::gen_binary_expression(const nodeBinaryExpression* node) {
    struct binary_expression_visitor {
        Generator* generator;
        void operator()(const nodeBinaryExpressionAdd* node) {
            generator->gen_expression(node->left_expression);
            generator->gen_expression(node->right_expression);
            generator->pop("x0");
            generator->pop("x1");
            generator->m_output << "    add x0, x0, x1\n";
        }
    };

    binary_expression_visitor visitor {.generator = this};
    std::visit(visitor, node->var);
}

void Generator::gen_expression(const nodeExpression* node) {
    struct expression_visitor {
        Generator *generator;
        void operator()(const nodeBinaryExpression* node) const {
            generator->gen_binary_expression(node);
        }
        void operator()(const nodeTermination* node) const {
            generator->gen_termination(node);
        }
    };

    expression_visitor visitor {.generator = this};
    std::visit(visitor, node->var);
}

void Generator::gen_statement(const nodeStatement* stmt) {
    struct statement_visitor {
        Generator* generator;
        void operator()(const nodeReturnStatement* return_statement) const{
            generator->gen_expression(return_statement->expression);
            generator->m_output << "    mov x16, 1\n    svc 128\n";
        }
        void operator()(const nodeLetStatement* let_statement) const{
            generator->gen_expression(let_statement->expression);
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

    return m_output.str();
}