#include "class.h"

typedef void LoadingShowCallback(PaintCanvas *, int, void *);

extern "C" void ApplicationManager_LoadingCallbackShow(ApplicationManager *self, int mode, void *data)
{
    LoadingShowCallback *callback = *(LoadingShowCallback **)((char *)self + 0x20);
    if (callback != 0) {
        callback(*(PaintCanvas **)self, mode, data);
    }
}
