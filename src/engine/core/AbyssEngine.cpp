#include "engine/core/AbyssEngine.h"
#include "engine/file/AEFile.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace AbyssEngine {
    struct CurveKeyframe {
        int8_t tag;  // 1 = step, 2 = linear, 3 = cubic Hermite
        uint8_t pad0x1[7];
        unsigned long long time;
        int32_t value;
        int32_t c0;
        int32_t c1;
    };

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
    static_assert(offsetof(CurveKeyframe, tag) == 0x0, "CurveKeyframe.tag");
    static_assert(offsetof(CurveKeyframe, time) == 0x8, "CurveKeyframe.time");
    static_assert(offsetof(CurveKeyframe, value) == 0x10, "CurveKeyframe.value");
    static_assert(offsetof(CurveKeyframe, c0) == 0x14, "CurveKeyframe.c0");
    static_assert(offsetof(CurveKeyframe, c1) == 0x18, "CurveKeyframe.c1");
#endif
}

namespace AbyssEngine {
    int currentUsedShaderIndex = -1;

    int currentFps;
    float debugTouch[4];
    int fpsCounter;
    int SwapCounter;
    unsigned char firstRender = 1;
    int loadTexture;
    float currentLODBias;
    int CubeMapSetIndex;
    int debugModusIndex;
    unsigned char firstRenderGlow = 1;
    unsigned char performanceTest;
    long long performanceTime;
    float timeBetweenFrames = 1000000.0f;
    bool BiasErrorOutputFlag = true;
    unsigned char performanceTestEnded;
    int orientationChangeTimer;
    long long performanceFrameCounter;
    long long performanceModulSwitches;
    long long performanceOverallModulSwitches;
    int lauf;
    float i_fov = -1.0f;
    float i_zFar;
    float i_zNear;
    int fpsTimer;
    int loadMesh;
    long long triDrawn;
    unsigned char ShaderSet;

    void AE_SpriteSystem_pushMatrix(unsigned int m0, unsigned int m1, unsigned int m2, unsigned int m3,
                                    unsigned int m4, unsigned int m5, unsigned int m6, unsigned int m7,
                                    unsigned int m8, unsigned int m9, unsigned int m10, unsigned int m11,
                                    unsigned int m12, unsigned int m13, unsigned int m14, int dst);
}

unsigned int AELabelObject(unsigned int glIdentifier, unsigned int name, const char *label) {
    (void) name;
    (void) label;
    return glIdentifier;
}

namespace AbyssEngine {
    // The original exports getAppVersion with size 0 at the same address as Engine::InitGL: an
    // Android stub whose body the compiler dropped entirely. The previous body here was a copy of
    // Engine::InitGL (GLES1Compat.cpp) routed through invented AE_getInitGL* shims.
    void getAppVersion() {
    }
}

namespace AbyssEngine {
    void CurveRelease(Curve **slot) {
        if (*slot == 0)
            return;

        for (unsigned short i = 0; i < (*slot)->count; i++) {
            CurveKeyframe *entry = (*slot)->entries[i];
            switch (entry->tag) {
                case 1:
                    operator delete(entry);
                    break;
                case 2:
                    operator delete(entry);
                    break;
                case 3:
                    operator delete(entry);
                    break;
            }
        }

        if ((*slot)->entries != 0)
            delete[] (*slot)->entries;
        (*slot)->entries = 0;

        if (*slot != 0)
            operator delete(*slot);
        *slot = 0;
    }
}

namespace AbyssEngine {
    void SetFXMaterial(BlendMode /*mode*/) {
    }
}

namespace AbyssEngine {
    int CurveCreate(void **src, unsigned short count, Curve **out) { // lint: void_ptr (exported CurveCreate signature; void** baked into mangled name)
        // lint: void_ptr (exported CurveCreate signature; void** baked into mangled name)
        Curve *curve = (Curve *) operator new(8);
        curve->entries = 0;
        *out = curve;
        curve->count = count;

        CurveKeyframe **data = (CurveKeyframe **) operator new[]((unsigned int) count << 2);
        curve->entries = data;
        memcpy(data, src, (unsigned int) count << 2);
        return 1;
    }
}

namespace AbyssEngine {
    void esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB) {
        ESMatrix tmp;
        for (int i = 0; i < 4; i++) {
            tmp.m[i][0] = (srcA->m[i][0] * srcB->m[0][0]) + (srcA->m[i][1] * srcB->m[1][0]) +
                          (srcA->m[i][2] * srcB->m[2][0]) + (srcA->m[i][3] * srcB->m[3][0]);
            tmp.m[i][1] = (srcA->m[i][0] * srcB->m[0][1]) + (srcA->m[i][1] * srcB->m[1][1]) +
                          (srcA->m[i][2] * srcB->m[2][1]) + (srcA->m[i][3] * srcB->m[3][1]);
            tmp.m[i][2] = (srcA->m[i][0] * srcB->m[0][2]) + (srcA->m[i][1] * srcB->m[1][2]) +
                          (srcA->m[i][2] * srcB->m[2][2]) + (srcA->m[i][3] * srcB->m[3][2]);
            tmp.m[i][3] = (srcA->m[i][0] * srcB->m[0][3]) + (srcA->m[i][1] * srcB->m[1][3]) +
                          (srcA->m[i][2] * srcB->m[2][3]) + (srcA->m[i][3] * srcB->m[3][3]);
        }
        memcpy(result, &tmp, sizeof(ESMatrix));
    }
}

namespace AbyssEngine {
    struct CurveRec {
        Curve *curve;
    };

    int CurveGetValue(unsigned long long time, Curve *curve) {
        CurveKeyframe **keys = curve->entries;
        CurveKeyframe *key = keys[0];
        if (key->time >= time)
            return key->value;
        int last = curve->count - 1;
        key = keys[last];
        if (time >= key->time)
            return key->value;

        unsigned short index;
        do {
            index = (unsigned short) last;
            key = keys[index];
            last--;
        } while (time < key->time);

        switch (key->tag) {
            case 1:
                return key->value;
            case 2: {
                CurveKeyframe *next = keys[index + 1];
                long long t = (long long) (((time - key->time) << 12) / (next->time - key->time));
                return (int) ((t * (next->value - key->value)) >> 12) + key->value;
            }
            case 3: {
                CurveKeyframe *next = keys[index + 1];
                int endValue = next->value;
                long long t = (long long) (((time - key->time) << 12) / (next->time - key->time));
                long long t2 = (t * t) >> 12;
                long long t3 = (t2 * t) >> 12;
                int startValue = key->value;
                long long sum = (((t - 2 * t2 + t3) * key->c0) >> 12) + (((t3 - t2) * key->c1) >> 12) +
                                (((3 * t2 - 2 * t3) * (endValue - startValue)) >> 12);
                return (int) sum + startValue;
            }
        }
        return 0;
    }
}

int GetStringLength(AbyssEngine::String str) {
    const unsigned short *p = str.GetAEWChar();
    while (*p++) {
    }
    return static_cast<int>(p - str.GetAEWChar()) - 1;
}
