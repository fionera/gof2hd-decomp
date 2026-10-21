#include "engine/core/JNIEnvCalls.h"

// Anchor the five inline `_JNIEnv` call helpers the Android JNI bridge relies on.
// Taking each method's address here makes clang emit the inline body out of line
// under its canonical `_ZN7_JNIEnv...` weak symbol:
//
//   _JNIEnv::CallIntMethod(_jobject*, _jmethodID*, ...)
//   _JNIEnv::CallVoidMethod(_jobject*, _jmethodID*, ...)
//   _JNIEnv::CallStaticIntMethod(_jclass*, _jmethodID*, ...)
//   _JNIEnv::CallStaticVoidMethod(_jclass*, _jmethodID*, ...)
//   _JNIEnv::CallStaticBooleanMethod(_jclass*, _jmethodID*, ...)
//
// Each forwards the trailing varargs to the matching CallXxxMethodV slot in the
// JNINativeInterface dispatch table (offsets 0xc8/0xf8/0x208/0x238/0x1d8), which
// is precisely what the original library's weak copies do.
namespace gof2 {

const JNIEnvCallAnchors kJNIEnvCallAnchors = {
    &_JNIEnv::CallIntMethod,
    &_JNIEnv::CallVoidMethod,
    &_JNIEnv::CallStaticIntMethod,
    &_JNIEnv::CallStaticVoidMethod,
    &_JNIEnv::CallStaticBooleanMethod,
};

} // namespace gof2
