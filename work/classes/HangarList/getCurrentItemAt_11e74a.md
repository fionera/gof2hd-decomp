# HangarList::getCurrentItemAt
elf 0x11e74a body 40
Sig: undefined __thiscall getCurrentItemAt(HangarList * this, int param_1)

## decompile
```c

/* HangarList::getCurrentItemAt(int) */

undefined4 __thiscall HangarList::getCurrentItemAt(HangarList *this,int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  
  if ((*(int *)this != 0) &&
     (piVar2 = *(int **)(*(int *)(*(int *)this + 4) + *(int *)(this + 4) * 4), piVar2 != (int *)0x0)
     ) {
    uVar1 = 0;
    if (-1 < param_1) {
      if (param_1 < *piVar2) {
        uVar1 = *(undefined4 *)(piVar2[1] + param_1 * 4);
      }
      return uVar1;
    }
    return 0;
  }
  return 0;
}

```

## target disasm
```
  0012e74a: ldr r2,[r0,#0x0]
  0012e74c: cbz r2,0x0012e76e
  0012e74e: ldr r0,[r0,#0x4]
  0012e750: ldr r2,[r2,#0x4]
  0012e752: ldr.w r2,[r2,r0,lsl #0x2]
  0012e756: cbz r2,0x0012e76e
  0012e758: movs r0,#0x0
  0012e75a: cmp r1,#0x0
  0012e75c: it lt
  0012e75e: bx.lt lr
  0012e760: ldr r3,[r2,#0x0]
  0012e762: cmp r3,r1
  0012e764: itt gt
  0012e766: ldr.gt r0,[r2,#0x4]
  0012e768: ldr.gt.w r0,[r0,r1,lsl #0x2]
  0012e76c: bx lr
  0012e76e: movs r0,#0x0
  0012e770: bx lr
```
