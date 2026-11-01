/* GOF2HD2012.c -- the Google-Play Games / leaderboard JNI glue.
 *
 * Original TU (binary evidence, no source-dump witness): the 11 GOF2HD2012_* wrappers and the
 * six g_android_* counters form one alphabetical .dynsym run (index 226..242) *after* main.c's
 * run (150..225), i.e. they were a separate object linked immediately after main.c.  Their
 * functions sit at .text 0x6b9d8..0x6ba90, right after main.c's last function (testPurchase
 * 0x6b8c4) and before Mesh.cpp (0x6c4d4), which is exactly where an object keyed on its first
 * strong-function address lands.
 *
 * The counters are C tentative definitions: the original places them in the COMMON block at the
 * end of .bss (0x21d2c0..0x21d31c), sorted by size then name, which only C tentative
 * definitions produce.
 */
#include <jni.h>

/* Purchase-in-flight flags raised by the NFC store buttons (declared extern in NdkHooks.h).
 * Strong definitions (explicit `= 0`) so they land in this object's .bss, not COMMON: the
 * original places them at .bss 0x2179b4..0x2179dc, immediately after main.c's zipDirectory
 * and before FileInterfaceAndroid, which is exactly this object's link slot (o005). */
int gi_iap_buy_credit_pack1_pressed = 0;
int gi_iap_buy_credit_pack2_pressed = 0;
int gi_iap_buy_credit_pack3_pressed = 0;
int gi_iap_buy_credit_pack4_pressed = 0;
int gi_iap_buy_credit_pack5_pressed = 0;
int gi_iap_buy_dlc1_pressed = 0;
int gi_iap_buy_dlc2_pressed = 0;
int gi_iap_buy_dlc3_pressed = 0;
int gi_iap_buy_dlc4_pressed = 0;
int gi_iap_buy_dlc5_pressed = 0;

int g_android_current_achievements[3];
int g_android_leaderboard_scores[8];
int g_android_link_game_gp;
int g_android_gp_is_linked;
int g_android_show_achievements;
int g_android_show_leaderboards;
int g_android_back_button_pressed;

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLeaderboardScore(JNIEnv *env, jclass clazz,
                                                                           jint index) {
    return g_android_leaderboard_scores[index];
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLeaderboardScore(JNIEnv *env, jclass clazz,
                                                                             jint index) {
    g_android_leaderboard_scores[index] = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetAchievementId(JNIEnv *env, jclass clazz, jint index) {
    return g_android_current_achievements[index];
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetAchievements(JNIEnv *env, jclass clazz) {
    g_android_current_achievements[0] = 0;
    g_android_current_achievements[1] = 0;
    g_android_current_achievements[2] = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowAchievements(JNIEnv *env, jclass clazz) {
    return g_android_show_achievements;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowAchievements(JNIEnv *env, jclass clazz) {
    g_android_show_achievements = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLinkGameGP(JNIEnv *env, jclass clazz) {
    return g_android_link_game_gp;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLinkGameGP(JNIEnv *env, jclass clazz) {
    g_android_link_game_gp = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowLeaderboards(JNIEnv *env, jclass clazz) {
    return g_android_show_leaderboards;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowLeaderboards(JNIEnv *env, jclass clazz) {
    g_android_show_leaderboards = 0;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetGPIsLinked(JNIEnv *env, jclass clazz, jint linked) {
    g_android_gp_is_linked = linked;
}
