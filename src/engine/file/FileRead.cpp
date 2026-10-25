#include "engine/file/FileRead.h"
#include "game/core/Globals.h"

#include "engine/core/AbyssEngine.h"
#include "engine/core/AERandom.h"
#include "engine/math/AEMath.h"
#include "game/mission/Item.h"
#include "game/ship/Agent.h"
#include "game/ship/Ship.h"
#include "game/world/NewsItem.h"
#include "game/world/SolarSystem.h"
#include "game/world/SpacePoint.h"
#include "game/world/Station.h"
#include "game/world/Wanted.h"
#include "game/mission/Mission.h"
#include "engine/core/Array.h"

using AbyssEngine::Vector;

FileRead::FileRead() {
}

FileRead::~FileRead() {
}

Station *FileRead::loadStation(int32_t id) {
    int16_t *ids = new int16_t[1];
    ids[0] = (int16_t) id;
    Station *station = loadStationsBinary(ids, 1)->data()[0];
    delete[] ids;
    return station;
}

Array<Station *> *FileRead::loadStationsBinary(int16_t *ids, int32_t count) {
    Array<Station *> *stations = new Array<Station *>();
    ArraySetLength<Station *>(count, *stations);

    if (AEFile::FileExist(String("data/bin/stations.bin")) == 0) {
        return 0;
    }

    uint32_t handle;
    AEFile::OpenRead("data/bin/stations.bin", &handle);

    String name;
    int32_t out = 0;
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t d;
    for (uint32_t stationId = 0; stationId < 0x87; stationId++) {
        AEFile::ReadSwitched(name, handle);
        name.ConvertFromUTF8();
        AEFile::ReadSwitched(a, handle);
        AEFile::ReadSwitched(b, handle);
        AEFile::ReadSwitched(c, handle);
        AEFile::ReadSwitched(d, handle);
        for (int32_t i = 0; i < count; i++) {
            if (stationId == (uint32_t) ids[i]) {
                stations->data()[out] = new Station(name, a, b, c, d);
                out++;
            }
        }
    }
    AEFile::Close(handle);
    return stations;
}

Array<int32_t> *FileRead::loadShipParts(int32_t id) {
    if (AEFile::FileExist(String("data/bin/shipparts.bin")) == 0) {
        return nullptr;
    }

    uint32_t handle;
    AEFile::OpenRead("data/bin/shipparts.bin", &handle);

    int32_t wanted = id + 1;
    Array<int32_t> *parts = nullptr;
    for (uint32_t i = 0;; i++) {
        if (i >= 0x40) {
            AEFile::Close(handle);
            return parts;
        }
        int8_t group;
        int8_t count;
        AEFile::Read(group, handle);
        AEFile::Read(count, handle);

        parts = new Array<int32_t>();
        ArraySetLength<int32_t>(count * 10, *parts);
        for (uint32_t off = 0; off < parts->size(); off += 10) {
            int16_t value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off] = value;
            AEFile::ReadSwitched((*parts)[off + 1], handle);
            AEFile::ReadSwitched((*parts)[off + 2], handle);
            AEFile::ReadSwitched((*parts)[off + 3], handle);
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 4] = value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 5] = value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 6] = value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 7] = value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 8] = value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 9] = value;
        }
        if (wanted == group) {
            return parts;
        }
    }
}

Array<int32_t> *FileRead::loadStationCollision(int32_t id) {
    if (AEFile::FileExist(String("data/bin/collision.bin")) == 0) {
        return nullptr;
    }

    uint32_t handle;
    AEFile::OpenRead("data/bin/collision.bin", &handle);

    int32_t key = 0;
    int32_t count = 0;
    for (uint32_t i = 0;; i++) {
        if (i >= 0x88) {
            AEFile::Close(handle);
            return nullptr;
        }
        AEFile::Read(key, handle);
        AEFile::Read(count, handle);
        count++;
        if (key == id) {
            Array<int32_t> *result = new Array<int32_t>();
            int32_t *buffer = new int32_t[(uint32_t) count];
            AEFile::Read(count << 2, buffer, handle);
            ArraySetLength<int32_t>(count, *result);
            for (int32_t j = 0; j < count; j++) {
                result->data()[j] = buffer[j];
            }
            delete[] buffer;
            AEFile::Close(handle);
            return result;
        }
        AEFile::Skip(count << 2, handle);
    }
}

Array<int32_t> *FileRead::loadWreckCollision(int32_t id) {
    if (AEFile::FileExist(String("data/bin/wreck_collisions.bin")) == 0) {
        return nullptr;
    }

    uint32_t handle;
    AEFile::OpenRead("data/bin/wreck_collisions.bin", &handle);

    int32_t key = 0;
    int32_t count = 0;
    for (uint32_t i = 0;; i++) {
        if (i >= 6) {
            AEFile::Close(handle);
            return nullptr;
        }
        AEFile::Read(key, handle);
        AEFile::Read(count, handle);
        count++;
        if (key == id) {
            Array<int32_t> *result = new Array<int32_t>();
            int32_t *buffer = new int32_t[(uint32_t) count];
            AEFile::Read(count << 2, buffer, handle);
            ArraySetLength<int32_t>(count, *result);
            for (int32_t j = 0; j < count; j++) {
                result->data()[j] = buffer[j];
            }
            delete[] buffer;
            AEFile::Close(handle);
            return result;
        }
        AEFile::Skip(count << 2, handle);
    }
}

Array<int32_t> *FileRead::loadStaticCollision(int32_t id) {
    if (AEFile::FileExist(String("data/bin/static_collisions.bin")) == 0) {
        return nullptr;
    }

    uint32_t handle;
    AEFile::OpenRead("data/bin/static_collisions.bin", &handle);

    int32_t key = 0;
    int32_t count = 0;
    for (uint32_t i = 0;; i++) {
        if (i >= 7) {
            AEFile::Close(handle);
            return nullptr;
        }
        AEFile::Read(key, handle);
        AEFile::Read(count, handle);
        count++;
        if (key == id) {
            Array<int32_t> *result = new Array<int32_t>();
            int32_t *buffer = new int32_t[(uint32_t) count];
            AEFile::Read(count << 2, buffer, handle);
            ArraySetLength<int32_t>(count, *result);
            for (int32_t j = 0; j < count; j++) {
                result->data()[j] = buffer[j];
            }
            delete[] buffer;
            AEFile::Close(handle);
            return result;
        }
        AEFile::Skip(count << 2, handle);
    }
}

Array<int32_t> *FileRead::loadStationParts(int32_t id, int32_t special) {
    Array<int32_t> *parts = nullptr;
    if (AEFile::FileExist(String("data/bin/stationparts.bin")) == 0) {
        return parts;
    }

    uint32_t handle;
    AEFile::OpenRead("data/bin/stationparts.bin", &handle);

    int32_t wanted = id + 1;
    if (special == 1) {
        wanted = 0x65;
    }

    int8_t group;
    int16_t station;
    int8_t count;
    for (uint32_t i = 0;; i++) {
        if (i >= 0x88) {
            AEFile::Close(handle);
            return parts;
        }
        AEFile::Read(group, handle);
        AEFile::ReadSwitched(station, handle);
        AEFile::Read(count, handle);

        parts = new Array<int32_t>();
        ArraySetLength<int32_t>(count * 7 + 7, *parts);
        int32_t *data = parts->data();
        data[0] = station;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 0x800;
        data[6] = 0;

        for (uint32_t off = 7; off < parts->size(); off += 7) {
            int16_t value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off] = value;
            AEFile::ReadSwitched((*parts)[off + 1], handle);
            AEFile::ReadSwitched((*parts)[off + 2], handle);
            AEFile::ReadSwitched((*parts)[off + 3], handle);
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 4] = value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 5] = value;
            AEFile::ReadSwitched(value, handle);
            (*parts)[off + 6] = value;
        }

        if (wanted == group) {
            return parts;
        }
        ArrayRelease(*parts);
    }
}

Array<Station *> *FileRead::loadStationsBinary(SolarSystem *system) {
    Array<Station *> *stations = new Array<Station *>();

    if (AEFile::FileExist(String("data/bin/stations.bin")) == 0) {
        return 0;
    }

    uint32_t handle;
    AEFile::OpenRead("data/bin/stations.bin", &handle);

    Array<int32_t> *ids = (Array<int32_t> *) system->getStations();
    ArraySetLength<Station *>(ids->size(), *stations);

    String name;
    uint32_t out = 0;
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t d;
    for (uint32_t stationId = 0;; stationId++) {
        if (stationId >= 0x87) {
            AEFile::Close(handle);
            return stations;
        }
        AEFile::ReadSwitched(name, handle);
        name.ConvertFromUTF8();
        AEFile::ReadSwitched(a, handle);
        AEFile::ReadSwitched(b, handle);
        AEFile::ReadSwitched(c, handle);
        AEFile::ReadSwitched(d, handle);
        for (uint32_t i = 0; i < ids->size(); i++) {
            if ((uint32_t) ids->data()[i] == stationId) {
                stations->data()[out++] = new Station(name, a, b, c, d);
            }
            if (out == ids->size()) {
                AEFile::Close(handle);
                return stations;
            }
        }
    }
}

Array<Agent *> *FileRead::loadAgents() {

    Array<Agent *> *agents = 0;
    if (AEFile::FileExist(String("data/bin/agents.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/agents.bin", &handle);

        agents = new Array<Agent *>();
        ArraySetLength<Agent *>(0x1b, *agents);

        String name;
        uint32_t index = 0;
        int32_t id;
        int32_t a;
        int32_t b;
        int32_t c;
        int32_t d;
        int32_t e;
        int32_t g;
        int32_t flag;
        int32_t imageCount;
        int32_t f = -1;
        bool male;
        while (index < agents->size()) {

            AEFile::ReadSwitched(name, handle);
            name.ConvertFromUTF8();
            AEFile::ReadSwitched(id, handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(flag, handle);
            male = flag == 1;
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);
            AEFile::ReadSwitched(g, handle);

            agents->data()[index] = new Agent(id, name, a, b, c, male, d, e, f, g);

            AEFile::ReadSwitched(imageCount, handle);
            uint32_t oldIndex = index++;
            if (imageCount > 0) {
                int32_t *parts = new int32_t[5];
                int8_t part;
                for (uint32_t i = 0; i < 5; i++) {
                    AEFile::Read(part, handle);
                    parts[i] = part;
                }
                agents->data()[oldIndex]->setImageParts(parts);
            }
        }
        AEFile::Close(handle);
    }
    return agents;
}

Array<Wanted *> *FileRead::loadWanted() {

    Array<Wanted *> *wanted = 0;
    if (AEFile::FileExist(String("data/bin/wanted.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/wanted.bin", &handle);

        wanted = new Array<Wanted *>();
        ArraySetLength<Wanted *>(0x19, *wanted);

        String name;
        uint32_t index = 0;
        int32_t id;
        int32_t a;
        int32_t b;
        int32_t d;
        int32_t e;
        int32_t f;
        int32_t c;
        int32_t imageCount;
        int32_t g;
        int32_t h;
        int32_t i;
        int32_t j;
        int32_t k;
        int32_t l;
        while (index < wanted->size()) {

            AEFile::ReadSwitched(name, handle);
            name.ConvertFromUTF8();
            AEFile::ReadSwitched(id, handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);
            AEFile::ReadSwitched(g, handle);
            AEFile::ReadSwitched(h, handle);
            AEFile::ReadSwitched(i, handle);
            AEFile::ReadSwitched(j, handle);
            AEFile::ReadSwitched(k, handle);
            AEFile::ReadSwitched(l, handle);

            wanted->data()[index++] = new Wanted(id, name, a, b, c == 1, d, e, f, g, h, i, j, k, l);

            AEFile::ReadSwitched(imageCount, handle);
            uint32_t oldIndex = index - 1;
            if (imageCount > 0) {
                int32_t *parts = new int32_t[5];
                int8_t part;
                for (uint32_t n = 0; n < 5; n++) {
                    AEFile::Read(part, handle);
                    parts[n] = part;
                }
                wanted->data()[oldIndex]->setImageParts(parts);
            }
        }
        AEFile::Close(handle);
    }
    return wanted;
}

int32_t FileRead::loadStationsBinary() {
    if (AEFile::FileExist(String("data/bin/stations.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/stations.bin", &handle);

        // The original sizes and fills a colour table through a pointer that is never allocated
        // (the result is discarded); reproduced as written.
        Array<int8_t> *colors = 0;
        ArraySetLength<int8_t>(0x195, *colors);

        String name;
        int32_t color0;
        int32_t color1;
        int32_t color2;
        int32_t offset = 0;
        for (uint32_t i = 0; i < 0x87; i++) {
            AEFile::ReadSwitched(name, handle);
            AEFile::ReadSwitched(offset, handle);
            AEFile::ReadSwitched(color0, handle);
            AEFile::ReadSwitched(color1, handle);
            AEFile::ReadSwitched(color2, handle);
            (*colors)[offset++] = (int8_t) color0;
            (*colors)[offset++] = (int8_t) color1;
            (*colors)[offset++] = (int8_t) color2;
        }
        AEFile::Close(handle);
    }
    return 0;
}

Array<SolarSystem *> *FileRead::loadSystemsBinary() {

    Array<SolarSystem *> *systems = 0;
    if (AEFile::FileExist(String("data/bin/systems.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/systems.bin", &handle);

        systems = new Array<SolarSystem *>();
        ArraySetLength<SolarSystem *>(0x22, *systems);

        String name;
        int32_t faction;
        int32_t a;
        int32_t b;
        int32_t c;
        int32_t d;
        int32_t e;
        int32_t f;
        int32_t flag;
        Array<int32_t> *stations;
        Array<int32_t> *wrecks;
        Array<int32_t> *statics;
        for (uint32_t i = 0; i < 0x22; i++) {
            AEFile::ReadSwitched(name, handle);
            name.ConvertFromUTF8();
            AEFile::ReadSwitched(faction, handle);
            bool visible;
            AEFile::ReadSwitched(flag, handle);
            visible = flag == 1;
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);

            int32_t count;
            AEFile::ReadSwitched(count, handle);
            int32_t *routes = new int32_t[count];
            for (int32_t j = 0; j < count; j++) {
                AEFile::ReadSwitched(routes[j], handle);
            }

            AEFile::ReadSwitched(count, handle);
            if (count < 1) {
                stations = 0;
            } else {
                stations = new Array<int32_t>();
                ArraySetLength<int32_t>(count, *stations);
                for (uint32_t j = 0; j < stations->size(); j++) {
                    AEFile::ReadSwitched(stations->data()[j], handle);
                }
            }

            AEFile::ReadSwitched(count, handle);
            if (count < 1) {
                wrecks = 0;
            } else {
                wrecks = new Array<int32_t>();
                ArraySetLength<int32_t>(count, *wrecks);
                for (uint32_t j = 0; j < wrecks->size(); j++) {
                    AEFile::ReadSwitched(wrecks->data()[j], handle);
                }
            }

            AEFile::ReadSwitched(count, handle);
            if (count < 1) {
                statics = 0;
            } else {
                statics = new Array<int32_t>();
                ArraySetLength<int32_t>(count, *statics);
                for (uint32_t j = 0; j < statics->size(); j++) {
                    AEFile::ReadSwitched(statics->data()[j], handle);
                }
            }

            systems->data()[i] = new SolarSystem((int32_t) i, name, faction, visible, a, b, c, d, e, f,
                                                 routes, stations, wrecks, statics);
            delete[] routes;
        }
        AEFile::Close(handle);
    }
    return systems;
}

Array<Item *> *FileRead::loadItemsBinary() {

    Array<Item *> *items = 0;
    if (AEFile::FileExist(String("data/bin/items.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/items.bin", &handle);

        items = new Array<Item *>();
        ArraySetLength<Item *>(0xe9, *items);

        uint32_t count0 = 0;
        uint32_t count1 = 0;
        uint32_t count2 = 0;
        for (uint32_t i = 0; i < 0xe9; i++) {
            Array<int> *a0;
            Array<int> *a1;
            Array<int> *a2;

            AEFile::ReadSwitched((int32_t &) count0, handle);
            if ((int32_t) count0 < 1) {
                a0 = 0;
            } else {
                a0 = new Array<int>();
                ArraySetLength<int32_t>(count0, *a0);
                for (int32_t j = 0; j < (int32_t) count0; j++) {
                    AEFile::ReadSwitched(a0->data()[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &) count1, handle);
            if ((int32_t) count1 < 1) {
                a1 = 0;
            } else {
                a1 = new Array<int>();
                ArraySetLength<int32_t>(count1, *a1);
                for (int32_t j = 0; j < (int32_t) count1; j++) {
                    AEFile::ReadSwitched(a1->data()[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &) count2, handle);
            if ((int32_t) count2 < 1) {
                a2 = 0;
            } else {
                a2 = new Array<int>();
                ArraySetLength<int32_t>(count2, *a2);
                for (int32_t j = 0; j < (int32_t) count2; j++) {
                    AEFile::ReadSwitched(a2->data()[j], handle);
                }
            }

            Item *item = new Item(a0, a1, a2);
            items->data()[i] = item;
        }
        AEFile::Close(handle);
    }
    return items;
}

Array<Ship *> *FileRead::loadShipsBinary() {

    Array<Ship *> *ships = 0;
    if (AEFile::FileExist(String("data/bin/ships.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/ships.bin", &handle);

        ships = new Array<Ship *>();
        ArraySetLength<Ship *>(0x40, *ships);

        int32_t a;
        int32_t b;
        int32_t c;
        int32_t d;
        int32_t e;
        int32_t f;
        int32_t g;
        int32_t h;
        int32_t speed;
        for (uint32_t i = 0; i < 0x40; i++) {
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);
            AEFile::ReadSwitched(g, handle);
            AEFile::ReadSwitched(h, handle);
            AEFile::ReadSwitched(speed, handle);
            Ship *ship = new Ship(a, b, c, d, e, f, g, h, (float) speed);
            ships->data()[i] = ship;
        }
        AEFile::Close(handle);
    }
    return ships;
}

Array<String *> *FileRead::loadNamesBinary(int32_t type, bool first, bool second) {
    Array<String *> *names = 0;
    String path("");

    switch (type) {
        case 0: {
            const char *file = "names_terran_0_m.bin";
            if (!first)
                file = "names_terran_0_w.bin";
            if (!second)
                file = "names_terran_1.bin";
            path = String(file);
            break;
        }
        case 1:
            path = String(second ? "names_vossk_0.bin" : "names_vossk_1.bin");
            break;
        case 2:
            path = String(second ? "names_nivelian_0.bin" : "names_nivelian_1.bin");
            break;
        case 3:
            if (second)
                path = String(Globals::rnd->nextInt(2) != 0 ? "names_nivelian_0.bin" : "names_terran_0_m.bin");
            else
                path = String(Globals::rnd->nextInt(2) != 0 ? "names_nivelian_1.bin" : "names_terran_1.bin");
            break;
        case 4:
            path = String(second ? "names_multipod_0.bin" : "names_multipod_1.bin");
            break;
        case 5:
            if (!second)
                return names;
            path = String("names_cyborg_0.bin");
            break;
        case 6:
            path = String(second ? "names_bobolan_0.bin" : "names_bobolan_1.bin");
            break;
        case 7:
            if (!second)
                return names;
            path = String("names_grey_0.bin");
            break;
        case 8:
            if (second)
                path = String(Globals::rnd->nextInt(2) != 0 ? "names_nivelian_0.bin" : "names_terran_0_m.bin");
            else
                path = String(Globals::rnd->nextInt(2) != 0 ? "names_nivelian_1.bin" : "names_terran_1.bin");
            break;
        default:
            return names;
    }

    path = String("data/bin/") + path;

    if (AEFile::FileExist(path) != 0) {
        uint32_t handle;
        AEFile::OpenRead(path, &handle);
        int32_t count;
        AEFile::ReadSwitched(count, handle);

        names = new Array<String *>();
        ArraySetLength<String *>(count, *names);

        String tmp;
        for (int32_t i = 0; i < count; i++) {
            AEFile::ReadSwitched(tmp, handle);
            names->data()[i] = new String(tmp);
        }
        AEFile::Close(handle);
    }

    return names;
}

Array<NewsItem *> *FileRead::loadTicker() {

    Array<NewsItem *> *items = 0;
    if (AEFile::FileExist(String("data/bin/ticker.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/ticker.bin", &handle);

        items = new Array<NewsItem *>();
        ArraySetLength<NewsItem *>(0x3b, *items);

        int32_t active;
        int32_t flags[4];
        int32_t a;
        int32_t b;
        for (uint32_t i = 0; i < 0x3b; i++) {
            AEFile::ReadSwitched(active, handle);
            AEFile::Read(flags[0], handle);
            AEFile::Read(flags[1], handle);
            AEFile::Read(flags[2], handle);
            AEFile::Read(flags[3], handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);

            bool *bits = new bool[4];
            for (int32_t j = 0; j != 4; j++) {
                bits[j] = flags[j] != 0;
            }

            NewsItem *item = new NewsItem((int32_t) i, active != 0, bits, 4, a, b);
            items->data()[i] = item;
        }
        AEFile::Close(handle);
    }
    return items;
}

Array<Array<Vector *> *> *FileRead::loadWeaponPositions(int32_t id) {

    Array<Array<Vector *> *> *positions = 0;
    if (AEFile::FileExist(String("data/bin/weapons_hd.bin")) != 0) {
        uint32_t handle;
        AEFile::OpenRead("data/bin/weapons_hd.bin", &handle);

        positions = new Array<Array<Vector *> *>();
        ArraySetLength<Array<Vector *> *>(4, *positions);
        for (uint32_t i = 0; i != positions->size(); i++) {
            positions->data()[i] = 0;
        }

        int16_t ship = 0;
        int16_t count = 0;
        // Only the last declarator of each list is initialised, as in the original.
        int16_t type, x, y, z = 0;
        float extraX, extraY, extraZ = 0.0f;
        int32_t current;

        do {
            AEFile::Read(ship, handle);
            AEFile::Read(count, handle);
            current = ship;
            for (int32_t i = 0; i < count; i++) {
                AEFile::Read(type, handle);
                AEFile::Read(x, handle);
                AEFile::Read(y, handle);
                AEFile::Read(z, handle);
                if (type == 3) {
                    AEFile::Read(extraX, handle);
                    AEFile::Read(extraY, handle);
                    AEFile::Read(extraZ, handle);
                }
                if (current == id) {
                    if (positions->data()[type] == 0) {
                        positions->data()[type] = new Array<Vector *>();
                    }
                    Vector *pos = new Vector;
                    float px = x;
                    float py = z;
                    float pz = -y;
                    pos->x = px;
                    pos->y = py;
                    pos->z = pz;
                    ArrayAdd<Vector *>(pos, *positions->data()[type]);
                    if (type == 3) {
                        Vector *extra = new Vector;
                        float ex = extraX;
                        float ey = extraZ;
                        float ez = extraY;
                        extra->x = ex;
                        extra->y = ey;
                        extra->z = ez;
                        ArrayAdd<Vector *>(extra, *positions->data()[3]);
                    }
                }
            }
        } while (current != id);
        AEFile::Close(handle);
    }
    return positions;
}

Array<SpacePoint *> *FileRead::loadSpacePoints(int32_t id, int32_t group) {

    Array<SpacePoint *> *points = 0;
    if (!AEFile::FileExist(String("data/bin/docks_hd.bin"))) {
        return points;
    }
    {
        uint32_t handle;
        AEFile::OpenRead("data/bin/docks_hd.bin", &handle);

        uint16_t current = 0;
        uint16_t count = 0;
        Vector position;
        Vector rotation;
        Vector unused;
        Vector direction;
        for (;;) {
            AEFile::Read(current, handle);
            AEFile::Read(count, handle);
            if ((uint32_t) current == (uint32_t) id)
                break;
            AEFile::Skip((uint32_t) count * 0x26, handle);
        }

        points = new Array<SpacePoint *>();

        const float toRadNeg = -0.017453292f;
        const float toRad = 0.017453292f;
        uint16_t type;
        for (uint32_t i = 0; i < count; i++) {
            AEFile::Read(type, handle);
            AEFile::Read(position.x, handle);
            AEFile::Read(position.y, handle);
            AEFile::Read(position.z, handle);
            AEFile::Read(rotation.x, handle);
            AEFile::Read(rotation.y, handle);
            AEFile::Read(rotation.z, handle);
            AEFile::Read(unused.x, handle);
            AEFile::Read(unused.y, handle);
            AEFile::Read(unused.z, handle);

            // File space is Y-up/right-handed the other way: swap Y/Z and negate.
            float py = position.y;
            position.y = position.z;
            position.z = -py;
            float rx = rotation.x * toRadNeg;
            float ry2 = rotation.z * toRadNeg;
            float rz2 = rotation.y * toRad;
            rotation.x = rx;
            rotation.y = ry2;
            rotation.z = rz2;

            Matrix matrix;
            matrix = MatrixSetRotation(matrix, rotation.x, rotation.y, rotation.z,
                                       AbyssEngine::AEMath::ROTATION_ORDER_XZY);
            direction = MatrixGetDir(matrix);

            if (group == -1 || (i >> 1) == (uint32_t) group) {
                ArrayAdd<SpacePoint *>(new SpacePoint((int32_t) type, position, direction, (int32_t) i), *points);
            }
        }
        AEFile::Close(handle);
    }
    return points;
}
