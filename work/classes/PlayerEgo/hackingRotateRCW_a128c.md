# PlayerEgo::hackingRotateRCW
elf 0xa128c body 46
Sig: undefined __stdcall hackingRotateRCW(void)

## decompile
```c

/* PlayerEgo::hackingRotateRCW() */

void PlayerEgo::hackingRotateRCW(void)

{
  int in_r0;
  int iVar1;
  
  if (((*(HackingGame **)(in_r0 + 0x1e8) != (HackingGame *)0x0) &&
      (iVar1 = HackingGame::isRotating(*(HackingGame **)(in_r0 + 0x1e8)), iVar1 == 0)) &&
     (iVar1 = HackingGame::gameWon(*(HackingGame **)(in_r0 + 0x1e8)), iVar1 == 0)) {
    (*(code *)(DAT_001abbb4 + 0x1abbb8))(*(undefined4 *)(in_r0 + 0x1e8),1);
    return;
  }
  return;
}

```

## target disasm
```
  000b128c: push {r4,r6,r7,lr}
  000b128e: add r7,sp,#0x8
  000b1290: mov r4,r0
  000b1292: ldr.w r0,[r0,#0x1e8]
  000b1296: cbz r0,0x000b12a8
  000b1298: blx 0x00072c4c
  000b129c: cbnz r0,0x000b12a8
  000b129e: ldr.w r0,[r4,#0x1e8]
  000b12a2: blx 0x00072700
  000b12a6: cbz r0,0x000b12aa
  000b12a8: pop {r4,r6,r7,pc}
  000b12aa: ldr.w r0,[r4,#0x1e8]
  000b12ae: movs r1,#0x1
  000b12b0: pop.w {r4,r6,r7,lr}
  000b12b4: b.w 0x001abba8
  001abba8: bx pc
```
