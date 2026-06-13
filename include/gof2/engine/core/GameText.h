#ifndef GOF2_GAMETEXT_H
#define GOF2_GAMETEXT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- GameText (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "GameText::..." (class not in a namespace;
// argument types like AbyssEngine::String are namespaced).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.
//
// Layout (from ctor/release decompiles): GameText embeds an Array<int> substitute table at +0x00
//   +0x00  uint32_t   substitute Array size
//   +0x04  int*       substitute Array data
//   +0x08  uint32_t   substitute Array cap
//   +0x0c  String**   text table (operator new[]; size at +0x1c)
//   +0x10  String     fallback / region string (12 bytes)
//   +0x1c  int        text table count

void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

struct GameText;

// AbyssEngine::String -- 12-byte value type (text*, size, ...). Pass-by-value uses the
// >8-byte struct ABI (sret / on stack), matching the target.
namespace AbyssEngine {

    // ---- methods (converted from free functions) ----
    void ReadLangFile(unsigned int file, int count);
    void ctor();
    void * getText(int key);
    void release();
    void setLanguage_i(int param_1);
    void setLanguage_si(int stringCount, int langId);
    void setSubstituteArray(int *param_1, unsigned param_2);
}
typedef AbyssEngine::String String;

// Field accessors via byte offset.

class GameText {
public:
    Array<int> substitutes;               // +0x0  substitute (from,to) pair table, Array<int>
    void* textTable;                    // +0xc  String** text table
    unsigned char fallbackText[12];       // +0x10 fallback / region String (12 bytes)
    int textCount;                     // +0x1c text table count

    // ---- constructor / destructor (demangle to GameText::GameText / ~GameText) ----
    // The embedded Array<int> substitute table is constructed/destroyed by the normal
    // C++ member init/teardown, so the recovered dtor_tail (which ran its ~vector) folds
    // into the implicit member destruction.
    GameText();
    ~GameText();

    // ---- methods (converted from free functions) ----
    void ReadLangFile(unsigned int file, int count);
    void * getText(int key);
    void release();
    void setLanguage_i(int param_1);
    void setLanguage_si(int stringCount, int langId);
    void setSubstituteArray(int *param_1, unsigned param_2);

    // Public language switch (the short/int two-argument entry the savegame loader
    // calls). Identical behaviour to setLanguage_si.
    void setLanguage(short stringCount, int langId);
};
#endif
