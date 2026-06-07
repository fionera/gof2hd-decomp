# Route::getDockingTime
elf 0x11be1c body 24
Sig: undefined __thiscall getDockingTime(Route * this, int param_1)

## decompile
```c

/* Route::getDockingTime(int) */

undefined4 __thiscall Route::getDockingTime(Route *this,int param_1)

{
  if ((*(int *)(this + 0x10) != 0) && (param_1 < **(int **)(this + 0x14))) {
    return *(undefined4 *)((*(int **)(this + 0x14))[1] + param_1 * 4);
  }
  return 0;
}

```

## target disasm
```
  0012be1c: ldr r2,[r0,#0x10]
  0012be1e: cbz r2,0x0012be30
  0012be20: ldr r0,[r0,#0x14]
  0012be22: ldr r2,[r0,#0x0]
  0012be24: cmp r2,r1
  0012be26: ittt gt
  0012be28: ldr.gt r0,[r0,#0x4]
  0012be2a: ldr.gt.w r0,[r0,r1,lsl #0x2]
  0012be2e: bx.gt lr
  0012be30: movs r0,#0x0
  0012be32: bx lr
```
