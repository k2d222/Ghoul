/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2019                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#include <ghoul/misc/exception.h>

#include <ghoul/misc/assert.h>

namespace ghoul {

RuntimeError::RuntimeError(std::string msg, std::string comp)
    : std::runtime_error(comp.empty() ? msg : "(" + comp + ") " + msg)
    , message(std::move(msg))
    , component(std::move(comp))
{
    ghoul_assert(!message.empty(), "Message must not be empty");
}

RuntimeError::~RuntimeError() {
    // I don't really understand why doing this manually is necessary;  but if the dtor is
    // not specified explicitly, the automatic cleanup of the two stored strings fails
    // catastrophically --abock  2018-11-30
    message.clear();
    component.clear();
}

FileNotFoundError::FileNotFoundError(std::string f, std::string comp)
    : RuntimeError("Could not find file '" + f + "'", std::move(comp))
    , file(std::move(f))
{}

} // namespace ghoul
