/*
 * main.c file
 */

#include "expression.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <iostream>
#include <string>

int yyparse(SExpression **expression, yyscan_t scanner);

SExpression *getAST(const char *expr)
{
    SExpression *expression;
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

int evaluate(SExpression *e)
{
    switch (e->type) {
        case eVALUE:
            return e->value;
        case eMULTIPLY:
            return evaluate(e->left) * evaluate(e->right);
        case ePLUS:
            return evaluate(e->left) + evaluate(e->right);
        default:
            // shouldn't be here
            return 0;
    }
}

int main(int argc, char *argv[])
{
    while (!std::cin.eof()) {
        SExpression *e = NULL;
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

        result = evaluate(e);

        printf("%d\n", result);

        deleteExpression(e);
    }

    return 0;
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
