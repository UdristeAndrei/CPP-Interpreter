#include<vector>
#include "token.h"

class Node {
    public:
        virtual std::string TokenLiteral(Program& p) {
            if (p.Statements.size() > 0) {
                return p.Statements[0].TokenLiteral(p);
            } else {
                return "";
            }
        }
};

class Statement: public Node {
    public:
        std::string TokenLiteral(Program& p) override;
};

class Expression: public Node {
    public:
        std::string TokenLiteral(Program& p) override;
};

struct Program {
    std::vector<Statement> Statements{};
};
