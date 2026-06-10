#include "gof2/SystemPathFinder.h"


extern "C" Array<Node *> *SystemPathFinder_constructPath(Node *node);

// ---- contains_11bb54.cpp ----
int SystemPathFinder::contains(Array<Node *> *nodes, Node *node) {
    uint32_t i = 0;
    do {
        if (i >= nodes->size()) {
            return 0;
        }
        Node *current = nodes->data()[i];
        ++i;
        if (current == node) {
            return 1;
        }
    } while (true);
}

// ---- search_11b968.cpp ----
Array<Node *> *SystemPathFinder::search(Node *start, Node *goal) {
    Array<Node *> *closed = new Array<Node *>;
    Array<Node *> *open = new Array<Node *>;
    ArrayAdd<Node *>(start, *open);
    start->field_0x4 = 0;

    while (open->size() != 0) {
        Node *current = open->data()[0];
        ArrayRemove<Node *>(current, *open);
        if (current == goal) {
            return SystemPathFinder_constructPath(goal);
        }

        ArrayAdd<Node *>(current, *closed);
        uint32_t i = 0;
        while (i < current->field_0x0->size()) {
            Node *next = current->field_0x0->data()[i];

            uint32_t j = 0;
            while (j < closed->size()) {
                if (closed->data()[j] == next) {
                    goto next_node;
                }
                ++j;
            }

            j = 0;
            while (j < open->size()) {
                if (open->data()[j] == next) {
                    goto next_node;
                }
                ++j;
            }

            next->field_0x4 = current;
            ArrayAdd<Node *>(next, *open);

        next_node:
            ++i;
        }
    }

    return 0;
}

// ---- getJumpDistance_11b780.cpp ----
int SystemPathFinder::getJumpDistance(Array<SolarSystem *> *systems, int from,
                                      int to) {
    Array<int> *path = getSystemPath(systems, from, to);
    if (path != 0) {
        int length = path->size();
        ArrayRelease<int>(*path);
        path->~Array<int>();
        operator delete(path);
        return length - 1;
    }
    return 0;
}

// ---- constructPath_11ba96.cpp ----
Array<Node *> *SystemPathFinder::constructPath(Node *node) {
    Array<Node *> *backwards = new Array<Node *>;
    for (; node->field_0x4 != 0; node = node->field_0x4) {
        ArrayAdd<Node *>(node, *backwards);
    }

    Array<Node *> *path = new Array<Node *>;
    ArraySetLength<Node *>(backwards->size(), *path);

    // Reverse-copy the back-pointer chain into the forward path.
    uint32_t i = backwards->size();
    uint32_t out = 0;
    while (--i < 0x80000000u) {
        path->data()[out] = backwards->data()[i];
        ++out;
    }

    ArrayRelease<Node *>(*backwards);
    return path;
}

// ---- getSystemPath_11b7a8.cpp ----
Array<int> *SystemPathFinder::getSystemPath(Array<SolarSystem *> *systems,
                                            int from, int to) {
    int start = from;
    SystemPathFinder *finder = this;
    Array<Node *> *nodes = new Array<Node *>;
    ArraySetLength<Node *>(systems->size(), *nodes);

    for (uint32_t i = 0; i < systems->size(); ++i) {
        Node *node = new Node(i);
        nodes->data()[i] = node;
    }

    Status **statusPtr = g_SystemPathFinder_status;
    from = 0;
    while ((uint32_t)from < systems->size()) {
        Array<int> *routes = systems->data()[from]->getRoutes();
        if (routes != 0) {
            for (uint32_t j = 0; j < routes->size(); ++j) {
                Array<uint8_t> *visibilities =
                    (*statusPtr)->getSystemVisibilities();
                if (visibilities != 0) {
                    uint32_t visibilityCount = visibilities->size();
                    if (visibilityCount >
                            (uint32_t)systems->data()[routes->data()[j]]
                                ->getIndex()) {
                        int visibleIndex =
                            systems->data()[routes->data()[j]]->getIndex();
                        uint8_t *visibleData = visibilities->data();
                        if (visibleData[visibleIndex] != 0) {
                            ArrayAdd<Node *>(
                                nodes->data()[routes->data()[j]],
                                *nodes->data()[from]->field_0x0);
                        }
                    }
                }
            }
        }
        ++from;
    }

    Array<Node *> *nodePath =
        finder->search(nodes->data()[start], nodes->data()[to]);
    Array<int> *path;
    if (nodePath == 0) {
        path = 0;
    } else {
        if (nodePath->size() == 0) {
            path = 0;
        } else {
            path = new Array<int>;
            ArraySetLength<int>(nodePath->size() + 1, *path);
            uint32_t i = 0;
            int *out = path->data();
            *out++ = start;
            while (i + 1 < path->size()) {
                out[i] = nodePath->data()[i]->field_0x8;
                ++i;
            }
        }
        ArrayReleaseClasses<Node *>(*nodePath);
        nodePath->~Array<Node *>();
        operator delete(nodePath);
    }
    return path;
}
