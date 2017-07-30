/* Implementation of possible program statements.  */

#ifndef STATEMENT_H
#define STATEMENT_H

#include <iostream>

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
	virtual void execute() = 0;
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

    virtual void execute() {
        std::cout << this->expr->evaluate() << std::endl;
	}
};

#endif // STATEMENT_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab