#ifndef WORK_CLASSES_CHEATHANDLER_SRC_CLASS_H
#define WORK_CLASSES_CHEATHANDLER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;
inline void *operator new(uint32_t, void *ptr) noexcept { return ptr; }

namespace AbyssEngine {

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    uint16_t *operator[](uint32_t index);
};

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

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
