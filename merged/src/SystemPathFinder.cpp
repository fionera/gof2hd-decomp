#include "SystemPathFinder.h"


extern "C" Array<Node *> *SystemPathFinder_constructPath(Node *node);

// ---- contains_11bb54.cpp ----
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

// ---- search_11b968.cpp ----
Array<Node *> *SystemPathFinder::search(Node *start, Node *goal) {
    Array<Node *> *closed = new Array<Node *>;
    Array<Node *> *open = new Array<Node *>;
    ArrayAdd<Node *>(start, *open);
    F<Node *>(start, 0x4) = 0;

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

            F<Node *>(next, 0x4) = current;
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
        int length = path->length;
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

// ---- getSystemPath_11b7a8.cpp ----
Array<int> *SystemPathFinder::getSystemPath(Array<SolarSystem *> *systems,
                                            int from, int to) {
    int start = from;
    SystemPathFinder *finder = this;
    Array<Node *> *nodes = new Array<Node *>;
    ArraySetLength<Node *>(systems->length, *nodes);

    for (uint32_t i = 0; i < systems->length; ++i) {
        Node *node = new Node(i);
        nodes->data[i] = node;
    }

    Status **statusPtr = g_SystemPathFinder_status;
    from = 0;
    while ((uint32_t)from < systems->length) {
        Array<int> *routes = systems->data[from]->getRoutes();
        if (routes != 0) {
            for (uint32_t j = 0; j < routes->length; ++j) {
                Array<uint8_t> *visibilities =
                    (*statusPtr)->getSystemVisibilities();
                if (visibilities != 0) {
                    uint32_t visibilityCount = visibilities->length;
                    if (visibilityCount >
                            (uint32_t)systems->data[routes->data[j]]
                                ->getIndex()) {
                        int visibleIndex =
                            systems->data[routes->data[j]]->getIndex();
                        uint8_t *visibleData = visibilities->data;
                        if (visibleData[visibleIndex] != 0) {
                            ArrayAdd<Node *>(
                                nodes->data[routes->data[j]],
                                *F<Array<Node *> *>(nodes->data[from], 0x0));
                        }
                    }
                }
            }
        }
        ++from;
    }

    Array<Node *> *nodePath =
        finder->search(nodes->data[start], nodes->data[to]);
    Array<int> *path;
    if (nodePath == 0) {
        path = 0;
    } else {
        if (nodePath->length == 0) {
            path = 0;
        } else {
            path = new Array<int>;
            ArraySetLength<int>(nodePath->length + 1, *path);
            uint32_t i = 0;
            int *out = path->data;
            *out++ = start;
            while (i + 1 < path->length) {
                out[i] = F<int>(nodePath->data[i], 0x8);
                ++i;
            }
        }
        ArrayReleaseClasses<Node *>(*nodePath);
        nodePath->~Array<Node *>();
        operator delete(nodePath);
    }
    return path;
}
