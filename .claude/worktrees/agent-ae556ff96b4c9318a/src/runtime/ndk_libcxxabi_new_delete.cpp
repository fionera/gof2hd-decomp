// Global C++ allocation operators (operator new/new[]/delete/delete[], including the
// nothrow_t and C++17 std::align_val_t overloads) as statically linked into
// libgof2hdaa.so from the NDK r18b libc++abi runtime.
//
// We do NOT re-implement these by hand: the binary was built with NDK r18b
// (Android … r328903, clang 7.0.2), so to byte-match its operators we compile the
// TOOLCHAIN'S OWN libc++abi source unchanged. This TU just pulls it in; the NDK
// libc++abi `src/` directory is put on the include path and the C++17 aligned-
// allocation feature is enabled for everything under src/runtime/ (see the
// `src/runtime/*` rule in tools/verify/build_objs.sh and CMakeLists.txt).
//
// The file self-defines _LIBCPP_BUILDING_LIBRARY / _LIBCPP_BUILDING_NEW; the
// libc++ headers (already on our -isystem path) then supply the _THROW_BAD_ALLOC /
// _NOEXCEPT / _LIBCXXABI_WEAK macros it needs. The resulting symbols are weak,
// matching the original.
#include <stdlib_new_delete.cpp>
