# ParticleSettings::multiplyAll
elf 0x183e68 body 220
Sig: undefined __stdcall multiplyAll(float param_1)

## decompile
```c

/* ParticleSettings::multiplyAll(float) */

void ParticleSettings::multiplyAll(float param_1)

{
  float fVar1;
  float in_r0;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint in_fpscr;
  float fVar9;
  uint uVar10;
  float fVar11;
  uint uVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  
  iVar2 = 0;
  iVar8 = *(int *)(DAT_00193f44 + 0x193e80);
  iVar4 = *(int *)(DAT_00193f48 + 0x193e90);
  fVar1 = 1.0 / ((in_r0 + 1.0) * 0.5);
  do {
    if (iVar2 == 0x1e00) {
      return;
    }
    iVar5 = iVar8 + iVar2;
    if ((*(uint *)(iVar5 + 0xc) & 0x20) == 0) {
      if ((int)(*(uint *)(iVar5 + 0xc) << 0x1b) < 0) {
        fVar9 = *(float *)(iVar4 + iVar2 + 0x2c);
        fVar11 = 1.0 / in_r0;
        goto LAB_00193ed0;
      }
    }
    else {
      fVar11 = *(float *)(iVar4 + iVar2 + 0x2c);
      fVar9 = in_r0;
LAB_00193ed0:
      iVar3 = iVar4 + iVar2;
      uVar6 = *(uint *)(iVar3 + 0x34);
      uVar7 = *(uint *)(iVar3 + 0x38);
      fVar13 = (float)VectorSignedToFloat(*(undefined4 *)(iVar3 + 0x10),(byte)(in_fpscr >> 0x16) & 3
                                         );
      fVar14 = (float)VectorSignedToFloat(uVar6 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
      fVar15 = (float)VectorSignedToFloat(uVar7 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
      fVar14 = fVar1 * fVar14;
      fVar15 = fVar1 * fVar15;
      *(float *)(iVar5 + 0x2c) = fVar11 * fVar9;
      uVar10 = (uint)(0.0 < fVar14) * (int)fVar14;
      uVar12 = (uint)(0.0 < fVar15) * (int)fVar15;
      if (0xfe < uVar10) {
        uVar10 = 0xff;
      }
      if (0xfe < uVar12) {
        uVar12 = 0xff;
      }
      *(uint *)(iVar5 + 0x34) = uVar10 | uVar6 & 0xffffff00;
      *(uint *)(iVar5 + 0x38) = uVar12 | uVar7 & 0xffffff00;
      *(int *)(iVar5 + 0x10) = (int)(fVar13 * in_r0);
    }
    iVar2 = iVar2 + 0xa0;
  } while( true );
}

```

## target disasm
```
  00193e68: push {r4,r5,r7,lr}
  00193e6a: add r7,sp,#0x8
  00193e6c: vmov.f32 s2,0x3f800000
  00193e70: ldr r1,[0x00193f44]
  00193e72: vmov s0,r0
  00193e76: movs r0,#0x0
  00193e78: vmov.f64 d17,0x3fe0000000000000
  00193e7c: add r1,pc
  00193e7e: mov.w r12,#0xff
  00193e82: ldr.w lr,[r1,#0x0]
  00193e86: ldr r1,[0x00193f48]
  00193e88: vadd.f32 s4,s0,s2
  00193e8c: add r1,pc
  00193e8e: vdiv.f32 s2,s2,s0
  00193e92: ldr r2,[r1,#0x0]
  00193e94: vcvt.f64.f32 d16,s4
  00193e98: vmul.f64 d16,d16,d17
  00193e9c: vmov.f64 d17,0x3ff0000000000000
  00193ea0: vdiv.f64 d16,d17,d16
  00193ea4: vcvt.f32.f64 s4,d16
  00193ea8: b 0x00193f3c
  00193eaa: add.w r3,lr,r0
  00193eae: ldr r1,[r3,#0xc]
  00193eb0: tst r1,#0x20
  00193eb4: bne 0x00193ec6
  00193eb6: lsls r1,r1,#0x1b
  00193eb8: bpl 0x00193f3a
  00193eba: vmov.f32 s8,s2
  00193ebe: adds r1,r2,r0
  00193ec0: vldr.32 s6,[r1,#0x2c]
  00193ec4: b 0x00193ed0
  00193ec6: adds r1,r2,r0
  00193ec8: vmov.f32 s6,s0
  00193ecc: vldr.32 s8,[r1,#0x2c]
  00193ed0: adds r1,r2,r0
  00193ed2: vmul.f32 s6,s8,s6
  00193ed6: ldrd r4,r5,[r1,#0x34]
  00193eda: vldr.32 s10,[r1,#0x10]
  00193ede: uxtb r1,r4
  00193ee0: vcvt.f32.s32 s10,s10
  00193ee4: vmov s12,r1
  00193ee8: uxtb r1,r5
  00193eea: vmov s14,r1
  00193eee: bic r4,r4,#0xff
  00193ef2: vcvt.f32.s32 s12,s12
  00193ef6: bic r5,r5,#0xff
  00193efa: vcvt.f32.s32 s14,s14
  00193efe: vmul.f32 s10,s10,s0
  00193f02: vmul.f32 s8,s4,s12
  00193f06: vmul.f32 s12,s4,s14
  00193f0a: vcvt.s32.f32 s10,s10
  00193f0e: vstr.32 s6,[r3,#0x2c]
  00193f12: vcvt.u32.f32 s6,s8
  00193f16: vcvt.u32.f32 s8,s12
  00193f1a: vmov r1,s6
  00193f1e: cmp r1,#0xff
  00193f20: it cs
  00193f22: mov.cs r1,r12
  00193f24: orrs r1,r4
  00193f26: vmov r4,s8
  00193f2a: cmp r4,#0xff
  00193f2c: it cs
  00193f2e: mov.cs r4,r12
  00193f30: orrs r4,r5
  00193f32: strd r1,r4,[r3,#0x34]
  00193f36: vstr.32 s10,[r3,#0x10]
  00193f3a: adds r0,#0xa0
  00193f3c: cmp.w r0,#0x1e00
  00193f40: bne 0x00193eaa
  00193f42: pop {r4,r5,r7,pc}
```
