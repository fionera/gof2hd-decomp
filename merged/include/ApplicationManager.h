#ifndef WORK_CLASSES_APPLICATIONMANAGER_SRC_CLASS_H
#define WORK_CLASSES_APPLICATIONMANAGER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

void *operator new(__SIZE_TYPE__ size);
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

namespace AbyssEngine {
struct ApplicationManager;
struct ConfigReader;
struct PaintCanvas;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    String(const String &other, bool copy);
    ~String();

    String &operator=(const String &other);
};
}

using ApplicationManager = AbyssEngine::ApplicationManager;
using ConfigReader = AbyssEngine::ConfigReader;
using PaintCanvas = AbyssEngine::PaintCanvas;
using String = AbyssEngine::String;

#endif
