#include "class.h"

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
