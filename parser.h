#include "lexer.h"
#include "ast.h"
#include<functional>

#define pfnPrefixParseFs std::function<std::shared_ptr<Expression>()>
#define pfninfixParseFs std::function<std::shared_ptr<Expression>(std::shared_ptr<Expression>)>


enum Preccedence {
    _ = 0,
    LOWEST = 1,
    EQUALS = 2,         // ==
    LESSGREATER = 3,    // > or <
    SUM = 4,            // +
    PRODUCT = 5,        // *
    PREFIX = 6,         // -X or !X
    CALL = 6            // foo(x);
};

class Parser {
    private:
        std::shared_ptr<Lexer> myLexer{};
        Token curToken{};
        Token peekToken{};
        std::vector<std::string> errors{};

        std::map<TokenTypeEnum, pfnPrefixParseFs> prefixParseFsn{};
        std::map<TokenTypeEnum, pfninfixParseFs> infixParseFsn{};

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

        void noPrefixParseFnError(TokenTypeEnum tokenType) {
            std::ostringstream error{};
            error << "no prefix parse function for " << tokenType << " found ";
            errors.push_back(error.str());
        }

        std::shared_ptr<Expression> parseExpression(Preccedence preccedence) {
            auto prefix = prefixParseFsn.at(curToken.TokenType);

            if (prefix == nullptr) { 
                noPrefixParseFnError(curToken.TokenType);
                return nullptr; 
            };

            auto leftExp = prefix();
            return leftExp;
        }

        std::shared_ptr<Expression> parseIdendifier() { return std::make_shared<Identifier>(curToken, curToken.Literal); }

        std::shared_ptr<Expression> parseIntegerLiteral() {
            auto lit = std::make_shared<IntegerLiteral>(curToken);
            
            ino64_t value{};
            try{
                value = std::stoi(curToken.Literal);
            } catch (...) {
                std::ostringstream error{};
                error << "could not parse " << curToken.Literal << "as integer";
                errors.push_back(error.str());
            }

            lit->Value = value;
            return lit;
        }

        std::shared_ptr<Expression> parsePrefixExpression() {
            auto expression = std::make_shared<PrefixExpression>(curToken, curToken.Literal);
            nextToken();
            expression->Right = parseExpression(PREFIX);
            return expression;
        }

    public:
        Parser(std::shared_ptr<Lexer> l) {
            myLexer = l;

            nextToken();
            nextToken();

            registerPrefix(TOKEN_BANG, [this]() {return parsePrefixExpression(); });
            registerPrefix(TOKEN_MINUS, [this]() {return parsePrefixExpression(); });
            registerPrefix(TOKEN_IDENT, [this]() { return parseIdendifier(); });
            registerPrefix(TOKEN_INT, [this]() {return parseIntegerLiteral(); });
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

        std::shared_ptr<ReturnStatement> parseReturnStatement() {
            auto returnStmt = std::make_shared<ReturnStatement>(curToken);

            nextToken();

            while (!curTokenIs(TOKEN_SEMICOLON)) {
                nextToken();
            }

            return returnStmt;
        }

        std::shared_ptr<ExpressionStatement> parseExpressionStatemnt() {
            auto expressionStmt = std::make_shared<ExpressionStatement>(curToken);

            expressionStmt->Value = parseExpression(LOWEST);
            
            if (peekTokenIs(TOKEN_SEMICOLON)) { nextToken(); };

            return expressionStmt;
        }

        std::shared_ptr<Statement> parseStatement() {
            switch (curToken.TokenType) {
                case TOKEN_LET:
                    return parseLetStatement();
                    break;

                case TOKEN_RETURN:
                    return parseReturnStatement();
                    break;

                default:
                    return parseExpressionStatemnt();
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

        std::shared_ptr<Expression> prefixParseFn() {}
        std::shared_ptr<Expression> infixParseFn(std::shared_ptr<Expression> expression) {}

        void registerPrefix(TokenTypeEnum tokenType, pfnPrefixParseFs fn) { prefixParseFsn.emplace(tokenType, fn); };
        void registerInfix(TokenTypeEnum tokenType, pfninfixParseFs fn) { infixParseFsn.emplace(tokenType, fn); };

        std::vector<std::string>& Errors() { return errors; }
};

