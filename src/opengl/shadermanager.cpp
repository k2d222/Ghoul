/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2020                                                               *
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

#include <ghoul/opengl/shadermanager.h>

#include <ghoul/misc/assert.h>
#include <ghoul/misc/crc32.h>
#include <ghoul/opengl/shaderobject.h>

namespace ghoul::opengl {

ShaderManager::ShaderManagerError::ShaderManagerError(std::string msg)
    : RuntimeError(std::move(msg), "ShaderManager")
{}

ShaderManager& ShaderManager::ref() {
    static ShaderManager manager;
    return manager;
}

ShaderObject* ShaderManager::shaderObject(unsigned int hashedName) {
    const auto it = _objects.find(hashedName);
    if (it == _objects.cend()) {
        throw ShaderManagerError(fmt::format(
            "Could not find ShaderObject for hash '{}'", hashedName
        ));
    }

    return it->second.get();
}

ShaderObject* ShaderManager::shaderObject(std::string_view name) {
    const unsigned int hash = hashCRC32(name);
    try {
        return shaderObject(hash);
    }
    catch (const ShaderManagerError&) {
        // Repackage the exception as it would otherwise contain only the hash
        throw ShaderManagerError(fmt::format(
            "Could not find ShaderObject for '{}'", name
        ));
    }
}

unsigned int ShaderManager::registerShaderObject(std::string_view name,
                                                 std::unique_ptr<ShaderObject> shader)
{
    const unsigned int hashedName = hashCRC32(name);
    const auto it = _objects.find(hashedName);
    if (it == _objects.cend()) {
        _objects[hashedName] = std::move(shader);
        return hashedName;
    }
    else {
        throw ShaderManagerError(fmt::format("Name '{}' was already registered", name));
    }
}

std::unique_ptr<ShaderObject> ShaderManager::unregisterShaderObject(std::string_view name)
{
    const unsigned int hashedName = hashCRC32(name);
    return unregisterShaderObject(hashedName);
}

std::unique_ptr<ShaderObject> ShaderManager::unregisterShaderObject(
                                                                  unsigned int hashedName)
{
    const auto it = _objects.find(hashedName);
    if (it == _objects.cend()) {
        return nullptr;
    }

    std::unique_ptr<ShaderObject> tmp = std::move(it->second);
    _objects.erase(hashedName);
    return tmp;
}

unsigned int ShaderManager::hashedNameForName(std::string_view name) const {
    return hashCRC32(name);
}

} // namespace ghoul::opengl
