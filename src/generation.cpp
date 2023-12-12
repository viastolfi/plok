#include "generation.hpp"

Generator::Generator(NodeRoot root)
    :m_root(root)
{}

std::string Generator::space2underscore(std::string in) {
    size_t size = 0;
    for(size_t i = 0; i < in.length(); i++) {
        if(std::isspace(in[i])) {
            in[i] = '_';
        }
        if(std::isalnum(in[i]) || in[i] == '_') {
            in[i] = std::tolower(in[i]);
            in[size++] = in[i];
        }
    }
    in.resize(size);
    return in;
}

void Generator::gen_expression(const ExpressionNode& node) {
    struct expression_visitor {
        Generator* generator;
        void operator()(const IntExpressionNode& node) {
            generator->m_output << "    mov x0, " << node.value << "\n";
        }
        void operator()(const StringExpressionNode& node) {
            std::string key = generator->space2underscore(node.value);
            generator->datas.insert({key, node.value});
            generator->m_output << "    mov x0, 1\n"
                                "    adrp x1, " << key << "@PAGE\n"
                                "    add x1, x1, " << key << "@PAGEOFF\n"
                                "    mov x2, " << node.value.length() + 1 << "\n";

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
            generator->m_output << "    mov x16, 1\n    svc 128\n";
        }
        void operator()(const PrintStatementNode print_statement) {
            generator->gen_expression(print_statement.expression);
            generator->m_output << "    mov x16, 4\n    svc 128\n";
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

    if (datas.size() != 0) {
        m_output << ".data\n";
        for (const std::pair<const std::string, const std::string>& data : datas) {
            m_output << data.first << ": .ascii \"" << data.second << "\\n\"\n";
        }
    }

    return m_output.str();
}