# Route::getDockingTarget
elf 0x11bde8 body 24
Sig: undefined __stdcall getDockingTarget(void)

## decompile
```c

/* Route::getDockingTarget() */

undefined4 Route::getDockingTarget(void)

{
  int *in_r0;
  int *piVar1;
  
  piVar1 = (int *)in_r0[4];
  if ((piVar1 != (int *)0x0) && (*in_r0 < *piVar1)) {
    return *(undefined4 *)(piVar1[1] + *in_r0 * 4);
  }
  return 0;
}

```

## target disasm
```
  0012bde8: ldr r1,[r0,#0x10]
  0012bdea: cbz r1,0x0012bdfc
  0012bdec: ldr r0,[r0,#0x0]
  0012bdee: ldr r2,[r1,#0x0]
  0012bdf0: cmp r2,r0
  0012bdf2: ittt gt
  0012bdf4: ldr.gt r1,[r1,#0x4]
  0012bdf6: ldr.gt.w r0,[r1,r0,lsl #0x2]
  0012bdfa: bx.gt lr
  0012bdfc: movs r0,#0x0
  0012bdfe: bx lr
```
