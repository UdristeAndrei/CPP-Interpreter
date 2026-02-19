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

        bool curTokenIs(TokenTypeEnum tokenType) {
            return curToken.TokenType == tokenType;
        }

        bool peekTokenIs(TokenTypeEnum tokenType) {
            return peekToken.TokenType == tokenType;
        }

        bool expectPeek(TokenTypeEnum tokenType) {
            if (peekTokenIs(tokenType)) {
                nextToken();
                return true;
            } else {
                return false;
            }
        }
    public:
        
        Parser(std::shared_ptr<Lexer> l) {
            myLexer = l;

            nextToken();
            nextToken();
        }
        
        ~Parser() = default;
        
        std::shared_ptr<LetStatement> parseLetStatement() {
            auto letStmt = std::make_shared<LetStatement>();
            letStmt->TokenStatement = curToken;

            if (!expectPeek(TOKEN_IDENT)) {
                return nullptr;
            }

            letStmt->Name = std::make_shared<Identifier>(curToken, curToken.Literal);

            if (!expectPeek(TOKEN_ASSIGN)) {
                return nullptr;
            }

            if (!peekTokenIs(TOKEN_SEMICOLON)) {
                return nullptr;
            }

            return letStmt;
        }

        std::shared_ptr<Statement> parseStatement() {
            switch (curToken.TokenType) {
                case TOKEN_LET:
                    return parseLetStatement();
                
                default:
                    return nullptr;
                }
        }

        std::shared_ptr<Program> parseProgram() {
            auto program = std::make_shared<Program>();

            while (curToken.TokenType != TOKEN_EOF) {
                auto stmt = parseStatement();
                if (stmt != nullptr) {
                    program->statements.push_back(stmt);
                }
                nextToken();
            }
            return program;
        }
};

