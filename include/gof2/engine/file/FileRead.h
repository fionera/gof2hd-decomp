#ifndef GOF2_FILEREAD_H
#define GOF2_FILEREAD_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct FileRead;

namespace AbyssEngine {



String operator+(const String &left, const String &right);

// AERandom is fully modeled in gof2/AERandom.h. When that header is also included
// we must not redefine it here; FileRead.cpp only relies on nextInt(int), which the
// real class also provides.
#ifndef GOF2_AERANDOM_H
struct AERandom {
    int32_t nextInt(int32_t max);
};
#else
class AERandom;
#endif

namespace AEMath {





void MatrixSetRotation(Matrix *out, Matrix *matrix, float x, float y, float z, int32_t order);
void MatrixGetDir(Vector *out, Matrix *matrix);

} // namespace AEMath
} // namespace AbyssEngine

using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;



struct Station {
    uint8_t storage[0x34];
    Station(String &name, int32_t a, int32_t b, int32_t c, int32_t d);
};

// SolarSystem is fully modeled in gof2/SolarSystem.h. When that header is also
// included (e.g. by PlayerStation.cpp) we must not redefine it here; FileRead.cpp
// only relies on the ctor + getStations() which the real class also provides.
#ifndef GOF2_SOLARSYSTEM_H
struct SolarSystem {
    uint8_t storage[0x48];
    SolarSystem(uint32_t index, String &name, int32_t faction, bool flag, int32_t a, int32_t b,
                int32_t c, int32_t d, int32_t e, int32_t f, int32_t *routes,
                Array<int32_t> *stations, Array<int32_t> *wrecks, Array<int32_t> *statics);
    Array<int32_t> *getStations();
};
#else
class SolarSystem;
#endif

struct Agent {
    uint8_t storage[0x98];
    Agent(int32_t id, String &name, int32_t a, int32_t b, int32_t c, bool flag, int32_t d,
          int32_t e, int32_t f, int32_t g);
    void setImageParts(int32_t *parts);
};

struct Wanted {
    uint8_t storage[0x54];
    Wanted(int32_t id, String &name, int32_t a, int32_t b, bool flag, int32_t c, int32_t d,
           int32_t e, int32_t f, int32_t g, int32_t h, int32_t i, int32_t j, int32_t k);
    void setImageParts(int32_t *parts);
};

struct Item {
    uint8_t storage[0x48];
    Item(Array<int32_t> *ingredients, Array<int32_t> *quantities, Array<int32_t> *attributes);
};

struct Ship {
    uint8_t storage[0x80];
    Ship(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g,
         int32_t h, float i);
};

struct NewsItem {
    uint8_t storage[0x1c];
    NewsItem(uint32_t index, bool active, bool *flags, int32_t count, int32_t a, int32_t b);
};

struct SpacePoint {
    uint8_t storage[0x24];
    SpacePoint(uint32_t type, Vector *position, Vector *direction, uint32_t index);
};

struct AEFile {
    static uint32_t FileExist(String &path);
    static uint32_t OpenRead(const char *path, uint32_t *handle);
    static uint32_t OpenRead(String &path, uint32_t *handle);
    static uint32_t Close(uint32_t handle);
    static uint32_t Skip(uint32_t bytes, uint32_t handle);

    static uint32_t Read(uint32_t bytes, void *buffer, uint32_t handle);
    static uint32_t Read(char &value, uint32_t handle);
    static uint32_t Read(uint16_t &value, uint32_t handle);
    static uint32_t Read(int16_t &value, uint32_t handle);
    static uint32_t Read(int32_t &value, uint32_t handle);
    static uint32_t Read(float &value, uint32_t handle);

    static uint32_t ReadSwitched(String &value, uint32_t handle);
    static uint32_t ReadSwitched(int16_t &value, uint32_t handle);
    static uint32_t ReadSwitched(uint16_t &value, uint32_t handle);
    static uint32_t ReadSwitched(int32_t &value, uint32_t handle);
    static uint32_t ReadSwitched(float &value, uint32_t handle);
};

template <class T>
void ArraySetLength(uint32_t length, Array<T> *array);

template <class T>
void ArrayAdd(T item, Array<T> *array);

template <class T>
void ArrayRelease(Array<T> *array);

#include <new>

// Helpers that stand in for the engine's UTF-16 String operations not present in the
// simplified common.h String (which only exposes a default ctor + text()/size()).
namespace gof2_fileread {
inline String makeString(const char *t)
{
    String r;
    if (t)
        while (*t)
            r.s.push_back((char16_t)(unsigned char)*t++);
    return r;
}
// Game-side CP-1252/UTF-8 transliteration; storage already holds the decoded text natively.
inline void ConvertFromUTF8(String &) {}
} // namespace gof2_fileread

// AbyssEngine::FileRead — loads the game's *.bin data tables (stations, systems, ships, items, ...).
// All methods are instance methods on the (stateless) loader object.
class FileRead {
public:
    // FileRead is a stateless loader: it owns no members, so construction and
    // destruction are trivial. They exist as real special members so the call
    // sites that build a transient reader on the stack/heap behave correctly.
    FileRead();
    ~FileRead();

    int32_t loadStation(int32_t id);
    int32_t loadStationsBinary();
    Array<Array<Vector *> *> *loadWeaponPositions(int32_t id);
    Array<SpacePoint *> *loadSpacePoints(int32_t id, int32_t group);
    Array<SolarSystem *> *loadSystemsBinary();
    Array<Wanted *> *loadWanted();
    Array<NewsItem *> *loadTicker();
    Array<Station *> *loadStationsBinary(int16_t *ids, int32_t count);
    Array<String *> *loadNamesBinary(int32_t type, bool first, bool second);
    Array<Station *> *loadStationsBinary(SolarSystem *system);
    Array<Agent *> *loadAgents();
    Array<int32_t> *loadWreckCollision(int32_t id);
    Array<int32_t> *loadStationCollision(int32_t id);
    Array<int32_t> *loadStaticCollision(int32_t id);
    Array<int32_t> *loadStationParts(int32_t id, int32_t special);
    Array<int32_t> *loadShipParts(int32_t id);
    Array<Item *> *loadItemsBinary();
    Array<Ship *> *loadShipsBinary();
};
#endif
