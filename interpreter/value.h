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

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class Value;
typedef std::shared_ptr<const Value> ValuePtr;

/* Creating a hierarchy of IntegerValue and FloatValue might be closer to
 * canonical object oriented design, but I'm not sure this would really make
 * code any simpler. */
class Value {
 private:
    enum ValueType {
        kNoneType,
        kInteger,
        kFloat,
        kIntegerRange,
    };

    ValueType type_;
    union {
        struct {
            int intValue_;
            int endValue_;
        };
        double floatValue_;
    };

    static const char kNoneString[];

    explicit Value(ValueType vt) : type_(vt) { }

    /**
     * @brief Check that arguments are valid for scalar arithmetic operations,
     * i.e. are not range values.
     */
    void checkScalarArgs(const ValuePtr &r) const {
        if ((this->type_ == kIntegerRange) || (r->type_ == kIntegerRange)) {
            auto msg = "Cannor perform arithmetic operation on range values.";
            throw std::invalid_argument(msg);
        }
    }

 public:
    static const ValuePtr kNone;

    /* This constructor is needed for map<> operations in context.h, but
     * otherwise should never be used.  */
    Value() : type_(kInteger), intValue_(0xDEADBEEF) { }

    explicit Value(int v) : type_(kInteger), intValue_(v) { }
    explicit Value(double v) : type_(kFloat), floatValue_(v) { }
    Value(int begin, int end) : type_(kIntegerRange), intValue_(begin),
            endValue_(end) { }

    Value(const Value& v) {
        this->type_ = v.type_;
        if (v.type_ == kInteger) {
            this->intValue_ = v.intValue_;
        } else if (v.type_ == kIntegerRange) {
            this->intValue_ = v.intValue_;
            this->endValue_ = v.endValue_;
        } else {
            this->floatValue_ = v.floatValue_;
        }
    }

    bool isNone() const {
        return (this->type_ == kNoneType);
    }

    virtual const std::string asString() const {
        if (this->type_ == kNoneType) {
            return kNoneString;
        } else if (this->type_ == kInteger) {
            return std::to_string(this->intValue_);
        } else if (this->type_ == kIntegerRange) {
            std::stringstream s;
            s << "{";

            s << this->intValue_;
            for (auto v = this->next(); !v->isNone(); v = v->next()) {
                s << ", " << v->intValue_;
            }

            s << "}";
            return s.str();
        } else {
            return std::to_string(this->floatValue_);
        }
    }

    int asInteger() const {
        switch (this->type_) {
            case kIntegerRange:
            // Fallthrough.
            case kInteger:
              return this->intValue_;
            case kFloat:
              return static_cast<int>(this->floatValue_);
            default:
              return 0;
        }
    }

    ValuePtr add(const ValuePtr &r) const;
    ValuePtr sub(const ValuePtr &r) const;
    ValuePtr mul(const ValuePtr &r) const;
    ValuePtr div(const ValuePtr &r) const;
    ValuePtr pow(const ValuePtr &r) const;

    /**
     * @brief Get next value in a sequence.
     * @returns Next value in an integer sequence or kNone if this is the last
     * item in sequence.
     */
    virtual ValuePtr next() const {
        if ((this->intValue_ == this->endValue_)
            || (this->type_ != kIntegerRange)) {
            return Value::kNone;
        } else {
            return std::make_shared<const Value>(this->intValue_ + 1,
                                                 this->endValue_);
        }
    }
};

class VectorValue : public Value {
 private:
    std::vector<ValuePtr> sequence_;
    int index_;

 public:
    VectorValue(const std::vector<ValuePtr> &v, int index)
        : Value(*v[index]), sequence_(v), index_(index) {
    }

    virtual const std::string asString() const {
        std::stringstream s;
        s << "{";

        s << this->sequence_[this->index_]->asString();
        for (int i = this->index_ + 1; i < this->sequence_.size(); i++) {
            s << ", " << this->sequence_[i]->asString();
        }

        s << "}";
        return s.str();
    }
};

#endif  // VALUE_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
