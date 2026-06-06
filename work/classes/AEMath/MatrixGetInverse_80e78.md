# AbyssEngine::AEMath::MatrixGetInverse
elf 0x80e78 body 158
Sig: undefined __stdcall MatrixGetInverse(Matrix * param_1)

## decompile
```c

/* AbyssEngine::AEMath::MatrixGetInverse(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::MatrixGetInverse(Matrix *param_1)

{
  float *pfVar1;
  float *in_r1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  
  pfVar1 = (float *)Matrix::Matrix(param_1);
  fVar5 = in_r1[4];
  fVar10 = *in_r1;
  fVar9 = in_r1[8];
  *pfVar1 = fVar10;
  pfVar1[1] = fVar5;
  pfVar1[2] = fVar9;
  fVar3 = in_r1[5];
  fVar7 = in_r1[9];
  fVar2 = in_r1[1];
  pfVar1[4] = fVar2;
  pfVar1[5] = fVar3;
  pfVar1[6] = fVar7;
  fVar4 = in_r1[6];
  fVar6 = in_r1[2];
  fVar8 = in_r1[10];
  pfVar1[8] = fVar6;
  pfVar1[9] = fVar4;
  pfVar1[10] = fVar8;
  fVar12 = in_r1[7];
  fVar11 = in_r1[3];
  fVar13 = in_r1[0xb];
  pfVar1[7] = (-(fVar12 * fVar3) + -(fVar11 * fVar2)) - fVar13 * fVar7;
  pfVar1[3] = (-(fVar12 * fVar5) + -(fVar11 * fVar10)) - fVar13 * fVar9;
  pfVar1[0xb] = (-(fVar12 * fVar4) + -(fVar11 * fVar6)) - fVar13 * fVar8;
  return;
}

```
## target disasm
```
  00090e78: push {r4,r5,r6,r7,lr}
  00090e7a: add r7,sp,#0xc
  00090e7c: push.w r11
  00090e80: mov r4,r1
  00090e82: blx 0x0006f118
  00090e86: ldr r3,[r4,#0x10]
  00090e88: ldr.w lr,[r4,#0x0]
  00090e8c: ldr.w r12,[r4,#0x20]
  00090e90: strd lr,r3,[r0,#0x0]
  00090e94: vmov s0,r3
  00090e98: str.w r12,[r0,#0x8]
  00090e9c: add.w r3,r0,#0x10
  00090ea0: ldr r2,[r4,#0x14]
  00090ea2: vmov s12,lr
  00090ea6: ldr r5,[r4,#0x24]
  00090ea8: ldr r1,[r4,#0x4]
  00090eaa: vmov s2,r2
  00090eae: stmia r3!,{r1,r2,r5}
  00090eb0: ldr r2,[r4,#0x18]
  00090eb2: ldr r3,[r4,#0x8]
  00090eb4: ldr r6,[r4,#0x28]
  00090eb6: strd r3,r2,[r0,#0x20]
  00090eba: vmov s10,r2
  00090ebe: str r6,[r0,#0x28]
  00090ec0: vmov s14,r3
  00090ec4: vldr.32 s6,[r4,#0x1c]
  00090ec8: vldr.32 s4,[r4,#0xc]
  00090ecc: vmul.f32 s0,s6,s0
  00090ed0: vldr.32 s8,[r4,#0x2c]
  00090ed4: vmul.f32 s2,s6,s2
  00090ed8: vmul.f32 s6,s6,s10
  00090edc: vmov s10,r1
  00090ee0: vnmla.f32 s0,s4,s12
  00090ee4: vnmla.f32 s2,s4,s10
  00090ee8: vnmla.f32 s6,s4,s14
  00090eec: vmov s4,r5
  00090ef0: vmov s10,r12
  00090ef4: vmov s12,r6
  00090ef8: vmls.f32 s0,s8,s10
  00090efc: vmls.f32 s2,s8,s4
  00090f00: vmls.f32 s6,s8,s12
  00090f04: vstr.32 s2,[r0,#0x1c]
  00090f08: vstr.32 s0,[r0,#0xc]
  00090f0c: vstr.32 s6,[r0,#0x2c]
  00090f10: pop.w r11
  00090f14: pop {r4,r5,r6,r7,pc}
```
