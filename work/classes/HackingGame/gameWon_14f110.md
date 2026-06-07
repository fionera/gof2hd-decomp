# HackingGame::gameWon
elf 0x14f110 body 40
Sig: undefined __thiscall gameWon(HackingGame * this)

## decompile
```c

/* HackingGame::gameWon() */

undefined4 __thiscall HackingGame::gameWon(HackingGame *this)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  if (0x5dc < *(int *)(this + 0x130)) {
    uVar3 = 0;
    do {
      if (5 < uVar3) {
        return 1;
      }
      iVar1 = uVar3 * 4;
      iVar2 = uVar3 * 4;
      uVar3 = uVar3 + 1;
    } while (*(int *)(this + iVar2 + 0x1c) == *(int *)(this + iVar1 + 4));
  }
  return 0;
}

```

## target disasm
```
  0015f110: ldr.w r1,[r0,#0x130]
  0015f114: movw r2,#0x5dd
  0015f118: cmp r1,r2
  0015f11a: blt 0x0015f134
  0015f11c: movs r1,#0x0
  0015f11e: cmp r1,#0x5
  0015f120: itt hi
  0015f122: mov.hi r0,#0x1
  0015f124: bx.hi lr
  0015f126: add.w r2,r0,r1, lsl #0x2
  0015f12a: adds r1,#0x1
  0015f12c: ldr r3,[r2,#0x4]
  0015f12e: ldr r2,[r2,#0x1c]
  0015f130: cmp r2,r3
  0015f132: beq 0x0015f11e
  0015f134: movs r0,#0x0
  0015f136: bx lr
```
