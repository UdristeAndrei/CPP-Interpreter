#include<vector>
#include<memory>
#include "token.h"

class Node {
    public:
        virtual ~Node() = default;
        virtual std::string TokenLiteral() const = 0;
};

class Statement : public Node {
    public:
        virtual ~Statement() = default;
        virtual void statementNode() const = 0;
};

class Expression : public Node {
    public:
        virtual ~Expression() = default;
        virtual void expressionNode() const = 0;
};

class Program : public Node {
    public:
        std::vector<std::shared_ptr<Statement>> statements{};

        std::string TokenLiteral() const override {
            return statements.empty() ? "" : statements.front()->TokenLiteral();
        }
};

class Identifier : public Expression {
    public:
        Token TokenIdent;
        std::string Value;

        void expressionNode() const override {};
        std::string TokenLiteral() const override { return TokenIdent.Literal; };
};

class LetStatement : public Statement {
    public:
        Token TokenStatement;
        std::shared_ptr<Identifier> Name;
        std::shared_ptr<Expression> Value;

        void statementNode() const override {};
        std::string TokenLiteral() const override { return TokenStatement.Literal; };
};