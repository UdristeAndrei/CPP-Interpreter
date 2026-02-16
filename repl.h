#include "lexer.h"
#include<iostream>

const std::string PROMPT{">>"};

void Start() {
    std::string line{};

    while(true) {
        std::cout << PROMPT;
        if (!std::getline(std::cin, line)) {
            break; // EOF or input error -> exit REPL
        }

        Lexer myLexer{line};

        for (Token tok = myLexer.nextToken(); tok.TokenType != TOKEN_EOF; tok = myLexer.nextToken()) {
            std::cout << "RokenType - " << tok.TokenType;
            std::cout << ", Literal - " << tok.Literal << "\n";
        }
    }
}