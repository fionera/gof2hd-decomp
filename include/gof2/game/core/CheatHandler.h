#ifndef GOF2_CHEATHANDLER_H
#define GOF2_CHEATHANDLER_H
#include "gof2/common.h"

namespace AbyssEngine {

template <class T>
void ArrayAdd(T item, Array<T> &array);

template <class T>
void ArrayRemoveAll(Array<T> &array);

struct CheatCode {
    Array<uint16_t> *keys;
    int32_t value;
    uint32_t pos;

    CheatCode();
    ~CheatCode();

    bool Update(uint16_t key);
};

// Maps a digit '0'..'9' to a hardware key code.
class KeyCode;

typedef void (*CheatFunc)(int, void *);

class CheatHandler {
public:
    CheatFunc func;
    void *context;
    Array<CheatCode *> *codes;
    KeyCode *keys;

    CheatHandler(KeyCode *keys);
    ~CheatHandler();

    void AddCheatCode(const String &code, int value);
    void SetCheatFunc(CheatFunc func, void *context);
    void Update(uint16_t key);
};

} // namespace AbyssEngine
#endif
