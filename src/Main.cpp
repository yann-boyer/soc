#include <iostream>
#include <cstdlib>
#include "Interpreter.hpp"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cerr << "[ERROR] Usage : ./soc <program.bf>\n";
        std::cerr << "[ERROR] No program provided !\n";
        return EXIT_FAILURE;
    }

    std::string programPath = std::string(argv[1]);

    Interpreter myInterpreter;

    try {
        myInterpreter.LoadProgramFromFile(programPath);
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    myInterpreter.Interpret();

    return EXIT_SUCCESS;
}