# AbyssEngine::AEMath::operator*
elf 0x802c8 body 378
Sig: undefined __stdcall operator*(Matrix * param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::operator*(Matrix *param_1,Matrix *param_2)

{
  undefined8 uVar1;
  float *in_r2;
  int *piVar2;
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
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  Vector aVStack_68 [12];
  int local_5c;
  
  piVar2 = *(int **)(DAT_00090444 + 0x902de);
  local_5c = *piVar2;
  fVar10 = *(float *)(param_2 + 0x24);
  fVar12 = in_r2[6];
  fVar24 = *(float *)(param_2 + 4);
  fVar14 = *(float *)(param_2 + 0x14);
  fVar19 = in_r2[5];
  fVar16 = in_r2[7];
  fVar3 = in_r2[4];
  fVar9 = in_r2[2];
  fVar13 = *(float *)(param_2 + 0x20);
  fVar15 = *(float *)(param_2 + 0x10);
  fVar23 = *(float *)param_2;
  fVar11 = in_r2[3];
  fVar8 = *(float *)(param_2 + 0x28);
  fVar18 = *(float *)(param_2 + 0x18);
  fVar20 = in_r2[0xb];
  fVar25 = *(float *)(param_2 + 8);
  fVar6 = in_r2[1];
  fVar4 = *in_r2;
  fVar17 = in_r2[10];
  uVar1 = *(undefined8 *)(in_r2 + 8);
  fVar26 = *(float *)(param_2 + 0xc);
  fVar7 = *(float *)(param_2 + 0x1c);
  fVar5 = *(float *)(param_2 + 0x2c);
  fVar21 = (float)uVar1;
  *(undefined4 *)(param_1 + 0x34) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x38) = 0x3f800000;
  fVar22 = (float)((ulonglong)uVar1 >> 0x20);
  *(float *)param_1 = fVar24 * fVar3 + fVar23 * fVar4 + fVar25 * fVar21;
  *(float *)(param_1 + 4) = fVar24 * fVar19 + fVar23 * fVar6 + fVar25 * fVar22;
  *(float *)(param_1 + 8) = fVar24 * fVar12 + fVar23 * fVar9 + fVar25 * fVar17;
  *(float *)(param_1 + 0xc) = fVar26 + fVar24 * fVar16 + fVar23 * fVar11 + fVar25 * fVar20;
  *(float *)(param_1 + 0x10) = fVar3 * fVar14 + fVar4 * fVar15 + fVar21 * fVar18;
  *(float *)(param_1 + 0x14) = fVar19 * fVar14 + fVar6 * fVar15 + fVar22 * fVar18;
  *(float *)(param_1 + 0x18) = fVar12 * fVar14 + fVar9 * fVar15 + fVar17 * fVar18;
  *(float *)(param_1 + 0x1c) = fVar7 + fVar16 * fVar14 + fVar11 * fVar15 + fVar20 * fVar18;
  *(float *)(param_1 + 0x20) = fVar3 * fVar10 + fVar4 * fVar13 + fVar21 * fVar8;
  *(float *)(param_1 + 0x24) = fVar19 * fVar10 + fVar6 * fVar13 + fVar22 * fVar8;
  *(float *)(param_1 + 0x28) = fVar12 * fVar10 + fVar9 * fVar13 + fVar17 * fVar8;
  *(float *)(param_1 + 0x2c) = fVar5 + fVar16 * fVar10 + fVar11 * fVar13 + fVar20 * fVar8;
  *(undefined4 *)(param_1 + 0x30) = 0x3f800000;
  operator*(aVStack_68,(Vector *)(param_2 + 0x30));
  Vector::operator=((Vector *)(param_1 + 0x30),aVStack_68);
  if (*piVar2 != local_5c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000902c8: push {r4,r5,r6,r7,lr}
  000902ca: add r7,sp,#0xc
  000902cc: push.w r11
  000902d0: vpush {d6,d7,d8,d9,d10,d11,d12,d13,d14,d15}
  000902d4: mov r4,r0
  000902d6: ldr r0,[0x00090444]
  000902d8: mov r5,sp
  000902da: add r0,pc
  000902dc: ldr r6,[r0,#0x0]
  000902de: ldr r0,[r6,#0x0]
  000902e0: str r0,[sp,#0xc]
  000902e2: mov.w r0,#0x3f800000
  000902e6: vldr.32 s8,[r1,#0x24]
  000902ea: vldr.32 s10,[r2,#0x18]
  000902ee: vldr.32 s24,[r1,#0x4]
  000902f2: vldr.32 s12,[r1,#0x14]
  000902f6: vmul.f32 s0,s10,s8
  000902fa: vmul.f32 s30,s24,s10
  000902fe: vldr.32 s17,[r2,#0x14]
  00090302: vmul.f32 s10,s10,s12
  00090306: vldr.32 s14,[r2,#0x1c]
  0009030a: vldr.32 s1,[r2,#0x10]
  0009030e: vmul.f32 s19,s24,s17
  00090312: vldr.32 s7,[r2,#0x8]
  00090316: vmul.f32 s4,s14,s8
  0009031a: vldr.32 s11,[r1,#0x20]
  0009031e: vmul.f32 s2,s1,s12
  00090322: vldr.32 s13,[r1,#0x10]
  00090326: vmul.f32 s23,s17,s8
  0009032a: vldr.32 s22,[r1]
  0009032e: vmla.f32 s0,s7,s11
  00090332: vmla.f32 s10,s7,s13
  00090336: vldr.32 s9,[r2,#0xc]
  0009033a: vmla.f32 s30,s22,s7
  0009033e: vldr.32 s6,[r1,#0x28]
  00090342: vmul.f32 s7,s17,s12
  00090346: vldr.32 s16,[r1,#0x18]
  0009034a: vmul.f32 s12,s14,s12
  0009034e: vldr.32 s18,[r2,#0x2c]
  00090352: vmul.f32 s14,s24,s14
  00090356: vldr.32 s26,[r1,#0x8]
  0009035a: vmla.f32 s4,s9,s11
  0009035e: vldr.32 s5,[r2,#0x4]
  00090362: vmul.f32 s8,s1,s8
  00090366: vldr.32 s3,[r2]
  0009036a: vmul.f32 s1,s24,s1
  0009036e: vldr.32 s15,[r2,#0x28]
  00090372: vmla.f32 s2,s3,s13
  00090376: vldr.32 s20,[r2,#0x20]
  0009037a: vmla.f32 s12,s9,s13
  0009037e: vldr.32 s21,[r2,#0x24]
  00090382: vmla.f32 s14,s22,s9
  00090386: vldr.32 s28,[r1,#0xc]
  0009038a: vmla.f32 s4,s18,s6
  0009038e: adds r2,#0x30
  00090390: vmla.f32 s19,s22,s5
  00090394: vmla.f32 s23,s5,s11
  00090398: vmla.f32 s7,s5,s13
  0009039c: vldr.32 s5,[r1,#0x1c]
  000903a0: vmla.f32 s12,s18,s16
  000903a4: vmla.f32 s14,s26,s18
  000903a8: vmla.f32 s8,s3,s11
  000903ac: vmla.f32 s1,s22,s3
  000903b0: vldr.32 s3,[r1,#0x2c]
  000903b4: vmla.f32 s0,s15,s6
  000903b8: adds r1,#0x30
  000903ba: vmla.f32 s2,s20,s16
  000903be: strd r0,r0,[r4,#0x34]
  000903c2: vmla.f32 s30,s26,s15
  000903c6: vmla.f32 s19,s26,s21
  000903ca: vmla.f32 s7,s21,s16
  000903ce: vmla.f32 s1,s26,s20
  000903d2: vmla.f32 s10,s15,s16
  000903d6: vmla.f32 s8,s20,s6
  000903da: vmla.f32 s23,s21,s6
  000903de: vadd.f32 s6,s28,s14
  000903e2: vstr.32 s1,[r4]
  000903e6: vadd.f32 s12,s5,s12
  000903ea: vstr.32 s19,[r4,#0x4]
  000903ee: vadd.f32 s4,s3,s4
  000903f2: vstr.32 s30,[r4,#0x8]
  000903f6: vstr.32 s6,[r4,#0xc]
  000903fa: vstr.32 s2,[r4,#0x10]
  000903fe: vstr.32 s7,[r4,#0x14]
  00090402: vstr.32 s10,[r4,#0x18]
  00090406: vstr.32 s12,[r4,#0x1c]
  0009040a: vstr.32 s8,[r4,#0x20]
  0009040e: vstr.32 s23,[r4,#0x24]
  00090412: vstr.32 s0,[r4,#0x28]
  00090416: vstr.32 s4,[r4,#0x2c]
  0009041a: str.w r0,[r4,#0x30]!
  0009041e: mov r0,r5
  00090420: blx 0x000701a4
  00090424: mov r0,r4
  00090426: mov r1,r5
  00090428: blx 0x0006eb3c
  0009042c: ldr r0,[sp,#0xc]
  0009042e: ldr r1,[r6,#0x0]
  00090430: subs r0,r1,r0
  00090432: ittt eq
  00090434: vpop.eq {d6,d7,d8,d9,d10,d11,d12,d13,d14,d15}
  00090438: pop.eq.w r11
  0009043c: pop.eq {r4,r5,r6,r7,pc}
  0009043e: blx 0x0006e824
```
