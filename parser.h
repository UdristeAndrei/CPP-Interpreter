#include "lexer.h"
#include "ast.h"

struct Parser {
    std::shared_ptr<Lexer> l;
    Token curToken;
    Token peekToken;

    void nextTokenParser() {
        curToken = peekToken;
        peekToken = nextToken(l);
    }

    std::shared_ptr<Parser> newParser(std::shared_ptr<Lexer> l) {
        std::shared_ptr<Parser> p = std::make_shared<Parser>();
        p->l = l;

        nextTokenParser();
        nextTokenParser();
    }
};

