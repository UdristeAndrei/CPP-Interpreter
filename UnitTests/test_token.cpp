#include<token.cpp>
#include<vector>

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

    // Lexer l = New_Lexer(input);
    // for (const auto testToke : testCase){
    //     tok = l.
    // }
}