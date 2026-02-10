#include "../lexer.h"
#include <vector>

void TestNextToken() {
    std::string input = R"(let five = 5;
                        let ten = 10;

                        let add = fn(x, y) {
                            x + y;
                        };

                        let result = add(five, ten);)";

    std::vector<Token> testCase = { 
        {TOKEN_LET, "let"},
        {TOKEN_IDENT, "five"},
        {TOKEN_ASSIGN, "="},
        {TOKEN_INT, "5"},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_LET, "let"},
        {TOKEN_IDENT, "ten"},
        {TOKEN_ASSIGN, "="},
        {TOKEN_INT, "10"},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_LET, "let"},
        {TOKEN_IDENT, "add"},
        {TOKEN_ASSIGN, "="},
        {TOKEN_FUNCTION, "fn"},
        {TOKEN_LPAREN, "("},
        {TOKEN_IDENT, "x"},
        {TOKEN_COMMA, ","},
        {TOKEN_IDENT, "y"},
        {TOKEN_RPAREN, ")"},
        {TOKEN_LBRACE, "{"},
        {TOKEN_IDENT, "x"},
        {TOKEN_PLUS, "+"},
        {TOKEN_IDENT, "y"},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_RBRACE, "}"},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_LET, "let"},
        {TOKEN_IDENT, "result"},
        {TOKEN_ASSIGN, "="},
        {TOKEN_IDENT, "add"},
        {TOKEN_LPAREN, "("},
        {TOKEN_IDENT, "five"},
        {TOKEN_COMMA, ","},
        {TOKEN_IDENT, "ten"},
        {TOKEN_RPAREN, ")"},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_EOF, ""}
    };

    std::shared_ptr<Lexer> l = newLexer(input);
    int i{0};
    for (const auto testToken : testCase){
        Token tok = nextToken(l);

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