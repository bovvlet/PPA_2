#ifndef VISITOR_H
#define VISITOR_H
#include "../ast.h"
#include <unordered_map>
#include <vector>
using namespace std;

class ConstantPropagation : public Visitor{
    // List of variables that can be replaced at some state
    vector<unordered_map<string, int>> listStates;
    // List of "In" variables to translate the "Out" states to the "In" state for the next
    unordered_map<string, int> currentIn = unordered_map<string, int>();
    // Return value for calculating expressions that can be calculated
    int returnVal = 0;
    // Checker if an expression can be calculated
    bool relevantReturn = 0;
    // The number of the current statement. Enables switching between states in the list
    int numberStatement = -1;
public:
    void printStates(){
        for(int i = 0; i < listStates.size(); i++){
            cout << "State: " << i << endl;
            for (const auto& [key, value] : listStates[i]){
                cout << key << " : " << value << endl;
            }
        }
    }

    int setReturnVal(int _returnVal){
        this->returnVal = _returnVal;
        this->relevantReturn = 1;

        return this->returnVal;
    }

    int getReturnVal(){
        this->relevantReturn = 0;
        return this->returnVal;
    }

    bool addToState(int _stateNum, string _name, int _value){
        if (_stateNum >= listStates.size()){
            std::cout << "Trying to add variable to nonexisting state" << std::endl;
            return false;
        }
        else{
            this->listStates[_stateNum][_name] = _value;
            return true;
        }
    }

    void visiteStatement(Statement * p);
    void visiteExpression(Expression * p);
    void visiteAssignStatement(AssignStatement * p);
    void visiteSeqStatement(SeqStatement * p);
    void visiteIfStatement(IfStatement * p);
    void visiteWhileStatement(WhileStatement * p);
    void visiteVarExpression(VarExpression * p);
    void visiteNumExpression(NumExpression * p);
    void visiteBoolExpression(BoolExpression * p);
    void visiteNotExpression(NotExpression * p);
    void visiteBinOpExpression(BinOpExpression * p);
};

#endif