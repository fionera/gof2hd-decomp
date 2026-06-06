# Level::pirateStationAction
elf 0xc62b0 body 106
Sig: undefined __stdcall pirateStationAction(bool param_1)

## decompile
```c

/* Level::pirateStationAction(bool) */

void Level::pirateStationAction(bool param_1)

{
  uint uVar1;
  Station *pSVar2;
  int iVar3;
  int in_r1;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  
  uVar1 = (uint)param_1;
  if (in_r1 == 0) {
    if (*(char *)(uVar1 + 0x68) == '\0') {
      piVar5 = *(int **)(DAT_000d631c + 0xd62d8);
      pSVar2 = (Station *)Status::getStation();
      iVar3 = Station::getPirateStationIndex(pSVar2);
      if (-1 < iVar3) {
        iVar6 = *(int *)(*piVar5 + 0x4c);
        pSVar2 = (Station *)Status::getStation();
        iVar3 = Station::getPirateStationIndex(pSVar2);
        *(undefined1 *)(*(int *)(iVar6 + 4) + iVar3) = 1;
        *(undefined1 *)(*piVar5 + 0xf9) = 1;
        goto LAB_000d6302;
      }
    }
  }
  else if (*(char *)(uVar1 + 0x1b0) == '\0') {
LAB_000d6302:
    uVar4 = 4;
    if (in_r1 != 0) {
      uVar4 = 3;
    }
    (*(code *)(DAT_001ac024 + 0x1ac028))(uVar1,uVar4,8);
    return;
  }
  return;
}

```
## target disasm
```
  000d62b0: push {r4,r5,r6,r7,lr}
  000d62b2: add r7,sp,#0xc
  000d62b4: push.w r8
  000d62b8: mov r5,r1
  000d62ba: mov r4,r0
  000d62bc: cbz r1,0x000d62c6
  000d62be: ldrb.w r0,[r4,#0x1b0]
  000d62c2: cbnz r0,0x000d62cc
  000d62c4: b 0x000d6302
  000d62c6: ldrb.w r0,[r4,#0x68]
  000d62ca: cbz r0,0x000d62d2
  000d62cc: pop.w r8
  000d62d0: pop {r4,r5,r6,r7,pc}
  000d62d2: ldr r0,[0x000d631c]
  000d62d4: add r0,pc
  000d62d6: ldr r6,[r0,#0x0]
  000d62d8: ldr r0,[r6,#0x0]
  000d62da: blx 0x00071c14
  000d62de: blx 0x000740b0
  000d62e2: cmp r0,#0x0
  000d62e4: blt 0x000d62cc
  000d62e6: ldr r0,[r6,#0x0]
  000d62e8: ldr.w r8,[r0,#0x4c]
  000d62ec: blx 0x00071c14
  000d62f0: blx 0x000740b0
  000d62f4: ldr.w r1,[r8,#0x4]
  000d62f8: movs r2,#0x1
  000d62fa: strb r2,[r1,r0]
  000d62fc: ldr r0,[r6,#0x0]
  000d62fe: strb.w r2,[r0,#0xf9]
  000d6302: movs r1,#0x4
  000d6304: cmp r5,#0x0
  000d6306: it ne
  000d6308: mov.ne r1,#0x3
  000d630a: mov r0,r4
  000d630c: movs r2,#0x8
  000d630e: pop.w r8
  000d6312: pop.w {r4,r5,r6,r7,lr}
  000d6316: b.w 0x001ac018
```
