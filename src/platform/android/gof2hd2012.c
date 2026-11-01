/* gof2hd2012.c -- GOF2HD2012 achievement/leaderboard JNI glue.
 *
 * The original keeps these wrappers in their own TU, linked after main.c and the
 * ndk23_* layer; the g_android_* state lives here with them.
 */
#include <jni.h>
#include <stddef.h>

char *zipDirectory = NULL;

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
