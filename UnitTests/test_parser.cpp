#include "../parser.h"

bool testLetStatement(std::shared_ptr<Statement> statement, std::string& name) {
    if (statement->TokenLiteral() != "let") {
        printf("statement->TokenLiteral is not 'let'. Got=%s", statement->TokenLiteral().c_str());
        return false;
    }

    auto letStmt = std::dynamic_pointer_cast<LetStatement>(statement);
    if (letStmt == nullptr) {
        printf("statement is not LetStatement");
        return false;
    } else if (letStmt->Name->Value != name) {
        printf("letStmt->Name->Value not '%s'. Got=%s", name.c_str(), letStmt->Name->Value.c_str());
        return false;
    } else if (letStmt->Name->TokenLiteral() != name) {
        printf("statement->name not '%s'. Got=%s", name.c_str(), letStmt->Name->TokenLiteral().c_str());
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

    if (program == nullptr) {
        printf("ParseProgram() return nullptr");
        return;
    }

    if (program->statements.size() != 3) {
        printf("program.statements does not contain 3 statements. Got=%li", program->statements.size());
        return;
    }

    std::vector<std::string> tests = {"x", "y", "foobar"};

    for (size_t i; i < tests.size(); i++) {
        auto stmt = program->statements.at(i);
        if (!testLetStatement(stmt, tests.at(i))) {
            return;
        }
    }
}

int main() {
    TestLetStatements();
}