/*
 * expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "expression.h"

#include <stdlib.h>

int Expression::evaluate() const {
    switch (this->type) {
        case eVALUE:
            return this->value;
        case eMULTIPLY:
            return this->left->evaluate() * this->right->evaluate();
        case ePLUS:
            return this->left->evaluate() + this->right->evaluate();
        default:
            // shouldn't be here
            return 0;
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
