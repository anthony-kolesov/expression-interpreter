/* Definition of the structure used to build the syntax tree.
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

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "context.h"
#include "value.h"

/**
 * @brief Interpeter expression.
 */
class Expression {
 public:
    Expression() { }

    virtual ~Expression() { }

    /**
     * @brief Evaluate value of this expression.
     */
    virtual ValuePtr evaluate(Context *ctx) const = 0;
};

class AddExpression : public Expression {
 private:
    std::unique_ptr<const Expression> left_;
    std::unique_ptr<const Expression> right_;

 public:
    AddExpression(const Expression* left, const Expression* right)
        : left_(left), right_(right) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        auto l = this->left_->evaluate(ctx);
        return l->add(this->right_->evaluate(ctx));
    }
};

class DivExpression : public Expression {
 private:
    std::unique_ptr<const Expression> left_;
    std::unique_ptr<const Expression> right_;

 public:
    DivExpression(const Expression* left, const Expression* right)
        : left_(left), right_(right) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        auto l = this->left_->evaluate(ctx);
        return l->div(this->right_->evaluate(ctx));
    }
};

class IdentifierExpression : public Expression {
 private:
    std::string identifier_;

 public:
    explicit IdentifierExpression(const std::string &identifier)
        : identifier_(identifier) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        return ctx->getVariable(this->identifier_);
    }
};

class MapExpression : public Expression {
 private:
    std::string paramName_;
    std::unique_ptr<const Expression> input_;
    std::unique_ptr<const Expression> func_;

 public:
    MapExpression(const Expression *input, const std::string &paramName,
                  const Expression *func)
        : input_(input), paramName_(paramName), func_(func) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        std::vector<ValuePtr> seq;
        Context funcCtx;
        for (auto i = this->input_->evaluate(ctx);
             !i->isNone();
             i = i->next()) {
            /* i is a range value, but context should contain a scalar value -
             * current value in a range.  */
            auto param = std::make_shared<const ScalarValue>(i->asInteger());
            funcCtx.setVariable(this->paramName_, param);
            auto newValue = this->func_->evaluate(&funcCtx);
            seq.push_back(newValue);
        }
        return std::make_shared<const VectorValue>(seq, 0);
    }
};

class MulExpression : public Expression {
 private:
    std::unique_ptr<const Expression> left_;
    std::unique_ptr<const Expression> right_;

 public:
    MulExpression(const Expression* left, const Expression* right)
        : left_(left), right_(right) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        auto l = this->left_->evaluate(ctx);
        return l->mul(this->right_->evaluate(ctx));
    }
};

class PowExpression : public Expression {
 private:
    std::unique_ptr<const Expression> left_;
    std::unique_ptr<const Expression> right_;

 public:
    PowExpression(const Expression* left, const Expression* right)
        : left_(left), right_(right) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        auto l = this->left_->evaluate(ctx);
        return l->pow(this->right_->evaluate(ctx));
    }
};

class RangeExpression : public Expression {
 private:
    std::unique_ptr<const Expression> begin_;
    std::unique_ptr<const Expression> end_;

 public:
    RangeExpression(const Expression* begin, const Expression* end)
        : begin_(begin), end_(end) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        int begin  = this->begin_->evaluate(ctx)->asInteger();
        int end = this->end_->evaluate(ctx)->asInteger();
        return std::make_shared<const IntegerRangeValue>(begin, end);
    }
};

class SubExpression : public Expression {
 private:
    std::unique_ptr<const Expression> left_;
    std::unique_ptr<const Expression> right_;

 public:
    SubExpression(const Expression* left, const Expression* right)
        : left_(left), right_(right) {
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        auto l = this->left_->evaluate(ctx);
        return l->sub(this->right_->evaluate(ctx));
    }
};

class ValueExpression : public Expression {
 private:
    ValuePtr value_;

 public:
    explicit ValueExpression(const ScalarValue &v) {
        this->value_ = std::make_shared<const ScalarValue>(v);
    }

    virtual ValuePtr evaluate(Context *ctx) const {
        return this->value_;
    }
};

#endif  // EXPRESSION_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
