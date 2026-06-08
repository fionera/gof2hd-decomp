#include "class.h"

int SystemPathFinder::contains(Array<Node *> *nodes, Node *node) {
    uint32_t i = 0;
    do {
        if (i >= nodes->length) {
            return 0;
        }
        Node *current = nodes->data[i];
        ++i;
        if (current == node) {
            return 1;
        }
    } while (true);
}
