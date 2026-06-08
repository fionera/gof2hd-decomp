#include "class.h"

Array<Node *> *SystemPathFinder::constructPath(Node *node) {
    Array<Node *> *backwards = new Array<Node *>;
    for (; node->f_4 != 0; node = node->f_4) {
        ArrayAdd<Node *>(node, *backwards);
    }

    Array<Node *> *path = new Array<Node *>;
    ArraySetLength<Node *>(backwards->length, *path);

    uint32_t i = backwards->f_0;
    int out = 0;
    while (--i < 0x80000000u) {
        *(Node **)(path->f_4 + out) =
            *(Node **)(backwards->f_4 + i * 4);
        out += 4;
    }

    ArrayRelease<Node *>(*backwards);
    return path;
}
