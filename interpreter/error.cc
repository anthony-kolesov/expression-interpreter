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

#include "error.h"

int user_error(const YYLTYPE *loc, const std::string &msg) {
    std::cerr << "ERROR:" << loc->first_line << "," << loc->first_column;
    if (loc->first_line != loc->last_line
            || loc->first_column != loc->last_column) {
        std::cerr << "-" << loc->last_line << "," << loc->last_column;
    }
    std::cerr << ":" << msg << std::endl;
}

int user_error(int line, const std::string &msg) {
    std::cerr << "ERROR:" << line << ":" << msg << std::endl;
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
