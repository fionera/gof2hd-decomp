#include "class.h"

extern "C" void IApplicationModule_SetApplicationManager(void *module, ApplicationManager *manager);
extern "C" void ArrayAdd_IApplicationModule(void *module, void *array);
extern "C" void ext_001ab5b8(unsigned int id, void *array);

__attribute__((minsize)) extern "C" void ApplicationManager_RegisterApplicationModule(ApplicationManager *self, unsigned int id, void *module)
{
    if (module != 0) {
        IApplicationModule_SetApplicationManager(module, self);
        ArrayAdd_IApplicationModule(module, (char *)self + 0x44);
        ext_001ab5b8(id, (char *)self + 0x50);
    }
}
