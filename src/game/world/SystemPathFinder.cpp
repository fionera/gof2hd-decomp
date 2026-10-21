#include "game/world/SystemPathFinder.h"
#include "game/core/Globals.h"
#include "engine/render/LODManager.h"

Node::Node(int value) {
    this->parent = nullptr;
    this->value = value;
    this->children = new Array<Node *>();
}

Node::~Node() {
    ArrayRelease(*this->children);
}

SystemPathFinder::SystemPathFinder() {
}

SystemPathFinder::~SystemPathFinder() {
}

int SystemPathFinder::getJumpDistance(Array<SolarSystem *> *systems, int from, int to) {
    Array<int> *path = getSystemPath(systems, from, to);
    if (path == nullptr)
        return 0;
    int length = (int) path->size();
    ArrayRelease(*path);
    delete path;
    return length - 1;
}

Array<int> *SystemPathFinder::getSystemPath(Array<SolarSystem *> *systems, int from, int to) {
    Array<Node *> *nodes = new Array<Node *>;
    ArraySetLength(systems->size(), *nodes);
    for (uint32_t i = 0; i < systems->size(); ++i)
        (*nodes)[i] = new Node((int) i);

    // Link every system to the visible systems its jump routes lead to.
    for (uint32_t s = 0; s < systems->size(); ++s) {
        Array<int> *routes = (Array<int> *) (*systems)[s]->getRoutes();
        if (routes == nullptr)
            continue;
        for (uint32_t j = 0; j < routes->size(); ++j) {
            Array<uint8_t> *visible = (Array<uint8_t> *) Globals::status->getSystemVisibilities();
            if (visible != nullptr && visible->size() > (uint32_t) (*systems)[(*routes)[j]]->getIndex() &&
                (*visible)[(*systems)[(*routes)[j]]->getIndex()])
                ArrayAdd((*nodes)[(*routes)[j]], *(*nodes)[s]->children);
        }
    }

    Array<Node *> *nodePath = search((*nodes)[from], (*nodes)[to]);
    if (nodePath == nullptr)
        return nullptr;

    Array<int> *path = nullptr;
    if (nodePath->size() != 0) {
        path = new Array<int>;
        ArraySetLength(nodePath->size() + 1, *path);
        int *out = path->data();
        *out++ = from;
        for (uint32_t i = 0; i + 1 < path->size(); i++)
            out[i] = (*nodePath)[i]->value;
    }
    ArrayReleaseClasses(*nodePath);
    delete nodePath;
    return path;
}

Array<Node *> *SystemPathFinder::search(Node *start, Node *goal) {
    Array<Node *> *closed = new Array<Node *>;
    Array<Node *> *open = new Array<Node *>;
    ArrayAdd(start, *open);
    start->parent = nullptr;

    while (!open->empty()) {
        Node *current = (*open)[0];
        ArrayRemove(current, *open);
        if (current == goal)
            return constructPath(goal);

        ArrayAdd(current, *closed);
        for (uint32_t i = 0; i < current->children->size(); ++i) {
            Node *next = (*current->children)[i];
            if (contains(closed, next) || contains(open, next))
                continue;
            next->parent = current;
            ArrayAdd(next, *open);
        }
    }
    return nullptr;
}

Array<Node *> *SystemPathFinder::constructPath(Node *node) {
    Array<Node *> *backwards = new Array<Node *>;
    for (; node->parent != nullptr; node = node->parent)
        ArrayAdd(node, *backwards);

    Array<Node *> *path = new Array<Node *>;
    ArraySetLength(backwards->size(), *path);
    int out = 0;
    for (int i = backwards->size() - 1; i >= 0; i--)
        (*path)[out++] = (*backwards)[i];

    ArrayRelease(*backwards);
    return path;
}

int SystemPathFinder::contains(Array<Node *> *nodes, Node *node) {
    for (uint32_t i = 0; i < nodes->size(); ++i) {
        if (nodes->data()[i] == node) {
            return 1;
        }
    }
    return 0;
}
