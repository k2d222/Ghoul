/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2022                                                               *
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

#include <ghoul/logging/callbacklog.h>

#include <ghoul/fmt.h>

namespace ghoul::logging {

CallbackLog::CallbackLog(CallbackFunction callbackFunction, TimeStamping timeStamping,
            DateStamping dateStamping, CategoryStamping categoryStamping,
            LogLevelStamping logLevelStamping,
            LogLevel minimumLogLevel)
    : Log(timeStamping, dateStamping, categoryStamping, logLevelStamping, minimumLogLevel)
    , _callbackFunction(std::move(callbackFunction))
{}

void CallbackLog::log(LogLevel level, std::string_view category, std::string_view message)
{
    _callbackFunction(createFullMessageString(level, category, message));
}

void CallbackLog::setCallback(CallbackFunction callbackFunction) {
    _callbackFunction = std::move(callbackFunction);
}

const CallbackLog::CallbackFunction& CallbackLog::callback() const {
    return _callbackFunction;
}

} // namespace ghoul::logging
