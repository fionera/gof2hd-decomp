#include "class.h"

typedef void LoadingCallback(PaintCanvas *, int, void *);

extern "C" void ApplicationManager_SetLoadingCallback(ApplicationManager *self, LoadingCallback *callback, void *data)
{
    *(LoadingCallback **)((char *)self + 0x20) = callback;
    *(void **)((char *)self + 0x24) = data;
}
