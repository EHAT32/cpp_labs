#include <stdio.h>
#include <string>
#include <cctype>
#include <cstdio>
#include <vector>

//AST
class ExprAST{
    public:
        virtual ~ExprAST(){}
};

class NumberExprAST : public ExprAST{
    public:
        NumberExprAST(double val) : Val(val){}
        double Val;
};

class VariableExprAST : public ExprAST{
    public:
        std::string Name;
        VariableExprAST(const std:: string& name) : Name(name){}
};

class BinaryExprAST{
    public:
        char Op;
        ExprAST *LHS, *RHS;
        BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) : Op(op), LHS(lhs), RHS(rhs) {}
};

//call function

class CallExprAST : public ExprAST{
    public:
        std::string Callee;
        std::vector<ExprAST*> Args;
        CallExprAST(const std::string& callee, std::vector<ExprAST*>& args) : Callee(callee), Args(args) {}
};

//function prototype and function itself

class PrototypeAST{
    public:
        std::string Name;
        std::vector<std::string> Args;
        PrototypeAST(const std::string& name, const std::vector<std::string>& args) : Name(name), Args(args) {}
};

class FunctionAST{
    public:
        PrototypeAST *Proto;
        ExprAST *Body;
        FunctionAST(PrototypeAST *proto, ExprAST *body) : Proto(proto), Body(body) {}
};



int main(int argc, char** argv){
    return 0;
}