# MineGun::~MineGun
elf 0x1565e4 body 88
Sig: undefined __thiscall ~MineGun(MineGun * this)

## decompile
```c

/* MineGun::~MineGun() */

void __thiscall MineGun::~MineGun(MineGun *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_0016663c + 0x1665f0) + 8;
  if (*(Array **)(this + 0xb4) != (Array *)0x0) {
    ArrayReleaseClasses<Explosion*>(*(Array **)(this + 0xb4));
    if (*(Array<Explosion*> **)(this + 0xb4) != (Array<Explosion*> *)0x0) {
      pvVar1 = (void *)Array<Explosion*>::~Array(*(Array<Explosion*> **)(this + 0xb4));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0xb4) = 0;
  }
  operator_delete__(*(void **)(this + 0xb8));
  *(undefined4 *)(this + 0xb8) = 0;
  if (*(AEGeometry **)(this + 0xbc) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0xbc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xbc) = 0;
  (*(code *)(DAT_001ac594 + 0x1ac598))(this);
  return;
}

```

## target disasm
```
  001665e4: push {r4,r5,r7,lr}
  001665e6: add r7,sp,#0x8
  001665e8: mov r4,r0
  001665ea: ldr r0,[0x0016663c]
  001665ec: add r0,pc
  001665ee: ldr r1,[r0,#0x0]
  001665f0: ldr.w r0,[r4,#0xb4]
  001665f4: adds r1,#0x8
  001665f6: str r1,[r4,#0x0]
  001665f8: cbz r0,0x00166612
  001665fa: blx 0x000775cc
  001665fe: ldr.w r0,[r4,#0xb4]
  00166602: cbz r0,0x0016660c
  00166604: blx 0x000775d8
  00166608: blx 0x0006eb48
  0016660c: movs r0,#0x0
  0016660e: str.w r0,[r4,#0xb4]
  00166612: ldr.w r0,[r4,#0xb8]
  00166616: blx 0x0006ebfc
  0016661a: ldr.w r0,[r4,#0xbc]
  0016661e: movs r5,#0x0
  00166620: str.w r5,[r4,#0xb8]
  00166624: cbz r0,0x0016662e
  00166626: blx 0x00071fc8
  0016662a: blx 0x0006eb48
  0016662e: str.w r5,[r4,#0xbc]
  00166632: mov r0,r4
  00166634: pop.w {r4,r5,r7,lr}
  00166638: b.w 0x001ac588
```
