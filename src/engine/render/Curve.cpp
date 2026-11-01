#include "engine/render/Curve.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

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
    static_assert(offsetof(CurveKeyframe, tag) == 0x0, "CurveKeyframe.tag");
    static_assert(offsetof(CurveKeyframe, time) == 0x8, "CurveKeyframe.time");
    static_assert(offsetof(CurveKeyframe, value) == 0x10, "CurveKeyframe.value");
    static_assert(offsetof(CurveKeyframe, c0) == 0x14, "CurveKeyframe.c0");
    static_assert(offsetof(CurveKeyframe, c1) == 0x18, "CurveKeyframe.c1");
#endif
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
