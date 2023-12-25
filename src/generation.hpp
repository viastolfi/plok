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
    void add_var(Token ident);

    void pop(const std::string& reg, Token ident);
    void pop(const std::string& reg);

    const nodeRoot m_root;
    std::stringstream m_output;
    int stack_place = -1;
    std::unordered_map<std::string, int> idents;
};
