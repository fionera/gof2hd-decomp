#include "class.h"

// AbyssEngine::TextureCreateFromFile(Engine*, char const*, void(*)(Image*, void*), void*,
//                                    unsigned int*, bool, float)
// Thin wrapper that forwards to the *Intern variant and always reports success (1).
namespace AbyssEngine {

typedef void (*ImageCallback)(Image *, void *);

int TextureCreateFromFileIntern(Engine *engine, const char *path, ImageCallback cb, void *user,
                                unsigned int *outIds, float scale, AELoadedTexture *outTex,
                                bool flag);

int TextureCreateFromFile(Engine *engine, const char *path, ImageCallback cb, void *user,
                          unsigned int *outIds, bool /*flag*/, float scale)
{
    TextureCreateFromFileIntern(engine, path, cb, user, outIds, scale,
                                (AELoadedTexture *)0, false);
    return 1;
}

} // namespace AbyssEngine
