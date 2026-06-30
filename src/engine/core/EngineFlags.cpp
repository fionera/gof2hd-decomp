// Definitions for the engine's global feature-flag pointers and hooks.
// These are accessed GOT-indirectly in the original (e.g. `**(char**)(DAT+off)`):
// a global pointer, stored in the data section, dereferenced to a config byte.
// They MUST live in their own translation unit and be referenced via `extern`
// from the engine code: if defined and used in the same TU while never written,
// the optimizer proves the dereferenced pointer is null and folds every body
// that gates on `*flag` down to its early-return stub (deleting the real logic
// and its gl*/libc calls). Keeping the definitions here makes their values
// opaque to the using TU, so the engine functions emit the real GOT-indirect
// load and match the original's codegen.
namespace AbyssEngine {
    class Engine;
    class Mesh;

    char *g_Camera_frustumEnabledFlag;
    char *g_Engine_fboEnabledFlag;
    char *g_Engine_shaderModeFlag;
    char *g_GameText_arabicEnabledFlag;
    char *g_MeshIntersect_flipVFlag;
    char *g_Mesh_extraArraysFlag;
    char *g_Mesh_keepCpuCopyFlag;
    char *g_Mesh_shaderPathFlag;
    char *g_Mesh_tangentDelFlag;
    char *g_Mesh_tangentEnabledFlag;
    int *g_Mesh_vboByteCounter;
    char *g_Mesh_vboEnabledFlag;
    char *g_SpriteSystem_tangentFlag;
    char *g_SpriteSystem_uvFlipFlag;

    void (*g_MeshRelease_freeFn)(AbyssEngine::Engine *, AbyssEngine::Mesh **);
}
