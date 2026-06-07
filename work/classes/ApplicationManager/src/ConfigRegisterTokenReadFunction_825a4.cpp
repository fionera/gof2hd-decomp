#include "class.h"

typedef void ConfigTokenReadFunction(ConfigReader *, void *);

extern "C" void ConfigReader_RegisterTokenReadFunction(
    ConfigReader *reader, String *name, ConfigTokenReadFunction *read, void *context);

extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_ConfigRegisterTokenReadFunction(
    ApplicationManager *self, String *name, ConfigTokenReadFunction *read, void *context)
{
    void * volatile cookie = __stack_chk_guard;
    unsigned char storage[sizeof(String)] __attribute__((aligned(4)));
    ConfigReader *reader = *(ConfigReader **)((char *)self + 0x38);
    if (reader != 0) {
        String *copy = (String *)storage;
        new (copy) String(*name, false);
        ConfigReader_RegisterTokenReadFunction(reader, copy, read, context);
        ((String *)storage)->~String();
    }
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
