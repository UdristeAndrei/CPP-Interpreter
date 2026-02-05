#include<string>
#include<memory>

struct Lexer{
    std::string input{};
    unsigned int position{0};
    unsigned int readPosition{0};
    char ch{0};
};

std::shared_ptr<Lexer> newLexer(std::string input) {
    std::shared_ptr<Lexer> l = std::make_unique<Lexer>();
    l->input = input;
    return l;
}

void readChar(std::shared_ptr<Lexer> l) {
    if (l->readPosition >= l->input.length()) {
        l->ch = 0;
    } else {
        l->ch = l->input.at(l->readPosition);
    }

    l->position = l->readPosition;
    l->readPosition++;
}