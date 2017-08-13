/* Interpreter expressions.
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

ValuePtr MapExpression::getResult(ValuePtr input, const std::string &paramName,
                                  std::shared_ptr<const Expression> func) {
    std::vector<ValuePtr> seq;
    Context funcCtx;
    for (; !input->isNone(); input = input->next()) {
        funcCtx.setVariable(paramName, input->asScalar());
        auto newValue = func->evaluate(&funcCtx);
        seq.push_back(newValue);
    }
    return std::make_shared<const VectorValue>(seq, 0);
}

ValuePtr MapExpression::evaluate(Context *ctx) const {
    auto inputVal = this->input_->evaluate(ctx);
    auto future = std::async(std::launch::async, getResult, inputVal,
                             this->paramName_, this->func_);
    return std::make_shared<const AsyncValue>(future);
}

ValuePtr ReduceExpression::getResult(ValuePtr input, ValuePtr dflt,
                                     const std::string &param1,
                                     const std::string &param2,
                                     std::shared_ptr<const Expression> func) {
    Context funcCtx;
    auto result = dflt;
    int i = 0;
    for (; !input->isNone(); input = input->next()) {
        funcCtx.setVariable(param1, result);
        funcCtx.setVariable(param2, input->asScalar());
        result = func->evaluate(&funcCtx);
    }
    return result;
}

ValuePtr ReduceExpression::evaluate(Context *ctx) const {
    auto inputVal = this->input_->evaluate(ctx);
    auto dflt = default_->evaluate(ctx);
    auto future = std::async(std::launch::async, getResult, inputVal,
                             dflt, this->param1Name_, this->param2Name_,
                             this->func_);
    return std::make_shared<const AsyncValue>(future);
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
