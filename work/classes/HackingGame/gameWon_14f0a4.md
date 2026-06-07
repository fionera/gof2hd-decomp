# HackingGame::gameWon
elf 0x14f0a4 body 34
Sig: undefined __thiscall gameWon(HackingGame * this, int * param_1)

## decompile
```c

/* HackingGame::gameWon(int*) */

undefined4 __thiscall HackingGame::gameWon(HackingGame *this,int *param_1)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  
  uVar3 = 0;
  do {
    if (5 < uVar3) {
      return 1;
    }
    iVar1 = uVar3 * 4;
    piVar2 = param_1 + uVar3;
    uVar3 = uVar3 + 1;
  } while (*piVar2 == *(int *)(this + iVar1 + 4));
  return 0;
}

```

## target disasm
```
  0015f0a4: add.w r12,r0,#0x4
  0015f0a8: movs r2,#0x0
  0015f0aa: cmp r2,#0x5
  0015f0ac: itt hi
  0015f0ae: mov.hi r0,#0x1
  0015f0b0: bx.hi lr
  0015f0b2: ldr.w r3,[r12,r2,lsl #0x2]
  0015f0b6: ldr.w r0,[r1,r2,lsl #0x2]
  0015f0ba: adds r2,#0x1
  0015f0bc: cmp r0,r3
  0015f0be: itt ne
  0015f0c0: mov.ne r0,#0x0
  0015f0c2: bx.ne lr
  0015f0c4: b 0x0015f0aa
```
