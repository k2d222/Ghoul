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

#ifndef __GHOUL___GLM___H__
#define __GHOUL___GLM___H__

#include <ghoul/misc/stringconversion.h>

#ifdef __APPLE__
// The GLM header throw 'anonymous struct' warnings that we do not want. By marking the
// following files as system headers, all warnings are ignored
#pragma clang diagnostic push
#pragma clang system_header
#endif // __APPLE__


#ifdef __unix__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif // __unix__


#ifndef GLM_META_PROG_HELPERS
#define GLM_META_PROG_HELPERS
#endif // GLM_META_PROG_HELPERS

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // GLM_ENABLE_EXPERIMENTAL

#ifndef __APPLE__
#ifndef GLM_FORCE_CTOR_INIT
#define GLM_FORCE_CTOR_INIT
#endif // GLM_FORCE_CTOR_INIT
#endif

// Enabling swizzling breaks binary compatibility between glm::vecX and float* ?
// #ifndef GLM_SWIZZLE
// #define GLM_SWIZZLE
// #endif // GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace {
    template <class T, class... Ts>
    struct is_any : std::disjunction<std::is_same<T, Ts>...> {};
} // namespace

namespace glm {

template <typename genType>
GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType tau() {
    return genType(6.28318530717958647692528676655900576);
}

} // namespace glm

namespace ghoul {

template <typename T>
struct glm_components : public std::integral_constant<glm::length_t, 0> {};

template <glm::length_t L, typename T, glm::qualifier Q>
struct glm_components<glm::vec<L, T, Q>> :
    public std::integral_constant<glm::length_t, L> {};

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct glm_components<glm::mat<C, R, T, Q>> :
    public std::integral_constant<glm::length_t, C * R> {};

template <typename T>
constexpr bool isGlmMatrix() {
    return is_any<T,
        glm::mat2x2, glm::mat2x3, glm::mat2x4,
        glm::mat3x2, glm::mat3x3, glm::mat3x4,
        glm::mat4x2, glm::mat4x3, glm::mat4x4,
        glm::dmat2x2, glm::dmat2x3, glm::dmat2x4,
        glm::dmat3x2, glm::dmat3x3, glm::dmat3x4,
        glm::dmat4x2, glm::dmat4x3, glm::dmat4x4>::value;
}

template <typename T>
constexpr bool isGlmVector() {
    return is_any<T,
        glm::vec2, glm::vec3, glm::vec4,
        glm::ivec2, glm::ivec3, glm::ivec4,
        glm::dvec2, glm::dvec3, glm::dvec4,
        glm::uvec2, glm::uvec3, glm::uvec4>::value;
}

/**
 * Compute a quaternion that represents the rotation looking from \p eye to
 * \p target, with the specified \p up direction
 */
template <typename valType>
glm::tquat<valType> lookAtQuaternion(const glm::tvec3<valType> eye,
                                     const glm::tvec3<valType> target,
                                     const glm::tvec3<valType> up)
{
    const glm::tmat4x4<valType> lookAtMat = glm::lookAt(eye, target, up);
    return glm::normalize(glm::inverse(glm::quat_cast(lookAtMat)));
}

/**
 * Check if quaternion \p q1 and \q2 represent the same spatial orientation.
 * The precision of the check can be controlled using the \p precision parameter
 */
template <typename valType>
bool isSameOrientation(const glm::tquat<valType> q1, const glm::tquat<valType> q2,
                       const valType precision)
{
    return 1.0 - std::abs(glm::dot(q1, q2)) < precision;
}

/**
 * Compute a view direction vector from a quaternion representing a rotation
 */
inline glm::dvec3 viewDirection(const glm::dquat& q) {
    return glm::normalize(q * glm::dvec3(0.0, 0.0, -1.0));
}

template <typename valType>
glm::tmat2x2<valType> createFillMat2x2(valType v) {
    return glm::tmat2x2<valType>(v, v, v, v);
}

template <typename valType>
glm::tmat2x3<valType> createFillMat2x3(valType v) {
    return glm::tmat2x3<valType>(v, v, v, v, v, v);
}

template <typename valType>
glm::tmat2x4<valType> createFillMat2x4(valType v) {
    return glm::tmat2x4<valType>(v, v, v, v, v, v, v, v);
}

template <typename valType>
glm::tmat3x3<valType> createFillMat3x3(valType v) {
    return glm::tmat3x3<valType>(v, v, v, v, v, v, v, v, v);
}

template <typename valType>
glm::tmat3x2<valType> createFillMat3x2(valType v) {
    return glm::tmat3x2<valType>(v, v, v, v, v, v);
}

template <typename valType>
glm::tmat3x4<valType> createFillMat3x4(valType v) {
    return glm::tmat3x4<valType>(v, v, v, v, v, v, v, v, v, v, v, v);
}

template <typename valType>
glm::tmat4x4<valType> createFillMat4x4(valType v) {
    return glm::tmat4x4<valType>(v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v);
}

template <typename valType>
glm::tmat4x2<valType> createFillMat4x2(valType v) {
    return glm::tmat4x2<valType>(v, v, v, v, v, v, v, v);
}

template <typename valType>
glm::tmat4x3<valType> createFillMat4x3(valType v) {
    return glm::tmat4x3<valType>(v, v, v, v, v, v, v, v, v, v, v, v);
}

inline std::string to_string(const glm::bvec2& _Val) {
    return "{" + std::to_string(_Val.x) + "," + std::to_string(_Val.y) + "}";
}

inline std::string to_string(const glm::bvec3& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "}";
}

inline std::string to_string(const glm::bvec4& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "," +
        std::to_string(_Val.w) + "}";
}

inline std::string to_string(const glm::vec2& _Val) {
    return "{" + std::to_string(_Val.x) + "," + std::to_string(_Val.y) + "}";
}

inline std::string to_string(const glm::vec3& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "}";
}

inline std::string to_string(const glm::vec4& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "," +
        std::to_string(_Val.w) + "}";
}

inline std::string to_string(const glm::quat& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "," +
        std::to_string(_Val.w) + "}";
}

inline std::string to_string(const glm::dvec2& _Val) {
    return "{" + std::to_string(_Val.x) + "," + std::to_string(_Val.y) + "}";
}

inline std::string to_string(const glm::dvec3& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "}";
}

inline std::string to_string(const glm::dvec4& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "," +
        std::to_string(_Val.w) + "}";
}

inline std::string to_string(const glm::dquat& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "," +
        std::to_string(_Val.w) + "}";
}

inline std::string to_string(const glm::ivec2& _Val) {
    return "{" + std::to_string(_Val.x) + "," + std::to_string(_Val.y) + "}";
}

inline std::string to_string(const glm::ivec3& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "}";
}

inline std::string to_string(const glm::ivec4& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "," +
        std::to_string(_Val.w) + "}";
}

inline std::string to_string(const glm::uvec2& _Val) {
    return "{" + std::to_string(_Val.x) + "," + std::to_string(_Val.y) + "}";
}

inline std::string to_string(const glm::uvec3& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "}";
}

inline std::string to_string(const glm::uvec4& _Val) {
    return "{" +
        std::to_string(_Val.x) + "," +
        std::to_string(_Val.y) + "," +
        std::to_string(_Val.z) + "," +
        std::to_string(_Val.w) + "}";
}

inline std::string to_string(const glm::mat2& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "}";
}

inline std::string to_string(const glm::mat2x3& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "}";
}

inline std::string to_string(const glm::mat2x4& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[0].w) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[1].w) + "}";
}

inline std::string to_string(const glm::mat3x2& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "}";
}

inline std::string to_string(const glm::mat3& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "}";
}

inline std::string to_string(const glm::mat3x4& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[0].w) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[1].w) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "," +
        std::to_string(_Val[2].w) + "}";
}

inline std::string to_string(const glm::mat4x2& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[3].x) + "," +
        std::to_string(_Val[3].y) + "}";
}

inline std::string to_string(const glm::mat4x3& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "," +
        std::to_string(_Val[3].x) + "," +
        std::to_string(_Val[3].y) + "," +
        std::to_string(_Val[3].z) + "}";
}

inline std::string to_string(const glm::mat4& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[0].w) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[1].w) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "," +
        std::to_string(_Val[2].w) + "," +
        std::to_string(_Val[3].x) + "," +
        std::to_string(_Val[3].y) + "," +
        std::to_string(_Val[3].z) + "," +
        std::to_string(_Val[3].w) + "}";
}

inline std::string to_string(const glm::dmat2& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "}";
}

inline std::string to_string(const glm::dmat2x3& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "}";
}

inline std::string to_string(const glm::dmat2x4& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[0].w) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[1].w) + "}";
}

inline std::string to_string(const glm::dmat3x2& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "}";
}

inline std::string to_string(const glm::dmat3& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "}";
}

inline std::string to_string(const glm::dmat3x4& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[0].w) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[1].w) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "," +
        std::to_string(_Val[2].w) + "}";
}

inline std::string to_string(const glm::dmat4x2& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[3].x) + "," +
        std::to_string(_Val[3].y) + "}";
}

inline std::string to_string(const glm::dmat4x3& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "," +
        std::to_string(_Val[3].x) + "," +
        std::to_string(_Val[3].y) + "," +
        std::to_string(_Val[3].z) + "}";
}

inline std::string to_string(const glm::dmat4& _Val) {
    return "{" +
        std::to_string(_Val[0].x) + "," +
        std::to_string(_Val[0].y) + "," +
        std::to_string(_Val[0].z) + "," +
        std::to_string(_Val[0].w) + "," +
        std::to_string(_Val[1].x) + "," +
        std::to_string(_Val[1].y) + "," +
        std::to_string(_Val[1].z) + "," +
        std::to_string(_Val[1].w) + "," +
        std::to_string(_Val[2].x) + "," +
        std::to_string(_Val[2].y) + "," +
        std::to_string(_Val[2].z) + "," +
        std::to_string(_Val[2].w) + "," +
        std::to_string(_Val[3].x) + "," +
        std::to_string(_Val[3].y) + "," +
        std::to_string(_Val[3].z) + "," +
        std::to_string(_Val[3].w) + "}";
}

} // namespace ghoul

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tvec2<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tvec3<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tvec4<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat2x2<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat2x3<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat2x4<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat3x2<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat3x3<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat3x4<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat4x2<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat4x3<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tmat4x4<T, P>& val) {
    return out << ghoul::to_string(val);
}

template <typename T, glm::precision P>
std::ostream& operator<<(std::ostream& out, const glm::tquat<T, P>& val) {
    return out << ghoul::to_string(val);
}


#ifdef __APPLE__
#pragma clang diagnostic pop
#endif

#ifdef __unix__
#pragma GCC diagnostic pop
#endif // __unix__

#endif // __GHOUL___GLM___H__
