#include "lexer.h"
#include "ast.h"
#include<sstream>


class Parser {
    private:
        std::shared_ptr<Lexer> myLexer{};
        Token curToken{};
        Token peekToken{};
        std::vector<std::string> errors{};

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
                peekError(tokenType);
                return false;
            }
        }

        void peekError(TokenTypeEnum tokenType) {
            std::ostringstream error{};
            error << "expected next token to be " << tokenType << ", got " << peekToken.TokenType << " instead";
            errors.push_back(error.str());
        }

    public:
        Parser(std::shared_ptr<Lexer> l) {
            myLexer = l;

            nextToken();
            nextToken();
        }
        ~Parser() = default;

        std::shared_ptr<LetStatement> parseLetStatement() {
            auto letStmt = std::make_shared<LetStatement>(curToken);

            if (!expectPeek(TOKEN_IDENT)) {
                return nullptr;
            }

            letStmt->Name = std::make_shared<Identifier>(curToken, curToken.Literal);

            if (!expectPeek(TOKEN_ASSIGN)) {
                return nullptr;
            }

            while (!curTokenIs(TOKEN_SEMICOLON)) {
                nextToken();
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

        std::vector<std::string>& Errors() { return errors; }
};

