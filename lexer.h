#pragma once
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

void skipWhitespace(std::shared_ptr<Lexer> l) {
    while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r'){
        readChar(l);
    }
}

std::shared_ptr<Lexer> newLexer(std::string& input) {
    std::shared_ptr<Lexer> l = std::make_shared<Lexer>();
    l->input = input;
    readChar(l);
    return l;
}


char peekChar(std::shared_ptr<Lexer> l) {
    if (l->readPosition >= l->input.length()){
        return 0;
    } else {
        return l->input[l->readPosition];
    }
}

bool isLetter(char c) {
    return std::isalpha(c) || c == '_';
}


std::string readIdentifier(std::shared_ptr<Lexer> l) {
    unsigned int startPosition = l->position;
    while (isLetter(l->ch)) {
        readChar(l);
    }
    return l->input.substr(startPosition, l->position - startPosition);
}

std::string readNumber(std::shared_ptr<Lexer> l) {
    unsigned int startPosition = l->position;
    while(std::isalnum(l->ch)) {
        readChar(l);
    }
    return l->input.substr(startPosition, l->position - startPosition);
}

Token nextToken (std::shared_ptr<Lexer> l) {
    Token tok{};
    skipWhitespace(l);

    switch (charToEnum[l->ch]) {
        case TOKEN_ASSIGN: 
            if (peekChar(l) == '=') {
                char ch = l->ch;
                readChar(l);
                tok = Token{TOKEN_EQ, std::string(1, ch) + l->ch};
            } else {
                tok = newToken(TOKEN_ASSIGN, l->ch);
            }
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

        case TOKEN_MINUS:
            tok = newToken(TOKEN_MINUS, l->ch);
            break;

        case TOKEN_BANG:
            if (peekChar(l) == '=') {
                char ch = l->ch;
                readChar(l);
                tok = Token{TOKEN_NOT_EQ, std::string(1, ch) + l->ch};
            } else {
                tok = newToken(TOKEN_BANG, l->ch);
            }
            break;

        case TOKEN_SLASH:
            tok = newToken(TOKEN_SLASH, l->ch);
            break;

        case TOKEN_ASTERISK:
            tok = newToken(TOKEN_ASTERISK, l->ch);
            break;

        case TOKEN_LT:
            tok = newToken(TOKEN_LT, l->ch);
            break;

        case TOKEN_GT:
            tok = newToken(TOKEN_GT, l->ch);
            break;
        
        case TOKEN_LBRACE:
            tok = newToken(TOKEN_LBRACE, l->ch);
            break;

        case TOKEN_RBRACE:
            tok = newToken(TOKEN_RBRACE, l->ch);
            break;

        case TOKEN_EOF:
            tok.Literal   = "";
            tok.TokenType = TOKEN_EOF;
            break;

        default:
            if (isLetter(l->ch)) {
                tok.Literal = readIdentifier(l);
                tok.TokenType = lookupIdent(tok.Literal);
                return tok;

            } else if (std::isalnum(l->ch)){
                tok.Literal = readNumber(l);
                tok.TokenType = TOKEN_INT;
                return tok;
            } else {
                tok = newToken(TOKEN_ILLEGAL, l->ch);
            }
        }

    readChar(l);
    return tok;
}