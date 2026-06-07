# PlayerEgo::PlayEngineSound
elf 0x9a550 body 132
Sig: undefined __stdcall PlayEngineSound(void)

## decompile
```c

/* PlayerEgo::PlayEngineSound() */

void PlayerEgo::PlayEngineSound(void)

{
  undefined4 *in_r0;
  int iVar1;
  Station *this;
  float fVar2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000aa5d4 + 0xaa562);
  iVar1 = Status::getShip();
  iVar1 = Ship::getFirstEquipmentOfSort(iVar1);
  if ((iVar1 != 0) && (iVar1 = Status::inAlienOrbit(), iVar1 == 0)) {
    iVar3 = *piVar4;
    this = (Station *)Status::getStation();
    iVar1 = Station::getIndex(this);
    Status::getCurrentCampaignMission();
    fVar2 = (float)Status::getGammaRayDamagePerSecond(iVar3,iVar1);
    if ((0.0 < fVar2) && ((Vector *)in_r0[0x2d] != (Vector *)0xffffffff)) {
      FModSound::play(**(int **)(DAT_000aa5d8 + 0xaa5b6),(Vector *)in_r0[0x2d],(Vector *)0x0,fVar2);
    }
  }
  (*(code *)(DAT_001aba14 + 0x1aba18))(*in_r0,in_r0[7],0);
  return;
}

```

## target disasm
```
  000aa550: push {r4,r5,r6,r7,lr}
  000aa552: add r7,sp,#0xc
  000aa554: push.w r8
  000aa558: sub sp,#0x8
  000aa55a: mov r4,r0
  000aa55c: ldr r0,[0x000aa5d4]
  000aa55e: add r0,pc
  000aa560: ldr r6,[r0,#0x0]
  000aa562: ldr r0,[r6,#0x0]
  000aa564: blx 0x00071a58
  000aa568: movs r1,#0x26
  000aa56a: blx 0x0007228c
  000aa56e: cbz r0,0x000aa5c0
  000aa570: ldr r0,[r6,#0x0]
  000aa572: blx 0x000723d0
  000aa576: cbnz r0,0x000aa5c0
  000aa578: ldr r5,[r6,#0x0]
  000aa57a: mov r0,r5
  000aa57c: blx 0x00071c14
  000aa580: blx 0x00071824
  000aa584: mov r8,r0
  000aa586: ldr r0,[r6,#0x0]
  000aa588: blx 0x00071770
  000aa58c: mov r2,r0
  000aa58e: mov r0,r5
  000aa590: mov r1,r8
  000aa592: blx 0x000723dc
  000aa596: vmov s0,r0
  000aa59a: vcmpe.f32 s0,#0
  000aa59e: vmrs apsr,fpscr
  000aa5a2: ble 0x000aa5c0
  000aa5a4: ldr.w r1,[r4,#0xb4]
  000aa5a8: adds r0,r1,#0x1
  000aa5aa: beq 0x000aa5c0
  000aa5ac: ldr r0,[0x000aa5d8]
  000aa5ae: movs r2,#0x0
  000aa5b0: movs r3,#0x0
  000aa5b2: add r0,pc
  000aa5b4: ldr r0,[r0,#0x0]
  000aa5b6: ldr r0,[r0,#0x0]
  000aa5b8: str r2,[sp,#0x0]
  000aa5ba: movs r2,#0x0
  000aa5bc: blx 0x00071548
  000aa5c0: ldr r0,[r4,#0x0]
  000aa5c2: movs r2,#0x0
  000aa5c4: ldr r1,[r4,#0x1c]
  000aa5c6: add sp,#0x8
  000aa5c8: pop.w r8
  000aa5cc: pop.w {r4,r5,r6,r7,lr}
  000aa5d0: b.w 0x001aba08
```
