#ifndef WORK_CLASSES_PENDINGPRODUCT_SRC_CLASS_H
#define WORK_CLASSES_PENDINGPRODUCT_SRC_CLASS_H

// Galaxy on Fire 2 -- PendingProduct (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (not namespaced). It begins with an AbyssEngine::String subobject
// (the station name) at +0x00, followed by scalar fields:
//   +0x00  String  stationName
//   +0x0c  int     stationIndex
//   +0x10  int     quantity
//   +0x14  int     blueprintIndex

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct PendingProduct;
struct BluePrint;

namespace AbyssEngine {
struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String() noexcept;
    ~String() noexcept;
    String &operator=(const String &other) noexcept;
};
}
typedef AbyssEngine::String String;

template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

// AbyssEngine::String members (mangled).
void _ZN11AbyssEngine6StringC1Ev(String *self);
String *_ZN11AbyssEngine6StringaSERKS0_(String *self, const String *other);
void _ZN11AbyssEngine6StringD1Ev(String *self);

// BluePrint accessors used by the ctor.
int BluePrint_getIndex(BluePrint *bp);
void BluePrint_getStationName(String *out, BluePrint *bp);
int BluePrint_getStationIndex(BluePrint *bp);
int BluePrint_getQuantity(BluePrint *bp);
}

namespace AbyssEngine {
inline String::String() noexcept { _ZN11AbyssEngine6StringC1Ev(this); }
inline String::~String() noexcept { _ZN11AbyssEngine6StringD1Ev(this); }
inline String &String::operator=(const String &other) noexcept {
    return *_ZN11AbyssEngine6StringaSERKS0_(this, &other);
}
}

#endif
