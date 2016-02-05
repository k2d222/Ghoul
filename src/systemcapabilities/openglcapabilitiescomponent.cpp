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

#include "ghoul/systemcapabilities/openglcapabilitiescomponent.h"

#include <ghoul/logging/logmanager.h>
#include <ghoul/opengl/ghoul_gl.h>

#include <algorithm>

#ifdef WIN32
    #include <Windows.h>
    #include <tchar.h>
    #pragma comment(lib, "User32.lib")
    #pragma comment(lib, "Kernel32.lib")
    typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
    typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, DWORD);
#else
    #include <sys/utsname.h>
#endif

namespace {
    const std::string _loggerCat = "OpenGLCapabilities";
}

namespace ghoul {
namespace systemcapabilities {
    
OpenGLCapabilitiesComponent::OpenGLCapabilitiesComponentError::
    OpenGLCapabilitiesComponentError(std::string message)
    : RuntimeError(std::move(message), "OpenGLCapabilitiesComponent")
{}

OpenGLCapabilitiesComponent::GPUVendorError::GPUVendorError(std::string message)
    : OpenGLCapabilitiesComponentError(std::move(message))
{}

void OpenGLCapabilitiesComponent::detectCapabilities() {
    clearCapabilities();

    detectGLSLVersion();
    detectGPUVendor();
    detectGLRenderer();
    detectExtensions();
    detectGLEWVersion();
    detectDriverInformation();

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &_maxTextureSize3D);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_numTextureUnits);
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &_maxFramebufferColorAttachments);
}

void OpenGLCapabilitiesComponent::detectGLSLVersion() {
    glGetIntegerv(GL_MAJOR_VERSION, &(_glVersion._major));
    glGetIntegerv(GL_MINOR_VERSION, &(_glVersion._minor));
}

void OpenGLCapabilitiesComponent::detectGPUVendor() {
    const char* vendor =
        reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    if (vendor)
        _glslCompiler = std::string(vendor);
    else
        throw GPUVendorError("Detection the GPU Vendor failed");

    if (_glslCompiler.find("NVIDIA") != std::string::npos)
        _vendor = Vendor::Nvidia;
    else if (_glslCompiler.find("ATI") != std::string::npos)
        _vendor = Vendor::ATI;
    else if ((_glslCompiler.find("INTEL") != std::string::npos)
        || (_glslCompiler.find("Intel") != std::string::npos))
    {
        _vendor = Vendor::Intel;
    }
    else {
        LINFO("Vendor of graphics card is not in the enum 'Vendor'. Vendor information: "
            << _glslCompiler);
        _vendor = Vendor::Other;
    }
}

void OpenGLCapabilitiesComponent::detectGLRenderer() {
    _glRenderer = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}

void OpenGLCapabilitiesComponent::detectExtensions() {
    GLint nExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
    for (GLint i = 0; i < nExtensions; ++i) {
        const GLubyte* ext = glGetStringi(GL_EXTENSIONS, i);
        const std::string extension = std::string(reinterpret_cast<const char*>(ext));
        _extensions.push_back(extension);
    }
}

void OpenGLCapabilitiesComponent::detectGLEWVersion() {
    _glewVersion._major = GLEW_VERSION_MAJOR;
    _glewVersion._minor = GLEW_VERSION_MINOR;
    _glewVersion._release = GLEW_VERSION_MICRO;
}

void OpenGLCapabilitiesComponent::detectDriverInformation() {
#ifdef GHOUL_USE_WMI
    queryWMI("Win32_VideoController", "DriverVersion", _driverVersion);
    
    std::string driverDateFull;
    queryWMI("Win32_VideoController", "DriverDate", driverDateFull);

    std::stringstream dateStream;
    dateStream << driverDateFull.substr(0,4) << "-"
        << driverDateFull.substr(4,2) << "-"
        << driverDateFull.substr(6,2);
    _driverDate = dateStream.str();


    queryWMI("Win32_VideoController", "AdapterRAM", _adapterRAM);
    // adapterRAM is in bytes
    _adapterRAM = (_adapterRAM / 1024) / 1024;

    queryWMI("Win32_VideoController", "Name", _adapterName);
#endif
}

void OpenGLCapabilitiesComponent::clearCapabilities() {
    _glVersion._major = 0;
    _glVersion._minor = 0;
    _glVersion._release = 0;
    _glslCompiler.clear();
    _vendor = Vendor::Other;
    _glRenderer = "";
    _extensions.clear();
    _glewVersion._major = 0;
    _glewVersion._minor = 0;
    _glewVersion._release = 0;

    _maxTextureSize = -1;
    _maxTextureSize3D = -1;
    _numTextureUnits = -1;

#ifdef GHOUL_USE_WMI
    _driverVersion = "";
    _driverDate = "";
    _adapterRAM = 0;
    _adapterName = "";
#endif
}

std::vector<SystemCapabilitiesComponent::CapabilityInformation>
    OpenGLCapabilitiesComponent::capabilities() const
{
    std::vector<SystemCapabilitiesComponent::CapabilityInformation> result;
    result.push_back({ "OpenGL Version", _glVersion.toString(), Verbosity::Minimal });
    result.push_back({ "OpenGL Compiler", _glslCompiler, Verbosity::Minimal });
    result.push_back({ "OpenGL Renderer", _glRenderer, Verbosity::Minimal });
    result.push_back({"GPU Vendor", gpuVendorString(), Verbosity::Minimal });
    result.push_back({"GLEW Version", _glewVersion.toString(),Verbosity::Minimal });     
#ifdef GHOUL_USE_WMI
    result.push_back({ "GPU Name", _adapterName, Verbosity::Minimal });
    result.push_back({ "GPU Driver Version", _driverVersion, Verbosity::Minimal });
    result.push_back({ "GPU Driver Date", _driverDate, Verbosity::Minimal });
    result.push_back({
        "GPU RAM", std::to_string(_adapterRAM) + " MB",Verbosity::Minimal }
    );
#endif

    result.push_back({
        "Max Texture Size", std::to_string(_maxTextureSize), Verbosity::Default }
    );
    result.push_back({
        "Max 3D Texture Size", std::to_string(_maxTextureSize3D), Verbosity::Default }
    );
    result.push_back({
        "Num of Texture Units", std::to_string(_numTextureUnits), Verbosity::Default }
    );
    result.push_back({
        "FBO Color Attachments",
        std::to_string(_maxFramebufferColorAttachments),
        Verbosity::Default
    });

    std::stringstream s;
    if (!_extensions.empty()) {
        for (size_t i = 0; i < _extensions.size() - 1; ++i)
            s << _extensions[i] << ", ";
        s << _extensions[_extensions.size() - 1] << "\n";
    }
    result.push_back({ "Extensions", s.str(), Verbosity::Full });
    return result;
}

OpenGLCapabilitiesComponent::Version OpenGLCapabilitiesComponent::openGLVersion() const {
    return _glVersion;
}

const std::string& OpenGLCapabilitiesComponent::glslCompiler() const {
    return _glslCompiler;
}

OpenGLCapabilitiesComponent::Vendor OpenGLCapabilitiesComponent::gpuVendor() const {
    return _vendor;
}

const std::vector<std::string>& OpenGLCapabilitiesComponent::extensions() const {
    return _extensions;
}

bool OpenGLCapabilitiesComponent::isExtensionSupported(const std::string& extension) const {
    auto result =
        std::find(_extensions.begin(), _extensions.end(), extension);
    return (result != _extensions.end());
}

int OpenGLCapabilitiesComponent::maximumNumberOfTextureUnits() const {
    return _numTextureUnits;
}

std::string OpenGLCapabilitiesComponent::gpuVendorString() const {
    switch (_vendor) {
        case Vendor::Nvidia:
            return "Nvidia";
        case Vendor::ATI:
            return "ATI";
        case Vendor::Intel:
            return "Intel";
        default:
            return "other";
    }
}

std::string OpenGLCapabilitiesComponent::name() const {
    return "OpenGL";
}

/////////////////////////////
/// OpenGLVersion
/////////////////////////////

unsigned int packVersion(int major, int minor, int release) {
    // safe since: 2^8 * 1000 * 1000 < 2^32
    return
        major * 1000 * 1000 +
        minor * 1000        +
        release;
}

bool OpenGLCapabilitiesComponent::Version::operator==(const Version& rhs) const {
    return (_major == rhs._major) && (_minor == rhs._minor) && (_release == rhs._release);
}

bool OpenGLCapabilitiesComponent::Version::operator!=(const Version& rhs) const {
    return !(*this == rhs);
}

bool OpenGLCapabilitiesComponent::Version::operator<(const Version& rhs) const {
    const unsigned int numThis = packVersion(_major, _minor, _release);
    const unsigned int numRhs = packVersion(rhs._major, rhs._minor, rhs._release);

    return numThis < numRhs;
}

bool OpenGLCapabilitiesComponent::Version::operator<=(const Version& rhs) const {
    const unsigned int numThis = packVersion(_major, _minor, _release);
    const unsigned int numRhs = packVersion(rhs._major, rhs._minor, rhs._release);

    return numThis <= numRhs;
}

bool OpenGLCapabilitiesComponent::Version::operator>(const Version& rhs) const {
    const unsigned int numThis = packVersion(_major, _minor, _release);
    const unsigned int numRhs = packVersion(rhs._major, rhs._minor, rhs._release);

    return numThis > numRhs;
}

bool OpenGLCapabilitiesComponent::Version::operator>=(const Version& rhs) const {
    const unsigned int numThis = packVersion(_major, _minor, _release);
    const unsigned int numRhs = packVersion(rhs._major, rhs._minor, rhs._release);

    return numThis >= numRhs;
}

std::string OpenGLCapabilitiesComponent::Version::toString() const {
    using std::to_string;
    if (_release != 0)
        return to_string(_major) + "." + to_string(_minor) + "." + to_string(_release);
    else
        return to_string(_major) + "." + to_string(_minor);
}

} // namespace ghoul
} // namespace systemcapabilities
