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

#include <future>
#include <memory>
#include <mutex>
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
            auto msg = "Cannor perform arithmetic operation on vector values.";
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
    std::vector<ValuePtr> sequence_;
    int index_;

 public:
    virtual bool isScalar() const {
        return false;
    }

    VectorValue(const std::vector<ValuePtr> &v, int index)
        : sequence_(v), index_(index) {
    }

    virtual int asInteger() const {
        return this->sequence_[this->index_]->asInteger();
    }

    virtual ValuePtr asScalar() const;

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

    virtual ValuePtr next() const;
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

class AsyncValue : public Value {
 private:
    /* Most of the Value classes are presumed to be immutable and therefore we
     * use mostly pointers to const Values. But async value are a special case,
     * since their value is unknown at creating, therefore they are mutable by
     * definition. I think it is valuable to keep values const in general,
     * therefore this class has to declare all of it's data-members mutable and
     * use mutex to ensure consistence in future usage. */
    mutable std::future<ValuePtr> futureValue_;
    mutable ValuePtr value_;
    mutable std::mutex mtx_;

    void sync() const {
        std::lock_guard<std::mutex> lock(this->mtx_);
        if (this->futureValue_.valid()) {
            this->value_ = this->futureValue_.get();
        }
    }

 public:
    virtual bool isScalar() const {
        this->sync();
        return this->value_->isScalar();
    }

    virtual bool isScalarFloat() const {
        this->sync();
        return this->value_->isScalarFloat();
    }

    AsyncValue(std::future<ValuePtr> &fv) {
        this->futureValue_ = std::move(fv);
    }

    virtual const std::string asString() const {
        this->sync();
        return this->value_->asString();
    }

    virtual int asInteger() const {
        this->sync();
        return this->value_->asInteger();
    }

    virtual double asFloat() const {
        this->sync();
        return this->value_->asFloat();
    }

    virtual ValuePtr next() const {
        this->sync();
        return Value::kNone;
    }
};

#endif  // VALUE_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
