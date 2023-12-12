#include "generation.hpp"

Generator::Generator(NodeRoot root)
    :m_root(root)
{}

void Generator::gen_expression(const ExpressionNode& node) {
    struct expression_visitor {
        Generator* generator;
        void operator()(const IntExpressionNode& node) {
            generator->m_output << "    mov x0, " << node.value << "\n";
        }
        void operator()(const StringExpressionNode& node) {
            // TODO
        }
    };

    expression_visitor visitor {.generator = this};
    std::visit(visitor, node.var);
}


void Generator::gen_statement(const StatementNode& node) {
    struct statement_visitor {
        Generator* generator;
        void operator()(const ReturnStatementNode return_statement) {
            generator->gen_expression(return_statement.expression);
            generator->m_output << "    mov x16, 1\n    svc 128";
        }
        void operator()(const PrintStatementNode print_statement) {
            // TODO
        }
    };

    statement_visitor visitor {.generator = this};
    std::visit(visitor, node.statement);
}


std::string Generator::generate(){
    m_output << ".global _main\n.align 2\n_main:\n";

    for (const StatementNode node: m_root.statements) {
        gen_statement(node);
    }

    return m_output.str();
}