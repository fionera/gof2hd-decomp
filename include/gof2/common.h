// gof2/common.h — unified shared types for the native (macOS) build of Galaxy on Fire 2.
//
// The byte-matching decomp redefined containers/math in every per-class header. For ONE coherent
// program they must be defined exactly once. We are NOT byte-matching, so we use natural 64-bit
// layout and the C++ standard library where the engine type IS a standard type:
//   * AbyssEngine::Array<T>   ==  std::vector<T>      (dynamic array: size/data/capacity)
//   * AbyssEngine::String     ==  UTF-16 string backed by std::u16string (+ game-specific methods)
//   * Vector/Matrix/Quaternion ARE 3D MATH (not std::vector) -> custom, see gof2/math.h
#ifndef GOF2_COMMON_H
#define GOF2_COMMON_H
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include "gof2/math.h"
#include "gof2/fwd.h"

// ---- Array<T> == std::vector<T> -------------------------------------------------------------
// The recovered code used `Array<T>` with public {size,data,capacity}. We alias it to std::vector
// and provide the engine's container free-functions over it, so call sites (ArrayAdd/ArraySetLength/
// ArrayRemoveAll) are unchanged. Raw field reads (`a->data`, `a->size`) are rewritten to a.data()/
// a.size() during the merge.
template <class T> using Array = std::vector<T>;

template <class T> inline void ArrayAdd(T item, std::vector<T>& a)            { a.push_back(item); }
template <class T> inline void ArraySetLength(unsigned int n, std::vector<T>& a){ a.resize(n); }
template <class T> inline void ArrayRemoveAll(std::vector<T>& a)              { a.clear(); }

// ---- String == game UTF-16 string (backed by std::u16string) -------------------------------
// Kept as a class because the engine's String has 43 game-specific methods (transliteration, Split,
// ReplaceString, ToLowerCase with CP-1252 tables, ...). Storage is std::u16string; method bodies
// come from the merged native/src/String.cpp.
namespace AbyssEngine {
struct String {
    std::u16string s;
    String() {}
    // (method declarations are folded in from the recovered String header during the merge)
    const char16_t* text() const { return s.c_str(); }
    uint32_t        size() const { return (uint32_t)s.size(); }
};
struct String12 { char16_t buf[6]; };   // small stack string temp
} // namespace AbyssEngine

using AbyssEngine::String;

// ---- global aliases so recovered code's short type names resolve --------------------------
namespace AEMath = AbyssEngine::AEMath;   // so global `AEMath::Vector` resolves
using AbyssEngine::AEMath::Vector;        // recovered structs use bare Vector/Matrix
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::Quaternion;
typedef unsigned char  byte;
typedef unsigned int   uint;
typedef unsigned short ushort;
struct u32x4 { uint32_t v[4]; };          // NEON 4x u32 (best-effort, non-matching)

#endif // GOF2_COMMON_H
