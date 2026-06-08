# AbyssEngine::MODF
elf 0x72adc body 26
Sig: undefined __stdcall MODF(float param_1, float * param_2)

## decompile
```c

/* AbyssEngine::MODF(float, float*) */

float AbyssEngine::MODF(float param_1,float *param_2)

{
  float *in_r1;
  uint in_fpscr;
  float fVar1;
  
  fVar1 = (float)VectorSignedToFloat((int)(float)param_2,(byte)(in_fpscr >> 0x16) & 3);
  *in_r1 = fVar1;
  return (float)param_2 - fVar1;
}

```

## target disasm
```
  00082adc: vmov s0,r0
  00082ae0: vcvt.s32.f32 s2,s0
  00082ae4: vcvt.f32.s32 s2,s2
  00082ae8: vsub.f32 s0,s0,s2
  00082aec: vstr.32 s2,[r1]
  00082af0: vmov r0,s0
  00082af4: bx lr
```
