#include<vector>
#include<memory>
#include "token.h"

struct Node {
    virtual ~Node() = default;
    virtual std::string TokenLiteral() const = 0;
};

struct Statement : Node {
    virtual ~Statement() = default;
    virtual void statementNode() = 0;
};

struct Expression : Node {
    virtual ~Expression() = default;
    virtual void expressionNode() = 0;
};

struct Program : Node {
    std::vector<std::shared_ptr<Statement>> statements{};

    std::string TokenLiteral() const override {
        return statements.empty() ? "" : statements.front()->TokenLiteral();
    }
};

struct Identifier : Expression {
    Token TokenIdent;
    std::string Value;

    void expressionNode() override {};
    std::string TokenLiteral() const override { return TokenIdent.Literal; };
};

struct LetStatement : Statement {
    Token TokenStatement;
    std::shared_ptr<Identifier> Name;
    std::shared_ptr<Expression> Value;

    void statementNode() override {};
    std::string TokenLiteral() const override { return TokenStatement.Literal; };
};