# PlayerStation::~PlayerStation
elf 0x122588 body 104
Sig: undefined __thiscall ~PlayerStation(PlayerStation * this)

## decompile
```c

/* PlayerStation::~PlayerStation() */

void __thiscall PlayerStation::~PlayerStation(PlayerStation *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_001325e8 + 0x132594) + 8;
  if (*(AEGeometry **)(this + 0x140) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x140));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x140) = 0;
  if (*(AEGeometry **)(this + 0x14c) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x14c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x14c) = 0;
  if (*(Array **)(this + 0x130) != (Array *)0x0) {
    ArrayReleaseClasses<BoundingVolume*>(*(Array **)(this + 0x130));
    if (*(Array<BoundingVolume*> **)(this + 0x130) != (Array<BoundingVolume*> *)0x0) {
      pvVar1 = (void *)Array<BoundingVolume*>::~Array(*(Array<BoundingVolume*> **)(this + 0x130));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x130) = 0;
  (*(code *)(DAT_001abdc4 + 0x1abdc8))(this);
  return;
}

```

## target disasm
```
  00132588: push {r4,r5,r7,lr}
  0013258a: add r7,sp,#0x8
  0013258c: mov r4,r0
  0013258e: ldr r0,[0x001325e8]
  00132590: add r0,pc
  00132592: ldr r1,[r0,#0x0]
  00132594: ldr.w r0,[r4,#0x140]
  00132598: adds r1,#0x8
  0013259a: str r1,[r4,#0x0]
  0013259c: cbz r0,0x001325a6
  0013259e: blx 0x00071fc8
  001325a2: blx 0x0006eb48
  001325a6: ldr.w r0,[r4,#0x14c]
  001325aa: movs r5,#0x0
  001325ac: str.w r5,[r4,#0x140]
  001325b0: cbz r0,0x001325ba
  001325b2: blx 0x00071fc8
  001325b6: blx 0x0006eb48
  001325ba: ldr.w r0,[r4,#0x130]
  001325be: str.w r5,[r4,#0x14c]
  001325c2: cbz r0,0x001325d6
  001325c4: blx 0x000757a8
  001325c8: ldr.w r0,[r4,#0x130]
  001325cc: cbz r0,0x001325d6
  001325ce: blx 0x000757b4
  001325d2: blx 0x0006eb48
  001325d6: movs r0,#0x0
  001325d8: str.w r0,[r4,#0x130]
  001325dc: mov r0,r4
  001325de: pop.w {r4,r5,r7,lr}
  001325e2: b.w 0x001abdb8
  001abdb8: bx pc
  001abdbc: ldr r12,[0x1abdc4]
  001abdc0: add pc,r12,pc
```
