%{
#include <iostream>
#include <string>
#include "ast.h"
#include "parser.hpp"
using namespace std;
%}

%option noyywrap
%option yylineno
%option outfile="lexer.cpp" header-file="lexer.hpp"

%%
[\t\f\r ]+                   /* ignore whitespace */
\n                           /* ignore newlines */
[0-9]+                       { yylval.num = atoi(yytext); return NUMBER; }
"="                          { return ASSIGN; }
"+"                          { return PLUS; }
"-"                          { return MINUS; }
"*"                          { return MULT; }
"/"                          { return DIV; }
";"                          { return SEMICOLON; }
"if"                         { return IF; }
"then"                       { return THEN; }
"else"                       { return ELSE; }
"while"                      { return WHILE; }
"do"                         { return DO; }
"<="                         { return LEQ; }
"<"                          { return LT; }
">="                         { return GEQ; }
">"                          { return GT; }
"=="                         { return EQ; }
"and"                        { return AND; }
"or"                         { return OR; }
"not"                        { return NOT; }
"true"                       { yylval.boolean = true; return BOOLEAN; }
"false"                      { yylval.boolean = false; return BOOLEAN; }
[a-zA-Z][a-zA-Z0-9]*         { yylval.str = yytext; return NAME; }
.                            { yylval.str = yytext; return INVALID_CHARACHER; }

%%
