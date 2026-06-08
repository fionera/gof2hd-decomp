#ifndef WORK_CLASSES_CHEATCODE_SRC_CLASS_H
#define WORK_CLASSES_CHEATCODE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

namespace AbyssEngine {

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

struct CheatCode {
    Array<uint16_t> *keys; // 0x00
    int32_t value;         // 0x04
    uint32_t pos;          // 0x08

    CheatCode();
    ~CheatCode();

    bool Update(uint16_t key);
};

} // namespace AbyssEngine

#endif
