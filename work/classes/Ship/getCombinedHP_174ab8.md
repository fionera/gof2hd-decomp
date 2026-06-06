# Ship::getCombinedHP
elf 0x174ab8 body 66
Sig: undefined __thiscall getCombinedHP(Ship * this)

## decompile
```c

/* Ship::getCombinedHP() */

int __thiscall Ship::getCombinedHP(Ship *this)

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
  return *(int *)(this + 0x20) + iVar2 + *(int *)(this + 4) + *(int *)(this + 0x1c);
}

```
## target disasm
```
  00184ab8: ldr.w r12,[r0,#0x78]
  00184abc: cmp.w r12,#0x0
  00184ac0: beq 0x00184aea
  00184ac2: push {r7,lr}
  00184ac4: mov r7,sp
  00184ac6: ldr.w lr,[r12,#0x0]
  00184aca: movs r1,#0x0
  00184acc: movs r2,#0x0
  00184ace: b 0x00184ae0
  00184ad0: ldr.w r3,[r12,#0x4]
  00184ad4: ldr.w r3,[r3,r2,lsl #0x2]
  00184ad8: adds r2,#0x1
  00184ada: cmp r3,#0x0
  00184adc: it eq
  00184ade: add.eq r1,#0x28
  00184ae0: cmp lr,r2
  00184ae2: bne 0x00184ad0
  00184ae4: pop.w {r7,lr}
  00184ae8: b 0x00184aec
  00184aea: movs r1,#0x0
  00184aec: ldr r2,[r0,#0x4]
  00184aee: ldrd r3,r0,[r0,#0x1c]
  00184af2: add r1,r2
  00184af4: add r1,r3
  00184af6: add r0,r1
  00184af8: bx lr
```
