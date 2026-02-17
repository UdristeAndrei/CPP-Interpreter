#include "parser.h"

bool testLetStatement(std::shared_ptr<Statement> statement, std::string& name) {
    
}

void TestLetStatements() {
    std::string input = R"(
    let x = 5;
    let y = 10;
    let foobar = 838383;)";

    auto myLexer = std::make_shared<Lexer>(input);
    Parser myParser(myLexer);

    auto program = myParser.ParseProgram();

    if (program == nullptr) {
        printf("ParseProgram() return nullptr");
    }

    if (program->statements.size() != 3) {
        printf("program.statements does not contain 3 statements. Got=%d", program->statements.size());
    }

    std::vector<std::string> tests = {"x", "y", "foobar"};

    for (size_t i; i < tests.size(); i++) {
        auto stmt = program->statements.at(i);
        if (!testLetStatement(stmt, tests.at(i))) {
            return;
        }
    }
}