#include<string>
#include<memory>
#include "token.h"


struct Lexer{
    std::string input{};
    unsigned int position{0};
    unsigned int readPosition{0};
    char ch{0};
};

void readChar(std::shared_ptr<Lexer> l) {
    if (l->readPosition >= l->input.length()) {
        l->ch = 0;
    } else {
        l->ch = l->input.at(l->readPosition);
    }

    l->position = l->readPosition;
    l->readPosition++;
}

std::shared_ptr<Lexer> newLexer(std::string& input) {
    std::shared_ptr<Lexer> l = std::make_shared<Lexer>();
    l->input = input;
    readChar(l);
    return l;
}

Token nextToken (std::shared_ptr<Lexer> l) {
    Token tok{};

    switch (charToEnum[l->ch]) {
        case TOKEN_ASSIGN: 
            tok = newToken(TOKEN_ASSIGN, l->ch);
            break;

        case TOKEN_SEMICOLON:
            tok = newToken(TOKEN_SEMICOLON, l->ch);
            break;

        case TOKEN_LPAREN:
            tok = newToken(TOKEN_LPAREN, l->ch);
            break;

        case TOKEN_RPAREN:
            tok = newToken(TOKEN_RPAREN, l->ch);
            break;

        case TOKEN_COMMA:
            tok = newToken(TOKEN_COMMA, l->ch);
            break;

        case TOKEN_PLUS:
            tok = newToken(TOKEN_PLUS, l->ch);
            break;
        
        case TOKEN_LBRACE:
            tok = newToken(TOKEN_LBRACE, l->ch);
            break;

        case TOKEN_RBRACE:
            tok = newToken(TOKEN_RBRACE, l->ch);
            break;

        case 0:
            tok.Literal   = "";
            tok.TokenType = TOKEN_EOF;
            break;
        }

    readChar(l);
    return tok;
}