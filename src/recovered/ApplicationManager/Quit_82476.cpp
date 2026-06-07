#include "class.h"

typedef void QuitCallback();

extern "C" void ApplicationManager_Quit(ApplicationManager *self)
{
    QuitCallback *callback = *(QuitCallback **)((char *)self + 0x1c);
    if (callback != 0) {
        callback();
    }
}
