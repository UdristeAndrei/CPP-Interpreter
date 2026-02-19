#include<vector>
#include<memory>
#include "token.h"

class Node {
    public:
        Node() = default;
        virtual ~Node() = default;
        virtual std::string TokenLiteral() const = 0;
};

class Statement : public Node {
    public:
        Statement() = default;
        virtual ~Statement() = default;
        virtual void statementNode() const = 0;
};

class Expression : public Node {
    public:
        Expression() = default;
        virtual ~Expression() = default;
        virtual void expressionNode() const = 0;
};

class Program : public Node {
    public:
        std::vector<std::shared_ptr<Statement>> statements{};

        Program() = default;
        Program(const std::vector<std::shared_ptr<Statement>>& stmts) : statements(stmts) {};
        ~Program() = default;

        std::string TokenLiteral() const override {
            return statements.empty() ? "" : statements.front()->TokenLiteral();
        }
};

class Identifier : public Expression {
    public:
        Token TokenIdent{};
        std::string Value{};

        Identifier() = default;
        Identifier(const Token& t, const std::string& s) : TokenIdent(t), Value(s) {};
        ~Identifier() = default;

        void expressionNode() const override {};
        std::string TokenLiteral() const override { return TokenIdent.Literal; };
};

class LetStatement : public Statement {
    public:
        Token TokenStatement{};
        std::shared_ptr<Identifier> Name{};
        std::shared_ptr<Expression> Value{};
        
        LetStatement() = default;
        LetStatement(const Token& t) : TokenStatement(t) {};
        LetStatement(const Token& t, std::shared_ptr<Identifier> i, std::shared_ptr<Expression> e) : TokenStatement(t), Name(i), Value(e) {};
        ~LetStatement() = default;

        void statementNode() const override {};
        std::string TokenLiteral() const override { return TokenStatement.Literal; };
};