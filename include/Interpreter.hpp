#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <cstdint>
#include <array>
#include <vector>
#include <optional>
#include <map>
#include "Lexer.hpp"

enum class IRInstructionKind {
    IncrementPointer = '>',
    DecrementPointer = '<',
    IncrementByte = '+',
    DecrementByte = '-',
    PrintByteAsChar = '.',
    ReadInputToByte = ',',
    JumpIfZero = '[',
    JumpIfNotZero = ']',
};

struct IRInstruction {
    IRInstructionKind kind;
    std::optional<uint8_t> operand;
};

class Interpreter {
    public:
        Interpreter();

        void LoadProgramFromFile(const std::string& programPath);
        void Interpret();
    private:
        void PrecomputeJumps();

        static constexpr uint32_t TOTAL_MEMORY_SIZE = 100000;

        std::array<uint8_t, TOTAL_MEMORY_SIZE> m_memory;
        std::vector<IRInstruction> m_program;
        std::map<uint64_t, uint64_t> m_jumpMap;

        uint64_t m_memoryPointer;
        uint64_t m_instructionPointer;
        Lexer m_lexer;
};

#endif