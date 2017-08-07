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
/* Can't use std::make_shared, because a private constructor is used.  */
const ValuePtr Value::kNone
    = std::shared_ptr<const Value>(new Value(ValueType::kNoneType));

ValuePtr Value::add(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r->type_ == kInteger) {
        return std::make_shared<const Value>(this->intValue_ + r->intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r->type_ == kInteger) {
            result += static_cast<double>(r->intValue_);
        } else {
            result += r->floatValue_;
        }
        return std::make_shared<const Value>(result);
    }
}

ValuePtr Value::sub(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r->type_ == kInteger) {
        return std::make_shared<const Value>(this->intValue_ - r->intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r->type_ == kInteger) {
            result -= static_cast<double>(r->intValue_);
        } else {
            result -= r->floatValue_;
        }
        return std::make_shared<const Value>(result);
    }
}

ValuePtr Value::mul(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r->type_ == kInteger) {
        return std::make_shared<const Value>(this->intValue_ * r->intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r->type_ == kInteger) {
            result *= static_cast<double>(r->intValue_);
        } else {
            result *= r->floatValue_;
        }
        return std::make_shared<const Value>(result);
    }
}

ValuePtr Value::div(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r->type_ == kInteger) {
        return std::make_shared<const Value>(this->intValue_ / r->intValue_);
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r->type_ == kInteger) {
            result /= static_cast<double>(r->intValue_);
        } else {
            result /= r->floatValue_;
        }
        return std::make_shared<const Value>(result);
    }
}

ValuePtr Value::pow(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (this->type_ == kInteger && r->type_ == kInteger) {
        /* std::pow always returns floating point type, but if both input
         * values were integer, then result should convert into int nicely,
         * unless, of course, it too big.  */
        auto v = std::pow(this->intValue_, r->intValue_);
        return std::make_shared<const Value>(static_cast<int>(v));
    } else {
        double result;
        if (this->type_ == kInteger) {
            result = static_cast<double>(this->intValue_);
        } else {
            result = this->floatValue_;
        }
        if (r->type_ == kInteger) {
            result = std::pow(result, static_cast<double>(r->intValue_));
        } else {
            result = std::pow(result, r->floatValue_);
        }
        return std::make_shared<const Value>(result);
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
