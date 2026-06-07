# Hud::getAnalogX
elf 0x166522 body 40
Sig: undefined __stdcall getAnalogX(void)

## decompile
```c

/* Hud::getAnalogX() */

float Hud::getAnalogX(void)

{
  int in_r0;
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar2 = (float)VectorSignedToFloat((uint)*(ushort *)(in_r0 + 0x41e) -
                                     (uint)*(ushort *)(in_r0 + 0x424),(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x4e0),(byte)(in_fpscr >> 0x16) & 3);
  return fVar2 / fVar1;
}

```

## target disasm
```
  00176522: ldrh.w r1,[r0,#0x424]
  00176526: ldrh.w r2,[r0,#0x41e]
  0017652a: add.w r0,r0,#0x4e0
  0017652e: vldr.32 s0,[r0]
  00176532: subs r0,r2,r1
  00176534: vmov s2,r0
  00176538: vcvt.f32.s32 s2,s2
  0017653c: vcvt.f32.s32 s0,s0
  00176540: vdiv.f32 s0,s2,s0
  00176544: vmov r0,s0
  00176548: bx lr
```
