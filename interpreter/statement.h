/* Implementation of possible program statements.  */

#ifndef STATEMENT_H
#define STATEMENT_H

#include <iostream>

#include "context.h"
#include "expression.h"

/**
 * @brief Abstract class to represent program statements
 */
class Statement
{
  public:
    /**
     * @brief Execute statement.
     */
    virtual void execute(Context *ctx) = 0;
};

/**
 * @brief Statements that print expression result to output.
 */
class OutStatement : public Statement
{
  private:
    const Expression *expr;

  public:
    OutStatement(const Expression *expr) : expr(expr) { }

    ~OutStatement() {
        delete expr;
    }

    virtual void execute(Context *ctx) {
        std::cout << this->expr->evaluate(ctx);
	}
};

class PrintStatement : public Statement {
  private:
    const std::string str;
  public:
    PrintStatement(const std::string &s) : str(s) { }

    virtual void execute(Context *ctx) {
        std::cout << this->str;
    }
};

#endif // STATEMENT_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
