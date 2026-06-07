#ifndef WORK_CLASSES_DIALOGUEWINDOW_SRC_CLASS_H
#define WORK_CLASSES_DIALOGUEWINDOW_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

struct DialogueWindow;
struct Mission;
struct Level;
struct Agent;
struct String;
struct Vector;

template <class T> static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

struct StringSlot {
    char bytes[12];
};

#endif
