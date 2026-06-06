# Level::getNumDockingTargets
elf 0xc66f8 body 46
Sig: undefined __stdcall getNumDockingTargets(void)

## decompile
```c

/* Level::getNumDockingTargets() */

int Level::getNumDockingTargets(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(in_r0 + 0xf8);
  if (piVar3 != (int *)0x0) {
    iVar1 = 0;
    for (iVar2 = 0; *piVar3 != iVar2; iVar2 = iVar2 + 1) {
      iVar1 = iVar1 + (uint)*(byte *)(*(int *)(piVar3[1] + iVar2 * 4) + 0x70);
    }
    return iVar1;
  }
  return 0;
}

```
## target disasm
```
  000d66f8: ldr.w r12,[r0,#0xf8]
  000d66fc: cmp.w r12,#0x0
  000d6700: beq 0x000d6722
  000d6702: ldr.w r2,[r12,#0x0]
  000d6706: movs r0,#0x0
  000d6708: movs r3,#0x0
  000d670a: cmp r2,r3
  000d670c: it eq
  000d670e: bx.eq lr
  000d6710: ldr.w r1,[r12,#0x4]
  000d6714: ldr.w r1,[r1,r3,lsl #0x2]
  000d6718: adds r3,#0x1
  000d671a: ldrb.w r1,[r1,#0x70]
  000d671e: add r0,r1
  000d6720: b 0x000d670a
  000d6722: movs r0,#0x0
  000d6724: bx lr
```
