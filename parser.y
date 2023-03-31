%{
#include <iostream>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "ast.h"

using namespace std;

void yyerror(const char* msg, ...) {
    std::cerr << "Syntax error at line " << yylineno << ": " << msg << std::endl;
}

Statement* root;
%}

%union {
    int num;
    char* str;
    bool boolean;
    Statement* statement;
    Expression* expression;
}

%define parse.error verbose

%token <num> NUMBER
%token <str> NAME
%token <boolean> BOOLEAN
%token <str> INVALID_CHARACHER

%token ASSIGN PLUS MINUS MULT DIV SEMICOLON IF THEN ELSE WHILE DO
%token LEQ LT GEQ GT EQ AND OR NOT 

%type <statement> stmt
%type <expression> a_expr bool_expr

%left OR
%left AND
%left EQ NEQ LT GT LEQ GEQ
%left PLUS MINUS
%left MULT DIV

%start stmt

%%

stmt:   NAME ASSIGN a_expr                      { $$ = new AssignStatement($1, $3); root = $$; }
    |   stmt SEMICOLON                          { $$ = $1; root = $$; }
    |   stmt SEMICOLON stmt                     { $$ = new SeqStatement($1, $3); root = $$; }
    |   IF bool_expr THEN stmt ELSE stmt        { $$ = new IfStatement($2, $4, $6); root = $$; }
    |   WHILE bool_expr DO stmt                 { $$ = new WhileStatement($2, $4); root = $$; }
    ;

a_expr: NAME                                    { $$ = new VarExpression($1); }
    |   NUMBER                                  { $$ = new NumExpression($1); }
    |   a_expr PLUS a_expr                      { $$ = new BinOpExpression("+", $1, $3); }
    |   a_expr MINUS a_expr                     { $$ = new BinOpExpression("-", $1, $3); }
    |   a_expr MULT a_expr                      { $$ = new BinOpExpression("*", $1, $3); }
    |   a_expr DIV a_expr                       { $$ = new BinOpExpression("/", $1, $3); }
    ;

bool_expr: BOOLEAN                              { $$ = new BoolExpression($1); }
    |   NOT bool_expr                           { $$ = new NotExpression($2); }
    |   bool_expr AND bool_expr                 { $$ = new BinOpExpression("and", $1, $3); }
    |   bool_expr OR bool_expr                  { $$ = new BinOpExpression("or", $1, $3); }
    |   a_expr LT a_expr                        { $$ = new BinOpExpression("<", $1, $3); }
    |   a_expr GT a_expr                        { $$ = new BinOpExpression(">", $1, $3); }
    |   a_expr LEQ a_expr                       { $$ = new BinOpExpression("<=", $1, $3); }
    |   a_expr GEQ a_expr                       { $$ = new BinOpExpression(">=", $1, $3); }
    |   a_expr EQ a_expr                        { $$ = new BinOpExpression("==", $1, $3); }
    |   a_expr NEQ a_expr                       { $$ = new BinOpExpression("!=", $1, $3); }
;

%% 

int main() {
    string tests[5];

    tests[0] += "x = 10;";

    tests[1] += "x = 10 + 20;\n";
    tests[1] += "y = x + 10 / 20 - x;";
    
    tests[2] += "a = 3 + 5;\n";
    tests[2] += "if a > 5 then b = 10 else b = 0\n";
    tests[2] += "c = a + b - a - b";

    tests[3] += "x = 5;\n";
    tests[3] += "y = 0;\n";
    tests[3] += "while x > 0 do\n";
    tests[3] += "  y = y + 2;\n";
    tests[3] += "  x = x - 1;\n";
    tests[3] += "z = y * 3;";

    tests[4] += "while true do\n";
    tests[4] += "  if true then\n";
    tests[4] += "    while false do\n";
    tests[4] += "       x = 1;\n";
    tests[4] += "  else\n";
    tests[4] += "    y = 2;\n";
    tests[4] += "z = 3;";

    // if you want to test them one by one
    // change this value to needed test
    // if want to check all tests, make it equal to -1

    int test = -1;
    if (test == -1) {
        for (int i = 0; i < 5; ++i) {
            cout << "\n--------------------------------------------\n";
            string input = tests[i];
            cout << "Our test with number " << i << " is\n";
            cout << input << "\n\n";
            cout << "And here is its AST\n";
            yy_scan_string(input.c_str());
            yyparse();
            print_ast(root);
            cout << "\n--------------------------------------------\n";
        }
    }
    else if (0 <= test && test <= 4) {
        cout << "\n--------------------------------------------\n";
        string input = tests[test];
        cout << "Our test with number " << test << " is\n";
        cout << input << "\n\n";
        cout << "And here is its AST\n";
        yy_scan_string(input.c_str());
        yyparse();
        print_ast(root);
        cout << "\n--------------------------------------------\n";
    } else {
        cout << "Read comments one more time\n";
    }
}