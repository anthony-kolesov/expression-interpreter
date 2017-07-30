/*
 * main.c file
 */

#include "expression.h"
#include "statement.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <iostream>
#include <string>

int yyparse(Statement **statement, yyscan_t scanner);

Statement *getAST(const char *stmt, int lineno)
{
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) {
        // couldn't initialize
        return NULL;
    }

    state = yy_scan_string(stmt, scanner);
    yyset_lineno(lineno, scanner);

    Statement *statement;
    if (yyparse(&statement, scanner)) {
        // error parsing
        return NULL;
    }

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return statement;
}

int main(int argc, char *argv[])
{
    /* Columns are counted starting with 0 and I cannot find the way to make
     * them start counting from 1, so for consistency lines should be counted
     * starting with 0 as well.  */
    int lineno = 0;
    while (!std::cin.eof()) {
        int result = 0;

        std::string inputString;
        std::getline(std::cin, inputString);

        if (inputString.size() == 0) {
            continue;
        }

#ifdef DEBUG
        std::cout << "DEBUG:Parsing input line:" << inputString << std::endl;
#endif

        Statement *stmt = getAST(inputString.c_str(), lineno);

        // Update line number,
        lineno += 1;

        if (stmt == NULL) {
            printf("Error!\n");
            return 1;
        }

        stmt->execute();

        delete stmt;
    }

    return 0;
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
