// SolarSystem::~SolarSystem() — real C++ destructor so the demangled symbol contains "~SolarSystem".
// Releases the three Array<int> members at +0x38, +0x3c, +0x40, then the String at +0xc.

typedef unsigned int uint32_t;

extern "C" void ArrayRelease_int(void *a) __attribute__((nothrow));
extern "C" void *Array_int_dtor(void *a) __attribute__((nothrow));
extern "C" void SolarSystem_operator_delete(void *p) __attribute__((nothrow));
// AbyssEngine::String::~String(String*) on the member at +0xc.
extern "C" void SolarSystem_baseStringDtor(void *strField) __attribute__((nothrow));

template <class T>
struct Array { uint32_t length; T *data; uint32_t cap; };

struct SolarSystem {
    unsigned char pad_0[0x38];
    Array<int> *stations;   // 0x38
    Array<int> *routes;     // 0x3c
    Array<int> *systems;    // 0x40
    char tail[0x4];         // 0x44 (visible flag etc.)
    ~SolarSystem() noexcept(false);
};

__attribute__((minsize)) SolarSystem::~SolarSystem() noexcept(false)
{
    if (this->stations != 0) {
        ArrayRelease_int(this->stations);
        if (this->stations != 0)
            SolarSystem_operator_delete(Array_int_dtor(this->stations));
    }
    this->stations = 0;
    if (this->routes != 0) {
        ArrayRelease_int(this->routes);
        if (this->routes != 0)
            SolarSystem_operator_delete(Array_int_dtor(this->routes));
    }
    this->routes = 0;
    if (this->systems != 0) {
        ArrayRelease_int(this->systems);
        if (this->systems != 0)
            SolarSystem_operator_delete(Array_int_dtor(this->systems));
    }
    this->systems = 0;
    SolarSystem_baseStringDtor((char *)this + 0xc);
}
