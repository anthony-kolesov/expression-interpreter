%{

/*
 * lexer.l file
 * To generate the lexical analyzer run: "flex lexer.l"
 */

#include <cstdio>
#include <sstream>

#include "error.h"
#include "expression.h"
#include "statement.h"
#include "parser.h"

/* Update line/column information for a token.  */
#define YY_USER_ACTION \
    yylloc->first_column = yycolumn; yylloc->last_column = yycolumn + yyleng - 1; \
    yylloc->first_line = yylloc->last_line = yylineno; \
    yycolumn += yyleng;

%}

%option outfile="lexer.c" header-file="lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge
%option bison-locations
%option yylineno

OUT "out"

LPAREN      "("
RPAREN      ")"
PLUS        "+"
MULTIPLY    "*"

NUMBER      [0-9]+
WS          [ \r\n\t]*

%%

{WS}            { /* Skip blanks. */ }
{NUMBER}        { sscanf(yytext, "%d", &yylval->value); return TOKEN_NUMBER; }

{MULTIPLY}      { return TOKEN_MULTIPLY; }
{PLUS}          { return TOKEN_PLUS; }
{LPAREN}        { return TOKEN_LPAREN; }
{RPAREN}        { return TOKEN_RPAREN; }

{OUT} { return TOKEN_OUT; }

. {
    std::stringstream msg;
    msg << "Unknown symbol `" << yytext << "'";
    user_error(yylloc, msg.str());
    return 1;
}

%%
