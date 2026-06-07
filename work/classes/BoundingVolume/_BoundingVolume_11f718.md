# BoundingVolume::~BoundingVolume
elf 0x11f718 body 44
Sig: undefined __thiscall ~BoundingVolume(BoundingVolume * this)

## decompile
```c

/* BoundingVolume::~BoundingVolume() */

BoundingVolume * __thiscall BoundingVolume::~BoundingVolume(BoundingVolume *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_0012f748 + 0x12f724) + 8;
  if (*(Array **)(this + 4) != (Array *)0x0) {
    ArrayReleaseClasses<BoundingVolume*>(*(Array **)(this + 4));
    if (*(Array<BoundingVolume*> **)(this + 4) != (Array<BoundingVolume*> *)0x0) {
      pvVar1 = (void *)Array<BoundingVolume*>::~Array(*(Array<BoundingVolume*> **)(this + 4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  0012f718: push {r4,r6,r7,lr}
  0012f71a: add r7,sp,#0x8
  0012f71c: mov r4,r0
  0012f71e: ldr r0,[0x0012f748]
  0012f720: add r0,pc
  0012f722: ldr r1,[r0,#0x0]
  0012f724: ldr r0,[r4,#0x4]
  0012f726: adds r1,#0x8
  0012f728: str r1,[r4,#0x0]
  0012f72a: cbz r0,0x0012f73c
  0012f72c: blx 0x000757a8
  0012f730: ldr r0,[r4,#0x4]
  0012f732: cbz r0,0x0012f73c
  0012f734: blx 0x000757b4
  0012f738: blx 0x0006eb48
  0012f73c: movs r0,#0x0
  0012f73e: str r0,[r4,#0x4]
  0012f740: mov r0,r4
  0012f742: pop {r4,r6,r7,pc}
```
