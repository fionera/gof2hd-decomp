#ifndef WORK_CLASSES_AESOUNDRESSOURCE_SRC_CLASS_H
#define WORK_CLASSES_AESOUNDRESSOURCE_SRC_CLASS_H

typedef unsigned int uint32_t;

namespace AbyssEngine {
struct AESoundRessource {
    ~AESoundRessource();
};
struct AESoundInfo {
    int id;
    int field_4;
    int field_8;
};
struct AESoundInterface;
}

struct Array {
    uint32_t size;
    void *data;
    uint32_t capacity;
};

struct AESoundStackFrame {
    char info[12];
    int index;
    volatile int cookie;
};

#endif
