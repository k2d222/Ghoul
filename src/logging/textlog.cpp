/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2016                                                               *
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

#include <ghoul/logging/textlog.h>

#include <cstdio>

namespace ghoul {
namespace logging {


TextLog::TextLog(const std::string& filename, bool writeToAppend, bool timeStamping,
                 bool dateStamping, bool categoryStamping, bool logLevelStamping)
    : Log(timeStamping, dateStamping, categoryStamping, logLevelStamping)
    , _printFooter(writeToAppend)
//    , _file(nullptr)
{
    ghoul_assert(!filename.empty(), "Filename must not be empty");
    
    _file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    if (writeToAppend)
        _file.open(filename, std::ofstream::out | std::ofstream::app);
    else
        _file.open(filename, std::ofstream::out | std::ostream::trunc);
}

TextLog::~TextLog() {
    if (_printFooter)
        _file << "--------" << std::endl;
}

void TextLog::log(LogManager::LogLevel level, const std::string& category,
                                                               const std::string& message)
{
    std::string output;
    if (isDateStamping())
        output += "[" + getDateString();
    if (isTimeStamping())
        output += " | " + getTimeString() + "] ";
    else
        output += "] ";
    if (isCategoryStamping())
        output += category + " ";
    if (isLogLevelStamping())
        output += "(" + LogManager::stringFromLevel(level) + ") ";
    if (output != "")
        output += ":\t";
    output += message + '\n';
    writeLine(std::move(output));
}

void TextLog::flush() {
    _file.flush();
}

void TextLog::writeLine(std::string line) {
    _file << std::move(line);
}

} // namespace logging
} // namespace ghoul
