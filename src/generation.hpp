#pragma once
#include "parse.hpp"
#include <sstream>
#include <unordered_map>

class Generator {
public:
    Generator(nodeRoot root);
    std::string generate();
private:
    void gen_statement(const nodeStatement* node);
    void gen_expression(const nodeExpression* node);
    void gen_termination(const nodeTermination* node);
    void gen_binary_expression(const nodeBinaryExpression* node);

    void push(const std::string& reg);
    void push_var(const size_t& stack_place);
    void pop(const std::string& reg);

    struct Var {
        std::string name;
        size_t stack_place;
    };

    const nodeRoot m_root;
    std::stringstream m_output;
    size_t m_stack_place = 0;
    std::vector<Var> m_idents {};
};
