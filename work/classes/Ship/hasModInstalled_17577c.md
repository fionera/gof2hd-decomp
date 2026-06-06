# Ship::hasModInstalled
elf 0x17577c body 36
Sig: undefined __thiscall hasModInstalled(Ship * this, int param_1)

## decompile
```c

/* Ship::hasModInstalled(int) */

undefined4 __thiscall Ship::hasModInstalled(Ship *this,int param_1)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(this + 0x78);
  if (puVar2 != (uint *)0x0) {
    uVar3 = 0;
    while (uVar3 < *puVar2) {
      iVar1 = uVar3 * 4;
      uVar3 = uVar3 + 1;
      if (*(int *)(puVar2[1] + iVar1) == param_1) {
        return 1;
      }
    }
  }
  return 0;
}

```
## target disasm
```
  0018577c: ldr r0,[r0,#0x78]
  0018577e: cbz r0,0x0018579c
  00185780: ldr.w r12,[r0,#0x0]
  00185784: movs r3,#0x0
  00185786: b 0x00185798
  00185788: ldr r2,[r0,#0x4]
  0018578a: ldr.w r2,[r2,r3,lsl #0x2]
  0018578e: adds r3,#0x1
  00185790: cmp r2,r1
  00185792: itt eq
  00185794: mov.eq r0,#0x1
  00185796: bx.eq lr
  00185798: cmp r3,r12
  0018579a: bcc 0x00185788
  0018579c: movs r0,#0x0
  0018579e: bx lr
```
