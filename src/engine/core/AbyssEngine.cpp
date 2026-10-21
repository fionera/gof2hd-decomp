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

namespace {
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
    // Defined in EngineFlags.cpp (separate TU). These MUST be declared in
    // namespace AbyssEngine (external linkage) so they bind to the real
    // definitions there; declaring them in an anonymous namespace would give
    // them internal linkage and leave them undefined in this TU.
    extern char *g_Camera_frustumEnabledFlag;
    extern char *g_Engine_fboEnabledFlag;
    extern char *g_Engine_shaderModeFlag;
    extern char *g_GameText_arabicEnabledFlag;
    extern char *g_MeshIntersect_flipVFlag;
    extern char *g_Mesh_extraArraysFlag;
    extern char *g_Mesh_keepCpuCopyFlag;
    extern char *g_Mesh_shaderPathFlag;
    extern char *g_Mesh_tangentDelFlag;
    extern char *g_Mesh_tangentEnabledFlag;
    extern int *g_Mesh_vboByteCounter;
    extern char *g_Mesh_vboEnabledFlag;
    extern char *g_SpriteSystem_tangentFlag;
    extern char *g_SpriteSystem_uvFlipFlag;

    int currentUsedShaderIndex;

    int currentFps;
    float debugTouch[4];
    int fpsCounter;
    int SwapCounter;
    unsigned char firstRender;
    int loadTexture;
    float currentLODBias;
    int CubeMapSetIndex;
    int debugModusIndex;
    unsigned char firstRenderGlow;
    unsigned char performanceTest;
    long long performanceTime;
    float timeBetweenFrames;
    unsigned char BiasErrorOutputFlag;
    unsigned char performanceTestEnded;
    int orientationChangeTimer;
    long long performanceFrameCounter;
    long long performanceModulSwitches;
    long long performanceOverallModulSwitches;
    int lauf;
    float i_fov;
    float i_zFar;
    float i_zNear;
    int fpsTimer;
    int loadMesh;
    long long triDrawn;
    unsigned char ShaderSet;

    Engine *AE_getInitGLThis();

    int AE_getInitGLWidth();

    int AE_getInitGLHeight();

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
    Quaternion operator*(const Quaternion &a, float s) {
        return Quaternion(a.x * s, a.y * s, a.z * s, a.w * s);
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    void getAppVersion() {
        Engine *self = AE_getInitGLThis();
        int width = AE_getInitGLWidth();
        int height = AE_getInitGLHeight();

        self->refractFBO = 0;
        self->displayWidth = width;
        self->displayHeight = height;
        self->viewportWidth = width;
        self->viewportHeight = height;

        FileInterfaceAndroid *fileIface = new FileInterfaceAndroid();
        self->fileInterface = fileIface;
        AEFile::SetInterface((FileInterface *) fileIface);

        self->lastGlError = 0;
        self->vibrationSupported = 0;
        self->hasVibration = false;

        char *shaderFlag = g_Engine_shaderModeFlag;
        self->viewFramebuffer = 0;
        self->ResetLightParam();
        glViewport(0, 0, self->viewportHeight, self->viewportWidth);

        if (*shaderFlag == 0) {
            glEnable(0x803a);
            glDisable(0xb50);
            glLineWidth(1.0f);
        } else {
            self->ShaderInit();
        }

        float zero3[3] = {0, 0, 0};
        self->field_0x468 = *(const Vector *) zero3;
        self->lightDirty[0] = 0;
        self->field_0x474 = *(const Vector *) zero3;
        self->lightDirty[1] = 0;

        glEnable(0xb71);
        self->GlEnable(0xde1, true);
        glDisable(0xbe2);
        glCullFace(0x405);
        glEnable(0xb44);
        self->AfterGLInit();
        (*(PaintCanvas **) self->appManager)->Initialize(false);

        self->maxTextureSize = 0;
        glGetIntegerv(0xd33, reinterpret_cast<GLint *>(&self->maxTextureSize));

        if (*shaderFlag != 0 && *g_Engine_fboEnabledFlag != 0) {
            FBOContainer *fbo = new FBOContainer(self, String(""));
            self->refractFBO = fbo;
            fbo->Create((int) self->displayWidth, (int) self->displayHeight, true, false);
        }
    }
}

namespace AbyssEngine {
    Quaternion operator-(const Quaternion &a, const Quaternion &b) {
        return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }
}


namespace AbyssEngine {
    void CurveRelease(Curve **slot) {
        Curve *curve = (Curve *) *slot;
        if (curve == 0)
            return;

        CurveKeyframe **data = 0;
        for (unsigned int i = 0; (data = (CurveKeyframe **) curve->entries),
                                 i < (unsigned int) curve->count; ++i) {
            CurveKeyframe *entry = data[i];
            int8_t tag = (int8_t) entry->tag;
            switch (tag) {
                case 3:
                    operator delete(entry);
                    break;
                case 2:
                    operator delete(entry);
                    break;
                case 1:
                    operator delete(entry);
                    break;
            }
            curve = (Curve *) *slot;
        }

        if (data != 0) {
            operator delete[](data);
            curve = (Curve *) *slot;
        }
        curve->entries = 0;

        if (*slot != 0)
            operator delete(*slot);
        *slot = 0;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const long long &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    String operator+(const float &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const int &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    String operator+(const String &a) {
        return String(a);
    }
}

namespace AbyssEngine {
    String operator+(const int &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    Quaternion operator+(const Quaternion &a, const Quaternion &b) {
        return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }
}

namespace AbyssEngine {
    float MODF(float value, float *intPart) {
        float ip = (float) (int) value;
        *intPart = ip;
        return value - ip;
    }
}

namespace AbyssEngine {
    void SetFXMaterial(BlendMode /*mode*/) {
    }
}

namespace AbyssEngine {
    String operator+(const long long &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    int CurveCreate(void **src, unsigned short count, Curve **out) { // lint: void_ptr (exported CurveCreate signature; void** baked into mangled name)
        // lint: void_ptr (exported CurveCreate signature; void** baked into mangled name)
        Curve *curve = (Curve *) operator new(8);
        curve->entries = 0;
        *out = curve;
        curve->count = count;

        unsigned char *data = (unsigned char *) operator new[]((unsigned int) count << 2);
        curve->entries = data;
        memcpy(data, src, (unsigned int) count << 2);
        return 1;
    }
}

namespace AbyssEngine {
    bool operator==(const String &a, const String &b) {
        String copy(a);
        return copy.Compare(b) == 0;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const float &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    void esMatrixMultiply(ESMatrix *out, ESMatrix *a, ESMatrix *b) {
        float local[4][4];
        const float (*A)[4] = a->m;
        const float (*B)[4] = b->m;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                local[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j] +
                              A[i][3] * B[3][j];
            }
        }

        memcpy(out, local, 0x40);
    }
}

namespace AbyssEngine {
    struct CurveRec {
        Curve *curve;
    };

    int CurveGetValue(unsigned long long time, Curve *curve) {
        CurveKeyframe **keys = (CurveKeyframe **) curve->entries;
        if (keys[0]->time >= time)
            return keys[0]->value;
        int last = curve->count - 1;
        if (time >= keys[last]->time)
            return keys[last]->value;

        unsigned short index;
        CurveKeyframe *key;
        do {
            index = (unsigned short) last;
            key = keys[index];
            last--;
        } while (time < key->time);

        if (key->tag == 3) {
            CurveKeyframe *next = keys[index + 1];
            int endValue = next->value;
            long long t = (long long) (((time - key->time) << 12) / (next->time - key->time));
            long long t2 = (t * t) >> 12;
            long long t3 = (t2 * t) >> 12;
            int startValue = key->value;
            return (int) (((t - 2 * t2 + t3) * key->c0) >> 12) + (int) (((t3 - t2) * key->c1) >> 12) +
                   (int) (((3 * t2 - 2 * t3) * (endValue - startValue)) >> 12) + startValue;
        }
        if (key->tag == 2) {
            CurveKeyframe *next = keys[index + 1];
            long long t = (long long) (((time - key->time) << 12) / (next->time - key->time));
            return (int) ((t * (next->value - key->value)) >> 12) + key->value;
        }
        if (key->tag == 1)
            return key->value;
        return 0;
    }
}

namespace AbyssEngine {
    unsigned short *computeFloatString(float value, int precision, int *decimalPoint, int *negative, int fixed) {
        unsigned short *buf = new unsigned short[33];
        if (precision < 0)
            precision = 0;
        if (precision >= 30)
            precision = 30;
        *negative = 0;
        if (value < 0.0f) {
            *negative = 1;
            value = -value;
        }
        float integral;
        int exponent = 0;
        unsigned short *out;

        float fraction = MODF(value, &integral);
        if (integral != 0.0f) {
            unsigned short *end = buf + 32;
            unsigned short *p = buf + 31;
            while (integral != 0.0f) {
                float digit = MODF(integral / 10.0f, &integral);
                exponent++;
                *p-- = (unsigned short) ((int) ((digit + 0.03) * 10.0) + '0');
            }
            unsigned short *src = p + 1;
            out = buf;
            while (src < end)
                *out++ = *src++;
        } else {
            if (fraction > 0.0f) {
                while (fraction * 10.0f < 1.0f) {
                    exponent--;
                    fraction *= 10.0f;
                }
            }
            out = buf;
        }
        *decimalPoint = exponent;

        unsigned short *limit = buf + precision;
        if (fixed == 0)
            limit += exponent;
        if (limit < buf) {
            *buf = 0;
            return buf;
        }
        while (out <= limit && out < buf + 32) {
            float digit;
            fraction = MODF(fraction * 10.0f, &digit);
            *out++ = (unsigned short) ((int) digit + '0');
        }
        if (limit >= buf + 32) {
            buf[31] = 0;
            return buf;
        }

        unsigned short *p = limit;
        unsigned short v = (unsigned short) (*limit + 5);
        *limit = v;
        while (v > '9') {
            *p = '0';
            if (p > buf) {
                p--;
                v = (unsigned short) (*p + 1);
                *p = v;
            } else {
                *p = '1';
                exponent++;
                *decimalPoint = exponent;
                if (fixed == 0) {
                    if (limit > buf)
                        *limit = '0';
                    limit++;
                }
                v = *p;
            }
        }
        *limit = 0;
        return buf;
    }
}

int GetStringLength(AbyssEngine::String str) {
    const unsigned short *p = str.GetAEWChar();
    while (*p++) {
    }
    return static_cast<int>(p - str.GetAEWChar()) - 1;
}
