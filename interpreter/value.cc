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

#include <cmath>

#include "value.h"

const std::string Value::kNoneString = "(none)";
const Value Value::kNone = Value(ValueType::kNoneType);

Value Value::operator+(const Value &r) const {
    if (this->isNone() || r.isNone()) {
        return Value::kNone;
    }

    checkScalarArgs(r);

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

Value Value::operator-(const Value &r) const {
    if (this->isNone() || r.isNone()) {
        return Value::kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r.type_ == kInteger) {
        return Value(this->intValue_ - r.intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r.type_ == kInteger) {
            result -= static_cast<double>(r.intValue_);
        } else {
            result -= r.floatValue_;
        }
        return Value(result);
    }
}

Value Value::operator*(const Value &r) const {
    if (this->isNone() || r.isNone()) {
        return Value::kNone;
    }

    checkScalarArgs(r);

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

Value Value::operator/(const Value &r) const {
    if (this->isNone() || r.isNone()) {
        return Value::kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r.type_ == kInteger) {
        return Value(this->intValue_ / r.intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r.type_ == kInteger) {
            result /= static_cast<double>(r.intValue_);
        } else {
            result /= r.floatValue_;
        }
        return Value(result);
    }
}

Value Value::pow(const Value &r) const {
    if (this->isNone() || r.isNone()) {
        return Value::kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r.type_ == kInteger) {
        /* std::pow always returns floating point type, but if both input
         * values were integer, then result should convert into int nicely,
         * unless, of course, it too big.  */
        return Value(static_cast<int>(std::pow(this->intValue_, r.intValue_)));
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r.type_ == kInteger) {
            result = std::pow(result, static_cast<double>(r.intValue_));
        } else {
            result = std::pow(result, r.floatValue_);
        }
        return Value(result);
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
