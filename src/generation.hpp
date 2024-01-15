#pragma once
#include "parse.hpp"
#include <sstream>
#include <unordered_map>
#include <utility>

class Generator {
public:
    Generator(nodeRoot root);
    std::string generate();
private:
    void gen_statement(const nodeStatement* node);
    std::optional<std::pair<TokenType, std::string>> gen_expression(const nodeExpression* node);
    std::optional<std::pair<TokenType, std::string>> gen_termination(const nodeTermination* node);
    void gen_binary_expression(const nodeBinaryExpression* node);

    void push(const size_t& stack_place);
    void pop(const size_t& stack_place);
    void pop(std::string reg,const size_t& stack_place);
    void pop(std::string reg, std::pair<TokenType, std::string> value);

    struct Var {
        std::string name;
        size_t stack_place;
    };

    const nodeRoot m_root;
    std::stringstream m_output;
    size_t m_stack_place = 1;
    std::vector<Var> m_idents {};
};
