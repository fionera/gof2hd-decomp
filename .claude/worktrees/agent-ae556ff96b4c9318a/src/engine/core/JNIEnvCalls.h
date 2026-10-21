#ifndef GOF2_JNIENVCALLS_H
#define GOF2_JNIENVCALLS_H

// The Android JNI bridge (NFC, FileInterfaceAndroid, ...) calls a handful of the
// inline `_JNIEnv` convenience methods from the NDK's <jni.h> -- CallIntMethod,
// CallVoidMethod and the CallStatic{Int,Void,Boolean}Method variants. With the
// matching -Oz build clang emits each of those inline methods exactly once, out
// of line, as a weak `_ZN7_JNIEnv...` symbol shared by every translation unit
// that uses it. Re-anchoring those five definitions in one place keeps that set
// of weak symbols present (and stable) in the recovered library.
//
// jni.h owns the real `_JNIEnv` layout and the inline bodies; we only reference
// the methods so the compiler is forced to materialise them.
#include <jni.h>

namespace gof2 {

// Pointers-to-member that pin the five inline `_JNIEnv` call helpers used by the
// JNI bridge, forcing the compiler to emit their out-of-line weak definitions.
struct JNIEnvCallAnchors {
    jint     (_JNIEnv::*callIntMethod)(jobject, jmethodID, ...);
    void     (_JNIEnv::*callVoidMethod)(jobject, jmethodID, ...);
    jint     (_JNIEnv::*callStaticIntMethod)(jclass, jmethodID, ...);
    void     (_JNIEnv::*callStaticVoidMethod)(jclass, jmethodID, ...);
    jboolean (_JNIEnv::*callStaticBooleanMethod)(jclass, jmethodID, ...);
};

extern const JNIEnvCallAnchors kJNIEnvCallAnchors;

} // namespace gof2

#endif
