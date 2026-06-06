# Status::wantedBoardAccessible
elf 0xace5c body 122
Sig: undefined __stdcall wantedBoardAccessible(void)

## decompile
```c

/* Status::wantedBoardAccessible() */

undefined4 Status::wantedBoardAccessible(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint *puVar4;
  int *piVar5;
  
  uVar3 = 0;
  piVar5 = *(int **)(DAT_000bced8 + 0xbce6c);
  puVar4 = *(uint **)*piVar5;
  while( true ) {
    if (*puVar4 <= uVar3) {
      return 0;
    }
    iVar1 = SolarSystem::getRace();
    iVar2 = Wanted::getBoard();
    if ((((iVar1 == iVar2) &&
         (iVar2 = *(int *)(*piVar5 + 0x1e8),
         iVar1 = Wanted::getRequiredMission(*(Wanted **)(puVar4[1] + uVar3 * 4)), iVar1 <= iVar2))
        && (iVar1 = inAlienOrbit(), iVar1 == 0)) &&
       (iVar1 = Station::getIndex(*(Station **)(*piVar5 + 0x19c)), iVar1 != 0x6c)) break;
    uVar3 = uVar3 + 1;
  }
  return 1;
}

```
## target disasm
```
  000bce5c: push {r4,r5,r6,r7,lr}
  000bce5e: add r7,sp,#0xc
  000bce60: push.w r8
  000bce64: ldr r0,[0x000bced8]
  000bce66: movs r5,#0x0
  000bce68: add r0,pc
  000bce6a: ldr.w r8,[r0,#0x0]
  000bce6e: ldr.w r0,[r8,#0x0]
  000bce72: ldr r6,[r0,#0x0]
  000bce74: b 0x000bcec4
  000bce76: ldr.w r0,[r8,#0x0]
  000bce7a: ldr.w r0,[r0,#0x1a4]
  000bce7e: blx 0x00071950
  000bce82: mov r4,r0
  000bce84: ldr r0,[r6,#0x4]
  000bce86: ldr.w r0,[r0,r5,lsl #0x2]
  000bce8a: blx 0x00073a08
  000bce8e: cmp r4,r0
  000bce90: bne 0x000bcec2
  000bce92: ldr r0,[r6,#0x4]
  000bce94: ldr.w r1,[r8,#0x0]
  000bce98: ldr.w r0,[r0,r5,lsl #0x2]
  000bce9c: ldr.w r4,[r1,#0x1e8]
  000bcea0: blx 0x00073a14
  000bcea4: cmp r4,r0
  000bcea6: blt 0x000bcec2
  000bcea8: ldr.w r0,[r8,#0x0]
  000bceac: blx 0x000723d0
  000bceb0: cbnz r0,0x000bcec2
  000bceb2: ldr.w r0,[r8,#0x0]
  000bceb6: ldr.w r0,[r0,#0x19c]
  000bceba: blx 0x00071824
  000bcebe: cmp r0,#0x6c
  000bcec0: bne 0x000bced2
  000bcec2: adds r5,#0x1
  000bcec4: ldr r0,[r6,#0x0]
  000bcec6: cmp r5,r0
  000bcec8: bcc 0x000bce76
  000bceca: movs r0,#0x0
  000bcecc: pop.w r8
  000bced0: pop {r4,r5,r6,r7,pc}
  000bced2: movs r0,#0x1
  000bced4: b 0x000bcecc
```
