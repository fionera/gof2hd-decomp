# Status::getNumberOfMissions
elf 0xa9a48 body 46
Sig: undefined __thiscall getNumberOfMissions(Status * this)

## decompile
```c

/* Status::getNumberOfMissions() */

int __thiscall Status::getNumberOfMissions(Status *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(this + 0x198);
  if (piVar4 == (int *)0x0) {
    return 0;
  }
  iVar2 = 0;
  iVar3 = 0;
  while (*piVar4 != iVar3) {
    iVar1 = iVar3 * 4;
    iVar3 = iVar3 + 1;
    if (*(int *)(piVar4[1] + iVar1) != 0) {
      iVar2 = iVar2 + 1;
    }
  }
  return iVar2;
}

```
## target disasm
```
  000b9a48: ldr.w r12,[r0,#0x198]
  000b9a4c: cmp.w r12,#0x0
  000b9a50: beq 0x000b9a72
  000b9a52: ldr.w r2,[r12,#0x0]
  000b9a56: movs r0,#0x0
  000b9a58: movs r3,#0x0
  000b9a5a: cmp r2,r3
  000b9a5c: it eq
  000b9a5e: bx.eq lr
  000b9a60: ldr.w r1,[r12,#0x4]
  000b9a64: ldr.w r1,[r1,r3,lsl #0x2]
  000b9a68: adds r3,#0x1
  000b9a6a: cmp r1,#0x0
  000b9a6c: it ne
  000b9a6e: add.ne r0,#0x1
  000b9a70: b 0x000b9a5a
  000b9a72: movs r0,#0x0
  000b9a74: bx lr
```
