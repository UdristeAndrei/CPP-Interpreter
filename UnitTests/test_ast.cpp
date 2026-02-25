#include "../ast.h"

void TestString() {
    Program myProgram{};
    LetStatement stmt{Token{TOKEN_LET, "let"}, std::make_shared<Identifier>(Token{TOKEN_IDENT, "myVar"}, "myVar"), std::make_shared<Identifier>(Token{TOKEN_IDENT, "anotherVar"}, "anotherVar")};
    myProgram.statements.push_back(std::make_shared<LetStatement>(stmt));

    if (myProgram.String() != "let myVar = anotherVar;") {
        printf("program.String() wrong. got=%s", myProgram.String().c_str());
    } else {
        printf("Test run successfully!\n");
    }
}

int main() {
    TestString();
}