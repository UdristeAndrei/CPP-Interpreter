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

int main() {
    // TestLetStatements();
    TestReturnStatement();
}