# AbyssEngine::Quaternion::Length
elf 0x814e4 body 60
Sig: undefined __stdcall Length(void)

## decompile
```c

/* AbyssEngine::Quaternion::Length() const */

float AbyssEngine::Quaternion::Length(void)

{
  float *in_r0;
  float fVar1;
  
  fVar1 = SQRT(in_r0[1] * in_r0[1] + *in_r0 * *in_r0 + in_r0[2] * in_r0[2] + in_r0[3] * in_r0[3]);
  if (NAN(fVar1)) {
    fVar1 = (float)(*(code *)(DAT_001ab144 + 0x1ab148))();
    return fVar1;
  }
  return fVar1;
}

```
## target disasm
```
  000914e4: vldr.32 s0,[r0,#0x4]
  000914e8: vldr.32 s2,[r0]
  000914ec: vmul.f32 s0,s0,s0
  000914f0: vldr.32 s4,[r0,#0x8]
  000914f4: vldr.32 s6,[r0,#0xc]
  000914f8: vmla.f32 s0,s2,s2
  000914fc: vmla.f32 s0,s4,s4
  00091500: vmla.f32 s0,s6,s6
  00091504: vsqrt.f32 s2,s0
  00091508: vcmpe.f32 s2,s2
  0009150c: vmrs apsr,fpscr
  00091510: itt vs
  00091512: vmov.vs r0,s0
  00091516: b.vs.w 0x001ab138
  0009151a: vmov r0,s2
  0009151e: bx lr
```
