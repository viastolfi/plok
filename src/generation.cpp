#include "generation.hpp"

Generator::Generator(nodeRoot root)
    :m_root(std::move(root))
{}
void Generator::push(const std::string &reg, Token ident){
    std::pair<std::string, int> var{ident.value.value(), stack_place};
    idents.emplace(var);
    m_output << "    str " << reg << ", [sp, " << stack_place << "]\n";
    stack_place++;
}

void Generator::pop(const std::string &reg, Token ident) {
    for(auto& it : idents) {
        if (it.first == ident.value.value()) {
            m_output << "    ldr " << reg << ", [sp, " << it.second << "]\n";
            return;
        }
    }
    std::cerr << "Unknown identifier : " << ident.value.value() << std::endl;
    exit(EXIT_FAILURE);
}

void Generator::gen_termination(const nodeTermination *node) {
    struct termination_visitor {
        Generator* generator;
        void operator()(const nodeTerminationIdentifier* node) {
            generator->pop("x0", node->identifier);
        }
        void operator()(const nodeTerminationIntLit* node) {
            generator->m_output << "    mov x0, " << node->int_lit.value.value() << "\n";
        }
    };

    termination_visitor visitor {.generator = this};
    std::visit(visitor, node->var);
}

void Generator::gen_expression(const nodeExpression* node) {
    struct expression_visitor {
        Generator *generator;
        void operator()(const nodeBinaryExpression* node) const {
            // TODO
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
            generator->push("x0", let_statement->idetifier);
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