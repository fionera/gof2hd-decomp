#include "class.h"

extern "C" void ConfigReader_ParseFile(ConfigReader *reader, String *name);

extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_ConfigReadFile(ApplicationManager *self, String *name)
{
    void * volatile cookie = __stack_chk_guard;
    unsigned char storage[sizeof(String)] __attribute__((aligned(4)));
    ConfigReader *reader = *(ConfigReader **)((char *)self + 0x38);
    if (reader != 0) {
        String *copy = (String *)storage;
        new (copy) String(*name, false);
        ConfigReader_ParseFile(reader, copy);
        ((String *)storage)->~String();
    }
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
