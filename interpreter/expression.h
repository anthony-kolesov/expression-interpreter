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
    virtual Value evaluate(Context *ctx) const = 0;
};

class AddExpression : public Expression {
 private:
    Expression *left_;
    Expression *right_;

 public:
    AddExpression(Expression* left, Expression* right) {
        this->left_ = left;
        this->right_ = right;
    }

    virtual ~AddExpression() {
        delete this->left_;
        delete this->right_;
    }

    virtual Value evaluate(Context *ctx) const {
        return this->left_->evaluate(ctx) + this->right_->evaluate(ctx);
    }
};

class DivExpression : public Expression {
 private:
    Expression *left_;
    Expression *right_;

 public:
    DivExpression(Expression* left, Expression* right) {
        this->left_ = left;
        this->right_ = right;
    }

    virtual ~DivExpression() {
        delete this->left_;
        delete this->right_;
    }

    virtual Value evaluate(Context *ctx) const {
        return this->left_->evaluate(ctx) / this->right_->evaluate(ctx);
    }
};

class IdentifierExpression : public Expression {
 private:
    std::string identifier_;

 public:
    explicit IdentifierExpression(const std::string &identifier) {
        this->identifier_ = identifier;
    }

    virtual Value evaluate(Context *ctx) const {
        return ctx->getVariable(this->identifier_);
    }
};

class MulExpression : public Expression {
 private:
    Expression *left_;
    Expression *right_;

 public:
    MulExpression(Expression* left, Expression* right) {
        this->left_ = left;
        this->right_ = right;
    }

    virtual ~MulExpression() {
        delete this->left_;
        delete this->right_;
    }

    virtual Value evaluate(Context *ctx) const {
        return this->left_->evaluate(ctx) * this->right_->evaluate(ctx);
    }
};

class PowExpression : public Expression {
 private:
    Expression *left_;
    Expression *right_;

 public:
    PowExpression(Expression* left, Expression* right) {
        this->left_ = left;
        this->right_ = right;
    }

    virtual ~PowExpression() {
        delete this->left_;
        delete this->right_;
    }

    virtual Value evaluate(Context *ctx) const {
        return this->left_->evaluate(ctx).pow(this->right_->evaluate(ctx));
    }
};

class RangeExpression : public Expression {
 private:
    Expression *begin_;
    Expression *end_;

 public:
    RangeExpression(Expression* begin, Expression* end) : begin_(begin),
            end_(end) { }

    virtual ~RangeExpression() {
        delete this->begin_;
        delete this->end_;
    }

    virtual Value evaluate(Context *ctx) const {
        int begin  = this->begin_->evaluate(ctx).asInteger();
        int end = this->end_->evaluate(ctx).asInteger();
        return Value(begin, end);
    }
};

class SubExpression : public Expression {
 private:
    Expression *left_;
    Expression *right_;

 public:
    SubExpression(Expression* left, Expression* right) {
        this->left_ = left;
        this->right_ = right;
    }

    virtual ~SubExpression() {
        delete this->left_;
        delete this->right_;
    }

    virtual Value evaluate(Context *ctx) const {
        return this->left_->evaluate(ctx) - this->right_->evaluate(ctx);
    }
};

class ValueExpression : public Expression {
 private:
    Value value_;

 public:
    explicit ValueExpression(const Value &v) {
        this->value_ = v;
    }

    virtual Value evaluate(Context *ctx) const {
        return this->value_;
    }
};

#endif  // EXPRESSION_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
