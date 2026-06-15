#ifndef GOF2_KEYCODE_H
#define GOF2_KEYCODE_H
#include "gof2/game/core/String.h"

namespace AbyssEngine {

// A key binding: a numeric key code paired with its human-readable name.
class KeyCode {
public:
    int code;
    String name;

    KeyCode &operator=(const KeyCode &other);
};

} // namespace AbyssEngine

#endif
