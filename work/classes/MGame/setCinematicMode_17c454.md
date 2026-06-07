# MGame::setCinematicMode
elf 0x17c454 body 142
Sig: undefined __stdcall setCinematicMode(bool param_1)

## decompile
```c

/* MGame::setCinematicMode(bool) */

void MGame::setCinematicMode(bool param_1)

{
  int iVar1;
  undefined1 uVar2;
  uint uVar3;
  undefined1 *puVar4;
  int in_r1;
  
  iVar1 = DAT_0018c4e0;
  uVar3 = (uint)param_1;
  *(char *)(uVar3 + 0x15e) = (char)in_r1;
  **(undefined1 **)(iVar1 + 0x18c466) = (char)in_r1;
  puVar4 = *(undefined1 **)(DAT_0018c4e4 + 0x18c46e);
  if (in_r1 == 0) {
    *puVar4 = *(undefined1 *)(uVar3 + 0x1dd);
    switchCamera(uVar3);
    (*(code *)(DAT_001ac864 + 0x1ac868))
              (*(undefined4 *)(uVar3 + 0xf4),*(undefined1 *)(uVar3 + 0x164));
    return;
  }
  *(undefined1 *)(uVar3 + 0x1dd) = *puVar4;
  *puVar4 = 1;
  if ((*(char *)(uVar3 + 0xdc) == '\0') && (*(char *)(uVar3 + 0x5f) == '\0')) {
    *(undefined4 *)(uVar3 + 0x160) = *(undefined4 *)(uVar3 + 0x14);
    uVar2 = TargetFollowCamera::isInLookAtMode(*(TargetFollowCamera **)(uVar3 + 0xf4));
    *(undefined1 *)(uVar3 + 0x164) = uVar2;
    TargetFollowCamera::setLookAtCam(SUB41(*(undefined4 *)(uVar3 + 0xf4),0));
    switchCamera(uVar3);
    (*(code *)(DAT_001ab8e4 + 0x1ab8e8))
              (**(undefined4 **)(uVar3 + 0x78),*(undefined4 *)(uVar3 + 0x40),1);
    return;
  }
  return;
}

```

## target disasm
```
  0018c454: push {r4,r6,r7,lr}
  0018c456: add r7,sp,#0x8
  0018c458: mov r4,r0
  0018c45a: ldr r0,[0x0018c4e0]
  0018c45c: strb.w r1,[r4,#0x15e]
  0018c460: cmp r1,#0x0
  0018c462: add r0,pc
  0018c464: ldr r0,[r0,#0x0]
  0018c466: strb r1,[r0,#0x0]
  0018c468: ldr r0,[0x0018c4e4]
  0018c46a: add r0,pc
  0018c46c: ldr r0,[r0,#0x0]
  0018c46e: beq 0x0018c488
  0018c470: ldrb r1,[r0,#0x0]
  0018c472: strb.w r1,[r4,#0x1dd]
  0018c476: movs r1,#0x1
  0018c478: strb r1,[r0,#0x0]
  0018c47a: ldrb.w r0,[r4,#0xdc]
  0018c47e: cbnz r0,0x0018c486
  0018c480: ldrb.w r0,[r4,#0x5f]
  0018c484: cbz r0,0x0018c4a8
  0018c486: pop {r4,r6,r7,pc}
  0018c488: ldrb.w r1,[r4,#0x1dd]
  0018c48c: strb r1,[r0,#0x0]
  0018c48e: mov r0,r4
  0018c490: ldr.w r1,[r4,#0x160]
  0018c494: blx 0x00078388
  0018c498: ldrb.w r1,[r4,#0x164]
  0018c49c: ldr.w r0,[r4,#0xf4]
  0018c4a0: pop.w {r4,r6,r7,lr}
  0018c4a4: b.w 0x001ac858
  0018c4a8: ldr.w r0,[r4,#0xf4]
  0018c4ac: ldr r1,[r4,#0x14]
  0018c4ae: str.w r1,[r4,#0x160]
  0018c4b2: blx 0x00078454
  0018c4b6: ldr.w r1,[r4,#0xf4]
  0018c4ba: strb.w r0,[r4,#0x164]
  0018c4be: mov r0,r1
  0018c4c0: movs r1,#0x0
  0018c4c2: blx 0x00072670
  0018c4c6: mov r0,r4
  0018c4c8: movs r1,#0x3
  0018c4ca: blx 0x00078388
  0018c4ce: ldr r0,[r4,#0x78]
  0018c4d0: movs r2,#0x1
  0018c4d2: ldr r1,[r4,#0x40]
  0018c4d4: ldr r0,[r0,#0x0]
  0018c4d6: pop.w {r4,r6,r7,lr}
  0018c4da: b.w 0x001ab8d8
  001ab8d8: bx pc
  001ac858: bx pc
```
