#include "game/world/SystemPathFinder.h"

// Stateless A* helper: the open/closed sets used by search() are created and
// released within each call, so construction and destruction do nothing.
SystemPathFinder::SystemPathFinder() {}
SystemPathFinder::~SystemPathFinder() {}

int SystemPathFinder::contains(Array<Node *> *nodes, Node *node) {
    for (uint32_t i = 0; i < nodes->size(); ++i) {
        if (nodes->data()[i] == node) {
            return 1;
        }
    }
    return 0;
}

Array<Node *> *SystemPathFinder::search(Node *start, Node *goal) {
    Array<Node *> *closed = new Array<Node *>;
    Array<Node *> *open = new Array<Node *>;
    open->push_back(start);
    start->parent = nullptr;

    while (!open->empty()) {
        Node *current = open->data()[0];
        open->erase(open->begin());
        if (current == goal) {
            return constructPath(goal);
        }

        closed->push_back(current);
        Array<Node *> *neighbours = current->children;
        for (uint32_t i = 0; i < neighbours->size(); ++i) {
            Node *next = neighbours->data()[i];

            if (contains(closed, next) != 0 || contains(open, next) != 0) {
                continue;
            }

            next->parent = current;
            open->push_back(next);
        }
    }

    return nullptr;
}

int SystemPathFinder::getJumpDistance(Array<SolarSystem *> *systems, int from,
                                      int to) {
    Array<int> *path = getSystemPath(systems, from, to);
    if (path != nullptr) {
        int length = (int)path->size();
        delete path;
        return length - 1;
    }
    return 0;
}

Array<Node *> *SystemPathFinder::constructPath(Node *node) {
    Array<Node *> *backwards = new Array<Node *>;
    for (; node->parent != nullptr; node = node->parent) {
        backwards->push_back(node);
    }

    Array<Node *> *path = new Array<Node *>;
    path->resize(backwards->size());

    // Reverse-copy the back-pointer chain into the forward path.
    uint32_t count = (uint32_t)backwards->size();
    for (uint32_t out = 0; out < count; ++out) {
        path->data()[out] = backwards->data()[count - 1 - out];
    }

    delete backwards;
    return path;
}

Array<int> *SystemPathFinder::getSystemPath(Array<SolarSystem *> *systems,
                                            int from, int to) {
    int start = from;
    Array<Node *> *nodes = new Array<Node *>;
    nodes->resize(systems->size());

    for (uint32_t i = 0; i < systems->size(); ++i) {
        nodes->data()[i] = new Node((int)i);
    }

    Status **statusPtr = g_SystemPathFinder_status;
    for (uint32_t s = 0; s < systems->size(); ++s) {
        Array<int> *routes = (Array<int> *)systems->data()[s]->getRoutes();
        if (routes == nullptr) {
            continue;
        }
        for (uint32_t j = 0; j < routes->size(); ++j) {
            Array<uint8_t> *visibilities =
                (Array<uint8_t> *)(*statusPtr)->getSystemVisibilities();
            if (visibilities == nullptr) {
                continue;
            }
            int routeIndex = routes->data()[j];
            int targetIndex = systems->data()[routeIndex]->getIndex();
            if (visibilities->size() > (uint32_t)targetIndex &&
                visibilities->data()[targetIndex] != 0) {
                nodes->data()[s]->children->push_back(
                    nodes->data()[routeIndex]);
            }
        }
    }

    Array<Node *> *nodePath =
        search(nodes->data()[start], nodes->data()[to]);
    Array<int> *path = nullptr;
    if (nodePath != nullptr) {
        if (!nodePath->empty()) {
            path = new Array<int>;
            path->resize(nodePath->size() + 1);
            int *out = path->data();
            out[0] = start;
            for (uint32_t i = 0; i + 1 < path->size(); ++i) {
                out[i + 1] = nodePath->data()[i]->value;
            }
        }
        for (uint32_t i = 0; i < nodePath->size(); ++i) {
            delete nodePath->data()[i];
        }
        delete nodePath;
    }
    return path;
}
