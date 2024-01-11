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

void Generator::push_var(const size_t& stack_place) {
    m_output << "    ldr x0, [sp, " << stack_place << "] \n";
    m_output << "    stp x0, xzr, [sp, -16]!\n";
    m_stack_place++;
}

void Generator::push(const std::string &reg) {
    m_output << "    stp " << reg << ", xzr, [sp, -16]!\n";
    m_stack_place++;
}

void Generator::pop(const std::string& reg) {
    m_output << "    ldr " << reg << ", [sp], 16\n";
    m_stack_place--;
}

void Generator::gen_termination(const nodeTermination *node) {
    struct termination_visitor {
        Generator* generator;
        void operator()(const nodeTerminationIdentifier* node) const{
            auto it = std::find_if(generator->m_idents.cbegin(), generator->m_idents.cend(), [&](const Var& var){
                return var.name == node->identifier.value.value();
            });
            if(it == generator->m_idents.cend()) {
                std::cerr << "unknown identifier : " << node->identifier.value.value() << std::endl;
            }
            generator->push_var(static_cast<Var>((*it)).stack_place);
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
        void operator()(const nodeBinaryExpressionAdd* node) const{
            generator->gen_expression(node->right_expression);
            generator->gen_expression(node->left_expression);
            generator->pop("x0");
            generator->pop("x1");
            generator->m_output << "    add x0, x0, x1\n";
            generator->push("x0");
        }
        void operator()(const nodeBinaryExpressionSub* node) const{
            generator->gen_expression(node->right_expression);
            generator->gen_expression(node->left_expression);
            generator->pop("x0");
            generator->pop("x1");
            generator->m_output << "    sub x0, x0, x1\n";
            generator->push("x0");
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
            generator->m_idents.push_back({.name = let_statement->idetifier.value.value(), .stack_place = generator->m_stack_place});
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