%{

/*
 * parser.y file
 * To generate the parser run: "bison parser.y"
 */

#include "expression.h"
#include "error.h"
#include "statement.h"
#include "parser.h"
#include "lexer.h"

int yyerror(const YYLTYPE *loc, Statement **statement, yyscan_t scanner,
        const char *msg) {
    user_error(loc, std::string(msg));
}

%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output  "parser.c"
%defines "parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { Statement **statement }
%parse-param { yyscan_t scanner }

%locations

%union {
    int value;
    Expression *expression;
    Statement *statement;
}

%left '+' TOKEN_PLUS
%left '*' TOKEN_MULTIPLY

%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_PLUS
%token TOKEN_MULTIPLY
%token <value> TOKEN_NUMBER
%token TOKEN_OUT

%type <expression> expr
%type <statement> stmt

%%

input
    : stmt { *statement = $1; }
    |
    ;

stmt
    : TOKEN_OUT expr[E] { $$ = new OutStatement($E); }
    ;

expr
    : expr[L] TOKEN_PLUS expr[R] { $$ = new Expression( ePLUS, $L, $R ); }
    | expr[L] TOKEN_MULTIPLY expr[R] { $$ = new Expression( eMULTIPLY, $L, $R ); }
    | TOKEN_LPAREN expr[E] TOKEN_RPAREN { $$ = $E; }
    | TOKEN_NUMBER { $$ = new Expression($1); }
    ;

%%

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
