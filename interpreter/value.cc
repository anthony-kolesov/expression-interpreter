/* Representation of value in program - could be integer or float.
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

#include "value.h"

Value Value::operator+(const Value &r) const {
    if (this->type_ == kInteger && r.type_ == kInteger) {
        return Value(this->intValue_ + r.intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r.type_ == kInteger) {
            result += static_cast<double>(r.intValue_);
        } else {
            result += r.floatValue_;
        }
        return Value(result);
    }
}

Value Value::operator*(const Value &r) const {
    if (this->type_ == kInteger && r.type_ == kInteger) {
        return Value(this->intValue_ * r.intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r.type_ == kInteger) {
            result *= static_cast<double>(r.intValue_);
        } else {
            result *= r.floatValue_;
        }
        return Value(result);
    }
}
