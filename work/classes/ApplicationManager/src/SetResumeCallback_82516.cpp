#include "class.h"

typedef bool ResumeCallback(PaintCanvas *, void *);

extern "C" void ApplicationManager_SetResumeCallback(ApplicationManager *self, ResumeCallback *callback, void *data)
{
    *(ResumeCallback **)((char *)self + 0x28) = callback;
    *(void **)((char *)self + 0x2c) = data;
}
