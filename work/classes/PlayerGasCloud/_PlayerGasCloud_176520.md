# PlayerGasCloud::~PlayerGasCloud
elf 0x176520 body 192
Sig: undefined __thiscall ~PlayerGasCloud(PlayerGasCloud * this)

## decompile
```c

/* PlayerGasCloud::~PlayerGasCloud() */

void __thiscall PlayerGasCloud::~PlayerGasCloud(PlayerGasCloud *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_001865e4 + 0x18652c) + 8;
  if (*(Array **)(this + 0x138) != (Array *)0x0) {
    ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0x138));
    if (*(Array<AEGeometry*> **)(this + 0x138) != (Array<AEGeometry*> *)0x0) {
      pvVar1 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0x138));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x138) = 0;
  }
  if (*(Array **)(this + 0x13c) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(*(Array **)(this + 0x13c));
    if (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x13c) !=
        (Array<AbyssEngine::AEMath::Vector*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::AEMath::Vector*>::~Array
                                 (*(Array<AbyssEngine::AEMath::Vector*> **)(this + 0x13c));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0x13c) = 0;
  }
  if (*(Array<float> **)(this + 0x140) != (Array<float> *)0x0) {
    pvVar1 = (void *)Array<float>::~Array(*(Array<float> **)(this + 0x140));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x140) = 0;
  if (*(Array<float> **)(this + 0x144) != (Array<float> *)0x0) {
    pvVar1 = (void *)Array<float>::~Array(*(Array<float> **)(this + 0x144));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x144) = 0;
  if (*(Array<float> **)(this + 0x148) != (Array<float> *)0x0) {
    pvVar1 = (void *)Array<float>::~Array(*(Array<float> **)(this + 0x148));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x148) = 0;
  if (*(Array<int> **)(this + 0x14c) != (Array<int> *)0x0) {
    pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x14c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x14c) = 0;
  if (*(AEGeometry **)(this + 0x134) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x134));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x134) = 0;
  (*(code *)(DAT_001ac1a4 + 0x1ac1a8))(this);
  return;
}

```

## target disasm
```
  00186520: push {r4,r5,r7,lr}
  00186522: add r7,sp,#0x8
  00186524: mov r4,r0
  00186526: ldr r0,[0x001865e4]
  00186528: add r0,pc
  0018652a: ldr r1,[r0,#0x0]
  0018652c: ldr.w r0,[r4,#0x138]
  00186530: adds r1,#0x8
  00186532: str r1,[r4,#0x0]
  00186534: cbz r0,0x0018654e
  00186536: blx 0x00071fec
  0018653a: ldr.w r0,[r4,#0x138]
  0018653e: cbz r0,0x00186548
  00186540: blx 0x000717c4
  00186544: blx 0x0006eb48
  00186548: movs r0,#0x0
  0018654a: str.w r0,[r4,#0x138]
  0018654e: ldr.w r0,[r4,#0x13c]
  00186552: cbz r0,0x0018656c
  00186554: blx 0x0007216c
  00186558: ldr.w r0,[r4,#0x13c]
  0018655c: cbz r0,0x00186566
  0018655e: blx 0x00072178
  00186562: blx 0x0006eb48
  00186566: movs r0,#0x0
  00186568: str.w r0,[r4,#0x13c]
  0018656c: ldr.w r0,[r4,#0x140]
  00186570: cbz r0,0x0018657a
  00186572: blx 0x00073528
  00186576: blx 0x0006eb48
  0018657a: ldr.w r0,[r4,#0x144]
  0018657e: movs r5,#0x0
  00186580: str.w r5,[r4,#0x140]
  00186584: cbz r0,0x0018658e
  00186586: blx 0x00073528
  0018658a: blx 0x0006eb48
  0018658e: ldr.w r0,[r4,#0x148]
  00186592: str.w r5,[r4,#0x144]
  00186596: cbz r0,0x001865a0
  00186598: blx 0x00073528
  0018659c: blx 0x0006eb48
  001865a0: ldr.w r0,[r4,#0x14c]
  001865a4: movs r5,#0x0
  001865a6: str.w r5,[r4,#0x148]
  001865aa: cbz r0,0x001865b4
  001865ac: blx 0x00070204
  001865b0: blx 0x0006eb48
  001865b4: ldr.w r0,[r4,#0x134]
  001865b8: str.w r5,[r4,#0x14c]
  001865bc: cbz r0,0x001865c6
  001865be: blx 0x00071fc8
  001865c2: blx 0x0006eb48
  001865c6: movs r0,#0x0
  001865c8: str.w r0,[r4,#0x134]
  001865cc: mov r0,r4
  001865ce: pop.w {r4,r5,r7,lr}
  001865d2: b.w 0x001ac198
  001ac198: bx pc
  001ac19c: ldr r12,[0x1ac1a4]
  001ac1a0: add pc,r12,pc
```
