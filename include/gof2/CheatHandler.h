#ifndef GOF2_CHEATHANDLER_H
#define GOF2_CHEATHANDLER_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
#include <new>
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

namespace AbyssEngine {





template <class T>
void ArrayAdd(T item, Array<T> &array);

template <class T>
void ArrayRemoveAll(Array<T> &array);

struct CheatCode {
    Array<uint16_t> *keys; // 0x00
    int32_t value;         // 0x04
    uint32_t pos;          // 0x08

    CheatCode();
    ~CheatCode();

    bool Update(uint16_t key);
};

// Maps a digit '0'..'9' to a hardware key code. Each entry is 0x10 bytes;
// the key value is the uint16_t at the start of the entry.
struct KeyCode;

typedef void (*CheatFunc)(int, void *);

struct CheatHandler {
    CheatFunc func;             // 0x00
    void *context;              // 0x04
    Array<CheatCode *> *codes;  // 0x08
    KeyCode *keys;              // 0x0c

    CheatHandler(KeyCode *keys);
    ~CheatHandler();

    void AddCheatCode(const String &code, int value);
    void SetCheatFunc(CheatFunc func, void *context);
    void Update(uint16_t key);
};

} // namespace AbyssEngine
#endif
