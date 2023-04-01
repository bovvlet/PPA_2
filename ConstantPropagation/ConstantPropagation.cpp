#include "ConstantPropagation.h"

void ConstantPropagation::visiteStatement(Statement *p){};   // abstract class
void ConstantPropagation::visiteExpression(Expression *p){}; // abstract class

void ConstantPropagation::visiteAssignStatement(AssignStatement *p)
{
    this->numberStatement += 1;
    if (this->numberStatement == this->listStates.size())
        listStates.push_back(currentIn);
    if (this->numberStatement < this->listStates.size())
        listStates[this->numberStatement] = currentIn;
    if (this->numberStatement > this->listStates.size())
        std::cout << "State Number greater then it should be" << std::endl;

    // Check expression
    if (p->expr_)
        p->expr_->accept(this);

    // assignment of non-comlutable, non-constant variable
    if (this->relevantReturn == false)
        return;

    // Add variable into this state
    this->addToState(this->numberStatement, p->var_, this->getReturnVal());
    // Add In for the next statement
    this->currentIn = listStates[this->numberStatement];
};
void ConstantPropagation::visiteSeqStatement(SeqStatement *p)
{
    if (p->s1_)
        p->s1_->accept(this);
    if (p->s2_)
        p->s2_->accept(this);
};
void ConstantPropagation::visiteIfStatement(IfStatement *p)
{
    int currentStatNum = this->numberStatement + 1;
    if (currentStatNum == this->listStates.size())
        listStates.push_back(currentIn);
    if (currentStatNum < this->listStates.size())
        listStates[currentStatNum] = currentIn;
    if (currentStatNum > this->listStates.size())
        std::cout << "State Number larger then it should be" << std::endl;

    this->numberStatement = currentStatNum;
    if (p->cond_)
        p->cond_->accept(this);

    // Going through the "then" statement
    this->currentIn = listStates[currentStatNum];
    if (p->s1_)
        p->s1_->accept(this);
    unordered_map<string, int> thenState = this->currentIn;

    // Going through the "else" statement
    this->currentIn = listStates[currentStatNum];
    if (p->s2_)
        p->s2_->accept(this);
    unordered_map<string, int> elseState = this->currentIn;

    // Combine "then" body and "else" body
    unordered_map<string, int> combineState = unordered_map<string, int>();
    for (const auto &[key, value] : thenState)
        if (elseState.count(key) != 0 && elseState[key] == value)
            combineState[key] = value;
    this->currentIn = combineState;
};

bool ifEqual(unordered_map<string, int> first, unordered_map<string, int> second)
{
    bool equal = true;

    for (const auto &[key, value] : first)
        if (second.count(key) == 0 || second[key] != value)
            equal = false;

    return equal;
}

void ConstantPropagation::visiteWhileStatement(WhileStatement *p)
{
    int currentStatNum = this->numberStatement + 1;
    if (currentStatNum == this->listStates.size())
        listStates.push_back(currentIn);
    if (currentStatNum < this->listStates.size())
        listStates[currentStatNum] = currentIn;
    if (currentStatNum > this->listStates.size())
        std::cout << "State Number larger then it should be" << std::endl;

    this->numberStatement = currentStatNum;
    if (p->cond_)
        p->cond_->accept(this);

    // First time going through body
    unordered_map<string, int> prevbodyState = listStates[currentStatNum];
    this->currentIn = prevbodyState;
    if (p->body_)
        p->body_->accept(this);
    unordered_map<string, int> bodyState = this->currentIn;

    // Intersecting previous result of while statement and the new one
    unordered_map<string, int> combineState = unordered_map<string, int>();
    for (const auto &[key, value] : prevbodyState)
        if (bodyState.count(key) != 0 && bodyState[key] == value)
            combineState[key] = value;
    bodyState = combineState;

    // Going through the body till the "while" statement will be stabilised
    while (ifEqual(prevbodyState, bodyState) == false)
    {
        prevbodyState = bodyState;

        this->numberStatement = currentStatNum;
        this->currentIn = bodyState;
        if (p->body_)
            p->body_->accept(this);
        bodyState = this->currentIn;

        // Intersecting previous result of while statement and the new one
        combineState = unordered_map<string, int>();
        for (const auto &[key, value] : prevbodyState)
            if (bodyState.count(key) != 0 && bodyState[key] == value)
                combineState[key] = value;
        bodyState = combineState;
    }

    listStates[currentStatNum] = bodyState;
    this->currentIn = bodyState;
};
void ConstantPropagation::visiteVarExpression(VarExpression *p)
{
    // If variable defined with constatns
    if (this->listStates[this->numberStatement].count(p->name_))
    {
        this->setReturnVal(this->listStates[this->numberStatement][p->name_]);
    }
    else
    {
        this->relevantReturn = false;
    }
};
void ConstantPropagation::visiteNumExpression(NumExpression *p)
{
    this->setReturnVal(p->value_);
};
void ConstantPropagation::visiteBoolExpression(BoolExpression *p)
{
    this->setReturnVal(p->value_);
};
void ConstantPropagation::visiteNotExpression(NotExpression *p)
{
    this->setReturnVal(p->value_);
};
void ConstantPropagation::visiteBinOpExpression(BinOpExpression *p)
{
    // Evaluating the left part of binary operation expression
    if (p->lhs_)
        p->lhs_->accept(this);
    if (this->relevantReturn == false)
    {
        return;
    }
    int lhs_int = this->getReturnVal();

    // Evaluating the right part of binary operation expression
    if (p->rhs_)
        p->rhs_->accept(this);
    if (this->relevantReturn == false)
    {
        return;
    }
    int rhs_int = this->getReturnVal();

    // Evaluating binary operation expression
    if (p->op_ == "+"){
        this->setReturnVal(lhs_int + rhs_int);
    }
    else if (p->op_ == "-"){
        this->setReturnVal(lhs_int - rhs_int);
    }
    else if (p->op_ == "*"){
        this->setReturnVal(lhs_int * rhs_int);
    }
    else if (p->op_ == "/"){
        this->setReturnVal(lhs_int / rhs_int);
    }
    else if (p->op_ == "<"){
        this->setReturnVal(lhs_int < rhs_int);
    }
    else if (p->op_ == ">"){
        this->setReturnVal(lhs_int > rhs_int);
    }
    else if (p->op_ == "<="){
        this->setReturnVal(lhs_int <= rhs_int);
    }
    else if (p->op_ == ">="){
        this->setReturnVal(lhs_int >= rhs_int);
    }
    else if (p->op_ == "="){
        this->setReturnVal(lhs_int == rhs_int);
    }
    else if (p->op_ == "and"){
        this->setReturnVal(lhs_int && rhs_int);
    }
    else if (p->op_ == "or"){
        this->setReturnVal(lhs_int || rhs_int);
    }
};