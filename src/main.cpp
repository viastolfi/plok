#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "generation.hpp"
#include "parse.hpp"
#include "tokenize.hpp"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Bad usage : plok your_file.pk" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    std::vector<Token> tokens;
    {
        Tokenizer tokenizer(contents);
        tokens = tokenizer.tokenize();
    }

    nodeRoot prog;
    {
        Parser parser(tokens);
        prog = parser.parse().value();
    }

    std::string asm_string;
    {
        Generator generator(prog);
        asm_string = generator.generate();
    }

    {
        std::fstream file("out.s", std::ios::out);
        file << asm_string;
    }

    system("as -o out.o out.s");
    system("ld out.o -o out");

    return EXIT_SUCCESS;
}