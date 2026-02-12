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
    TOKEN_ASSIGN    = 300,
    TOKEN_PLUS      = 301,
    TOKEN_MINUS     = 302,
    TOKEN_BANG      = 303, // !
    TOKEN_ASTERISK  = 304,
    TOKEN_SLASH     = 305,

    TOKEN_LT        = 306, // <
    TOKEN_GT        = 307, // >
    TOKEN_EQ        = 308, // ==
    TOKEN_NOT_EQ    = 309, // !=

    // Delimiters
    TOKEN_COMMA     = 400,
    TOKEN_SEMICOLON = 401,

    TOKEN_LPAREN = 402, // (
    TOKEN_RPAREN = 403, // )
    TOKEN_LBRACE = 404, // {
    TOKEN_RBRACE = 405, // }

    // Keywords
    TOKEN_FUNCTION  = 500,
    TOKEN_LET       = 501,
    TOKEN_TRUE      = 502,
    TOKEN_FALSE     = 503,
    TOKEN_IF        = 504,
    TOKEN_ELSE      = 505,
    TOKEN_RETURN    = 505
};

std::map<char, TokenTypeEnum> charToEnum { 
    {'=', TOKEN_ASSIGN},
    {'+', TOKEN_PLUS},
    {'-', TOKEN_MINUS},
    {'!', TOKEN_BANG},
    {'*', TOKEN_ASTERISK},
    {'/', TOKEN_SLASH},
    {'<', TOKEN_LT},
    {'>', TOKEN_GT},
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
    {"let", TOKEN_LET},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},
    {"return", TOKEN_RETURN}
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