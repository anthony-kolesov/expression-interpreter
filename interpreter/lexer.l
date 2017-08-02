%{
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab

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

COMMENT #.*
OUT "out"
PRINT "print"
VAR "var"

ASSIGN "="
LPAREN "("
RPAREN ")"
PLUS "+"
MULTIPLY "*"

IDENTIFIER [a-zA-Z_][a-zA-Z_0-9]*
STRING_LITERAL "\""[^\"]*"\""
NUMBER [0-9]+
WS [ \r\n\t]*

%%

{COMMENT} { return 0; /* Skip comments.  */ }
{WS} { /* Skip blanks. */ }
{NUMBER} { sscanf(yytext, "%d", &yylval->value); return TOKEN_NUMBER; }
{STRING_LITERAL} {
    /* Strip the quotes.  */
    std::string input_str(yytext+1);
    input_str.pop_back();

    /* This is a very convoluted way to escape `\n' sequence to a real newline.
     * Standard C++ remove_if, replace_if and friends don't work here because
     * they are designed to work on individual elements, regardless of order of
     * those elements. And regular expressions which could have solved this
     * problem quite nicely are not available in GCC 4.8 which I'm using.
     */
    std::string escaped_str;
    escaped_str.reserve(input_str.size());
    for (int i = 0; i < input_str.size(); i++) {
        /* If this is a `\', not a last char in string and next char is `n'. */
        if ((input_str[i] != '\\') || (i == input_str.size() - 1)
            || (input_str[i+1] != 'n')) {
            escaped_str.push_back(input_str[i]);
        } else {
            escaped_str.push_back('\n');
            i++;
        }
    }

    yylval->strvalue = strdup(escaped_str.c_str());
    return TOKEN_STRING;
}

{ASSIGN} { return TOKEN_ASSIGN; }
{MULTIPLY} { return TOKEN_MULTIPLY; }
{PLUS} { return TOKEN_PLUS; }
{LPAREN} { return TOKEN_LPAREN; }
{RPAREN} { return TOKEN_RPAREN; }

{OUT} { return TOKEN_OUT; }
{PRINT} { return TOKEN_PRINT; }
{VAR} { return TOKEN_VAR; }

{IDENTIFIER} {
    yylval->identifier = strdup(yytext);
    return TOKEN_IDENTIFIER;
}

. {
    std::stringstream msg;
    msg << "Unknown symbol `" << yytext << "'";
    user_error(yylloc, msg.str());
    return 1;
}

%%
