# Ship::getModdedLoad
elf 0x17569c body 46
Sig: undefined __thiscall getModdedLoad(Ship * this)

## decompile
```c

/* Ship::getModdedLoad() */

int __thiscall Ship::getModdedLoad(Ship *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  iVar2 = *(int *)(this + 0xc);
  piVar4 = *(int **)(this + 0x78);
  if (piVar4 == (int *)0x0) {
    return iVar2;
  }
  iVar3 = 0;
  while (*piVar4 != iVar3) {
    iVar1 = iVar3 * 4;
    iVar3 = iVar3 + 1;
    if (*(int *)(piVar4[1] + iVar1) == 1) {
      iVar2 = iVar2 + 0x1e;
    }
  }
  return iVar2;
}

```
## target disasm
```
  0018569c: mov r1,r0
  0018569e: ldr r0,[r0,#0xc]
  001856a0: ldr.w r12,[r1,#0x78]
  001856a4: cmp.w r12,#0x0
  001856a8: beq 0x001856c8
  001856aa: ldr.w r2,[r12,#0x0]
  001856ae: movs r3,#0x0
  001856b0: cmp r2,r3
  001856b2: it eq
  001856b4: bx.eq lr
  001856b6: ldr.w r1,[r12,#0x4]
  001856ba: ldr.w r1,[r1,r3,lsl #0x2]
  001856be: adds r3,#0x1
  001856c0: cmp r1,#0x1
  001856c2: it eq
  001856c4: add.eq r0,#0x1e
  001856c6: b 0x001856b0
  001856c8: bx lr
```
