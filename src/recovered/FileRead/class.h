#ifndef WORK_CLASSES_FILEREAD_SRC_CLASS_H
#define WORK_CLASSES_FILEREAD_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;


struct FileRead;

namespace AbyssEngine {

struct String {
    char storage[0x0c];

    String();
    String(const char *text, bool copy);
    String(const String &other, bool copy);
    ~String();

    String &operator=(const String &other);
    void ConvertFromUTF8();
};

String operator+(const String &left, const String &right);

struct AERandom {
    int32_t nextInt(int32_t max);
};

namespace AEMath {

struct Vector {
    float x;
    float y;
    float z;

    Vector &operator=(const Vector &other);
};

struct Matrix {
    float m[16];

    Matrix &operator=(const Matrix &other);
};

void MatrixSetRotation(Matrix *out, Matrix *matrix, float x, float y, float z, int32_t order);
void MatrixGetDir(Vector *out, Matrix *matrix);

} // namespace AEMath
} // namespace AbyssEngine

using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

struct Station {
    uint8_t storage[0x34];
    Station(String &name, int32_t a, int32_t b, int32_t c, int32_t d);
};

struct SolarSystem {
    uint8_t storage[0x48];
    SolarSystem(uint32_t index, String &name, int32_t faction, bool flag, int32_t a, int32_t b,
                int32_t c, int32_t d, int32_t e, int32_t f, int32_t *routes,
                Array<int32_t> *stations, Array<int32_t> *wrecks, Array<int32_t> *statics);
    Array<int32_t> *getStations();
};

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

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void *operator new[](__SIZE_TYPE__ size);
void operator delete[](void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

struct FileRead {
    int32_t loadStation(int32_t id);
    Array<Station *> *loadStationsBinary(int16_t *ids, int32_t count);
    Array<int32_t> *loadStationCollision(int32_t id);
    Array<int32_t> *loadWreckCollision(int32_t id);
    Array<int32_t> *loadStaticCollision(int32_t id);
    Array<Ship *> *loadShipsBinary();
    Array<NewsItem *> *loadTicker();
    Array<Station *> *loadStationsBinary(SolarSystem *system);
    Array<int32_t> *loadShipParts(int32_t id);
    Array<int32_t> *loadStationParts(int32_t id, int32_t special);
    Array<Item *> *loadItemsBinary();
    Array<Agent *> *loadAgents();
    Array<Array<Vector *> *> *loadWeaponPositions(int32_t id);
    Array<Wanted *> *loadWanted();
    Array<SpacePoint *> *loadSpacePoints(int32_t id, int32_t group);
    Array<SolarSystem *> *loadSystemsBinary();
    Array<String *> *loadNamesBinary(int32_t type, bool first, bool second);
    int32_t loadStationsBinary();
};

#endif
