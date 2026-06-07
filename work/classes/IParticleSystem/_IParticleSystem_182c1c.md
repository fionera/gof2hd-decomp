# IParticleSystem::~IParticleSystem
elf 0x182c1c body 38
Sig: undefined __thiscall ~IParticleSystem(IParticleSystem * this)

## decompile
```c

/* IParticleSystem::~IParticleSystem() */

IParticleSystem * __thiscall IParticleSystem::~IParticleSystem(IParticleSystem *this)

{
  *(int *)this = *(int *)(DAT_00192c44 + 0x192c2a) + 8;
  Array<ParticleSettings::ParticleSet>::~Array
            ((Array<ParticleSettings::ParticleSet> *)(this + 0x38));
  AbyssEngine::AERandom::~AERandom((AERandom *)(this + 0x10));
  return this;
}

```

## target disasm
```
  00192c1c: push {r4,r5,r7,lr}
  00192c1e: add r7,sp,#0x8
  00192c20: mov r4,r0
  00192c22: ldr r0,[0x00192c44]
  00192c24: mov r5,r4
  00192c26: add r0,pc
  00192c28: ldr r0,[r0,#0x0]
  00192c2a: adds r0,#0x8
  00192c2c: str.w r0,[r5],#0x10
  00192c30: add.w r0,r4,#0x38
  00192c34: blx 0x0007885c
  00192c38: mov r0,r5
  00192c3a: blx 0x00075cc4
  00192c3e: mov r0,r4
  00192c40: pop {r4,r5,r7,pc}
```
