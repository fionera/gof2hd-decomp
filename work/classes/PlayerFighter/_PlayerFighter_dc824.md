# PlayerFighter::~PlayerFighter
elf 0xdc824 body 132
Sig: undefined __thiscall ~PlayerFighter(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::~PlayerFighter() */

void __thiscall PlayerFighter::~PlayerFighter(PlayerFighter *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_000ec8b8 + 0xec830) + 8;
  if (*(Route **)(this + 0x144) != (Route *)0x0) {
    pvVar1 = (void *)Route::~Route(*(Route **)(this + 0x144));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x144) = 0;
  if (*(Array **)(this + 0x150) != (Array *)0x0) {
    ArrayReleaseClasses<BoundingVolume*>(*(Array **)(this + 0x150));
    if (*(Array<BoundingVolume*> **)(this + 0x150) != (Array<BoundingVolume*> *)0x0) {
      pvVar1 = (void *)Array<BoundingVolume*>::~Array(*(Array<BoundingVolume*> **)(this + 0x150));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x150) = 0;
  if (*(Trail **)(this + 0x154) != (Trail *)0x0) {
    pvVar1 = (void *)Trail::~Trail(*(Trail **)(this + 0x154));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x154) = 0;
  if (*(Explosion **)(this + 0x124) != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion(*(Explosion **)(this + 0x124));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x124) = 0;
  if (*(EaseInOutMatrix **)(this + 0x2b8) != (EaseInOutMatrix *)0x0) {
    pvVar1 = (void *)AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix
                               (*(EaseInOutMatrix **)(this + 0x2b8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x2b8) = 0;
  (*(code *)(DAT_001ac1a4 + 0x1ac1a8))(this);
  return;
}

```

## target disasm
```
  000ec824: push {r4,r5,r7,lr}
  000ec826: add r7,sp,#0x8
  000ec828: mov r4,r0
  000ec82a: ldr r0,[0x000ec8b8]
  000ec82c: add r0,pc
  000ec82e: ldr r1,[r0,#0x0]
  000ec830: ldr.w r0,[r4,#0x144]
  000ec834: adds r1,#0x8
  000ec836: str r1,[r4,#0x0]
  000ec838: cbz r0,0x000ec842
  000ec83a: blx 0x00072310
  000ec83e: blx 0x0006eb48
  000ec842: ldr.w r0,[r4,#0x150]
  000ec846: movs r5,#0x0
  000ec848: str.w r5,[r4,#0x144]
  000ec84c: cbz r0,0x000ec860
  000ec84e: blx 0x000757a8
  000ec852: ldr.w r0,[r4,#0x150]
  000ec856: cbz r0,0x000ec860
  000ec858: blx 0x000757b4
  000ec85c: blx 0x0006eb48
  000ec860: ldr.w r0,[r4,#0x154]
  000ec864: str.w r5,[r4,#0x150]
  000ec868: cbz r0,0x000ec872
  000ec86a: blx 0x000757c0
  000ec86e: blx 0x0006eb48
  000ec872: ldr.w r0,[r4,#0x124]
  000ec876: movs r5,#0x0
  000ec878: str.w r5,[r4,#0x154]
  000ec87c: cbz r0,0x000ec886
  000ec87e: blx 0x00072334
  000ec882: blx 0x0006eb48
  000ec886: ldr.w r0,[r4,#0x2b8]
  000ec88a: str.w r5,[r4,#0x124]
  000ec88e: cbz r0,0x000ec898
  000ec890: blx 0x00072340
  000ec894: blx 0x0006eb48
  000ec898: movs r0,#0x0
  000ec89a: str.w r0,[r4,#0x2b8]
  000ec89e: mov r0,r4
  000ec8a0: pop.w {r4,r5,r7,lr}
  000ec8a4: b.w 0x001ac198
```
