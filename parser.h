#include "lexer.h"
#include "ast.h"

class Parser {
    private:
        std::shared_ptr<Lexer> myLexer;
        Token curToken;
        Token peekToken;

        void nextToken() {
            curToken = peekToken;
            peekToken = myLexer->nextToken();
        }

    public:
        
        Parser(std::shared_ptr<Lexer> l) {
            myLexer = l;

            nextToken();
            nextToken();
        }
        
        ~Parser() = default;
        
        std::shared_ptr<Program> ParseProgram() {
            return nullptr;
        }
};

