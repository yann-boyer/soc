#include "Lexer.hpp"

Lexer::Lexer() {
    m_positionInCode = 0;
    m_code.resize(0);
}

void Lexer::Fill(const std::string& code) {
    for (std::size_t i = 0; i < code.size(); i++) {
        m_code.push_back(code[i]);
    }
}

bool Lexer::IsValidBrainfuckInstruction(char instruction) {
    std::string valid = "><+-.,[]";
    if (valid.find(instruction) != std::string::npos)
        return true;
    else
        return false;
}

char Lexer::Next() {
    while (m_positionInCode < m_code.size() && !IsValidBrainfuckInstruction(m_code[m_positionInCode]))
        m_positionInCode++;

    if (m_positionInCode >= m_code.size()) return 0;
    
    return m_code[m_positionInCode++];
}