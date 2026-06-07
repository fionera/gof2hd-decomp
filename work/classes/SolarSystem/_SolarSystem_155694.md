# SolarSystem::~SolarSystem
elf 0x155694 body 88
Sig: undefined __thiscall ~SolarSystem(SolarSystem * this)

## decompile
```c

/* SolarSystem::~SolarSystem() */

SolarSystem * __thiscall SolarSystem::~SolarSystem(SolarSystem *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0x38) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 0x38));
    if (*(Array<int> **)(this + 0x38) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x38));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x38) = 0;
  if (*(Array **)(this + 0x3c) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 0x3c));
    if (*(Array<int> **)(this + 0x3c) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x3c));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x3c) = 0;
  if (*(Array **)(this + 0x40) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 0x40));
    if (*(Array<int> **)(this + 0x40) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x40));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x40) = 0;
  AbyssEngine::String::~String((String *)(this + 0xc));
  return this;
}

```

## target disasm
```
  00165694: push {r4,r5,r7,lr}
  00165696: add r7,sp,#0x8
  00165698: mov r4,r0
  0016569a: ldr r0,[r0,#0x38]
  0016569c: cbz r0,0x001656ae
  0016569e: blx 0x0007369c
  001656a2: ldr r0,[r4,#0x38]
  001656a4: cbz r0,0x001656ae
  001656a6: blx 0x00070204
  001656aa: blx 0x0006eb48
  001656ae: ldr r0,[r4,#0x3c]
  001656b0: movs r5,#0x0
  001656b2: str r5,[r4,#0x38]
  001656b4: cbz r0,0x001656c6
  001656b6: blx 0x0007369c
  001656ba: ldr r0,[r4,#0x3c]
  001656bc: cbz r0,0x001656c6
  001656be: blx 0x00070204
  001656c2: blx 0x0006eb48
  001656c6: ldr r0,[r4,#0x40]
  001656c8: str r5,[r4,#0x3c]
  001656ca: cbz r0,0x001656dc
  001656cc: blx 0x0007369c
  001656d0: ldr r0,[r4,#0x40]
  001656d2: cbz r0,0x001656dc
  001656d4: blx 0x00070204
  001656d8: blx 0x0006eb48
  001656dc: movs r0,#0x0
  001656de: str r0,[r4,#0x40]
  001656e0: add.w r0,r4,#0xc
  001656e4: blx 0x0006ee30
  001656e8: mov r0,r4
  001656ea: pop {r4,r5,r7,pc}
```
