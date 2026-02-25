#include<vector>
#include<memory>
#include<sstream>
#include "token.h"

class Node {
    public:
        Node() = default;
        virtual ~Node() = default;
        virtual std::string TokenLiteral() const = 0;
        virtual std::string String() const = 0;
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

        std::string String() const override {
            std::ostringstream buffer{};
            for (const auto& statement : statements) {
                buffer << statement->String();
            }
            return buffer.str();
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
        std::string String() const override { return Value; };
};

class LetStatement : public Statement {
    public:
        Token TokenLetStatement{};
        std::shared_ptr<Identifier> Name{};
        std::shared_ptr<Expression> Value{};
        
        LetStatement() = default;
        LetStatement(const Token& t) : TokenLetStatement(t) {};
        LetStatement(const Token& t, std::shared_ptr<Identifier> i, std::shared_ptr<Expression> e) : TokenLetStatement(t), Name(i), Value(e) {};
        ~LetStatement() = default;

        void statementNode() const override {};
        std::string TokenLiteral() const override { return TokenLetStatement.Literal; };

        std::string String() const override {
            std::ostringstream buffer{};
            buffer << TokenLetStatement.Literal << " " << Name->String() << " = ";
            if (Value != nullptr) {
                buffer << Value->String();
            }
            buffer << ";";
            return buffer.str();
        }
};

class ReturnStatement : public Statement {
    public:
        Token TokenReturnStatement{};
        std::shared_ptr<Expression> Value{};

        ReturnStatement() = default;
        ReturnStatement(const Token& t) : TokenReturnStatement(t) {};
        ~ReturnStatement() = default;

        void statementNode() const override {};
        std::string TokenLiteral() const override { return TokenReturnStatement.Literal; };

        std::string String() const override {
            std::ostringstream buffer{};
            buffer << TokenReturnStatement.Literal << " ";
            if (Value != nullptr) {
                buffer << Value->String();
            }
            buffer << ";";
            return buffer.str();
        }
};

class ExpressionStatement : public Statement {
    public:
        Token TokenExpressionStatement{};
        std::shared_ptr<Expression> Value{};

        ExpressionStatement() = default;
        ExpressionStatement(const Token& t) : TokenExpressionStatement(t) {};
        ~ExpressionStatement() = default;

        void statementNode() const override {};
        std::string TokenLiteral() const override { return TokenExpressionStatement.Literal; };

        std::string String() const override { return (Value == nullptr) ? "" : Value->String(); }; 
};