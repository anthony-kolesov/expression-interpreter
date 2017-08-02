/* Main body of interpreter application
 * Copyright (C) 2017 Anton Kolesov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "context.h"
#include "expression.h"
#include "statement.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <iostream>
#include <string>

int yyparse(Statement **statement, yyscan_t scanner);

Statement *getAST(const char *stmt, int lineno) {
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) {
        // couldn't initialize
        return NULL;
    }

    state = yy_scan_string(stmt, scanner);
    state->yy_bs_column = 0;
    yyset_lineno(lineno, scanner);

    Statement *statement = nullptr;
    if (yyparse(&statement, scanner)) {
        // error parsing
        return NULL;
    }

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return statement;
}

int main(int argc, char *argv[]) {
    /* Columns are counted starting with 0 and I cannot find the way to make
     * them start counting from 1, so for consistency lines should be counted
     * starting with 0 as well.  */
    int lineno = 0;

    Context ctx;

    while (!std::cin.eof()) {
        std::string input_string;
        std::getline(std::cin, input_string);

        // Update line number,
        lineno += 1;

        if (input_string.size() == 0) {
            continue;
        }

#ifdef DEBUG
        std::cout << "DEBUG:Parsing input line:" << input_string << std::endl;
#endif

        Statement *stmt = getAST(input_string.c_str(), lineno);

        if (stmt == NULL) {
            continue;
        }

        stmt->execute(&ctx);

        delete stmt;
    }

    return 0;
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
