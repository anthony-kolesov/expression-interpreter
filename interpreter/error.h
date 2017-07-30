/* Code to report errors to the user.
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

#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>

#include "expression.h"
#include "statement.h"
#include "parser.h"

/**
 * @brief Report an error to the user in format "ERROR:<line>,<column>:msg".
 *
 * @param loc Location of error in code.  last_{line,column} are printed only
 * when different from first_{line,column}.
 * @param msg Error text to show to the user.
 */
int user_error(const YYLTYPE *loc, const std::string &msg);

#endif // ERROR_H
