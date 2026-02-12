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

        std::shared_ptr<Lexer> l = newLexer(line);

        for (Token tok = nextToken(l); tok.TokenType != TOKEN_EOF; tok = nextToken(l)) {
            std::cout << "RokenType - " << tok.TokenType;
            std::cout << ", Literal - " << tok.Literal << "\n";
        }
    }
}