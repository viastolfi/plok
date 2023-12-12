#pragma once
#include "parse.hpp"
#include <sstream>

class Generator {
public:
    Generator(NodeRoot root);
    std::string generate();
private:
    const NodeRoot m_root;
    int m_index = 0;
    std::optional<StatementNode> peek(int offset = 0) const;
    StatementNode consume();
};
