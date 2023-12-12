#pragma once
#include "parse.hpp"
#include <sstream>

class Generator {
public:
    Generator(NodeRoot root);
    std::string generate();
private:
    void gen_statement(const StatementNode& node);
    void gen_expression(const ExpressionNode& node);
    const NodeRoot m_root;
    std::stringstream m_output;
};
