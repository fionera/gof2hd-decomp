# PlayerTurret::~PlayerTurret
elf 0x157340 body 102
Sig: undefined __thiscall ~PlayerTurret(PlayerTurret * this)

## decompile
```c

/* PlayerTurret::~PlayerTurret() */

void __thiscall PlayerTurret::~PlayerTurret(PlayerTurret *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_001673a8 + 0x16734c) + 8;
  if (*(Explosion **)(this + 0x13c) != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion(*(Explosion **)(this + 0x13c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x13c) = 0;
  if (*(AEGeometry **)(this + 0x140) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x140));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x140) = 0;
  if (*(AEGeometry **)(this + 0x144) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x144));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x144) = 0;
  if (*(AEGeometry **)(this + 0x148) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x148));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x148) = 0;
  (*(code *)(DAT_001ac1a4 + 0x1ac1a8))(this);
  return;
}

```

## target disasm
```
  00167340: push {r4,r5,r7,lr}
  00167342: add r7,sp,#0x8
  00167344: mov r4,r0
  00167346: ldr r0,[0x001673a8]
  00167348: add r0,pc
  0016734a: ldr r1,[r0,#0x0]
  0016734c: ldr.w r0,[r4,#0x13c]
  00167350: adds r1,#0x8
  00167352: str r1,[r4,#0x0]
  00167354: cbz r0,0x0016735e
  00167356: blx 0x00072334
  0016735a: blx 0x0006eb48
  0016735e: ldr.w r0,[r4,#0x140]
  00167362: movs r5,#0x0
  00167364: str.w r5,[r4,#0x13c]
  00167368: cbz r0,0x00167372
  0016736a: blx 0x00071fc8
  0016736e: blx 0x0006eb48
  00167372: ldr.w r0,[r4,#0x144]
  00167376: str.w r5,[r4,#0x140]
  0016737a: cbz r0,0x00167384
  0016737c: blx 0x00071fc8
  00167380: blx 0x0006eb48
  00167384: ldr.w r0,[r4,#0x148]
  00167388: movs r5,#0x0
  0016738a: str.w r5,[r4,#0x144]
  0016738e: cbz r0,0x00167398
  00167390: blx 0x00071fc8
  00167394: blx 0x0006eb48
  00167398: str.w r5,[r4,#0x148]
  0016739c: mov r0,r4
  0016739e: pop.w {r4,r5,r7,lr}
  001673a2: b.w 0x001ac198
```
