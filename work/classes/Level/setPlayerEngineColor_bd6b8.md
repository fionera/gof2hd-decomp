# Level::setPlayerEngineColor
elf 0xbd6b8 body 54
Sig: undefined __thiscall setPlayerEngineColor(Level * this, short param_1)

## decompile
```c

/* Level::setPlayerEngineColor(short) */

void __thiscall Level::setPlayerEngineColor(Level *this,short param_1)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  
  iVar2 = (int)param_1;
  if ((*(int *)(this + 0xf0) != 0) && (*(int **)(this + 0xa4) != (int *)0x0)) {
    puVar3 = (uint *)(*(int *)(DAT_000cd6f0 + 0xcd6d0) + 0x1254);
    for (iVar1 = **(int **)(this + 0xa4); iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = iVar2 << 0x10 | iVar2 << 0x18 | iVar2 << 8 | 0xff;
      puVar3 = puVar3 + 0x28;
    }
  }
  return;
}

```
## target disasm
```
  000cd6b8: ldr.w r2,[r0,#0xf0]
  000cd6bc: cbz r2,0x000cd6ec
  000cd6be: ldr.w r0,[r0,#0xa4]
  000cd6c2: cbz r0,0x000cd6ec
  000cd6c4: ldr r2,[0x000cd6f0]
  000cd6c6: movw r3,#0x1254
  000cd6ca: ldr r0,[r0,#0x0]
  000cd6cc: add r2,pc
  000cd6ce: ldr r2,[r2,#0x0]
  000cd6d0: add r2,r3
  000cd6d2: lsls r3,r1,#0x10
  000cd6d4: orr.w r3,r3,r1, lsl #0x18
  000cd6d8: orr.w r1,r3,r1, lsl #0x8
  000cd6dc: orr r1,r1,#0xff
  000cd6e0: b 0x000cd6e8
  000cd6e2: str.w r1,[r2],#0xa0
  000cd6e6: subs r0,#0x1
  000cd6e8: cmp r0,#0x0
  000cd6ea: bne 0x000cd6e2
  000cd6ec: bx lr
```
