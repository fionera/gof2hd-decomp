# StarSystem::~StarSystem
elf 0x134d10 body 132
Sig: undefined __thiscall ~StarSystem(StarSystem * this)

## decompile
```c

/* StarSystem::~StarSystem() */

StarSystem * __thiscall StarSystem::~StarSystem(StarSystem *this)

{
  void *pvVar1;
  
  if (*(Array<AEGeometry*> **)(this + 0x1c) != (Array<AEGeometry*> *)0x0) {
    pvVar1 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0x1c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1c) = 0;
  if (*(Array **)(this + 0x18) != (Array *)0x0) {
    ArrayReleaseClasses<KIPlayer*>(*(Array **)(this + 0x18));
    if (*(Array<KIPlayer*> **)(this + 0x18) != (Array<KIPlayer*> *)0x0) {
      pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0x18));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(LensFlare **)(this + 0x2c) != (LensFlare *)0x0) {
    pvVar1 = (void *)LensFlare::~LensFlare(*(LensFlare **)(this + 0x2c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x2c) = 0;
  if (*(Array **)(this + 0x14) != (Array *)0x0) {
    ArrayRelease<unsigned_int>(*(Array **)(this + 0x14));
    if (*(Array<unsigned_int> **)(this + 0x14) != (Array<unsigned_int> *)0x0) {
      pvVar1 = (void *)Array<unsigned_int>::~Array(*(Array<unsigned_int> **)(this + 0x14));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x14) = 0;
  if (*(Array **)(this + 0x24) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 0x24));
    if (*(Array<int> **)(this + 0x24) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x24));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x24) = 0;
  if (*(Array **)(this + 0x20) != (Array *)0x0) {
    ArrayRelease<AbyssEngine::AEMath::Vector>(*(Array **)(this + 0x20));
    if (*(Array<AbyssEngine::AEMath::Vector> **)(this + 0x20) !=
        (Array<AbyssEngine::AEMath::Vector> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::AEMath::Vector>::~Array
                                 (*(Array<AbyssEngine::AEMath::Vector> **)(this + 0x20));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x20) = 0;
  return this;
}

```

## target disasm
```
  00144d10: push {r4,r5,r7,lr}
  00144d12: add r7,sp,#0x8
  00144d14: mov r4,r0
  00144d16: ldr r0,[r0,#0x1c]
  00144d18: cbz r0,0x00144d22
  00144d1a: blx 0x000717c4
  00144d1e: blx 0x0006eb48
  00144d22: ldr r0,[r4,#0x18]
  00144d24: movs r5,#0x0
  00144d26: str r5,[r4,#0x1c]
  00144d28: cbz r0,0x00144d3a
  00144d2a: blx 0x00073b88
  00144d2e: ldr r0,[r4,#0x18]
  00144d30: cbz r0,0x00144d3a
  00144d32: blx 0x00073b94
  00144d36: blx 0x0006eb48
  00144d3a: ldr r0,[r4,#0x2c]
  00144d3c: str r5,[r4,#0x18]
  00144d3e: cbz r0,0x00144d48
  00144d40: blx 0x000769f0
  00144d44: blx 0x0006eb48
  00144d48: ldr r0,[r4,#0x14]
  00144d4a: movs r5,#0x0
  00144d4c: str r5,[r4,#0x2c]
  00144d4e: cbz r0,0x00144d60
  00144d50: blx 0x0006f97c
  00144d54: ldr r0,[r4,#0x14]
  00144d56: cbz r0,0x00144d60
  00144d58: blx 0x0006f904
  00144d5c: blx 0x0006eb48
  00144d60: ldr r0,[r4,#0x24]
  00144d62: str r5,[r4,#0x14]
  00144d64: cbz r0,0x00144d76
  00144d66: blx 0x0007369c
  00144d6a: ldr r0,[r4,#0x24]
  00144d6c: cbz r0,0x00144d76
  00144d6e: blx 0x00070204
  00144d72: blx 0x0006eb48
  00144d76: ldr r0,[r4,#0x20]
  00144d78: movs r5,#0x0
  00144d7a: str r5,[r4,#0x24]
  00144d7c: cbz r0,0x00144d8e
  00144d7e: blx 0x000769fc
  00144d82: ldr r0,[r4,#0x20]
  00144d84: cbz r0,0x00144d8e
  00144d86: blx 0x00076a08
  00144d8a: blx 0x0006eb48
  00144d8e: mov r0,r4
  00144d90: str r5,[r4,#0x20]
  00144d92: pop {r4,r5,r7,pc}
```
