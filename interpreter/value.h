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

#include <future> // NOLINT
#include <memory>
#include <mutex> // NOLINT
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
 protected:
    /**
     * @brief Check that arguments are valid for scalar arithmetic operations,
     * i.e. are not range values.
     */
    void checkScalarArgs(const ValuePtr &r) const {
        if (!this->isScalar() || !r->isScalar()) {
            auto msg = "Cannot perform arithmetic operation on vector values.";
            throw std::invalid_argument(msg);
        }
    }

 public:
    virtual bool isScalar() const {
        return true;
    }

    virtual bool isScalarFloat() const {
        return false;
    }

    static const ValuePtr kNone;

    virtual bool isNone() const {
        return false;
    }

    virtual const std::string asString() const = 0;

    virtual int asInteger() const = 0;

    virtual double asFloat() const {
        return static_cast<double>(this->asInteger());
    }

    ValuePtr add(const ValuePtr &r) const;
    ValuePtr sub(const ValuePtr &r) const;
    ValuePtr mul(const ValuePtr &r) const;
    ValuePtr div(const ValuePtr &r) const;
    ValuePtr pow(const ValuePtr &r) const;

    virtual ValuePtr asScalar() const {
        return std::shared_ptr<const Value>(this);
    }

    /**
     * @brief Get next value in a sequence.
     * @returns Next value in an integer sequence or kNone if this is the last
     * item in sequence.
     */
    virtual ValuePtr next() const {
        return Value::kNone;
    }

    /**
     * @brief Size of this vector. Makes sense only for non-scalar types.
     */
    virtual int getSize() const {
        return 1;
    }

    /**
     * @brief Get a slice of the original vector.
     */
    virtual ValuePtr getSlice(int begin, int end) const {
        return Value::kNone;
    }
};

class NoneValue : public Value {
 public:
    virtual int asInteger() const {
        return 0xdeadbeef;
    }
    virtual const std::string asString() const {
        static const std::string s = "(none)";
        return s;
    }

    virtual bool isNone() const {
        return true;
    }
};

class VectorValue : public Value {
 private:
    std::shared_ptr< std::vector< ValuePtr > > vec_;
    int begin_;
    int end_;

 public:
    virtual bool isScalar() const {
        return false;
    }

    explicit VectorValue(std::vector<ValuePtr> *vec)
        : vec_(vec), begin_(0), end_(vec->size()) {
    }

    VectorValue(const VectorValue &vv, int begin, int end)
        : vec_(vv.vec_), begin_(begin), end_(end) {
    }

    virtual int asInteger() const {
        return (*this->vec_)[this->begin_]->asInteger();
    }

    virtual ValuePtr asScalar() const;

    virtual const std::string asString() const;

    virtual ValuePtr next() const;

    virtual int getSize() const {
        return this->end_ - this->begin_;
    }

    virtual ValuePtr getSlice(int begin, int end) const {
        return std::make_shared<const VectorValue>(
            *this, this->begin_ + begin, this->begin_ + end);
    }
};

class IntegerRangeValue : public Value {
 private:
    int current_;
    int end_;

 public:
    virtual bool isScalar() const {
        return false;
    }

    IntegerRangeValue(int begin, int end)
        : current_(begin), end_(end) {
    }

    virtual const std::string asString() const;

    virtual int asInteger() const {
        return this->current_;
    }

    virtual ValuePtr asScalar() const;

    /**
     * @brief Get next value in a sequence.
     * @returns Next value in an integer sequence or kNone if this is the last
     * item in sequence.
     */
    virtual ValuePtr next() const;

    virtual int getSize() const {
        return this->end_ - this->current_ + 1;
    }

    virtual ValuePtr getSlice(int begin, int end) const {
        return std::make_shared<const IntegerRangeValue>(
            this->current_ + begin,
            std::min(this->end_, this->current_ + end - 1));
    }
};

class ScalarValue : public Value {
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

    explicit ScalarValue(ValueType vt) : type_(vt) { }

 public:
    virtual bool isScalar() const {
        return true;
    }

    virtual bool isScalarFloat() const {
        return (this->type_ == kFloat);
    }

    explicit ScalarValue(int v)
        : type_(kInteger), intValue_(v) {
    }

    explicit ScalarValue(double v)
        : type_(kFloat), floatValue_(v) {
    }

    ScalarValue(const ScalarValue& v)
        : Value(v) {
        this->type_ = v.type_;
        if (v.type_ == kInteger) {
            this->intValue_ = v.intValue_;
        } else {
            this->floatValue_ = v.floatValue_;
        }
    }

    virtual const std::string asString() const;

    virtual int asInteger() const;

    virtual double asFloat() const;
};

#endif  // VALUE_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
