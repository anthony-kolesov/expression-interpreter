/* Implementation of possible program statements.
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

#ifndef STATEMENT_H_
#define STATEMENT_H_

#include <iostream>

#include "context.h"
#include "expression.h"

/**
 * @brief Abstract class to represent program statements
 */
class Statement {
 public:
    /**
     * @brief Execute statement.
     */
    virtual void execute(Context *ctx) = 0;
};

/**
 * @brief Statements that print expression result to output.
 */
class OutStatement : public Statement {
 private:
    const Expression *expr_;

 public:
    explicit OutStatement(const Expression *expr) : expr_(expr) { }

    ~OutStatement() {
        delete expr_;
    }

    virtual void execute(Context *ctx) {
        std::cout << this->expr_->evaluate(ctx);
    }
};

class PrintStatement : public Statement {
 private:
    const std::string str_;

 public:
    explicit PrintStatement(const std::string &s) : str_(s) { }

    virtual void execute(Context *ctx) {
        std::cout << this->str_;
    }
};

class VarStatement : public Statement {
 private:
    const std::string name_;
    const Expression *expr_;

 public:

    VarStatement(const std::string &name, const Expression *expr) :
            name_(name), expr_(expr) { }

    ~VarStatement() {
        delete this->expr_;
    }

    virtual void execute(Context *ctx) {
        ctx->setVariable(this->name_, this->expr_->evaluate(ctx));
    }
};

#endif  // STATEMENT_H_

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
