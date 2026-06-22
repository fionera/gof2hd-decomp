#ifndef GOF2_FBOCONTAINER_H
#define GOF2_FBOCONTAINER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class FBOContainer {
    public:
        uint32_t framebuffer; // GL framebuffer object id
        uint32_t texture; // GL color texture id
        uint32_t renderbuffer; // GL depth renderbuffer id
        int32_t width; // capture width
        int32_t height; // capture height
        Engine *engine; // owning engine (default framebuffer source)
        uint8_t created; // true once Create() has built the GL objects
        String name; // debug name
        uint8_t field_0x28; //
        uint32_t extraRenderbuffer0; // additional renderbuffer ids released on teardown
        uint32_t extraRenderbuffer1;
        uint32_t extraRenderbuffer2;

        FBOContainer(Engine *engine, String name);

        ~FBOContainer();

        void Create(int width, int height, bool a, bool linear);

        void Release();

        void Activate();

        void BeginCapture();

        void EndCapture();
    };
} // namespace AbyssEngine

#endif
