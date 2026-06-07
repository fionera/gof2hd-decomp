# Hud::getAnalogY
elf 0x16654a body 40
Sig: undefined __stdcall getAnalogY(void)

## decompile
```c

/* Hud::getAnalogY() */

float Hud::getAnalogY(void)

{
  int in_r0;
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar2 = (float)VectorSignedToFloat((uint)*(ushort *)(in_r0 + 0x420) -
                                     (uint)*(ushort *)(in_r0 + 0x426),(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x4e0),(byte)(in_fpscr >> 0x16) & 3);
  return fVar2 / fVar1;
}

```

## target disasm
```
  0017654a: ldrh.w r1,[r0,#0x426]
  0017654e: ldrh.w r2,[r0,#0x420]
  00176552: add.w r0,r0,#0x4e0
  00176556: vldr.32 s0,[r0]
  0017655a: subs r0,r2,r1
  0017655c: vmov s2,r0
  00176560: vcvt.f32.s32 s2,s2
  00176564: vcvt.f32.s32 s0,s0
  00176568: vdiv.f32 s0,s2,s0
  0017656c: vmov r0,s0
  00176570: bx lr
```
