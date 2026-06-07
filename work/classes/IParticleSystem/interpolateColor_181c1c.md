# IParticleSystem::interpolateColor
elf 0x181c1c body 302
Sig: undefined __stdcall interpolateColor(int param_1, float * param_2, float * param_3, float * param_4, float * param_5)

## decompile
```c

/* IParticleSystem::interpolateColor(int, float&, float&, float&, float&) */

void IParticleSystem::interpolateColor
               (int param_1,float *param_2,float *param_3,float *param_4,float *param_5)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  float fVar7;
  undefined8 in_d1;
  undefined8 uVar8;
  float fVar9;
  undefined4 in_s5;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float *in_stack_00000004;
  
  iVar3 = *(int *)(*(int *)(param_1 + 0x68) + (int)param_2 * 4);
  iVar4 = *(int *)(DAT_00191d50 + 0x191c34) +
          *(char *)(*(int *)(param_1 + 0x6c) + (int)param_2) * 0xa0;
  fVar5 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(iVar4 + 0x28),(byte)(in_fpscr >> 0x16) & 3);
  uVar2 = *(uint *)(iVar4 + 0x34);
  uVar1 = *(uint *)(iVar4 + 0x38);
  uVar8 = FloatVectorMin(CONCAT44((int)((ulonglong)in_d1 >> 0x20),fVar5 / fVar6),
                         CONCAT44(in_s5,0x3f800000),2,0x20);
  fVar11 = (float)VectorUnsignedToFloat((uVar2 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
  fVar13 = (float)VectorUnsignedToFloat(uVar2 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
  fVar7 = (float)uVar8;
  fVar9 = 1.0 - fVar7;
  fVar14 = (float)VectorUnsignedToFloat((uVar1 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
  fVar15 = (float)VectorUnsignedToFloat((uVar2 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
  fVar16 = (float)VectorUnsignedToFloat(uVar2 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
  fVar6 = (float)VectorUnsignedToFloat(uVar1 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
  fVar10 = (float)VectorUnsignedToFloat((uVar1 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
  fVar12 = (float)VectorUnsignedToFloat(uVar1 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
  fVar11 = (fVar9 * fVar11 + fVar7 * fVar14) * DAT_00191d4c;
  fVar14 = (fVar9 * fVar15 + fVar7 * fVar10) * DAT_00191d4c;
  fVar10 = (fVar9 * fVar16 + fVar7 * fVar12) * DAT_00191d4c;
  *param_3 = (fVar9 * fVar13 + fVar7 * fVar6) * DAT_00191d4c;
  *param_4 = fVar11;
  *param_5 = fVar14;
  *in_stack_00000004 = fVar10;
  if (iVar3 < *(int *)(iVar4 + 0x3c)) {
    fVar6 = (float)VectorSignedToFloat(*(int *)(iVar4 + 0x3c),(byte)(in_fpscr >> 0x16) & 3);
    fVar5 = fVar5 / fVar6;
    if (*(char *)(param_1 + 0x45) != '\0') {
      *param_3 = fVar5 * *param_3;
      *param_4 = fVar5 * *param_4;
      fVar10 = *param_5;
      in_stack_00000004 = param_5;
    }
    *in_stack_00000004 = fVar5 * fVar10;
  }
  return;
}

```

## target disasm
```
  00191c1c: push {r4,r5,r6,r7,lr}
  00191c1e: add r7,sp,#0xc
  00191c20: push.w r8
  00191c24: ldrd r5,r4,[r0,#0x68]
  00191c28: vmov.f32 s4,0x3f800000
  00191c2c: ldr r6,[0x00191d50]
  00191c2e: ldrsb r4,[r4,r1]
  00191c30: add r6,pc
  00191c32: ldr.w r12,[r5,r1,lsl #0x2]
  00191c36: ldr r1,[r6,#0x0]
  00191c38: add.w r6,r4,r4, lsl #0x2
  00191c3c: add.w lr,r1,r6, lsl #0x5
  00191c40: vmov s0,r12
  00191c44: vldr.32 s2,[lr,#0x28]
  00191c48: vcvt.f32.s32 s0,s0
  00191c4c: vcvt.f32.s32 s2,s2
  00191c50: ldrd r4,r1,[lr,#0x34]
  00191c54: ubfx r6,r1,#0x10,#0x8
  00191c58: ubfx r5,r4,#0x10,#0x8
  00191c5c: uxtb.w r8,r4
  00191c60: vmov s10,r6
  00191c64: vmov s6,r5
  00191c68: lsrs r5,r4,#0x18
  00191c6a: vmov s8,r5
  00191c6e: vdiv.f32 s2,s0,s2
  00191c72: vmin.f32 d1,d1,d2
  00191c76: vcvt.f32.u32 s6,s6
  00191c7a: vcvt.f32.u32 s8,s8
  00191c7e: ubfx r4,r4,#0x8,#0x8
  00191c82: vmov s14,r8
  00191c86: vmov s12,r4
  00191c8a: lsrs r4,r1,#0x18
  00191c8c: vsub.f32 s4,s4,s2
  00191c90: vcvt.f32.u32 s10,s10
  00191c94: vcvt.f32.u32 s12,s12
  00191c98: vcvt.f32.u32 s14,s14
  00191c9c: vmov s1,r4
  00191ca0: vcvt.f32.u32 s1,s1
  00191ca4: ubfx r4,r1,#0x8,#0x8
  00191ca8: uxtb r1,r1
  00191caa: vmul.f32 s6,s4,s6
  00191cae: vmov s5,r4
  00191cb2: vmov s7,r1
  00191cb6: vcvt.f32.u32 s5,s5
  00191cba: vcvt.f32.u32 s7,s7
  00191cbe: ldrd r5,r4,[r7,#0x8]
  00191cc2: vmul.f32 s8,s4,s8
  00191cc6: vmul.f32 s12,s4,s12
  00191cca: vmul.f32 s4,s4,s14
  00191cce: vmla.f32 s6,s2,s10
  00191cd2: vmla.f32 s8,s2,s1
  00191cd6: vmla.f32 s12,s2,s5
  00191cda: vmla.f32 s4,s2,s7
  00191cde: vldr.32 s2,[pc,#0x6c]
  00191ce2: vmul.f32 s6,s6,s2
  00191ce6: vmul.f32 s8,s8,s2
  00191cea: vmul.f32 s10,s12,s2
  00191cee: vmul.f32 s2,s4,s2
  00191cf2: vstr.32 s8,[r2]
  00191cf6: vstr.32 s6,[r3]
  00191cfa: vstr.32 s10,[r5]
  00191cfe: vstr.32 s2,[r4]
  00191d02: ldr.w r1,[lr,#0x3c]
  00191d06: cmp r12,r1
  00191d08: bge 0x00191d44
  00191d0a: vmov s4,r1
  00191d0e: vcvt.f32.s32 s4,s4
  00191d12: ldrb.w r0,[r0,#0x45]
  00191d16: cmp r0,#0x0
  00191d18: vdiv.f32 s0,s0,s4
  00191d1c: beq 0x00191d3c
  00191d1e: vldr.32 s2,[r2]
  00191d22: mov r4,r5
  00191d24: vmul.f32 s2,s0,s2
  00191d28: vstr.32 s2,[r2]
  00191d2c: vldr.32 s2,[r3]
  00191d30: vmul.f32 s2,s0,s2
  00191d34: vstr.32 s2,[r3]
  00191d38: vldr.32 s2,[r5]
  00191d3c: vmul.f32 s0,s0,s2
  00191d40: vstr.32 s0,[r4]
  00191d44: pop.w r8
  00191d48: pop {r4,r5,r6,r7,pc}
```
