# ObjectGun::~ObjectGun
elf 0x15f904 body 68
Sig: undefined __thiscall ~ObjectGun(ObjectGun * this)

## decompile
```c

/* ObjectGun::~ObjectGun() */

ObjectGun * __thiscall ObjectGun::~ObjectGun(ObjectGun *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_0016f948 + 0x16f910) + 8;
  if (*(AEGeometry **)(this + 0x18) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x18));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(Array **)(this + 0x2c) != (Array *)0x0) {
    ArrayReleaseClasses<Explosion*>(*(Array **)(this + 0x2c));
    if (*(Array<Explosion*> **)(this + 0x2c) != (Array<Explosion*> *)0x0) {
      pvVar1 = (void *)Array<Explosion*>::~Array(*(Array<Explosion*> **)(this + 0x2c));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x2c) = 0;
  }
  operator_delete__(*(void **)(this + 0x30));
  *(undefined4 *)(this + 0x30) = 0;
  return this;
}

```

## target disasm
```
  0016f904: push {r4,r5,r7,lr}
  0016f906: add r7,sp,#0x8
  0016f908: mov r4,r0
  0016f90a: ldr r0,[0x0016f948]
  0016f90c: add r0,pc
  0016f90e: ldr r1,[r0,#0x0]
  0016f910: ldr r0,[r4,#0x18]
  0016f912: adds r1,#0x8
  0016f914: str r1,[r4,#0x0]
  0016f916: cbz r0,0x0016f920
  0016f918: blx 0x00071fc8
  0016f91c: blx 0x0006eb48
  0016f920: ldr r0,[r4,#0x2c]
  0016f922: movs r5,#0x0
  0016f924: str r5,[r4,#0x18]
  0016f926: cbz r0,0x0016f93c
  0016f928: blx 0x000775cc
  0016f92c: ldr r0,[r4,#0x2c]
  0016f92e: cbz r0,0x0016f938
  0016f930: blx 0x000775d8
  0016f934: blx 0x0006eb48
  0016f938: movs r0,#0x0
  0016f93a: str r0,[r4,#0x2c]
  0016f93c: ldr r0,[r4,#0x30]
  0016f93e: blx 0x0006ebfc
  0016f942: mov r0,r4
  0016f944: str r5,[r4,#0x30]
  0016f946: pop {r4,r5,r7,pc}
```
