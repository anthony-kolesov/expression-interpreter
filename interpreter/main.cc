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
#include "error.h"
#include "expression.h"
#include "statement.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <string>

int yyparse(Statement **statement, yyscan_t scanner);

/**
 * @param out_statement isn't modified if there was a syntax error parsing the
 * string, or this was a comment line.
 */
bool getAST(const char *stmt, int lineno, Statement **out_statement ) {
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) {
        // couldn't initialize
        return false;
    }

    state = yy_scan_string(stmt, scanner);
    state->yy_bs_column = 0;
    yyset_lineno(lineno, scanner);

    /* Notice how yyparse doesn't follow Google Code style (and otherwise
     * pretty common) rule that output paramater should after the input
     * parameter.  */
    if (yyparse(out_statement, scanner)) {
        // error parsing
        return false;
    }

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return true;
}

int main(int argc, char *argv[]) {
    /* Columns are counted starting with 0 and I cannot find the way to make
     * them start counting from 1, so for consistency lines should be counted
     * starting with 0 as well.  */
    int lineno = 0;

    Context ctx;
    bool had_error = false;

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

        Statement *stmt = nullptr;
        if (!getAST(input_string.c_str(), lineno, &stmt)) {
            /* There was a syntax error, so interpreter shouldn't execute this
             * or any following statements, but should try to move on with
             * parsing, so it will find as many syntax errors, as possible.  */
            had_error = true;
        }

        if (had_error || stmt == nullptr) {
            continue;
        }

        try {
            stmt->execute(&ctx);
        } catch (std::exception &e) {
            user_error(lineno, e.what());
            had_error = true;
        }

        delete stmt;
    }

    if (had_error) {
        return 1;
    } else {
        return 0;
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
