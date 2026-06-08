#include "class.h"

extern "C" Array<Node *> *SystemPathFinder_constructPath(Node *node);

Array<Node *> *SystemPathFinder::search(Node *start, Node *goal) {
    Array<Node *> *closed = new Array<Node *>;
    Array<Node *> *open = new Array<Node *>;
    ArrayAdd<Node *>(start, *open);
    start->f_4 = 0;

    while (open->length != 0) {
        Node *current = open->data[0];
        ArrayRemove<Node *>(current, *open);
        if (current == goal) {
            return SystemPathFinder_constructPath(goal);
        }

        ArrayAdd<Node *>(current, *closed);
        uint32_t i = 0;
        while (i < F<Array<Node *> *>(current, 0x0)->length) {
            Node *next = F<Array<Node *> *>(current, 0x0)->data[i];

            uint32_t j = 0;
            while (j < closed->length) {
                if (closed->data[j] == next) {
                    goto next_node;
                }
                ++j;
            }

            j = 0;
            while (j < open->length) {
                if (open->data[j] == next) {
                    goto next_node;
                }
                ++j;
            }

            next->f_4 = current;
            ArrayAdd<Node *>(next, *open);

        next_node:
            ++i;
        }
    }

    return 0;
}
