#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <cstdint>

class Lexer {
    public:
        Lexer();

        void Fill(const std::string& code);
        char Next();
    private:
        bool IsValidBrainfuckInstruction(char instruction);

        uint64_t m_positionInCode;
        std::vector<char> m_code;
};

#endif