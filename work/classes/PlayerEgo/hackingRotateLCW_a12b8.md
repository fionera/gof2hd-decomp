# PlayerEgo::hackingRotateLCW
elf 0xa12b8 body 46
Sig: undefined __stdcall hackingRotateLCW(void)

## decompile
```c

/* PlayerEgo::hackingRotateLCW() */

void PlayerEgo::hackingRotateLCW(void)

{
  int in_r0;
  int iVar1;
  
  if (((*(HackingGame **)(in_r0 + 0x1e8) != (HackingGame *)0x0) &&
      (iVar1 = HackingGame::isRotating(*(HackingGame **)(in_r0 + 0x1e8)), iVar1 == 0)) &&
     (iVar1 = HackingGame::gameWon(*(HackingGame **)(in_r0 + 0x1e8)), iVar1 == 0)) {
    (*(code *)(DAT_001abbc4 + 0x1abbc8))(*(undefined4 *)(in_r0 + 0x1e8),1);
    return;
  }
  return;
}

```

## target disasm
```
  000b12b8: push {r4,r6,r7,lr}
  000b12ba: add r7,sp,#0x8
  000b12bc: mov r4,r0
  000b12be: ldr.w r0,[r0,#0x1e8]
  000b12c2: cbz r0,0x000b12d4
  000b12c4: blx 0x00072c4c
  000b12c8: cbnz r0,0x000b12d4
  000b12ca: ldr.w r0,[r4,#0x1e8]
  000b12ce: blx 0x00072700
  000b12d2: cbz r0,0x000b12d6
  000b12d4: pop {r4,r6,r7,pc}
  000b12d6: ldr.w r0,[r4,#0x1e8]
  000b12da: movs r1,#0x1
  000b12dc: pop.w {r4,r6,r7,lr}
  000b12e0: b.w 0x001abbb8
  001abbb8: bx pc
```
