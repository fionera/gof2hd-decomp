#include "gof2/engine/file/FileRead.h"


// ---- FileRead_11f944 / ~FileRead_11f946 ----
// FileRead carries no state of its own (every loadXxx() opens, reads and closes
// its file locally), so both the constructor and destructor have empty bodies.
FileRead::FileRead() {}
FileRead::~FileRead() {}


// ---- loadStation_11f948.cpp ----
__attribute__((minsize))
int32_t FileRead::loadStation(int32_t id)
{
    int16_t *selected = new int16_t[1];
    selected[0] = (int16_t)id;
    Array<Station *> *stations = ((FileRead *)selected)->loadStationsBinary(selected, 1);
    int32_t result = *(int32_t *)stations->data();
    delete[] selected;
    return result;
}

// ---- loadStationsBinary_1206b0.cpp ----
__attribute__((minsize))
int32_t FileRead::loadStationsBinary()
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("stations_binary.bin");
        exists = AEFile::FileExist(path);
    }

    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("stations_binary.bin", &handle);
        ArraySetLength<int8_t>(0x195, (Array<int8_t> *)0);

        int32_t color2;
        int32_t color1;
        int32_t color0;
        int32_t offset = 0;
        String name;
        for (uint32_t i = 0; i < 0x87; i++) {
            AEFile::ReadSwitched(name, handle);
            AEFile::ReadSwitched(offset, handle);
            AEFile::ReadSwitched(color0, handle);
            AEFile::ReadSwitched(color1, handle);
            AEFile::ReadSwitched(color2, handle);
            char *colors = *(char **)4;
            colors[offset++] = (char)color0;
            colors[offset++] = (char)color1;
            colors[offset++] = (char)color2;
        }
        AEFile::Close(handle);
    }
    return 0;
}

// ---- loadWeaponPositions_1212e4.cpp ----
__attribute__((minsize))
Array<Array<Vector *> *> *FileRead::loadWeaponPositions(int32_t id)
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("weapon_positions.bin");
        exists = AEFile::FileExist(path);
    }

    Array<Array<Vector *> *> *positions = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("weapon_positions.bin", &handle);

        positions = new Array<Array<Vector *> *>();
        ArraySetLength<Array<Vector *> *>(4, positions);
        for (uint32_t i = 0; i != positions->size(); i++) {
            positions->data()[i] = 0;
        }

        int16_t ship = 0;
        int16_t count = 0;
        int16_t type = 0;
        int16_t x = 0;
        int16_t y = 0;
        int16_t z = 0;
        float extraX = 0.0f;
        float extraY = 0.0f;
        float extraZ = 0.0f;
        uint32_t (*readShort)(int16_t &, uint32_t) = &AEFile::ReadSwitched;
        uint32_t (*readFloat)(float &, uint32_t) = &AEFile::ReadSwitched;

        do {
            AEFile::Read(ship, handle);
            AEFile::Read(count, handle);
            for (int32_t i = 0; i < count; i++) {
                readShort(type, handle);
                readShort(x, handle);
                readShort(y, handle);
                readShort(z, handle);
                if (type == 3) {
                    readFloat(extraX, handle);
                    readFloat(extraY, handle);
                    readFloat(extraZ, handle);
                }
                if (ship == id) {
                    if (positions->data()[type] == 0) {
                        positions->data()[type] = new Array<Vector *>();
                    }
                    Vector *pos = new Vector;
                    pos->x = (float)x;
                    pos->y = (float)z;
                    pos->z = (float)-y;
                    ArrayAdd<Vector *>(pos, positions->data()[type]);
                    if (type == 3) {
                        Vector *extra = new Vector;
                        extra->x = extraX;
                        extra->y = extraZ;
                        extra->z = extraY;
                        ArrayAdd<Vector *>(extra, positions->data()[3]);
                    }
                }
            }
        } while (ship != id);
        AEFile::Close(handle);
    }
    return positions;
}

// ---- loadSpacePoints_121550.cpp ----
__attribute__((minsize))
Array<SpacePoint *> *FileRead::loadSpacePoints(int32_t id, int32_t group)
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("space_points.bin");
        exists = AEFile::FileExist(path);
    }

    Array<SpacePoint *> *points = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("space_points.bin", &handle);

        uint16_t current = 0;
        uint16_t count = 0;
        do {
            AEFile::Read(current, handle);
            AEFile::Read(count, handle);
            if ((uint32_t)current != (uint32_t)id) {
                AEFile::Skip((uint32_t)count * 0x26, handle);
            }
        } while ((uint32_t)current != (uint32_t)id);

        points = new Array<SpacePoint *>();

        Matrix matrix;
        Matrix rotation;
        Vector direction;
        Vector position;
        uint32_t (*readFloat)(float &, uint32_t) = &AEFile::ReadSwitched;
        const float angleScale = 0.017453292f;
        const float rollScale = -0.017453292f;

        for (uint32_t i = 0; i < count; i++) {
            uint16_t type;
            float a;
            float b;
            float c;
            float rx;
            float ry;
            float rz;
            float sx;
            float sy;
            float sz;

            AEFile::Read(type, handle);
            readFloat(a, handle);
            readFloat(b, handle);
            readFloat(c, handle);
            readFloat(rx, handle);
            readFloat(ry, handle);
            readFloat(rz, handle);
            readFloat(sx, handle);
            readFloat(sy, handle);
            readFloat(sz, handle);

            position.x = a;
            position.y = c;
            position.z = -b;

            matrix.m[0] = 1.0f;
            matrix.m[1] = 0.0f;
            matrix.m[2] = 0.0f;
            matrix.m[3] = 0.0f;
            matrix.m[4] = 0.0f;
            matrix.m[5] = 1.0f;
            matrix.m[6] = 0.0f;
            matrix.m[7] = 0.0f;
            matrix.m[8] = 0.0f;
            matrix.m[9] = 0.0f;
            matrix.m[10] = 1.0f;
            matrix.m[11] = 0.0f;
            matrix.m[12] = 0.0f;
            matrix.m[13] = 0.0f;
            matrix.m[14] = 0.0f;
            matrix.m[15] = 1.0f;

            MatrixSetRotation(&rotation, &matrix, rx * angleScale, rz * angleScale, ry * rollScale, 1);
            matrix = rotation;
            MatrixGetDir((Vector *)&rotation, &matrix);
            direction = *(Vector *)&rotation;

            uint32_t selected = 0;
            uint32_t requested = group;
            if (group != -1) {
                selected = i >> 1;
            }
            if (group == -1 || selected == requested) {
                SpacePoint *point = new SpacePoint(type, &position, &direction, i);
                ArrayAdd<SpacePoint *>(point, points);
            }
        }
        AEFile::Close(handle);
    }
    return points;
}

// ---- loadSystemsBinary_1207e0.cpp ----
__attribute__((minsize))
Array<SolarSystem *> *FileRead::loadSystemsBinary()
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("systems_binary.bin");
        exists = AEFile::FileExist(path);
    }

    Array<SolarSystem *> *systems = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("systems_binary.bin", &handle);

        systems = new Array<SolarSystem *>();
        ArraySetLength<SolarSystem *>(0x22, systems);

        String name;
        for (uint32_t i = 0; i < 0x22; i++) {
            int32_t faction;
            int32_t flag;
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t d;
            int32_t e;
            int32_t f;
            uint32_t count;

            AEFile::ReadSwitched(name, handle);
            gof2_fileread::ConvertFromUTF8(name);
            AEFile::ReadSwitched(faction, handle);
            AEFile::ReadSwitched(flag, handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);

            AEFile::ReadSwitched((int32_t &)count, handle);
            int32_t *routes = new int32_t[count];
            for (int32_t j = 0; j < (int32_t)count; j++) {
                AEFile::ReadSwitched(routes[j], handle);
            }

            AEFile::ReadSwitched((int32_t &)count, handle);
            Array<int32_t> *stations;
            if ((int32_t)count < 1) {
                stations = 0;
            } else {
                stations = new Array<int32_t>();
                ArraySetLength<int32_t>(count, stations);
                for (uint32_t j = 0; j < stations->size(); j++) {
                    AEFile::ReadSwitched(stations->data()[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count, handle);
            Array<int32_t> *wrecks;
            if ((int32_t)count < 1) {
                wrecks = 0;
            } else {
                wrecks = new Array<int32_t>();
                ArraySetLength<int32_t>(count, wrecks);
                for (uint32_t j = 0; j < wrecks->size(); j++) {
                    AEFile::ReadSwitched(wrecks->data()[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count, handle);
            Array<int32_t> *statics;
            if ((int32_t)count < 1) {
                statics = 0;
            } else {
                statics = new Array<int32_t>();
                ArraySetLength<int32_t>(count, statics);
                for (uint32_t j = 0; j < statics->size(); j++) {
                    AEFile::ReadSwitched(statics->data()[j], handle);
                }
            }

            SolarSystem *system = new SolarSystem(i, name, faction, flag == 1, a, b, c, d, e, f,
                                                  routes, stations, wrecks, statics);
            systems->data()[i] = system;
            delete[] routes;
        }
        AEFile::Close(handle);
    }
    return systems;
}

// ---- loadWanted_120490.cpp ----
__attribute__((minsize))
Array<Wanted *> *FileRead::loadWanted()
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("wanted_binary.bin");
        exists = AEFile::FileExist(path);
    }

    Array<Wanted *> *wanted = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("wanted_binary.bin", &handle);

        wanted = new Array<Wanted *>();
        ArraySetLength<Wanted *>(0x19, wanted);

        String name;
        uint32_t index = 0;
        while (index < wanted->size()) {
            int32_t id;
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t d;
            int32_t e;
            int32_t f;
            int32_t g;
            int32_t h;
            int32_t i;
            int32_t j;
            int32_t k;
            int32_t l;
            int32_t imageCount;

            AEFile::ReadSwitched(name, handle);
            gof2_fileread::ConvertFromUTF8(name);
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

            Wanted *entry = new Wanted(id, name, a, b, c == 1, d, e, f, g, h, i, j, k, l);
            wanted->data()[index] = entry;

            AEFile::ReadSwitched(imageCount, handle);
            uint32_t oldIndex = index++;
            if (imageCount > 0) {
                int32_t *parts = new int32_t[5];
                for (uint32_t n = 0; n < 5; n++) {
                    char part;
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

// ---- loadTicker_121144.cpp ----
__attribute__((minsize))
Array<NewsItem *> *FileRead::loadTicker()
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("ticker_binary.bin");
        exists = AEFile::FileExist(path);
    }

    Array<NewsItem *> *items = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("ticker_binary.bin", &handle);

        items = new Array<NewsItem *>();
        ArraySetLength<NewsItem *>(0x3b, items);

        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;
        for (uint32_t i = 0; i < 0x3b; i++) {
            int32_t active;
            int32_t flags[4];
            int32_t a;
            int32_t b;
            readInt(active, handle);
            readInt(flags[0], handle);
            readInt(flags[1], handle);
            readInt(flags[2], handle);
            readInt(flags[3], handle);
            readInt(a, handle);
            readInt(b, handle);

            bool *bits = new bool[4];
            for (int32_t j = 0; j != 4; j++) {
                bits[j] = flags[j] != 0;
            }

            NewsItem *item = new NewsItem(i, active != 0, bits, 4, a, b);
            items->data()[i] = item;
        }
        AEFile::Close(handle);
    }
    return items;
}

// ---- loadStationsBinary_11f970.cpp ----
__attribute__((minsize))
Array<Station *> *FileRead::loadStationsBinary(int16_t *ids, int32_t count)
{
    Array<Station *> *stations = new Array<Station *>();
    ArraySetLength<Station *>(count, stations);

    uint32_t exists;
    {
        String path = gof2_fileread::makeString("stations_binary.bin");
        exists = AEFile::FileExist(path);
    }
    if (exists == 0) {
        return 0;
    }

    uint32_t handle;
    AEFile::OpenRead("stations_binary.bin", &handle);

    String name;
    int32_t out = 0;
    for (uint32_t stationId = 0; stationId < 0x87; stationId++) {
        int32_t a;
        int32_t b;
        int32_t c;
        int32_t d;
        AEFile::ReadSwitched(name, handle);
        gof2_fileread::ConvertFromUTF8(name);
        AEFile::ReadSwitched(a, handle);
        AEFile::ReadSwitched(b, handle);
        AEFile::ReadSwitched(c, handle);
        AEFile::ReadSwitched(d, handle);
        for (int32_t i = 0; i < count; i++) {
            if (stationId == (uint32_t)ids[i]) {
                Station *station = new Station(name, a, b, c, d);
                stations->data()[out] = station;
                out++;
            }
        }
    }
    AEFile::Close(handle);
    return stations;
}

// ---- loadNamesBinary_120dd0.cpp ----
extern AbyssEngine::AERandom *gNameRandomA;
extern AbyssEngine::AERandom *gNameRandomB;

__attribute__((minsize))
Array<String *> *FileRead::loadNamesBinary(int32_t type, bool first, bool second)
{
    Array<String *> *names = 0;
    String path = gof2_fileread::makeString("");

    switch (type) {
    case 0: {
        const char *text = "names_0_ab";
        if (!first) {
            text = "names_0_b";
        }
        if (!second) {
            text = "names_0_a";
        }
        String tmp = gof2_fileread::makeString(text);
        path = tmp;
        break;
    }
    case 1: {
        const char *text = "names_1_b";
        if (!second) {
            text = "names_1_a";
        }
        String tmp = gof2_fileread::makeString(text);
        path = tmp;
        break;
    }
    case 2: {
        const char *text = "names_2_b";
        if (!second) {
            text = "names_2_a";
        }
        String tmp = gof2_fileread::makeString(text);
        path = tmp;
        break;
    }
    case 3: {
        if (second) {
            int32_t value = gNameRandomA->nextInt(2);
            const char *text = "names_3_a";
            if (value != 0) {
                text = "names_3_b";
            }
            String tmp = gof2_fileread::makeString(text);
            path = tmp;
        } else {
            int32_t value = gNameRandomA->nextInt(2);
            const char *text = "names_3_c";
            if (value != 0) {
                text = "names_3_d";
            }
            String tmp = gof2_fileread::makeString(text);
            path = tmp;
        }
        break;
    }
    case 4: {
        const char *text = "names_4_b";
        if (!second) {
            text = "names_4_a";
        }
        String tmp = gof2_fileread::makeString(text);
        path = tmp;
        break;
    }
    case 5: {
        if (!second) {
            goto done;
        }
        String tmp = gof2_fileread::makeString("names_5");
        path = tmp;
        break;
    }
    case 6: {
        const char *text = "names_6_b";
        if (!second) {
            text = "names_6_a";
        }
        String tmp = gof2_fileread::makeString(text);
        path = tmp;
        break;
    }
    case 7: {
        if (!second) {
            goto done;
        }
        String tmp = gof2_fileread::makeString("names_7");
        path = tmp;
        break;
    }
    case 8: {
        if (second) {
            int32_t value = gNameRandomB->nextInt(2);
            const char *text = "names_8_a";
            if (value != 0) {
                text = "names_8_b";
            }
            String tmp = gof2_fileread::makeString(text);
            path = tmp;
        } else {
            int32_t value = gNameRandomB->nextInt(2);
            const char *text = "names_8_c";
            if (value != 0) {
                text = "names_8_d";
            }
            String tmp = gof2_fileread::makeString(text);
            path = tmp;
        }
        break;
    }
    default:
        goto done;
    }

    {
        String suffix = gof2_fileread::makeString(".bin");
        String combined = suffix + path;
        path = combined;
    }

    if (AEFile::FileExist(path) != 0) {
        uint32_t handle;
        uint32_t count;
        AEFile::OpenRead(path, &handle);
        AEFile::ReadSwitched((int32_t &)count, handle);

        names = new Array<String *>();
        ArraySetLength<String *>(count, names);

        String tmp;
        for (int32_t i = 0; i < (int32_t)count; i++) {
            AEFile::ReadSwitched(tmp, handle);
            String *entry = new String(tmp);
            names->data()[i] = entry;
        }
        AEFile::Close(handle);
    }

done:
    return names;
}

// ---- loadStationsBinary_12014c.cpp ----
__attribute__((minsize))
Array<Station *> *FileRead::loadStationsBinary(SolarSystem *system)
{
    Array<Station *> *stations = new Array<Station *>();

    uint32_t exists;
    {
        String path = gof2_fileread::makeString("stations_binary.bin");
        exists = AEFile::FileExist(path);
    }
    if (exists == 0) {
        return 0;
    }

    uint32_t handle;
    AEFile::OpenRead("stations_binary.bin", &handle);

    Array<int32_t> *ids = system->getStations();
    ArraySetLength<Station *>(ids->size(), stations);

    String name;
    uint32_t out = 0;
    for (uint32_t stationId = 0; stationId < 0x87; stationId++) {
        int32_t a;
        int32_t b;
        int32_t c;
        int32_t d;
        AEFile::ReadSwitched(name, handle);
        gof2_fileread::ConvertFromUTF8(name);
        AEFile::ReadSwitched(a, handle);
        AEFile::ReadSwitched(b, handle);
        AEFile::ReadSwitched(c, handle);
        AEFile::ReadSwitched(d, handle);
        for (uint32_t i = 0; i < ids->size(); i++) {
            if ((uint32_t)ids->data()[i] == stationId) {
                Station *station = new Station(name, a, b, c, d);
                stations->data()[out++] = station;
            }
            if (out == ids->size()) {
                AEFile::Close(handle);
                return stations;
            }
        }
    }
    AEFile::Close(handle);
    return stations;
}

// ---- loadAgents_1202b8.cpp ----
__attribute__((minsize))
Array<Agent *> *FileRead::loadAgents()
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("agents_binary.bin");
        exists = AEFile::FileExist(path);
    }

    Array<Agent *> *agents = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("agents_binary.bin", &handle);

        agents = new Array<Agent *>();
        ArraySetLength<Agent *>(0x1b, agents);

        String name;
        int32_t f = -1;
        uint32_t index = 0;
        while (index < agents->size()) {
            int32_t id;
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t flag;
            int32_t d;
            int32_t e;
            int32_t g;
            int32_t h;
            int32_t imageCount;

            AEFile::ReadSwitched(name, handle);
            gof2_fileread::ConvertFromUTF8(name);
            AEFile::ReadSwitched(id, handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(flag, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);
            AEFile::ReadSwitched(g, handle);

            Agent *agent = new Agent(id, name, a, b, c, flag == 1, d, e, f, g);
            agents->data()[index] = agent;

            AEFile::ReadSwitched(imageCount, handle);
            uint32_t oldIndex = index++;
            if (imageCount > 0) {
                int32_t *parts = new int32_t[5];
                for (uint32_t i = 0; i < 5; i++) {
                    char part;
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

// ---- loadWreckCollision_11fd78.cpp ----
__attribute__((minsize))
Array<int32_t> *FileRead::loadWreckCollision(int32_t id)
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("wreck_collision.bin");
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *result = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("wreck_collision.bin", &handle);

        int32_t wreck = 0;
        uint32_t count = 0;
        for (uint32_t i = 0; i < 6; i++) {
            AEFile::Read(wreck, handle);
            AEFile::Read((int32_t &)count, handle);
            count++;
            if (wreck == id) {
                result = new Array<int32_t>();
                int32_t *buffer = new int32_t[count];
                AEFile::Read(count << 2, buffer, handle);
                ArraySetLength<int32_t>(count, result);
                for (int32_t j = 0; j < (int32_t)count; j++) {
                    result->data()[j] = buffer[j];
                }
                delete[] buffer;
                AEFile::Close(handle);
                return result;
            }
            AEFile::Skip(count << 2, handle);
        }
        AEFile::Close(handle);
        result = 0;
    }
    return result;
}

// ---- loadStationCollision_11fc5c.cpp ----
__attribute__((minsize))
Array<int32_t> *FileRead::loadStationCollision(int32_t id)
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("station_collision.bin");
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *result = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("station_collision.bin", &handle);

        int32_t station = 0;
        uint32_t count = 0;
        for (uint32_t i = 0; i < 0x88; i++) {
            AEFile::Read(station, handle);
            AEFile::Read((int32_t &)count, handle);
            count++;
            if (station == id) {
                result = new Array<int32_t>();
                int32_t *buffer = new int32_t[count];
                AEFile::Read(count << 2, buffer, handle);
                ArraySetLength<int32_t>(count, result);
                for (int32_t j = 0; j < (int32_t)count; j++) {
                    result->data()[j] = buffer[j];
                }
                delete[] buffer;
                AEFile::Close(handle);
                return result;
            }
            AEFile::Skip(count << 2, handle);
        }
        AEFile::Close(handle);
        result = 0;
    }
    return result;
}

// ---- loadStaticCollision_11fe94.cpp ----
__attribute__((minsize))
Array<int32_t> *FileRead::loadStaticCollision(int32_t id)
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("static_collision.bin");
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *result = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("static_collision.bin", &handle);

        int32_t object = 0;
        uint32_t count = 0;
        for (uint32_t i = 0; i < 7; i++) {
            AEFile::Read(object, handle);
            AEFile::Read((int32_t &)count, handle);
            count++;
            if (object == id) {
                result = new Array<int32_t>();
                int32_t *buffer = new int32_t[count];
                AEFile::Read(count << 2, buffer, handle);
                ArraySetLength<int32_t>(count, result);
                for (int32_t j = 0; j < (int32_t)count; j++) {
                    result->data()[j] = buffer[j];
                }
                delete[] buffer;
                AEFile::Close(handle);
                return result;
            }
            AEFile::Skip(count << 2, handle);
        }
        AEFile::Close(handle);
        result = 0;
    }
    return result;
}

// ---- loadStationParts_11ffb0.cpp ----
__attribute__((minsize))
Array<int32_t> *FileRead::loadStationParts(int32_t id, int32_t special)
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("station_parts.bin");
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *parts = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("station_parts.bin", &handle);

        int32_t wanted = id + 1;
        if (special == 1) {
            wanted = 0x65;
        }

        uint32_t (*readShort)(int16_t &, uint32_t) = &AEFile::ReadSwitched;
        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;

        for (uint32_t i = 0; i < 0x88; i++) {
            char group;
            char count;
            int16_t station;
            AEFile::Read(group, handle);
            AEFile::ReadSwitched(station, handle);
            AEFile::Read(count, handle);

            parts = new Array<int32_t>();
            ArraySetLength<int32_t>((int8_t)count * 7 + 7, parts);
            int32_t *data = parts->data();
            data[0] = station;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            data[4] = 0;
            data[5] = 0x800;
            data[6] = 0;

            for (uint32_t j = 7, off = 7; j < parts->size(); j += 7, off += 7) {
                int16_t value;
                readShort(value, handle);
                data[off] = value;
                readInt(data[off + 1], handle);
                readInt(data[off + 2], handle);
                readInt(data[off + 3], handle);
                readShort(value, handle);
                data[off + 4] = value;
                readShort(value, handle);
                data[off + 5] = value;
                readShort(value, handle);
                data[off + 6] = value;
            }

            if (wanted == (int8_t)group) {
                return parts;
            }
            ArrayRelease<int32_t>(parts);
        }
        AEFile::Close(handle);
    }
    return parts;
}

// ---- loadShipParts_11fad0.cpp ----
__attribute__((minsize))
Array<int32_t> *FileRead::loadShipParts(int32_t id)
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("ship_parts.bin");
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *parts = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("ship_parts.bin", &handle);

        uint32_t (*readShort)(int16_t &, uint32_t) = &AEFile::ReadSwitched;
        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;
        int32_t wanted = id + 1;
        char group;
        char count;

        for (uint32_t i = 0; i <= 0x40; i++) {
            if (i > 0x3f) {
                AEFile::Close(handle);
                break;
            }
            AEFile::Read(group, handle);
            AEFile::Read(count, handle);

            parts = new Array<int32_t>();
            ArraySetLength<int32_t>((int8_t)count * 10, parts);
            int32_t *data = parts->data();
            for (uint32_t j = 0, off = 0; j < parts->size(); j += 10, off += 10) {
                int16_t value;
                readShort(value, handle);
                data[off] = value;
                readInt(data[off + 1], handle);
                readInt(data[off + 2], handle);
                readInt(data[off + 3], handle);
                readShort(value, handle);
                data[off + 4] = value;
                readShort(value, handle);
                data[off + 5] = value;
                readShort(value, handle);
                data[off + 6] = value;
                readShort(value, handle);
                data[off + 7] = value;
                readShort(value, handle);
                data[off + 8] = value;
                readShort(value, handle);
                data[off + 9] = value;
            }
            if (wanted == (int8_t)group) {
                break;
            }
        }
    }
    return parts;
}

// ---- loadItemsBinary_120afc.cpp ----
__attribute__((minsize))
Array<Item *> *FileRead::loadItemsBinary()
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("items_binary.bin");
        exists = AEFile::FileExist(path);
    }

    Array<Item *> *items = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("items_binary.bin", &handle);

        items = new Array<Item *>();
        ArraySetLength<Item *>(0xe9, items);

        uint32_t count0 = 0;
        uint32_t count1 = 0;
        uint32_t count2 = 0;
        for (uint32_t i = 0; i < 0xe9; i++) {
            Array<int32_t> *a0;
            Array<int32_t> *a1;
            Array<int32_t> *a2;

            AEFile::ReadSwitched((int32_t &)count0, handle);
            if ((int32_t)count0 < 1) {
                a0 = 0;
            } else {
                a0 = new Array<int32_t>();
                ArraySetLength<int32_t>(count0, a0);
                for (int32_t j = 0; j < (int32_t)count0; j++) {
                    AEFile::ReadSwitched(a0->data()[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count1, handle);
            if ((int32_t)count1 < 1) {
                a1 = 0;
            } else {
                a1 = new Array<int32_t>();
                ArraySetLength<int32_t>(count1, a1);
                for (int32_t j = 0; j < (int32_t)count1; j++) {
                    AEFile::ReadSwitched(a1->data()[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count2, handle);
            if ((int32_t)count2 < 1) {
                a2 = 0;
            } else {
                a2 = new Array<int32_t>();
                ArraySetLength<int32_t>(count2, a2);
                for (int32_t j = 0; j < (int32_t)count2; j++) {
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

// ---- loadShipsBinary_120ca8.cpp ----
__attribute__((minsize))
Array<Ship *> *FileRead::loadShipsBinary()
{
    uint32_t exists;
    {
        String path = gof2_fileread::makeString("ships_binary.bin");
        exists = AEFile::FileExist(path);
    }

    Array<Ship *> *ships = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("ships_binary.bin", &handle);

        ships = new Array<Ship *>();
        ArraySetLength<Ship *>(0x40, ships);

        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;
        for (uint32_t i = 0; i < 0x40; i++) {
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t d;
            int32_t e;
            int32_t f;
            int32_t g;
            int32_t h;
            int32_t speed;
            readInt(a, handle);
            readInt(b, handle);
            readInt(c, handle);
            readInt(d, handle);
            readInt(e, handle);
            readInt(f, handle);
            readInt(g, handle);
            readInt(h, handle);
            readInt(speed, handle);
            Ship *ship = new Ship(a, b, c, d, e, f, g, h, (float)speed);
            ships->data()[i] = ship;
        }
        AEFile::Close(handle);
    }
    return ships;
}
