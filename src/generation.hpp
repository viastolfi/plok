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
    void push(const std::string& reg, Token ident);
    void pop(const std::string& reg, Token ident);

    const nodeRoot m_root;
    std::stringstream m_output;
    int stack_place = 0;
    std::unordered_map<std::string, int> idents;
};
