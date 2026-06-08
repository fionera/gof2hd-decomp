#include "class.h"

// AbyssEngine::PaintCanvas::TextureCreateGlobal(String name, unsigned unit) — loads a texture
// file into the given texture unit and, on success, leaves it unbound on that unit.
extern "C" char *tcg_String_GetAEChar(AbyssEngine::String *name);
extern "C" int tcg_TextureCreateFromFile(void *engine, const char *path, void *cb, void *ud,
                                         unsigned *outId, bool b, float f);
extern "C" void tcg_glActiveTexture(unsigned unit);
extern "C" void tcg_glBindTexture(unsigned target, unsigned tex);
extern "C" void tcg_operator_delete_arr(void *p);
extern "C" void tcg___stack_chk_fail();

__attribute__((visibility("hidden"))) extern int **g_tcg_canary;

void TextureCreateGlobal(AbyssEngine::PaintCanvas *self, AbyssEngine::String *name, int unit)
{
    int *canary = *g_tcg_canary;
    int saved = *canary;

    char *path = tcg_String_GetAEChar(name);
    unsigned outId;
    int rc = tcg_TextureCreateFromFile(*(void **)((char *)self + 0x34), path, 0, 0, &outId, false,
                                       0.0f);
    if (rc == 1) {
        tcg_glActiveTexture(unit + 0x84c0);
        tcg_glBindTexture(0xde1, 0);
        tcg_glActiveTexture(0x84c0);
    }
    tcg_operator_delete_arr(path);

    if (*canary != saved)
        tcg___stack_chk_fail();
}
