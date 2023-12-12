#include "generation.hpp"

Generator::Generator(NodeRoot root)
    :m_root(root)
{}

std::optional<StatementNode> Generator::peek(int offset) const {
    if(m_index + offset >= m_root.statements.size()) {
        return {};
    } else {
        return m_root.statements.at(m_index + offset);
    }
}

StatementNode Generator::consume() {
    return m_root.statements.at(m_index++);
}

std::string Generator::generate(){
    std::stringstream stringstream;
    stringstream << ".global _main\n.align 2\n_main:\n";
    while(peek().has_value()) {
        if(peek().value().instruction.token.type == TokenType::_return) {
            stringstream << "    mov x0, " << peek().value().value.value << "\n    mov x16, 1\n    svc 128";
            consume();
        }
    }

    return stringstream.str();
}

