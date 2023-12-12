#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

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

    std::string parsed;
    {
        Parser parser(tokens);
        parsed = parser.parse();
    }

    std::cout << "Hello World !" <<std::endl;

    return EXIT_SUCCESS;
}