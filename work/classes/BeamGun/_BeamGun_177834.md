# BeamGun::~BeamGun
elf 0x177834 body 50
Sig: undefined __thiscall ~BeamGun(BeamGun * this)

## decompile
```c

/* BeamGun::~BeamGun() */

BeamGun * __thiscall BeamGun::~BeamGun(BeamGun *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_00187868 + 0x187840) + 8;
  if (*(AEGeometry **)(this + 0x18) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x18));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(AEGeometry **)(this + 0x1c) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x1c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1c) = 0;
  return this;
}

```

## target disasm
```
  00187834: push {r4,r5,r7,lr}
  00187836: add r7,sp,#0x8
  00187838: mov r4,r0
  0018783a: ldr r0,[0x00187868]
  0018783c: add r0,pc
  0018783e: ldr r1,[r0,#0x0]
  00187840: ldr r0,[r4,#0x18]
  00187842: adds r1,#0x8
  00187844: str r1,[r4,#0x0]
  00187846: cbz r0,0x00187850
  00187848: blx 0x00071fc8
  0018784c: blx 0x0006eb48
  00187850: ldr r0,[r4,#0x1c]
  00187852: movs r5,#0x0
  00187854: str r5,[r4,#0x18]
  00187856: cbz r0,0x00187860
  00187858: blx 0x00071fc8
  0018785c: blx 0x0006eb48
  00187860: mov r0,r4
  00187862: str r5,[r4,#0x1c]
  00187864: pop {r4,r5,r7,pc}
```
