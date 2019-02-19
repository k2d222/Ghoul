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

#include <ghoul/lua/luastate.h>

#include <ghoul/lua/lua_helper.h>

namespace ghoul::lua {

LuaState::LuaState(IncludeStandardLibrary include)
    : _state(ghoul::lua::createNewLuaState(include))
{}

LuaState::LuaState(LuaState&& other) noexcept
    : _state(other._state)
{
    other._state = nullptr;
}

LuaState::~LuaState() {
    if (_state) {
        ghoul::lua::destroyLuaState(_state);
    }
}

LuaState& LuaState::operator=(LuaState&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    _state = other._state;
    other._state = nullptr;
    return *this;
}

LuaState::operator lua_State*() const {
    return _state;
}

}  // namespace ghoul::lua
