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

std::vector<ValuePtr> MapExpression::getResult(
    ValuePtr input, const std::string &paramName,
    std::shared_ptr<const Expression> func) {

    std::vector<ValuePtr> seq;
    seq.reserve(input->getSize());
    Context funcCtx;
    for (; !input->isNone(); input = input->next()) {
        funcCtx.setVariable(paramName, input->asScalar());
        auto newValue = func->evaluate(&funcCtx);
        seq.push_back(newValue);
    }
    return seq;
}

ValuePtr MapExpression::evaluate(Context *ctx) const {
    auto inputVal = this->input_->evaluate(ctx);
    auto inputSize = inputVal->getSize();
    auto slicesCount = std::min<int>(inputSize,
                                     std::thread::hardware_concurrency());
    auto begin = 0, end = 0;
    auto sliceSize = inputSize / slicesCount;
    std::vector< std::future< std::vector<ValuePtr> > > intermediate;

    for (auto i = 0; i < slicesCount; i++) {
        begin = end;
        /* Make sure we don't last elements in last slice, in case inputSize
         * doesn't divide evenly.  */
        if (i == slicesCount - 1) {
            end = inputSize;
        } else {
            end += sliceSize;
        }

        auto future = std::async(std::launch::async, getResult,
                                 inputVal->getSlice(begin, end),
                                 this->paramName_, this->func_);
        intermediate.push_back(std::move(future));
    }

    auto vector = new std::vector<ValuePtr>();
    vector->reserve(inputSize);
    for (auto &&future : intermediate) {
        auto slice = future.get();
        vector->insert(vector->end(), slice.begin(), slice.end());
    }
    return std::make_shared<const VectorValue>(vector);
}

ValuePtr ReduceExpression::getResult(ValuePtr input, ValuePtr dflt,
                                     const std::string &param1,
                                     const std::string &param2,
                                     std::shared_ptr<const Expression> func) {
    Context funcCtx;
    auto result = dflt;
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
    auto inputSize = inputVal->getSize();
    auto slicesCount = std::min<int>(inputSize,
                                     std::thread::hardware_concurrency());

    std::vector< std::future< ValuePtr > > intermediate;
    auto begin = 0, end = 0;
    auto sliceSize = inputSize / slicesCount;

    for (auto i = 0; i < slicesCount; i++) {
        begin = end;
        /* Make sure we don't last elements in last slice, in case inputSize
         * doesn't divide evenly.  */
        if (i == slicesCount - 1) {
            end = inputSize;
        } else {
            end += sliceSize;
        }

        auto future = std::async(std::launch::async, getResult,
                                 inputVal->getSlice(begin, end),
                                 dflt, this->param1Name_, this->param2Name_,
                                 this->func_);
        intermediate.push_back(std::move(future));
    }

    auto intermediateValues = new std::vector<ValuePtr>();
    intermediateValues->reserve(slicesCount);
    for (auto &&future : intermediate) {
        intermediateValues->push_back(future.get());
    }

    auto finalValue = std::make_shared<const VectorValue>(intermediateValues);
    return getResult(finalValue, dflt, this->param1Name_, this->param2Name_,
                     this->func_);
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab
