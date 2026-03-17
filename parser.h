#include "lexer.h"
#include "ast.h"
#include<functional>

#define pfnPrefixParseFs std::function<std::shared_ptr<Expression>()>
#define pfninfixParseFs std::function<std::shared_ptr<Expression>(std::shared_ptr<Expression>)>


enum Precedence {
    _ = 0,
    LOWEST = 1,
    EQUALS = 2,         // ==
    LESSGREATER = 3,    // > or <
    SUM = 4,            // +
    PRODUCT = 5,        // *
    PREFIX = 6,         // -X or !X
    CALL = 6            // foo(x);
};

std::map<TokenTypeEnum, Precedence> precedences{
    {TOKEN_EQ, EQUALS},
    {TOKEN_NOT_EQ, EQUALS},
    {TOKEN_LT, LESSGREATER},
    {TOKEN_GT, LESSGREATER},
    {TOKEN_PLUS, SUM},
    {TOKEN_MINUS, SUM},
    {TOKEN_SLASH, PRODUCT},
    {TOKEN_ASTERISK, PRODUCT}
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

        Precedence peekPrecedence() {
            if (precedences.find(peekToken.TokenType) != precedences.end()) {
                return precedences[peekToken.TokenType];
            } else {
                return LOWEST;
            }
        }

        Precedence curPrecedence() {
            if (precedences.find(curToken.TokenType) != precedences.end()) {
                return precedences[curToken.TokenType];
            } else {
                return LOWEST;
            }
        }

        void noPrefixParseFnError(TokenTypeEnum tokenType) {
            std::ostringstream error{};
            error << "no prefix parse function for " << tokenType << " found ";
            errors.push_back(error.str());
        }

        std::shared_ptr<Expression> parseExpression(Precedence preccedence) {
            auto prefix = prefixParseFsn.at(curToken.TokenType);

            if (prefix == nullptr) { 
                noPrefixParseFnError(curToken.TokenType);
                return nullptr; 
            };
            auto leftExp = prefix();

            while (!peekTokenIs(TOKEN_SEMICOLON) && preccedence < peekPrecedence()){
                auto infix = infixParseFsn.at(peekToken.TokenType);
                if (infix == nullptr) {
                    return leftExp;
                } else {
                    
                    nextToken();
                    leftExp = infix(leftExp);
                }
            }

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

        std::shared_ptr<Expression> parseInfixExpression(std::shared_ptr<Expression> left) {
            auto expression = std::make_shared<InfixExpression>(curToken, curToken.Literal, left);
            auto tokenPrecedence = curPrecedence();
            nextToken();
            expression->Right = parseExpression(tokenPrecedence);
            return expression;
        }

    public:
        Parser(std::shared_ptr<Lexer> l) {
            myLexer = l;

            nextToken();
            nextToken();

            // Register the basic tokens
            registerPrefix(TOKEN_IDENT, [this]() { return parseIdendifier(); });
            registerPrefix(TOKEN_INT, [this]() {return parseIntegerLiteral(); });

            // Register the prefixes
            registerPrefix(TOKEN_BANG, [this]() {return parsePrefixExpression(); });
            registerPrefix(TOKEN_MINUS, [this]() {return parsePrefixExpression(); });

            // Register the infixes
            registerInfix(TOKEN_PLUS, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
            registerInfix(TOKEN_MINUS, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
            registerInfix(TOKEN_SLASH, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
            registerInfix(TOKEN_ASTERISK, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
            registerInfix(TOKEN_EQ, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
            registerInfix(TOKEN_NOT_EQ, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
            registerInfix(TOKEN_LT, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
            registerInfix(TOKEN_GT, [this](std::shared_ptr<Expression> left) {return parseInfixExpression(left); });
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

