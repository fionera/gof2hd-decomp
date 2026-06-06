# Player::~Player
elf 0xa2864 body 124
Sig: undefined __thiscall ~Player(Player * this)

## decompile
```c

/* Player::~Player() */

Player * __thiscall Player::~Player(Player *this)

{
  Array *pAVar1;
  Array<Gun*> *this_00;
  undefined4 *puVar2;
  void *pvVar3;
  uint uVar4;
  int iVar5;
  
  pAVar1 = *(Array **)this;
  if (pAVar1 != (Array *)0x0) {
    iVar5 = 0;
    for (uVar4 = 0; uVar4 < *(uint *)pAVar1; uVar4 = uVar4 + 1) {
      if (*(Array **)(*(int *)(pAVar1 + 4) + iVar5) != (Array *)0x0) {
        ArrayReleaseClasses<Gun*>(*(Array **)(*(int *)(pAVar1 + 4) + iVar5));
        this_00 = *(Array<Gun*> **)(*(int *)(*(int *)this + 4) + iVar5);
        if (this_00 == (Array<Gun*> *)0x0) {
          puVar2 = (undefined4 *)(*(int *)(*(int *)this + 4) + uVar4 * 4);
        }
        else {
          pvVar3 = (void *)Array<Gun*>::~Array(this_00);
          operator_delete(pvVar3);
          puVar2 = (undefined4 *)(*(int *)(*(int *)this + 4) + iVar5);
        }
        *puVar2 = 0;
        pAVar1 = *(Array **)this;
      }
      iVar5 = iVar5 + 4;
    }
    ArrayReleaseClasses<Array<Gun*>*>(pAVar1);
    if (*(Array<Array<Gun*>*> **)this != (Array<Array<Gun*>*> *)0x0) {
      pvVar3 = (void *)Array<Array<Gun*>*>::~Array(*(Array<Array<Gun*>*> **)this);
      operator_delete(pvVar3);
    }
    *(undefined4 *)this = 0;
  }
  if (*(Array<Player*> **)(this + 0x74) != (Array<Player*> *)0x0) {
    pvVar3 = (void *)Array<Player*>::~Array(*(Array<Player*> **)(this + 0x74));
    operator_delete(pvVar3);
  }
  *(undefined4 *)(this + 0x74) = 0;
  return this;
}

```
## target disasm
```
  000b2864: push {r4,r5,r6,r7,lr}
  000b2866: add r7,sp,#0xc
  000b2868: push.w r8
  000b286c: mov r4,r0
  000b286e: ldr r0,[r0,#0x0]
  000b2870: cbz r0,0x000b28c8
  000b2872: mov.w r8,#0x0
  000b2876: movs r6,#0x0
  000b2878: movs r5,#0x0
  000b287a: b 0x000b28ae
  000b287c: ldr r1,[r0,#0x4]
  000b287e: ldr r1,[r1,r6]
  000b2880: cbz r1,0x000b28aa
  000b2882: mov r0,r1
  000b2884: blx 0x00072e2c
  000b2888: ldr r0,[r4,#0x0]
  000b288a: ldr r1,[r0,#0x4]
  000b288c: ldr r0,[r1,r6]
  000b288e: cbz r0,0x000b28a0
  000b2890: blx 0x00072e38
  000b2894: blx 0x0006eb48
  000b2898: ldr r0,[r4,#0x0]
  000b289a: ldr r0,[r0,#0x4]
  000b289c: add r0,r6
  000b289e: b 0x000b28a4
  000b28a0: add.w r0,r1,r5, lsl #0x2
  000b28a4: str.w r8,[r0,#0x0]
  000b28a8: ldr r0,[r4,#0x0]
  000b28aa: adds r6,#0x4
  000b28ac: adds r5,#0x1
  000b28ae: ldr r1,[r0,#0x0]
  000b28b0: cmp r5,r1
  000b28b2: bcc 0x000b287c
  000b28b4: blx 0x00072e44
  000b28b8: ldr r0,[r4,#0x0]
  000b28ba: cbz r0,0x000b28c4
  000b28bc: blx 0x00072e50
  000b28c0: blx 0x0006eb48
  000b28c4: movs r0,#0x0
  000b28c6: str r0,[r4,#0x0]
  000b28c8: ldr r0,[r4,#0x74]
  000b28ca: cbz r0,0x000b28d4
  000b28cc: blx 0x00072e5c
  000b28d0: blx 0x0006eb48
  000b28d4: movs r0,#0x0
  000b28d6: str r0,[r4,#0x74]
  000b28d8: mov r0,r4
  000b28da: pop.w r8
  000b28de: pop {r4,r5,r6,r7,pc}
```
