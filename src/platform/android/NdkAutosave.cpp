#include "platform/android/NdkHooks.h"

#include "engine/render/Touch.h"

#include <cstdlib>
#include <cstring>

// The original allocates a one-entry buffer at static-init time (its second .init_array entry:
// touches = realloc(touches, 16)).
Touch *touches;
int maxTouchSize = 0;
int curTouchSize = 0;

// The original NdkHooks TU ends at ndk_isInMainMenu (0x947e0); this function starts the next
// TU (0x947f0).  Keeping a real function here preserves that TU boundary and its .ARM.exidx
// marker.  The touch globals above are first mentioned in this TU, after NdkHooks' zip_open
// and before LODManager's ArrayRemove<AEGeometry*>, exactly as in the original .dynsym.
char *pConstToNonConst(const char *s) {
    if (s != nullptr) {
        char *copy = static_cast<char *>(std::malloc(std::strlen(s) + 1));
        if (copy != nullptr) {
            std::strcpy(copy, s);
            return copy;
        }
    }
    return nullptr;
}

