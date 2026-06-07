#ifndef WORK_CLASSES_CONFIGREADER_SRC_CLASS_H
#define WORK_CLASSES_CONFIGREADER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;
inline void *operator new(uint32_t, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

namespace AbyssEngine {

struct Engine;
struct ConfigReader;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    String(const uint16_t *text, bool copy);
    String(const String &other, bool copy);
    ~String();

    String &operator=(const String &other);
    String &operator+=(const char &value);
    uint16_t *operator[](int32_t index);

    int32_t Compare(const char *text) const;
    int32_t Compare(const String &other) const;
    int32_t IndexOf(const String &needle) const;
    String SubString(uint32_t begin, uint32_t end) const;
    void Set(const char *text);
    void Trim();
};

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

template <class T>
void ArrayAdd(T item, Array<T> &array);

struct AEFile {
    static uint32_t OpenRead(String &path, uint32_t *handle);
    static uint32_t Read(char &value, uint32_t handle);
    static void Close(uint32_t handle);
};

typedef void (*ConfigTokenReadFunction)(ConfigReader *, void *);

struct TokenStruct {
    String name;
    ConfigTokenReadFunction read;
    void *context;
};

struct ConfigReader {
    ConfigReader(Engine *engine);
    ~ConfigReader();

    void RegisterTokenReadFunction(String name, ConfigTokenReadFunction read, void *context);
    void ParseFile(String name);
    String GetNewLine();
};

static inline uint32_t &config_count(ConfigReader *self)
{
    return *(uint32_t *)((char *)self + 0x0);
}

static inline TokenStruct **&config_tokens(ConfigReader *self)
{
    return *(TokenStruct ***)((char *)self + 0x4);
}

static inline Engine *&config_engine(ConfigReader *self)
{
    return *(Engine **)((char *)self + 0xc);
}

static inline uint32_t &config_file_handle(ConfigReader *self)
{
    return *(uint32_t *)((char *)self + 0x10);
}

} // namespace AbyssEngine

#endif
