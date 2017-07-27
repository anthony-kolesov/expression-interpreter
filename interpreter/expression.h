/*
 * expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

/**
 * @brief The operation type
 */
typedef enum tagEOperationType
{
    eVALUE,
    eMULTIPLY,
    ePLUS
} EOperationType;

/**
 * @brief Interpeter expression.
 */
class Expression
{
  private:
    EOperationType type;///< type of operation

    int value;///< valid only when type is eVALUE
    Expression *left; ///< left side of the tree
    Expression *right;///< right side of the tree

  public:
    Expression(): type(eVALUE), value(0), left(nullptr), right(nullptr) { };
    ~Expression() {
        delete this->left;
        delete this->right;
    }

    /**
     * @param value The number value
     */
    Expression(int value): Expression() {
        this->type = eVALUE;
        this->value = value;
    }

    /**
     * @param type The operation type
     * @param left The left operand
     * @param right The right operand
     */
    Expression(EOperationType type, Expression* left, Expression* right):
        Expression() {

        this->type = type;
        this->left = left;
        this->right = right;
    }

    /**
     * @brief Evaluate value of this expression.
     */
    int evaluate() const;
};

#endif // __EXPRESSION_H__

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
