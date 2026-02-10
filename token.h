#include<string>
#include<map>

enum TokenTypeEnum {
    TOKEN_INIT = 0,

    // Ilegal token
    TOKEN_ILLEGAL = 100,
    TOKEN_EOF     = 101,

    // Identifiers + literals
    TOKEN_IDENT = 200, // add, foobar, x, y, ...
    TOKEN_INT   = 201,   // 1343456

    // Operators
    TOKEN_ASSIGN = 300,
    TOKEN_PLUS   = 301,

    // Delimiters
    TOKEN_COMMA      = 400,
    TOKEN_SEMICOLON = 401,

    TOKEN_LPAREN = 402, // (
    TOKEN_RPAREN = 403, // )
    TOKEN_LBRACE = 404, // {
    TOKEN_RBRACE = 405, // }

    // Keywords
    TOKEN_FUNCTION = 500,
    TOKEN_LET      = 501
};

std::map<char, TokenTypeEnum> charToEnum { 
    {'=', TOKEN_ASSIGN},
    {'+', TOKEN_PLUS},
    {'(', TOKEN_LPAREN},
    {')', TOKEN_RPAREN},
    {'{', TOKEN_LBRACE},
    {'}', TOKEN_RBRACE},
    {',', TOKEN_COMMA},
    {';', TOKEN_SEMICOLON},
    {0, TOKEN_EOF}, 
};

std::map<std::string, TokenTypeEnum> keywords {
    {"fn", TOKEN_FUNCTION},
    {"let", TOKEN_LET}
};

struct Token {
    TokenTypeEnum TokenType{TOKEN_INIT};
    std::string Literal{};
};

Token newToken(TokenTypeEnum tokenType, char& ch) {
    return Token{tokenType, std::string(1, ch)};
}

TokenTypeEnum lookupIdent(std::string& ident) {
    return (keywords.find(ident) != keywords.end()) ? keywords[ident] : TOKEN_IDENT;
}