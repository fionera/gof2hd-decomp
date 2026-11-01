/* JniBackButton.c -- the Android back-key JNI entry.
 *
 * Original TU (binary evidence, no source-dump witness): Java_..._BackButtonPressed sits at
 * .text 0x69578, immediately before NFC.cpp's first function (NFC::NFC at 0x69588), and its
 * .dynsym entry (index 0) is followed directly by g_android_back_button_pressed (index 1) --
 * a separate object whose symtab is [Java_..._BackButtonPressed (defined),
 * g_android_back_button_pressed (undefined reference)], linked before NFC.cpp.
 */
#include <jni.h>

extern int g_android_back_button_pressed;

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed() {
    g_android_back_button_pressed = 1;
}
