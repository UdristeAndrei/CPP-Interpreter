#pragma once
#include<memory>
#include "token.h"


class Lexer{
    private:
        std::string input{};
        unsigned int position{0};
        unsigned int readPosition{0};
        char ch{0};

        void readChar() {
            if (readPosition >= input.length()) {
                ch = 0;
            } else {
                ch = input.at(readPosition);
            }

            position = readPosition;
            readPosition++;
        }

        void skipWhitespace() {
            while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'){
                readChar();
            }
        }

        bool isLetter(char c) {
            return std::isalpha(c) || c == '_';
        }

        char peekChar() {
            if (readPosition >= input.length()){
                return 0;
            } else {
                return input[readPosition];
            }
        }

        std::string readIdentifier() {
            unsigned int startPosition = position;
            while (isLetter(ch)) {
                readChar();
            }
            return input.substr(startPosition, position - startPosition);
        }

        std::string readNumber() {
            unsigned int startPosition = position;
            while(std::isalnum(ch)) {
                readChar();
            }
            return input.substr(startPosition, position - startPosition);
        }

    public:

        Lexer(std::string& input) {
            this->input = input;
            readChar();
        }
        ~Lexer() = default;

        Token nextToken () {
            Token tok{};
            skipWhitespace();

            switch (charToEnum[ch]) {
                case TOKEN_ASSIGN: 
                    if (peekChar() == '=') {
                        char ch = this->ch;
                        readChar();
                        tok = Token{TOKEN_EQ, std::string(1, ch) + this->ch};
                    } else {
                        tok = newToken(TOKEN_ASSIGN, ch);
                    }
                    break;

                case TOKEN_SEMICOLON:
                    tok = newToken(TOKEN_SEMICOLON, ch);
                    break;

                case TOKEN_LPAREN:
                    tok = newToken(TOKEN_LPAREN, ch);
                    break;

                case TOKEN_RPAREN:
                    tok = newToken(TOKEN_RPAREN, ch);
                    break;

                case TOKEN_COMMA:
                    tok = newToken(TOKEN_COMMA, ch);
                    break;

                case TOKEN_PLUS:
                    tok = newToken(TOKEN_PLUS, ch);
                    break;

                case TOKEN_MINUS:
                    tok = newToken(TOKEN_MINUS, ch);
                    break;

                case TOKEN_BANG:
                    if (peekChar() == '=') {
                        char ch = this->ch;
                        readChar();
                        tok = Token{TOKEN_NOT_EQ, std::string(1, ch) + this->ch};
                    } else {
                        tok = newToken(TOKEN_BANG, ch);
                    }
                    break;

                case TOKEN_SLASH:
                    tok = newToken(TOKEN_SLASH, ch);
                    break;

                case TOKEN_ASTERISK:
                    tok = newToken(TOKEN_ASTERISK, ch);
                    break;

                case TOKEN_LT:
                    tok = newToken(TOKEN_LT, ch);
                    break;

                case TOKEN_GT:
                    tok = newToken(TOKEN_GT, ch);
                    break;
                
                case TOKEN_LBRACE:
                    tok = newToken(TOKEN_LBRACE, ch);
                    break;

                case TOKEN_RBRACE:
                    tok = newToken(TOKEN_RBRACE, ch);
                    break;

                case TOKEN_EOF:
                    tok.Literal   = "";
                    tok.TokenType = TOKEN_EOF;
                    break;

                default:
                    if (isLetter(ch)) {
                        tok.Literal = readIdentifier();
                        tok.TokenType = lookupIdent(tok.Literal);
                        return tok;

                    } else if (std::isalnum(ch)){
                        tok.Literal = readNumber();
                        tok.TokenType = TOKEN_INT;
                        return tok;
                    } else {
                        tok = newToken(TOKEN_ILLEGAL, ch);
                    }
                }

            readChar();
            return tok;
        }
};



