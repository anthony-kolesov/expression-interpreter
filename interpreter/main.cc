/*
 * main.c file
 */

#include "expression.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <iostream>
#include <string>

int yyparse(Expression **expression, yyscan_t scanner);

Expression *getAST(const char *expr)
{
    Expression *expression;
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) {
        // couldn't initialize
        return NULL;
    }

    state = yy_scan_string(expr, scanner);

    if (yyparse(&expression, scanner)) {
        // error parsing
        return NULL;
    }

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return expression;
}

int main(int argc, char *argv[])
{
    while (!std::cin.eof()) {
        Expression *e = NULL;
        int result = 0;

        std::string inputString;
        std::getline(std::cin, inputString);

        if (inputString.size() == 0) {
            continue;
        }

        e = getAST(inputString.c_str());

        if (e == NULL) {
            printf("Error!\n");
            return 1;
        }

        result = e->evaluate();

        printf("%d\n", result);

        delete e;
    }

    return 0;
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
