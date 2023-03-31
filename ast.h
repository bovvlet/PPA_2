#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>

using namespace std;

class Statement {
public:
    virtual ~Statement() {}
    virtual void execute() = 0;
};

class Expression {
public:
    virtual ~Expression() {}
    virtual int evaluate() = 0;
};

class AssignStatement : public Statement {
public:
    AssignStatement(std::string var, Expression* expr) : var_(var), expr_(expr) {}
    virtual void execute() {
        std::cout << var_ << " = " << expr_->evaluate() << std::endl;
    }
    std::string var_;
    Expression* expr_;
};

class SeqStatement : public Statement {
public:
    SeqStatement(Statement* s1, Statement* s2) : s1_(s1), s2_(s2) {}
    virtual void execute() {
        s1_->execute();
        s2_->execute();
    }
    Statement* s1_;
    Statement* s2_;
};

class IfStatement : public Statement {
public:
    IfStatement(Expression* cond, Statement* s1, Statement* s2) : cond_(cond), s1_(s1), s2_(s2) {}
    virtual void execute() {
        if (cond_->evaluate()) {
            s1_->execute();
        } else {
            s2_->execute();
        }
    }
    Expression* cond_;
    Statement* s1_;
    Statement* s2_;
};

class WhileStatement : public Statement {
public:
    WhileStatement(Expression* cond, Statement* body) : cond_(cond), body_(body) {}
    virtual void execute() {
        while (cond_->evaluate()) {
            body_->execute();
        }
    }
    Expression* cond_;
    Statement* body_;
};

class VarExpression : public Expression {
public:
    VarExpression(std::string name) : name_(name) {}
    virtual int evaluate() { return 0; }
    std::string name_;
};

class NumExpression : public Expression {
public:
    NumExpression(int value) : value_(value) {}
    virtual int evaluate() { return value_; }
    int value_;
};

class BoolExpression : public Expression {
public:
    BoolExpression(bool value) : value_(value) {}
    virtual int evaluate() { return value_; }
    bool value_;
};

class NotExpression : public Expression {
public:
    NotExpression(bool value) : value_((1 ^ value)) {}
    virtual int evaluate() { return value_; }
    bool value_;
};

class BinOpExpression : public Expression {
public:
    BinOpExpression(std::string op, Expression* lhs, Expression* rhs) : op_(op), lhs_(lhs), rhs_(rhs) {}
    virtual int evaluate() {
        if (op_ == "+") {
            return lhs_->evaluate() + rhs_->evaluate();
        } else if (op_ == "-") {
            return lhs_->evaluate() - rhs_->evaluate();
        } else if (op_ == "*") {
            return lhs_->evaluate() * rhs_->evaluate();
        } else if (op_ == "/") {
            return lhs_->evaluate() / rhs_->evaluate();
        } else if (op_ == "<") {
            return lhs_->evaluate() < rhs_->evaluate();
        } else if (op_ == ">") {
            return lhs_->evaluate() > rhs_->evaluate();
        } else if (op_ == "<=") {
            return lhs_->evaluate() <= rhs_->evaluate();
        } else if (op_ == ">=") {
            return lhs_->evaluate() >= rhs_->evaluate();
        } else if (op_ == "=") {
            return lhs_->evaluate() == rhs_->evaluate();
        } else if (op_ == "and") {
            return lhs_->evaluate() && rhs_->evaluate();
        } else if (op_ == "or") {
            return lhs_->evaluate() || rhs_->evaluate();
        }
        return 0;
    }
    std::string op_;
    Expression* lhs_;
    Expression* rhs_;
};

inline void print_ast(Expression* expr, int indent=0) {
    if (VarExpression* var = dynamic_cast<VarExpression*>(expr)) {
        std::cout << std::string(indent, ' ') << "VarExpression: " << var->name_ << std::endl;
    } else if (NumExpression* num = dynamic_cast<NumExpression*>(expr)) {
        std::cout << std::string(indent, ' ') << "NumExpression: " << num->value_ << std::endl;
    } else if (BinOpExpression* binop = dynamic_cast<BinOpExpression*>(expr)) {
        std::cout << std::string(indent, ' ') << "BinOpExpression: " << binop->op_ << std::endl;
        print_ast(binop->lhs_, indent+2);
        print_ast(binop->rhs_, indent+2);
    } else if (BoolExpression* bool_expr = dynamic_cast<BoolExpression*>(expr)) {
        std::cout << std::string(indent, ' ') << "BoolExpression: " << bool_expr->value_ << std::endl;
    } else if (NotExpression* not_expr = dynamic_cast<NotExpression*>(expr)) {
        std::cout << std::string(indent, ' ') << "NotExpression" << not_expr->value_ << std::endl;
    } else {
        std::cerr << "Unknown expression type" << std::endl;
    }
}

inline void print_ast(Statement* stmt, int indent=0) {
    if (AssignStatement* assign = dynamic_cast<AssignStatement*>(stmt)) {
        std::cout << std::string(indent, ' ') << "AssignStatement: " << assign->var_ << std::endl;
        print_ast(assign->expr_, indent+2);
    } else if (SeqStatement* seq = dynamic_cast<SeqStatement*>(stmt)) {
        std::cout << std::string(indent, ' ') << "SeqStatement" << std::endl;
        print_ast(seq->s1_, indent+2);
        print_ast(seq->s2_, indent+2);
    } else if (IfStatement* if_stmt = dynamic_cast<IfStatement*>(stmt)) {
        std::cout << std::string(indent, ' ') << "IfStatement" << std::endl;
        print_ast(if_stmt->cond_, indent+2);
        print_ast(if_stmt->s1_, indent+2);
        print_ast(if_stmt->s2_, indent+2);
    } else if (WhileStatement* while_stmt = dynamic_cast<WhileStatement*>(stmt)) {
        std::cout << std::string(indent, ' ') << "WhileStatement" << std::endl;
        print_ast(while_stmt->cond_, indent+2);
        print_ast(while_stmt->body_, indent+2);
    } else {
        std::cerr << "Unknown statement type" << std::endl;
    }
}

#endif // AST_H