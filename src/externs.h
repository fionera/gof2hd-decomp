#ifndef GOF2_EXTERNS_H
#define GOF2_EXTERNS_H
#include <cstdint>
#include <cstddef>
#include "engine/core/Array.h"
#include "engine/core/AEString.h"
#include "fieldaccess.h"



using ::AbyssEngine::Engine;

extern "C" {
unsigned char g_BloomShader_internalInitNeeded;
unsigned int g_BloomShader_shaderMode;
char *g_Camera_frustumEnabledFlag;

int g_Engine_activeShader;
int g_Engine_altShader;
int g_Engine_cloakShader;
int g_Engine_currentShader;
int g_Engine_defaultShader;
char *g_Engine_fboEnabledFlag;
int g_Engine_lineShader;
int g_Engine_postEffectBW;
int g_Engine_postEffectBlur;
int g_Engine_postEffectCounter;
uint8_t g_Engine_postEffectFlag;
int g_Engine_postEffectPending;
uint8_t g_Engine_shaderDirty;
uint8_t g_Engine_shaderDrew;
char *g_Engine_shaderModeFlag;
int g_Engine_shaderPostA;
int g_Engine_shaderPostB;
int g_Engine_shaderPostC;
uint8_t g_Engine_supportsFBO;
float g_Engine_texEnv;
uint8_t g_Engine_texEnvDirty;
uint8_t g_Engine_useShaders;
void *g_FMod_singleton;
char *g_GameText_arabicEnabledFlag;
uint8_t *g_GlowPPShader_internalInitNeededPtr;
uint32_t *g_GlowPPShader_shaderModePtr;
char *g_MeshIntersect_flipVFlag;
float g_MeshIntersect_missValue;
char *g_Mesh_extraArraysFlag;
char *g_Mesh_keepCpuCopyFlag;
char *g_Mesh_shaderPathFlag;
char *g_Mesh_tangentDelFlag;
char *g_Mesh_tangentEnabledFlag;
int *g_Mesh_vboByteCounter;
char *g_Mesh_vboEnabledFlag;
void *g_ObjectGunRenderScaleFlag;
void *g_ObjectGunScaleFlag;
void *g_PaintCanvas;
unsigned int g_SpriteSystem_oneHalf;
char *g_SpriteSystem_tangentFlag;
char *g_SpriteSystem_uvFlipFlag;
void *g_boost_fmod;
float *g_bsv3_floatA;
float *g_bsv3_floatB;
int *g_campaignSentinel;
int *g_cws_items;
int *g_cws_sound;
int *g_cws_sound2;
int *g_cws_sound3;
void **g_damageEmp_achievements;
int **g_damageEmp_progress;
int **g_damage_globals;
int **g_damage_text;
void *g_dockToPlanet_fmod;
void *g_incKillsHook;
void *g_incPirateKillsHook;
int g_orientationFlat;
int g_orientationInactive;
int g_orientationLeft;
int g_orientationRight;
int g_orientationUpsideDown;
float g_particleGlobalA;
float g_particleGlobalB;
uint64_t g_perfActionCount;
uint64_t g_perfCounter;
uint64_t g_perfElapsed;
char g_perfEnabled;
char g_perfExpired;
uint64_t g_perfFrame;
uint64_t g_perfLimit;
uint64_t g_perfLimitValue;
char g_perfPending;
char g_perfPendingFlag;
uint64_t g_perfTotal;
unsigned char g_rimByteGlobal;
float g_rimGlobalA;
float g_rimGlobalB;
unsigned char g_rimnByteGlobal;
float g_rimnGlobalA;
float g_rimnGlobalB;
float g_rimnGlobalC;
void *g_rotate_transform;
void *g_setRotation_transform;
unsigned int g_shoot_mask;
void *g_stopBoost_obj;
char g_touchDown;
float g_touchFloat;
int g_touchMode;
char g_touchToggle;
int g_touchValue;
int **g_update_clock;
char **g_update_flag;
void **g_update_sound;
float **g_update_speed;
}

extern "C" {
void AE_AEMath_matMul(AbyssEngine::Matrix *out, const AbyssEngine::Matrix *in);

void AE_SpriteSystem_pushMatrix(
    unsigned int m0, unsigned int m1, unsigned int m2, unsigned int m3, unsigned int m4,
    unsigned int m5, unsigned int m6, unsigned int m7, unsigned int m8, unsigned int m9,
    unsigned int m10, unsigned int m11, unsigned int m12, unsigned int m13, unsigned int m14,
    int dst);

Engine *AE_getInitGLThis();

int AE_getInitGLWidth();

int AE_getInitGLHeight();

void *__aeabi_memclr(void *dst, size_t n);

void *__aeabi_memclr4(void *dst, size_t n);

void *__aeabi_memcpy4(void *dst, const void *src, size_t n);

long long __aeabi_uldivmod(unsigned int nlo, unsigned int nhi, unsigned int dlo,
                           unsigned int dhi);
}

extern "C" {
long long __aeabi_f2lz(float v);

void *operator_new_helper(size_t);

float sqrtf(float);
}

#endif
