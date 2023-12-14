#pragma once
#include "parse.hpp"
#include <sstream>
#include <unordered_map>

class Generator {
public:
    Generator(NodeRoot root);
    std::string generate();
private:
    void gen_statement(const StatementNode& node);
    void gen_expression(const ExpressionNode& node);

    const NodeRoot m_root;
    std::stringstream m_output;
    std::unordered_map<std::string, std::string> datas;
    std::string space2underscore(std::string text);


    struct variable {
        std::string name;
        size_t stack_place;
    };
    std::vector<variable> m_variables {};
    TokenType m_actual_scope;
    size_t m_actual_stack_place = 0;
};
