/* Definition of the structure that describes program context.
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

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <string>
#include <unordered_map>

/**
 * @brief Program context
 */
class Context {
 private:
    std::unordered_map<std::string, int> variables_;

 public:
    Context() : variables_() { }

    void setVariable(const std::string &name, int value);
    int getVariable(const std::string &name);
};

#endif  // CONTEXT_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
