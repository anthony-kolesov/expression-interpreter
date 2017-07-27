%{

/*
 * parser.y file
 * To generate the parser run: "bison parser.y"
 */

#include "expression.h"
#include "parser.h"
#include "lexer.h"

int yyerror(Expression **expression, yyscan_t scanner, const char *msg) {
    // Add error handling routine as needed
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
%parse-param { Expression **expression }
%parse-param { yyscan_t scanner }

%union {
    int value;
    Expression *expression;
}

%left '+' TOKEN_PLUS
%left '*' TOKEN_MULTIPLY

%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_PLUS
%token TOKEN_MULTIPLY
%token <value> TOKEN_NUMBER

%type <expression> expr

%%

input
    : expr { *expression = $1; }
    ;

expr
    : expr[L] TOKEN_PLUS expr[R] { $$ = new Expression( ePLUS, $L, $R ); }
    | expr[L] TOKEN_MULTIPLY expr[R] { $$ = new Expression( eMULTIPLY, $L, $R ); }
    | TOKEN_LPAREN expr[E] TOKEN_RPAREN { $$ = $E; }
    | TOKEN_NUMBER { $$ = new Expression($1); }
    ;

%%
