#include "../parser.h"

void checkParserErrors(Parser& parser) {
    std::vector<std::string> errors = parser.Errors();

    if (!errors.size()) {
        return;
    }

    printf("parser has %li errors\n", errors.size());
    for (const std::string& error : errors) {
        printf("parser error: %s\n", error.c_str());
    }
}

bool testLetStatement(std::shared_ptr<Statement> statement, std::string& name) {
    if (statement->TokenLiteral() != "let") {
        printf("statement->TokenLiteral is not 'let'. Got=%s", statement->TokenLiteral().c_str());
        return false;
    }

    auto letStmt = std::dynamic_pointer_cast<LetStatement>(statement);
    if (letStmt == nullptr) {
        printf("statement is not LetStatement\n");
        return false;
    } else if (letStmt->Name->Value != name) {
        printf("letStmt->Name->Value not '%s'. Got=%s\n", name.c_str(), letStmt->Name->Value.c_str());
        return false;
    } else if (letStmt->Name->TokenLiteral() != name) {
        printf("statement->name not '%s'. Got=%s\n", name.c_str(), letStmt->Name->TokenLiteral().c_str());
        return false;
    }
    return true;
}

void TestLetStatements() {
    std::string input = R"(
    let x = 5;
    let y = 10;
    let foobar = 838383;)";

    auto myLexer = std::make_shared<Lexer>(input);
    Parser myParser(myLexer);

    auto program = myParser.parseProgram();
    checkParserErrors(myParser);
    if (myParser.Errors().size()) { return; }

    if (program == nullptr) {
        printf("ParseProgram() return nullptr\n");
        return;
    }

    if (program->statements.size() != 3) {
        printf("program.statements does not contain 3 statements. Got=%li\n", program->statements.size());
        return;
    }

    std::vector<std::string> tests = {"x", "y", "foobar"};

    for (size_t i; i < tests.size(); i++) {
        auto stmt = program->statements.at(i);
        if (!testLetStatement(stmt, tests.at(i))) {
            return;
        }
    }
    printf("Test run successfully!\n");
}

void TestReturnStatement() {
    std::string input = R"(
    return 5;
    return 10;
    return 123456;)";

    auto myLexer = std::make_shared<Lexer>(input);
    Parser myParser(myLexer);

    auto program = myParser.parseProgram();
    checkParserErrors(myParser);
    if (myParser.Errors().size()) { return; }

    if (program == nullptr) {
        printf("ParseProgram() return nullptr\n");
        return;
    }

    if (program->statements.size() != 3) {
        printf("program.statements does not contain 3 statements. Got=%li\n", program->statements.size());
        return;
    }

    for (auto& stmt : program->statements) {
        auto returnStmt = std::dynamic_pointer_cast<ReturnStatement>(stmt);
        if (returnStmt == nullptr) {
            printf("statement is not ReturnStatement\n");
            continue;
        } 
        if (returnStmt->TokenLiteral() != "return") {
            printf("returnStmt.TokenLiteral() in not 'return', got %s\n", returnStmt->TokenLiteral().c_str());
        }
    }

    printf("Test run successfully!\n");
}

void TestIdentifierExpression() {
    std::string input{"foobar;"};

    Lexer myLexer{input};
    Parser myParser{std::make_shared<Lexer>(myLexer)};
    std::shared_ptr<Program> program = myParser.parseProgram();

    checkParserErrors(myParser);
    if (myParser.Errors().size()) { return; }

    if (program->statements.size() != 1) {
        printf("program.statements does not have enough statements. Got=%li\n", program->statements.size());
    }

    auto stmt = std::dynamic_pointer_cast<ExpressionStatement>(program->statements[0]);
    if (stmt == nullptr) {
        printf("program.statements[0] is not a ExpressionStatement.\n");
    }

    auto ident = std::dynamic_pointer_cast<Identifier>(stmt->Value);
    if (ident == nullptr) {
        printf("program.statements[0].Value not a Identifier.\n");
    }

    if (ident->Value != "foobar") {
        printf("ident.Value not %s. Got=%s\n", "foobar", ident->Value.c_str());
    }

    if (ident->TokenLiteral() != "foobar"){
        printf("ident.TokenLiteral() not %s. Got=%s\n", "foobar", ident->TokenLiteral().c_str());
    }

    printf("Test run successfully!\n");
}

void TestIntegerLiteralExpression() {
    std::string input{"5;"};

    Lexer myLexer{input};
    Parser myParser{std::make_shared<Lexer>(myLexer)};
    std::shared_ptr<Program> program = myParser.parseProgram();

    checkParserErrors(myParser);
    if (myParser.Errors().size()) { return; }

    if (program->statements.size() != 1) {
        printf("program.statements does not have enough statements. Got=%li\n", program->statements.size());
    }

    auto stmt = std::dynamic_pointer_cast<ExpressionStatement>(program->statements[0]);
    if (stmt == nullptr) {
        printf("program.statements[0] is not a ExpressionStatement.\n");
    }

    auto ident = std::dynamic_pointer_cast<IntegerLiteral>(stmt->Value);
    if (ident == nullptr) {
        printf("program.statements[0].Value not a Identifier.\n");
    }

    if (ident->Value != 5) {
        printf("ident.Value not %i. Got=%li\n", 5, ident->Value);
    }

    if (ident->TokenLiteral() != "5"){
        printf("ident.TokenLiteral() not %s. Got=%s\n", "5", ident->TokenLiteral().c_str());
    }

    printf("Test run successfully!\n");
}


bool testIntegerLiteral(std::shared_ptr<Expression> il, int64_t value) {
    auto integer = std::dynamic_pointer_cast<IntegerLiteral>(il);
    if (integer == nullptr) {
        printf("il is not a IntegerLiteral\n");
        return false;
    }

    if (integer->Value != value) {
        printf("integer->Value not %li. Got=%li\n", value, integer->Value);
        return false;
    }

    if (integer->TokenLiteral() != std::to_string(value)) {
        printf("integer->TokenLiteral() not %li. Got=%s\n", value, integer->TokenLiteral().c_str());
        return false;
    }
    return true;
}

void TestParsingPrefixExpressions() {
    struct PrefixTest {
        std::string input{};
        std::string operatorValue{};
        int64_t integerValue{};
    };

    std::vector<PrefixTest> prefixTests{{"!5", "!", 5}, {"-15", "-", 15}};

    for(auto& test : prefixTests) {
        Lexer myLexer{test.input};
        Parser myParser{std::make_shared<Lexer>(myLexer)};

        auto program = myParser.parseProgram();
        checkParserErrors(myParser);

        if (myParser.Errors().size()) { return; }

        if (program->statements.size() != 1) {
            printf("program.statements does not contain %i statements. Got=%li\n", 1, program->statements.size());
        }

        auto stmt = std::dynamic_pointer_cast<ExpressionStatement>(program->statements[0]);
        if (stmt == nullptr) {
            printf("program.statements[0] is not a ExpressionStatement.\n");
        }

        auto exp = std::dynamic_pointer_cast<PrefixExpression>(stmt->Value);
        if (exp == nullptr) {
            printf("program.statements[0].Value not a PrefixExpression.\n");
        }

        if (exp->OperatorValue != test.operatorValue) {
            printf("exp.OperatorValue not %s. Got=%s\n", test.operatorValue.c_str(), exp->OperatorValue.c_str());
        }

        testIntegerLiteral(exp->Right, test.integerValue);
        }
    printf("Test run successfully!\n");
}

void TestParsingInfixExpression() {
    struct InfixTest {
        std::string input{};
        int64_t leftValue{};
        std::string operatorValue{};
        int64_t rightValue{};
        
    };

    std::vector<InfixTest> infixTests{  {"5 + 5;", 5, "+", 5}, 
                                        {"5 - 5;", 5, "-", 5},
                                        {"5 * 5;", 5, "*", 5},
                                        {"5 / 5;", 5, "/", 5},
                                        {"5 > 5;", 5, ">", 5},
                                        {"5 < 5;", 5, "<", 5},
                                        {"5 == 5;", 5, "==", 5},
                                        {"5 != 5;", 5, "!=", 5}};

    for(auto& test : infixTests) {
        Lexer myLexer{test.input};
        Parser myParser{std::make_shared<Lexer>(myLexer)};

        auto program = myParser.parseProgram();
        checkParserErrors(myParser);

        if (myParser.Errors().size()) { return; }

        if (program->statements.size() != 1) {
            printf("program.statements does not contain %i statements. Got=%li\n", 1, program->statements.size());
        }

        auto stmt = std::dynamic_pointer_cast<ExpressionStatement>(program->statements[0]);
        if (stmt == nullptr) {
            printf("program.statements[0] is not a ExpressionStatement.\n");
        }

        auto exp = std::dynamic_pointer_cast<InfixExpression>(stmt->Value);
        if (exp == nullptr) {
            printf("program.statements[0].Value not a InfixExpression.\n");
        }

        testIntegerLiteral(exp->Right, test.leftValue);

        if (exp->OperatorValue != test.operatorValue) {
            printf("exp.OperatorValue not %s. Got=%s\n", test.operatorValue.c_str(), exp->OperatorValue.c_str());
        }

        testIntegerLiteral(exp->Right, test.rightValue);
        }
    printf("Test run successfully!\n");
}

int main() {
    // TestLetStatements();
    // TestReturnStatement();   
    // TestIdentifierExpression();
    // TestIntegerLiteralExpression();
    // TestParsingPrefixExpressions();
    TestParsingInfixExpression();
}