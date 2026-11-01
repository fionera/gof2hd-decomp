/* BackButton.c -- the NFC back-key JNI hook and the flag it raises.
 *
 * Binary evidence for this being its own TU (not part of main.c): the original's .ARM.exidx has a
 * TU sentinel at .text 0x69588, immediately after Java_..._ToJNI_BackButtonPressed (0x69578..0x69588),
 * and the .dynsym puts g_android_back_button_pressed directly after that function, before the
 * NFC.cpp class methods.  main.c's own symbols start much later (JNI_OnLoad at 0x6b551).
 */
#include <jni.h>

/* Raised by the NFC back key, consumed by ndk23_newrender. */
int g_android_back_button_pressed;

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed(JNIEnv *env, jclass clazz) {
    g_android_back_button_pressed = 1;
}
