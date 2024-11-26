#include "Interpreter.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

Interpreter::Interpreter() {
    m_memoryPointer = 0;
    m_instructionPointer = 0;

    m_memory.fill(0);
    m_program.resize(0);
}

void Interpreter::LoadProgramFromFile(const std::string& programPath) {
    std::ifstream programFile(programPath);

    if (!programFile.is_open())
        throw std::runtime_error("[ERROR] Unable to open the program !");

    char ch;
    std::string programBuffer;
    while (programFile.good()) {
        programFile.get(ch);
        programBuffer.push_back(ch);
    }

    programFile.close();

    m_lexer.Fill(programBuffer);

    char c = m_lexer.Next();

    while (c) {
        IRInstruction inst;
        switch (c) {
            case '>':
            case '<':
            case '+':
            case '-':
                {
                    uint8_t streak = 1;
                    char s = m_lexer.Next();

                    while (c == s) {
                        streak += 1;
                        s = m_lexer.Next();
                    }

                    inst = { static_cast<IRInstructionKind>(c), streak };
                    c = s;
                }
                break;
            case '.':
            case ',':
            case '[':
            case ']':
                {
                    inst = { static_cast<IRInstructionKind>(c), {} };
                    c = m_lexer.Next();
                }
                break;
            default: continue;
        }

        m_program.push_back(inst);
    }
}

void Interpreter::PrecomputeJumps() {
    std::vector<uint64_t> stack;

    uint64_t localInstructionPointer = 0;

    while (localInstructionPointer < m_program.size()) {
        IRInstruction inst = m_program[localInstructionPointer];

        switch (inst.kind) {
            case IRInstructionKind::JumpIfZero: stack.push_back(localInstructionPointer); break;
            case IRInstructionKind::JumpIfNotZero:
                {
                    uint64_t target = stack.back();
                    stack.pop_back();
                    m_jumpMap[localInstructionPointer] = target;
                    m_jumpMap[target] = localInstructionPointer;
                }
                break;
            default: break; // Other instructions aren't jump related.
        }

        localInstructionPointer++;
    }
}

void Interpreter::Interpret() {
    PrecomputeJumps();

    while (m_instructionPointer < m_program.size()) {
        IRInstruction inst = m_program[m_instructionPointer];

        switch (inst.kind) {
            case IRInstructionKind::IncrementPointer: m_memoryPointer += inst.operand.value(); break;
            case IRInstructionKind::DecrementPointer: m_memoryPointer -= inst.operand.value(); break;
            case IRInstructionKind::IncrementByte: m_memory[m_memoryPointer] += inst.operand.value(); break;
            case IRInstructionKind::DecrementByte: m_memory[m_memoryPointer] -= inst.operand.value(); break;
            case IRInstructionKind::PrintByteAsChar:
                {
                    char byteAsChar = static_cast<char>(m_memory[m_memoryPointer]);
                    std::cout << byteAsChar;
                    std::cout.flush();
                }
                break;
            case IRInstructionKind::ReadInputToByte:
                {
                    char inputByte;
                    std::cin >> inputByte;
                    std::cin.clear();
                    m_memory[m_memoryPointer] = static_cast<uint8_t>(inputByte);
                }
                break;
            case IRInstructionKind::JumpIfZero:
                if (m_memory[m_memoryPointer] == 0)
                    m_instructionPointer = m_jumpMap[m_instructionPointer];
                break;
            case IRInstructionKind::JumpIfNotZero:
                if (m_memory[m_memoryPointer] != 0)
                    m_instructionPointer = m_jumpMap[m_instructionPointer];
                break;
        }

        m_instructionPointer++;
    }
}