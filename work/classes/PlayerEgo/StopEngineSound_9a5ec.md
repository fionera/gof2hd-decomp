# PlayerEgo::StopEngineSound
elf 0x9a5ec body 142
Sig: undefined __stdcall StopEngineSound(void)

## decompile
```c

/* PlayerEgo::StopEngineSound() */

void PlayerEgo::StopEngineSound(void)

{
  undefined4 *in_r0;
  int iVar1;
  Station *this;
  float fVar2;
  int iVar3;
  int *piVar4;
  
  if ((*(char *)((int)in_r0 + 0x356) == '\0') || (in_r0[0x71] != 1)) {
    piVar4 = *(int **)(DAT_000aa67c + 0xaa60c);
    iVar1 = Status::getShip();
    iVar1 = Ship::getFirstEquipmentOfSort(iVar1);
    if ((iVar1 != 0) && (iVar1 = Status::inAlienOrbit(), iVar1 == 0)) {
      iVar3 = *piVar4;
      this = (Station *)Status::getStation();
      iVar1 = Station::getIndex(this);
      Status::getCurrentCampaignMission();
      fVar2 = (float)Status::getGammaRayDamagePerSecond(iVar3,iVar1);
      if ((0.0 < fVar2) && ((Vector *)in_r0[0x2d] != (Vector *)0xffffffff)) {
        FModSound::play(**(int **)(DAT_000aa680 + 0xaa660),(Vector *)in_r0[0x2d],(Vector *)0x0,fVar2
                       );
      }
    }
  }
  (*(code *)(DAT_001aba44 + 0x1aba48))(*in_r0);
  return;
}

```

## target disasm
```
  000aa5ec: push {r4,r5,r6,r7,lr}
  000aa5ee: add r7,sp,#0xc
  000aa5f0: push.w r8
  000aa5f4: sub sp,#0x8
  000aa5f6: mov r4,r0
  000aa5f8: ldrb.w r0,[r0,#0x356]
  000aa5fc: cbz r0,0x000aa606
  000aa5fe: ldr.w r0,[r4,#0x1c4]
  000aa602: cmp r0,#0x1
  000aa604: beq 0x000aa66a
  000aa606: ldr r0,[0x000aa67c]
  000aa608: add r0,pc
  000aa60a: ldr r6,[r0,#0x0]
  000aa60c: ldr r0,[r6,#0x0]
  000aa60e: blx 0x00071a58
  000aa612: movs r1,#0x26
  000aa614: blx 0x0007228c
  000aa618: cbz r0,0x000aa66a
  000aa61a: ldr r0,[r6,#0x0]
  000aa61c: blx 0x000723d0
  000aa620: cbnz r0,0x000aa66a
  000aa622: ldr r5,[r6,#0x0]
  000aa624: mov r0,r5
  000aa626: blx 0x00071c14
  000aa62a: blx 0x00071824
  000aa62e: mov r8,r0
  000aa630: ldr r0,[r6,#0x0]
  000aa632: blx 0x00071770
  000aa636: mov r2,r0
  000aa638: mov r0,r5
  000aa63a: mov r1,r8
  000aa63c: blx 0x000723dc
  000aa640: vmov s0,r0
  000aa644: vcmpe.f32 s0,#0
  000aa648: vmrs apsr,fpscr
  000aa64c: ble 0x000aa66a
  000aa64e: ldr.w r1,[r4,#0xb4]
  000aa652: adds r0,r1,#0x1
  000aa654: beq 0x000aa66a
  000aa656: ldr r0,[0x000aa680]
  000aa658: movs r2,#0x0
  000aa65a: movs r3,#0x0
  000aa65c: add r0,pc
  000aa65e: ldr r0,[r0,#0x0]
  000aa660: ldr r0,[r0,#0x0]
  000aa662: str r2,[sp,#0x0]
  000aa664: movs r2,#0x0
  000aa666: blx 0x00071548
  000aa66a: ldr r0,[r4,#0x0]
  000aa66c: add sp,#0x8
  000aa66e: pop.w r8
  000aa672: pop.w {r4,r5,r6,r7,lr}
  000aa676: b.w 0x001aba38
```
