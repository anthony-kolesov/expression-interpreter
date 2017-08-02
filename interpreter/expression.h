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

/**
 * @brief The operation type
 */
typedef enum tagEOperationType {
    eVALUE,
    eMULTIPLY,
    ePLUS
} EOperationType;

/**
 * @brief Interpeter expression.
 */
class Expression {
 private:
    EOperationType type_;  ///< type of operation

    int value_;  ///< valid only when type is eVALUE
    Expression *left_;  ///< left side of the tree
    Expression *right_;  ///< right side of the tree

 public:
    Expression(): type_(eVALUE), value_(0), left_(nullptr), right_(nullptr) { }
    ~Expression() {
        delete this->left_;
        delete this->right_;
    }

    /**
     * @param value The number value
     */
    explicit Expression(int value): Expression() {
        this->type_ = eVALUE;
        this->value_ = value;
    }

    /**
     * @param type The operation type
     * @param left The left operand
     * @param right The right operand
     */
    Expression(EOperationType type, Expression* left, Expression* right):
        Expression() {
        this->type_ = type;
        this->left_ = left;
        this->right_ = right;
    }

    /**
     * @brief Evaluate value of this expression.
     */
    int evaluate(Context *ctx) const;
};

#endif  // EXPRESSION_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
