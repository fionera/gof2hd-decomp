# Ship::priceDecline
elf 0x1754c8 body 42
Sig: undefined __thiscall priceDecline(Ship * this)

## decompile
```c

/* Ship::priceDecline() */

void __thiscall Ship::priceDecline(Ship *this)

{
  uint in_fpscr;
  float fVar1;
  
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)
                                      (*(int *)(*(int *)(**(int **)(DAT_001854f4 + 0x1854d4) + 4) +
                                               *(int *)this * 4) + 0x14),
                                     (byte)(in_fpscr >> 0x16) & 3);
  *(int *)(this + 0x14) = (int)(fVar1 / 1.25);
  return;
}

```
## target disasm
```
  001854c8: ldr r1,[0x001854f4]
  001854ca: vmov.f32 s2,0x3fa00000
  001854ce: ldr r2,[r0,#0x0]
  001854d0: add r1,pc
  001854d2: ldr r1,[r1,#0x0]
  001854d4: ldr r1,[r1,#0x0]
  001854d6: ldr r1,[r1,#0x4]
  001854d8: ldr.w r1,[r1,r2,lsl #0x2]
  001854dc: vldr.32 s0,[r1,#0x14]
  001854e0: vcvt.f32.s32 s0,s0
  001854e4: vdiv.f32 s0,s0,s2
  001854e8: vcvt.s32.f32 s0,s0
  001854ec: vstr.32 s0,[r0,#0x14]
  001854f0: bx lr
```
