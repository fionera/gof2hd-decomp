# ParticleSettings::Interpolate
elf 0x183f4c body 146
Sig: undefined __stdcall Interpolate(ParticleSet param_1, ParticleSet param_2, float param_3, ParticleSet param_4)

## decompile
```c

/* ParticleSettings::Interpolate(ParticleSettings::ParticleSet, ParticleSettings::ParticleSet,
   float, ParticleSettings::ParticleSet) */

void ParticleSettings::Interpolate(int param_1,int param_2,float param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  
  iVar2 = *(int *)(DAT_00193fe0 + 0x193f62) + param_2 * 0xa0;
  iVar1 = *(int *)(DAT_00193fe0 + 0x193f62) + param_1 * 0xa0;
  fVar4 = 1.0 - param_3;
  fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(iVar2 + 0x28),(byte)(in_fpscr >> 0x16) & 3);
  fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(iVar1 + 0x28),(byte)(in_fpscr >> 0x16) & 3);
  iVar3 = *(int *)(DAT_00193fe4 + 0x193f8c) + param_4 * 0xa0;
  fVar7 = *(float *)(iVar1 + 0x14);
  *(int *)(iVar3 + 0x28) = (int)(fVar5 * param_3 + fVar4 * fVar6);
  fVar5 = *(float *)(iVar1 + 0x20);
  *(float *)(iVar3 + 0x14) = *(float *)(iVar2 + 0x14) * param_3 + fVar4 * fVar7;
  *(float *)(iVar3 + 0x20) = *(float *)(iVar2 + 0x20) * param_3 + fVar4 * fVar5;
  *(float *)(iVar3 + 0x70) = *(float *)(iVar2 + 0x70) * param_3 + fVar4 * *(float *)(iVar1 + 0x70);
  return;
}

```

## target disasm
```
  00193f4c: push {r4,r6,r7,lr}
  00193f4e: add r7,sp,#0x8
  00193f50: ldr r4,[0x00193fe0]
  00193f52: add.w r1,r1,r1, lsl #0x2
  00193f56: vmov.f32 s0,0x3f800000
  00193f5a: add.w r0,r0,r0, lsl #0x2
  00193f5e: add r4,pc
  00193f60: vmov s4,r2
  00193f64: add.w r3,r3,r3, lsl #0x2
  00193f68: ldr r4,[r4,#0x0]
  00193f6a: add.w r1,r4,r1, lsl #0x5
  00193f6e: add.w r0,r4,r0, lsl #0x5
  00193f72: vldr.32 s2,[r1,#0x28]
  00193f76: vsub.f32 s0,s0,s4
  00193f7a: vcvt.f32.s32 s2,s2
  00193f7e: vldr.32 s6,[r0,#0x28]
  00193f82: vcvt.f32.s32 s6,s6
  00193f86: ldr r2,[0x00193fe4]
  00193f88: add r2,pc
  00193f8a: ldr r2,[r2,#0x0]
  00193f8c: vmul.f32 s2,s2,s4
  00193f90: add.w r2,r2,r3, lsl #0x5
  00193f94: vmla.f32 s2,s0,s6
  00193f98: vcvt.s32.f32 s2,s2
  00193f9c: vldr.32 s6,[r0,#0x14]
  00193fa0: vstr.32 s2,[r2,#0x28]
  00193fa4: vldr.32 s2,[r1,#0x14]
  00193fa8: vmul.f32 s2,s2,s4
  00193fac: vmla.f32 s2,s0,s6
  00193fb0: vldr.32 s6,[r0,#0x20]
  00193fb4: vstr.32 s2,[r2,#0x14]
  00193fb8: vldr.32 s2,[r1,#0x20]
  00193fbc: vmul.f32 s2,s2,s4
  00193fc0: vmla.f32 s2,s0,s6
  00193fc4: vstr.32 s2,[r2,#0x20]
  00193fc8: vldr.32 s2,[r1,#0x70]
  00193fcc: vmul.f32 s2,s2,s4
  00193fd0: vldr.32 s4,[r0,#0x70]
  00193fd4: vmla.f32 s2,s0,s4
  00193fd8: vstr.32 s2,[r2,#0x70]
  00193fdc: pop {r4,r6,r7,pc}
```
