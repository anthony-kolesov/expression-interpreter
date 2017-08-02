/* Implementation of functions used to build the syntax tree.
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

#include "expression.h"

#include <stdlib.h>

int Expression::evaluate(Context *ctx) const {
    switch (this->type_) {
        case eVALUE:
            return this->value_;
        case eMULTIPLY:
            return this->left_->evaluate(ctx) * this->right_->evaluate(ctx);
        case ePLUS:
            return this->left_->evaluate(ctx) + this->right_->evaluate(ctx);
        case eIDENTIFIER:
            return ctx->getVariable(this->identifier_);
        default:
            // shouldn't be here
            return 0;
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
