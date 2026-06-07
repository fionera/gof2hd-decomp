#include "class.h"

__attribute__((visibility("hidden"))) extern void *IParticleSystem_vtable;

extern "C" void Array_ParticleSet_dtor(void *self);
extern "C" void AERandom_dtor(void *self);

IParticleSystem::~IParticleSystem()
{
    volatile char *random = (char *)this;
    *(void * volatile *)random = (char *)IParticleSystem_vtable + 8;
    random += 0x10;
    Array_ParticleSet_dtor((char *)(random + 0x28));
    AERandom_dtor((void *)random);
}
