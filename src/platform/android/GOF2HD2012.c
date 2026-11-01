/* GOF2HD2012.c -- the achievement / leaderboard JNI surface of the GOF2HD2012 Java class.
 *
 * Binary evidence for this being its own TU (not part of main.c): the original's .ARM.exidx has a
 * TU sentinel at .text 0x6b9d8 (end of main.c, right after ToJNI_isInMainMenu) and another at
 * 0x6ba90 (end of this TU).  The .dynsym likewise groups the eleven GOF2HD2012_* functions and the
 * six g_android_* achievement globals after main.c's symbols, not among them.
 */
#include <jni.h>

int g_android_current_achievements[3];
int g_android_leaderboard_scores[8];
int g_android_link_game_gp;
int g_android_gp_is_linked;
int g_android_show_achievements;
int g_android_show_leaderboards;

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
