/* Program context.
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
#include <sstream>
#include <stdexcept>

void Context::setVariable(const std::string &name, const Value &value) {
    this->variables_[name] = value;
}

Value Context::getVariable(const std::string &name) {
    if (this->variables_.count(name) == 0) {
        std::stringstream s;
        s << "Unknown identifier: " << name;
        throw std::out_of_range(s.str().c_str());
    }
    return this->variables_[name];
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
