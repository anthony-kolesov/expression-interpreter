%{
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab

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

%output  "parser.cc"
%defines "parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { Statement **statement }
%parse-param { yyscan_t scanner }

%locations

%union {
    int value;
    double floatValue;
    char *strvalue;
    Expression *expression;
    Statement *statement;
    char *identifier;
}

%left '+' TOKEN_PLUS
%left '-' TOKEN_MINUS
%left '*' TOKEN_MULTIPLY
%left '/' TOKEN_DIVIDE
%left '^' TOKEN_POW
%left '=' TOKEN_ASSIGN

%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LCURLY
%token TOKEN_RCURLY
%token TOKEN_COMMA
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MULTIPLY
%token TOKEN_DIVIDE
%token TOKEN_POW
%token TOKEN_ASSIGN
%token TOKEN_LAMBDA
%token <floatValue> TOKEN_FLOAT_NUMBER
%token <value> TOKEN_NUMBER
%token <strvalue> TOKEN_STRING
%token <identifier> TOKEN_IDENTIFIER
%token TOKEN_OUT
%token TOKEN_MAP
%token TOKEN_PRINT
%token TOKEN_VAR

%type <expression> expr
%type <statement> stmt

%%

input
    : stmt { *statement = $1; }
    |
    ;

stmt
    : TOKEN_OUT expr[E] { $$ = new OutStatement($E); }
    | TOKEN_PRINT TOKEN_STRING[S] { $$ = new PrintStatement(std::string($S)); }
    | TOKEN_VAR TOKEN_IDENTIFIER[I] TOKEN_ASSIGN expr[E] {
        $$ = new VarStatement(std::string($I), $E);
    }
    ;

expr
    : expr[L] TOKEN_PLUS expr[R] { $$ = new AddExpression($L, $R); }
    | expr[L] TOKEN_MINUS expr[R] { $$ = new SubExpression($L, $R); }
    | expr[L] TOKEN_MULTIPLY expr[R] { $$ = new MulExpression($L, $R); }
    | expr[L] TOKEN_DIVIDE expr[R] { $$ = new DivExpression($L, $R); }
    | expr[L] TOKEN_POW expr[R] { $$ = new PowExpression($L, $R); }
    | TOKEN_LPAREN expr[E] TOKEN_RPAREN { $$ = $E; }
    | TOKEN_NUMBER { $$ = new ValueExpression(ScalarValue($1));}
    | TOKEN_FLOAT_NUMBER { $$ = new ValueExpression(ScalarValue($1)); }
    | TOKEN_IDENTIFIER { $$ = new IdentifierExpression(std::string($1)); }
    | TOKEN_LCURLY expr[B] TOKEN_COMMA expr[E] TOKEN_RCURLY {
        $$ = new RangeExpression($B, $E);
    }
    | TOKEN_MAP TOKEN_LPAREN expr[E] TOKEN_COMMA TOKEN_IDENTIFIER[I]
        TOKEN_LAMBDA expr[L] TOKEN_RPAREN {
        $$ = new MapExpression($E, $I, $L);
    }
    ;

%%

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
