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

%token <num> NUMBER
%token <str> NAME
%token <boolean> BOOLEAN
%token <str> INVALID_CHARACHER

%token ASSIGN PLUS MINUS MULT DIV SEMICOLON IF ELSE WHILE
%token LEQ LT GEQ GT EQ AND OR NOT 
%token LPARENT RPARENT

%type <statement> stmt
%type <expression> a_expr bool_expr

%left OR
%left AND
%left EQ NEQ LT GT LEQ GEQ
%left PLUS MINUS
%left MULT DIV

%start stmt

%%

stmt:   NAME ASSIGN a_expr                                                      { $$ = new AssignStatement($1, $3); root = $$;}
    |   stmt SEMICOLON                                                          { $$ = $1; root = $$; }
    |   stmt SEMICOLON stmt                                                     { $$ = new SeqStatement($1, $3); root = $$; }
    |   IF bool_expr LPARENT stmt RPARENT ELSE LPARENT stmt RPARENT        { $$ = new IfStatement($2, $4, $8); root = $$; }
    |   WHILE bool_expr LPARENT stmt RPARENT                                 { $$ = new WhileStatement($2, $4); root = $$; }
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