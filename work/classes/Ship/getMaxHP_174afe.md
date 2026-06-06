# Ship::getMaxHP
elf 0x174afe body 58
Sig: undefined __thiscall getMaxHP(Ship * this)

## decompile
```c

/* Ship::getMaxHP() */

int __thiscall Ship::getMaxHP(Ship *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(this + 0x78);
  if (piVar4 == (int *)0x0) {
    iVar2 = 0;
  }
  else {
    iVar2 = 0;
    iVar3 = 0;
    while (*piVar4 != iVar3) {
      iVar1 = iVar3 * 4;
      iVar3 = iVar3 + 1;
      if (*(int *)(piVar4[1] + iVar1) == 0) {
        iVar2 = iVar2 + 0x28;
      }
    }
  }
  return *(int *)(this + 4) + iVar2;
}

```
## target disasm
```
  00184afe: ldr.w r12,[r0,#0x78]
  00184b02: cmp.w r12,#0x0
  00184b06: beq 0x00184b30
  00184b08: push {r7,lr}
  00184b0a: mov r7,sp
  00184b0c: ldr.w lr,[r12,#0x0]
  00184b10: movs r1,#0x0
  00184b12: movs r2,#0x0
  00184b14: b 0x00184b26
  00184b16: ldr.w r3,[r12,#0x4]
  00184b1a: ldr.w r3,[r3,r2,lsl #0x2]
  00184b1e: adds r2,#0x1
  00184b20: cmp r3,#0x0
  00184b22: it eq
  00184b24: add.eq r1,#0x28
  00184b26: cmp lr,r2
  00184b28: bne 0x00184b16
  00184b2a: pop.w {r7,lr}
  00184b2e: b 0x00184b32
  00184b30: movs r1,#0x0
  00184b32: ldr r0,[r0,#0x4]
  00184b34: add r0,r1
  00184b36: bx lr
```
