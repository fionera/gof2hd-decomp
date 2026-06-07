# Route::getDockingTarget
elf 0x11be00 body 22
Sig: undefined __thiscall getDockingTarget(Route * this, int param_1)

## decompile
```c

/* Route::getDockingTarget(int) */

undefined4 __thiscall Route::getDockingTarget(Route *this,int param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)(this + 0x10);
  if ((piVar1 != (int *)0x0) && (param_1 < *piVar1)) {
    return *(undefined4 *)(piVar1[1] + param_1 * 4);
  }
  return 0;
}

```

## target disasm
```
  0012be00: ldr r0,[r0,#0x10]
  0012be02: cbz r0,0x0012be12
  0012be04: ldr r2,[r0,#0x0]
  0012be06: cmp r2,r1
  0012be08: ittt gt
  0012be0a: ldr.gt r0,[r0,#0x4]
  0012be0c: ldr.gt.w r0,[r0,r1,lsl #0x2]
  0012be10: bx.gt lr
  0012be12: movs r0,#0x0
  0012be14: bx lr
```
