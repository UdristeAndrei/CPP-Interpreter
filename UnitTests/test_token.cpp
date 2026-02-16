#include "../lexer.h"
#include <vector>
#include <cstdio>

void TestNextToken() {
    std::string input = "=+(){},;";
    std::vector<Token> testCase = { 
        {TOKEN_ASSIGN, "="},
        {TOKEN_PLUS, "+"},
        {TOKEN_LPAREN, "("},
        {TOKEN_RPAREN, ")"},
        {TOKEN_LBRACE, "{"},
        {TOKEN_RBRACE, "}"},
        {TOKEN_COMMA, ","},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_EOF, ""}
    };

    Lexer myLexer{input};
    int i{0};
    for (const auto testToken : testCase){
        Token tok = myLexer.nextToken();

        if (tok.TokenType != testToken.TokenType) {
            printf("tests[%d] - tokentype wrong. expected=%d, got=%d\n", i, testToken.TokenType, tok.TokenType);
        }

        if (tok.Literal != testToken.Literal) {
            printf("tests[%d] - literal wrong. expected=%s, got=%s\n", i, testToken.Literal.c_str(), tok.Literal.c_str());
        }
        i++;
    }
    printf("Test run successfully!\n");
}

int main() {
    TestNextToken();
}