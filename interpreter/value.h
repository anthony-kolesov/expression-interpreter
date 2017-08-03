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

#ifndef VALUE_H_
#define VALUE_H_

/* Creating a hierarchy of IntegerValue and FloatValue might be closer to
 * canonical object oriented design, but I'm not sure this would really make
 * code any simpler. */
class Value {
 private:
    enum ValueType {
        kInteger,
        kFloat,
    };

    ValueType type_;
    union {
        int intValue_;
        double floatValue_;
    };

 public:
    /* This constructor is needed for map<> operations in context.h, but
     * otherwise should never be used.  */
    Value() : type_(kInteger), intValue_(0xDEADBEEF) { }

    explicit Value(int v) : type_(kInteger), intValue_(v) { }
    explicit Value(double v) : type_(kFloat), floatValue_(v) { }

    Value(const Value& v) {
        this->type_ = v.type_;
        if (v.type_ == kInteger) {
            this->intValue_ = v.intValue_;
        } else {
            this->floatValue_ = v.floatValue_;
        }
    }

    const std::string asString() const {
        if (this->type_ == kInteger) {
            return std::to_string(this->intValue_);
        } else {
            return std::to_string(this->floatValue_);
        }
    }

    Value operator+(const Value &r) const {
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

    Value operator*(const Value &r) const {
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
};

#endif  // VALUE_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab