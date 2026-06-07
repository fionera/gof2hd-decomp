# TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion
elf 0x15a688 body 426
Sig: undefined __thiscall aproximateCooefficientsForAproximationOfDampingFunktion(TargetFollowCamera * this, float param_1, double * param_2, double * param_3, double * param_4, double * param_5, double * param_6)

## decompile
```c

/* TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(float, double&,
   double&, double&, double&, double&) */

void __thiscall
TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion
          (TargetFollowCamera *this,float param_1,double *param_2,double *param_3,double *param_4,
          double *param_5,double *param_6)

{
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double *in_stack_00000008;
  
  dVar1 = (double)(float)param_2;
  dVar6 = dVar1 * dVar1;
  dVar7 = dVar6 * dVar1;
  dVar8 = dVar7 * dVar1;
  dVar9 = dVar8 * dVar1;
  dVar10 = dVar9 * dVar1;
  dVar11 = dVar10 * dVar1;
  dVar12 = dVar11 * dVar1;
  dVar2 = dVar12 * DAT_0016a838 + dVar11 * DAT_0016a860 + dVar10 * DAT_0016a888 +
          dVar9 * DAT_0016a8b0 + dVar8 * DAT_0016a8d8 + dVar7 * DAT_0016a900 + dVar6 * DAT_0016a928
          + dVar1 * DAT_0016a950 + DAT_0016a978;
  dVar3 = dVar11 * DAT_0016a840 + dVar12 * DAT_0016a868 + dVar10 * DAT_0016a890 +
          dVar9 * DAT_0016a8b8 + dVar8 * DAT_0016a8e0 + dVar7 * DAT_0016a908 + dVar6 * DAT_0016a930
          + dVar1 * DAT_0016a958 + DAT_0016a980;
  dVar4 = dVar12 * DAT_0016a848 + dVar11 * DAT_0016a870 + dVar10 * DAT_0016a898 +
          dVar9 * DAT_0016a8c0 + dVar8 * DAT_0016a8e8 + dVar7 * DAT_0016a910 + dVar6 * DAT_0016a938
          + dVar1 * DAT_0016a960 + DAT_0016a988;
  dVar5 = dVar11 * DAT_0016a850 + dVar12 * DAT_0016a878 + dVar10 * DAT_0016a8a0 +
          dVar9 * DAT_0016a8c8 + dVar8 * DAT_0016a8f0 + dVar7 * DAT_0016a918 + dVar6 * DAT_0016a940
          + dVar1 * DAT_0016a968 + DAT_0016a990;
  *param_3 = dVar12 * DAT_0016a858 + dVar11 * DAT_0016a880 + dVar10 * DAT_0016a8a8 +
             dVar9 * DAT_0016a8d0 + dVar8 * DAT_0016a8f8 + dVar7 * DAT_0016a920 +
             dVar6 * DAT_0016a948 + dVar1 * DAT_0016a970;
  *param_4 = -dVar2;
  *param_5 = dVar4;
  *param_6 = dVar5;
  *in_stack_00000008 = -dVar3;
  return;
}

```

## target disasm
```
  0016a688: vmov s0,r1
  0016a68c: vldr.64 d17,[pc,#0x1a8]
  0016a690: vldr.64 d19,[pc,#0x1ac]
  0016a694: vcvt.f64.f32 d16,s0
  0016a698: ldr.w r12,[sp,#0x4]
  0016a69c: ldr r1,[sp,#0x8]
  0016a69e: ldr r0,[sp,#0x0]
  0016a6a0: vmul.f64 d22,d16,d16
  0016a6a4: vmul.f64 d23,d22,d16
  0016a6a8: vmul.f64 d24,d23,d16
  0016a6ac: vmul.f64 d25,d24,d16
  0016a6b0: vmul.f64 d26,d25,d16
  0016a6b4: vmul.f64 d27,d26,d16
  0016a6b8: vmul.f64 d28,d27,d16
  0016a6bc: vmul.f64 d18,d28,d17
  0016a6c0: vmul.f64 d19,d27,d19
  0016a6c4: vldr.64 d20,[pc,#0x180]
  0016a6c8: vldr.64 d21,[pc,#0x184]
  0016a6cc: vmul.f64 d20,d28,d20
  0016a6d0: vmul.f64 d21,d27,d21
  0016a6d4: vldr.64 d30,[pc,#0x188]
  0016a6d8: vldr.64 d31,[pc,#0x18c]
  0016a6dc: vmla.f64 d18,d27,d30
  0016a6e0: vmla.f64 d19,d28,d31
  0016a6e4: vldr.64 d29,[pc,#0x170]
  0016a6e8: vldr.64 d0,[pc,#0x184]
  0016a6ec: vldr.64 d1,[pc,#0x188]
  0016a6f0: vmul.f64 d17,d28,d29
  0016a6f4: vmla.f64 d20,d27,d0
  0016a6f8: vmla.f64 d21,d28,d1
  0016a6fc: vldr.64 d29,[pc,#0x180]
  0016a700: vldr.64 d30,[pc,#0x184]
  0016a704: vmla.f64 d17,d27,d29
  0016a708: vldr.64 d31,[pc,#0x184]
  0016a70c: vmla.f64 d18,d26,d30
  0016a710: vmla.f64 d19,d26,d31
  0016a714: vldr.64 d0,[pc,#0x180]
  0016a718: vldr.64 d28,[pc,#0x184]
  0016a71c: vmla.f64 d20,d26,d0
  0016a720: vmla.f64 d21,d26,d28
  0016a724: vldr.64 d27,[pc,#0x180]
  0016a728: vldr.64 d29,[pc,#0x184]
  0016a72c: vmla.f64 d17,d26,d27
  0016a730: vldr.64 d30,[pc,#0x184]
  0016a734: vmla.f64 d18,d25,d29
  0016a738: vmla.f64 d19,d25,d30
  0016a73c: vldr.64 d31,[pc,#0x180]
  0016a740: vldr.64 d28,[pc,#0x184]
  0016a744: vmla.f64 d20,d25,d31
  0016a748: vmla.f64 d21,d25,d28
  0016a74c: vldr.64 d26,[pc,#0x180]
  0016a750: vldr.64 d27,[pc,#0x184]
  0016a754: vmla.f64 d17,d25,d26
  0016a758: vldr.64 d29,[pc,#0x184]
  0016a75c: vmla.f64 d18,d24,d27
  0016a760: vmla.f64 d19,d24,d29
  0016a764: vldr.64 d30,[pc,#0x180]
  0016a768: vldr.64 d28,[pc,#0x184]
  0016a76c: vmla.f64 d20,d24,d30
  0016a770: vmla.f64 d21,d24,d28
  0016a774: vldr.64 d25,[pc,#0x180]
  0016a778: vldr.64 d26,[pc,#0x184]
  0016a77c: vmla.f64 d17,d24,d25
  0016a780: vldr.64 d27,[pc,#0x184]
  0016a784: vmla.f64 d18,d23,d26
  0016a788: vmla.f64 d19,d23,d27
  0016a78c: vldr.64 d29,[pc,#0x180]
  0016a790: vldr.64 d28,[pc,#0x184]
  0016a794: vmla.f64 d20,d23,d29
  0016a798: vmla.f64 d21,d23,d28
  0016a79c: vldr.64 d24,[pc,#0x180]
  0016a7a0: vldr.64 d25,[pc,#0x184]
  0016a7a4: vmla.f64 d17,d23,d24
  0016a7a8: vldr.64 d26,[pc,#0x184]
  0016a7ac: vmla.f64 d18,d22,d25
  0016a7b0: vmla.f64 d19,d22,d26
  0016a7b4: vldr.64 d27,[pc,#0x180]
  0016a7b8: vldr.64 d28,[pc,#0x184]
  0016a7bc: vmla.f64 d20,d22,d27
  0016a7c0: vmla.f64 d21,d22,d28
  0016a7c4: vldr.64 d23,[pc,#0x180]
  0016a7c8: vldr.64 d24,[pc,#0x184]
  0016a7cc: vmla.f64 d17,d22,d23
  0016a7d0: vldr.64 d25,[pc,#0x184]
  0016a7d4: vmla.f64 d18,d16,d24
  0016a7d8: vmla.f64 d19,d16,d25
  0016a7dc: vldr.64 d26,[pc,#0x180]
  0016a7e0: vldr.64 d27,[pc,#0x184]
  0016a7e4: vmla.f64 d20,d16,d26
  0016a7e8: vmla.f64 d21,d16,d27
  0016a7ec: vldr.64 d22,[pc,#0x180]
  0016a7f0: vldr.64 d23,[pc,#0x184]
  0016a7f4: vmla.f64 d17,d16,d22
  0016a7f8: vldr.64 d24,[pc,#0x184]
  0016a7fc: vadd.f64 d16,d18,d23
  0016a800: vadd.f64 d18,d19,d24
  0016a804: vldr.64 d25,[pc,#0x180]
  0016a808: vldr.64 d26,[pc,#0x184]
  0016a80c: vadd.f64 d19,d20,d25
  0016a810: vadd.f64 d20,d21,d26
  0016a814: vneg.f64 d16,d16
  0016a818: vneg.f64 d18,d18
  0016a81c: vstr.64 d17,[r2]
  0016a820: vstr.64 d16,[r3]
  0016a824: vstr.64 d19,[r0]
  0016a828: vstr.64 d20,[r12]
  0016a82c: vstr.64 d18,[r1]
  0016a830: bx lr
```
