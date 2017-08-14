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

const ValuePtr Value::kNone = std::make_shared<const NoneValue>();

ValuePtr Value::add(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (!this->isScalarFloat() && !r->isScalarFloat()) {
        auto result = this->asInteger() + r->asInteger();
        return std::make_shared<const ScalarValue>(result);
    } else {
        auto result = this->asFloat() + r->asFloat();
        return std::make_shared<const ScalarValue>(result);
    }
}

ValuePtr Value::sub(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (!this->isScalarFloat() && !r->isScalarFloat()) {
        auto result = this->asInteger() - r->asInteger();
        return std::make_shared<const ScalarValue>(result);
    } else {
        auto result = this->asFloat() - r->asFloat();
        return std::make_shared<const ScalarValue>(result);
    }
}

ValuePtr Value::mul(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (!this->isScalarFloat() && !r->isScalarFloat()) {
        auto result = this->asInteger() * r->asInteger();
        return std::make_shared<const ScalarValue>(result);
    } else {
        auto result = this->asFloat() * r->asFloat();
        return std::make_shared<const ScalarValue>(result);
    }
}

ValuePtr Value::div(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (!this->isScalarFloat() && !r->isScalarFloat()) {
        auto result = this->asInteger() / r->asInteger();
        return std::make_shared<const ScalarValue>(result);
    } else {
        auto result = this->asFloat() / r->asFloat();
        return std::make_shared<const ScalarValue>(result);
    }
}

ValuePtr Value::pow(const ValuePtr &r) const {
    if (this->isNone() || r->isNone()) {
        return kNone;
    }

    checkScalarArgs(r);

    if (!this->isScalarFloat() && !r->isScalarFloat()) {
        /* std::pow always returns floating point type, but if both input
         * values were integer, then result should convert into int nicely,
         * unless, of course, it too big.  */
        auto v = std::pow(this->asInteger(), r->asInteger());
        return std::make_shared<const ScalarValue>(static_cast<int>(v));
    } else {
        auto result = std::pow(this->asFloat(), r->asFloat());
        return std::make_shared<const ScalarValue>(result);
    }
}

ValuePtr VectorValue::asScalar() const {
    return (*this->vec_)[this->begin_];
}

const std::string VectorValue::asString() const {
    std::stringstream s;
    s << "{";

    s << (*this->vec_)[this->begin_]->asString();
    for (int i = this->begin_ + 1; i < this->end_; i++) {
        s << ", " << (*this->vec_)[i]->asString();
    }

    s << "}";
    return s.str();
}

ValuePtr VectorValue::next() const {
    if (this->begin_ + 1 == this->end_) {
        return Value::kNone;
    } else {
        return std::make_shared<const VectorValue>(*this,
                                                   this->begin_ + 1,
                                                   this->end_);
    }
}

const std::string IntegerRangeValue::asString() const {
    std::stringstream s;
    s << "{";

    s << this->asInteger();
    for (auto v = this->next(); !v->isNone(); v = v->next()) {
        s << ", " << v->asInteger();
    }

    s << "}";
    return s.str();
}

ValuePtr IntegerRangeValue::asScalar() const {
    return std::make_shared<const ScalarValue>(this->asInteger());
}

ValuePtr IntegerRangeValue::next() const {
    if (this->current_ == this->end_) {
        return Value::kNone;
    } else {
        return std::make_shared<const IntegerRangeValue>(this->current_ + 1,
                                                         this->end_);
    }
}

const std::string ScalarValue::asString() const {
    if (this->type_ == kInteger) {
        return std::to_string(this->intValue_);
    } else {
        return std::to_string(this->floatValue_);
    }
}

int ScalarValue::asInteger() const {
    switch (this->type_) {
        case kInteger:
          return this->intValue_;
        case kFloat:
          return static_cast<int>(this->floatValue_);
        default:
          return 0;
    }
}

double ScalarValue::asFloat() const {
    switch (this->type_) {
        case kInteger:
          return static_cast<double>(this->intValue_);
        case kFloat:
          return this->floatValue_;
        default:
          return 0.0;
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
