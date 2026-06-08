#ifndef WORK_CLASSES_GAMEDATA_SRC_CLASS_H
#define WORK_CLASSES_GAMEDATA_SRC_CLASS_H

// Galaxy on Fire 2 -- GameData (top-level class, Android libgof2hdaa.so, armv7 Thumb).
// Constructor and destructor manage 8 embedded AbyssEngine::String members.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {
struct String {
    String() noexcept;
    ~String() noexcept;
};
} // namespace AbyssEngine

struct GameData {
    GameData();
    ~GameData();
};

#endif
