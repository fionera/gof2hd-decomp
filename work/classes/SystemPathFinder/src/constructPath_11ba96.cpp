#include "class.h"

Array<Node *> *SystemPathFinder::constructPath(Node *node) {
    Array<Node *> *backwards = new Array<Node *>;
    for (; F<Node *>(node, 0x4) != 0; node = F<Node *>(node, 0x4)) {
        ArrayAdd<Node *>(node, *backwards);
    }

    Array<Node *> *path = new Array<Node *>;
    ArraySetLength<Node *>(backwards->length, *path);

    uint32_t i = F<uint32_t>(backwards, 0x0);
    int out = 0;
    while (--i < 0x80000000u) {
        *(Node **)(F<char *>(path, 0x4) + out) =
            *(Node **)(F<char *>(backwards, 0x4) + i * 4);
        out += 4;
    }

    ArrayRelease<Node *>(*backwards);
    return path;
}
