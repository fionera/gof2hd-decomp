# PlayerGasCloud::explode
elf 0x176660 body 702
Sig: undefined __stdcall explode(int param_1, Vector param_2, float param_3)

## decompile
```c

/* PlayerGasCloud::explode(int, AbyssEngine::AEMath::Vector, float) */

void PlayerGasCloud::explode
               (int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
               float param_6)

{
  float fVar1;
  float fVar2;
  Array<AEGeometry*> *this;
  Array<AbyssEngine::AEMath::Vector*> *this_00;
  Array<float> *pAVar3;
  void *pvVar4;
  Array<bool> *this_01;
  float fVar5;
  undefined4 uVar6;
  AEGeometry *this_02;
  undefined4 uVar7;
  int iVar8;
  Vector *pVVar9;
  int *piVar10;
  code *pcVar11;
  int iVar12;
  undefined4 *puVar13;
  uint in_fpscr;
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
  Vector aVStack_ac [12];
  undefined8 local_a0;
  undefined4 local_98;
  float local_90;
  float local_8c;
  float local_88;
  float local_84;
  float local_80;
  float local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 uStack_70;
  int local_6c;
  
  piVar10 = *(int **)(DAT_00186950 + 0x18667a);
  local_6c = *piVar10;
  uStack_70 = param_5;
  local_78 = param_3;
  local_74 = param_4;
  if (*(char *)(param_1 + 0x154) == '\0') {
    *(undefined4 *)(param_1 + 0x88) = 3;
    KIPlayer::setActive(SUB41(param_1,0));
    *(undefined1 *)(param_1 + 0x154) = 1;
    this = operator_new(0xc);
    Array<AEGeometry*>::Array(this);
    *(Array<AEGeometry*> **)(param_1 + 0x138) = this;
    this_00 = operator_new(0xc);
    Array<AbyssEngine::AEMath::Vector*>::Array(this_00);
    *(Array<AbyssEngine::AEMath::Vector*> **)(param_1 + 0x13c) = this_00;
    pAVar3 = operator_new(0xc);
    Array<float>::Array(pAVar3);
    *(Array<float> **)(param_1 + 0x140) = pAVar3;
    pAVar3 = operator_new(0xc);
    Array<float>::Array(pAVar3);
    *(Array<float> **)(param_1 + 0x144) = pAVar3;
    pvVar4 = operator_new(0xc);
    Array<int>::Array();
    *(void **)(param_1 + 0x14c) = pvVar4;
    this_01 = operator_new(0xc);
    Array<bool>::Array(this_01);
    *(Array<bool> **)(param_1 + 0x150) = this_01;
    pAVar3 = operator_new(0xc);
    Array<float>::Array(pAVar3);
    *(Array<float> **)(param_1 + 0x148) = pAVar3;
    AbyssEngine::AEMath::operator-((Vector *)&local_84,(Vector *)&local_78);
    fVar5 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_84);
    fVar18 = 1.5 - fVar5 / param_6;
    fVar20 = fVar18 * DAT_00186940;
    uVar6 = Item::getAttribute(*(int *)(*(int *)(**(int **)(DAT_00186954 + 0x186742) + 4) +
                                       param_2 * 4));
    fVar5 = DAT_0018694c;
    iVar12 = 0;
    fVar14 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
    fVar14 = fVar14 / DAT_00186944;
    fVar19 = fVar18 * DAT_00186948;
    for (; iVar12 < (int)((fVar20 / 1.5 + 10.0) * fVar14); iVar12 = iVar12 + 1) {
      this_02 = operator_new(0xc0);
      AEGeometry::AEGeometry
                (this_02,*(ushort *)(param_1 + 0x164),
                 (PaintCanvas *)**(undefined4 **)(DAT_00186958 + 0x1867b0),false);
      AEGeometry::setPosition(this_02,(Vector *)(param_1 + 0x128));
      fVar16 = local_84;
      fVar22 = *(float *)(param_1 + 0x128);
      puVar13 = *(undefined4 **)(DAT_0018695c + 0x1867cc);
      pcVar11 = *(code **)(DAT_00186960 + 0x1867d2);
      uVar6 = (*pcVar11)(*puVar13,10000);
      fVar1 = local_80;
      fVar23 = *(float *)(param_1 + 300);
      uVar7 = (*pcVar11)(*puVar13,10000);
      fVar2 = local_7c;
      fVar17 = *(float *)(param_1 + 0x130);
      fVar24 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
      fVar21 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
      uVar6 = (*pcVar11)(*puVar13,10000);
      fVar15 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
      local_90 = ((fVar22 + fVar16) - fVar19) + fVar18 * fVar24;
      local_8c = ((fVar23 + fVar1) - fVar19) + fVar18 * fVar21;
      local_88 = (fVar17 - fVar19) + fVar2 + fVar18 * fVar15;
      AbyssEngine::AEMath::operator-(aVStack_ac,(Vector *)(param_1 + 0x128));
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_a0,aVStack_ac);
      uVar6 = (*pcVar11)(*puVar13,200);
      fVar16 = (float)VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
      fVar16 = (fVar16 / fVar5) * 3.0 + 3.0;
      iVar8 = (*pcVar11)(*puVar13,14000);
      pcVar11 = *(code **)(DAT_00186964 + 0x1868a4);
      (*pcVar11)(fVar16 * 7.0,*(undefined4 *)(param_1 + 0x140));
      (*pcVar11)(fVar16,*(undefined4 *)(param_1 + 0x144));
      ArrayAdd<int>(iVar8 + 8000,*(Array **)(param_1 + 0x14c));
      pVVar9 = operator_new(0xc);
      *(undefined4 *)(pVVar9 + 8) = local_98;
      *(undefined8 *)pVVar9 = local_a0;
      ArrayAdd<AbyssEngine::AEMath::Vector*>(pVVar9,*(Array **)(param_1 + 0x13c));
      ArrayAdd<AEGeometry*>(this_02,*(Array **)(param_1 + 0x138));
      ArrayAdd<bool>(false,*(Array **)(param_1 + 0x150));
      (*pcVar11)(0x3f800000,*(undefined4 *)(param_1 + 0x148));
    }
  }
  if (*piVar10 == local_6c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00186660: push {r4,r5,r6,r7,lr}
  00186662: add r7,sp,#0xc
  00186664: push {r7,r8,r9,r10,r11}
  00186668: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  0018666c: sub sp,#0x50
  0018666e: mov r11,r0
  00186670: ldr r0,[0x00186950]
  00186672: str r2,[sp,#0x40]
  00186674: mov r8,r1
  00186676: add r0,pc
  00186678: ldr r4,[r0,#0x0]
  0018667a: ldr r0,[r7,#0x8]
  0018667c: ldr r1,[r4,#0x0]
  0018667e: strd r3,r0,[sp,#0x44]
  00186682: str r1,[sp,#0x4c]
  00186684: ldrb.w r0,[r11,#0x154]
  00186688: cmp r0,#0x0
  0018668a: bne.w 0x00186902
  0018668e: movs r0,#0x3
  00186690: movs r1,#0x0
  00186692: str.w r0,[r11,#0x88]
  00186696: mov r0,r11
  00186698: blx 0x000732f4
  0018669c: movs r0,#0x1
  0018669e: strb.w r0,[r11,#0x154]
  001866a2: movs r0,#0xc
  001866a4: blx 0x0006eb24
  001866a8: mov r5,r0
  001866aa: blx 0x000717b8
  001866ae: movs r0,#0xc
  001866b0: str.w r5,[r11,#0x138]
  001866b4: blx 0x0006eb24
  001866b8: mov r5,r0
  001866ba: blx 0x00074530
  001866be: movs r0,#0xc
  001866c0: str.w r5,[r11,#0x13c]
  001866c4: blx 0x0006eb24
  001866c8: mov r5,r0
  001866ca: blx 0x00073510
  001866ce: movs r0,#0xc
  001866d0: str.w r5,[r11,#0x140]
  001866d4: blx 0x0006eb24
  001866d8: mov r5,r0
  001866da: blx 0x00073510
  001866de: movs r0,#0xc
  001866e0: str.w r5,[r11,#0x144]
  001866e4: blx 0x0006eb24
  001866e8: mov r5,r0
  001866ea: blx 0x000701f8
  001866ee: movs r0,#0xc
  001866f0: str.w r5,[r11,#0x14c]
  001866f4: blx 0x0006eb24
  001866f8: mov r5,r0
  001866fa: blx 0x000735a0
  001866fe: movs r0,#0xc
  00186700: str r4,[sp,#0x0]
  00186702: str.w r5,[r11,#0x150]
  00186706: blx 0x0006eb24
  0018670a: mov r5,r0
  0018670c: blx 0x00073510
  00186710: add r6,sp,#0x34
  00186712: add.w r2,r11,#0x128
  00186716: add r1,sp,#0x40
  00186718: vldr.32 s16,[r7,#0xc]
  0018671c: mov r0,r6
  0018671e: str.w r5,[r11,#0x148]
  00186722: str r2,[sp,#0x4]
  00186724: blx 0x0006ec38
  00186728: mov r0,r6
  0018672a: blx 0x0006ec44
  0018672e: vmov s0,r0
  00186732: ldr r0,[0x00186954]
  00186734: vmov.f32 s18,0x3fc00000
  00186738: movs r1,#0x38
  0018673a: vdiv.f32 s0,s0,s16
  0018673e: add r0,pc
  00186740: ldr r0,[r0,#0x0]
  00186742: ldr r0,[r0,#0x0]
  00186744: ldr r0,[r0,#0x4]
  00186746: ldr.w r0,[r0,r8,lsl #0x2]
  0018674a: vsub.f32 s16,s18,s0
  0018674e: vldr.32 s0,[pc,#0x1f0]
  00186752: vmul.f32 s20,s16,s0
  00186756: blx 0x000718e4
  0018675a: vmov s0,r0
  0018675e: vldr.32 s2,[pc,#0x1e4]
  00186762: vdiv.f32 s4,s20,s18
  00186766: mov.w r9,#0x0
  0018676a: vcvt.f32.s32 s0,s0
  0018676e: vldr.32 s20,[pc,#0x1dc]
  00186772: vmov.f32 s22,0x40400000
  00186776: vmov.f32 s24,0x40e00000
  0018677a: vdiv.f32 s0,s0,s2
  0018677e: vmov.f32 s2,0x41200000
  00186782: vadd.f32 s2,s4,s2
  00186786: vmul.f32 s0,s2,s0
  0018678a: vldr.32 s2,[pc,#0x1bc]
  0018678e: vmul.f32 s18,s16,s2
  00186792: vcvt.s32.f32 s0,s0
  00186796: vmov r0,s0
  0018679a: str r0,[sp,#0x8]
  0018679c: b 0x001868f8
  0018679e: movs r0,#0xc0
  001867a0: blx 0x0006eb24
  001867a4: mov r6,r0
  001867a6: ldr r0,[0x00186958]
  001867a8: ldrh.w r1,[r11,#0x164]
  001867ac: add r0,pc
  001867ae: ldr r0,[r0,#0x0]
  001867b0: ldr r2,[r0,#0x0]
  001867b2: mov r0,r6
  001867b4: movs r3,#0x0
  001867b6: blx 0x0007207c
  001867ba: ldr r4,[sp,#0x4]
  001867bc: mov r0,r6
  001867be: mov r1,r4
  001867c0: blx 0x00072148
  001867c4: ldr r0,[0x0018695c]
  001867c6: ldr r1,[0x00186960]
  001867c8: add r0,pc
  001867ca: vldr.32 s26,[r11,#0x128]
  001867ce: add r1,pc
  001867d0: vldr.32 s28,[sp,#0x34]
  001867d4: ldr.w r10,[r0,#0x0]
  001867d8: ldr.w r8,[r1,#0x0]
  001867dc: movw r1,#0x2710
  001867e0: ldr.w r0,[r10,#0x0]
  001867e4: blx r8
  001867e6: mov r5,r0
  001867e8: ldr.w r0,[r10,#0x0]
  001867ec: movw r1,#0x2710
  001867f0: vldr.32 s30,[r11,#0x12c]
  001867f4: vldr.32 s17,[sp,#0x38]
  001867f8: blx r8
  001867fa: vadd.f32 s0,s26,s28
  001867fe: vldr.32 s2,[r11,#0x130]
  00186802: vmov s4,r5
  00186806: vadd.f32 s26,s30,s17
  0018680a: vcvt.f32.s32 s30,s4
  0018680e: ldr.w r1,[r10,#0x0]
  00186812: vsub.f32 s28,s2,s18
  00186816: vldr.32 s19,[sp,#0x3c]
  0018681a: vsub.f32 s17,s0,s18
  0018681e: vmov s0,r0
  00186822: mov r0,r1
  00186824: movw r1,#0x2710
  00186828: vcvt.f32.s32 s21,s0
  0018682c: blx r8
  0018682e: vmov s0,r0
  00186832: add r5,sp,#0xc
  00186834: vsub.f32 s2,s26,s18
  00186838: add r2,sp,#0x28
  0018683a: vcvt.f32.s32 s0,s0
  0018683e: mov r0,r5
  00186840: vadd.f32 s4,s28,s19
  00186844: mov r1,r4
  00186846: vmla.f32 s17,s16,s30
  0018684a: vmla.f32 s2,s16,s21
  0018684e: vmla.f32 s4,s16,s0
  00186852: vstr.32 s17,[sp,#0x28]
  00186856: vstr.32 s2,[sp,#0x2c]
  0018685a: vstr.32 s4,[sp,#0x30]
  0018685e: blx 0x0006ec38
  00186862: add r0,sp,#0x18
  00186864: mov r1,r5
  00186866: blx 0x0006ec80
  0018686a: ldr.w r0,[r10,#0x0]
  0018686e: movs r1,#0xc8
  00186870: blx r8
  00186872: vmov s0,r0
  00186876: movw r1,#0x36b0
  0018687a: vmov.f32 s26,s22
  0018687e: vcvt.f32.s32 s0,s0
  00186882: ldr.w r0,[r10,#0x0]
  00186886: vdiv.f32 s0,s0,s20
  0018688a: vmla.f32 s26,s0,s22
  0018688e: vmul.f32 s28,s26,s24
  00186892: blx r8
  00186894: mov r5,r0
  00186896: vmov r0,s28
  0018689a: ldr r2,[0x00186964]
  0018689c: ldr.w r1,[r11,#0x140]
  001868a0: add r2,pc
  001868a2: ldr r4,[r2,#0x0]
  001868a4: blx r4
  001868a6: vmov r0,s26
  001868aa: ldr.w r1,[r11,#0x144]
  001868ae: blx r4
  001868b0: ldr.w r1,[r11,#0x14c]
  001868b4: add.w r0,r5,#0x1f40
  001868b8: blx 0x0007021c
  001868bc: movs r0,#0xc
  001868be: blx 0x0006eb24
  001868c2: vldr.64 d16,[sp,#0x18]
  001868c6: ldr r1,[sp,#0x20]
  001868c8: str r1,[r0,#0x8]
  001868ca: vstr.64 d16,[r0]
  001868ce: ldr.w r1,[r11,#0x13c]
  001868d2: blx 0x000763c0
  001868d6: ldr.w r1,[r11,#0x138]
  001868da: mov r0,r6
  001868dc: blx 0x000717dc
  001868e0: ldr.w r1,[r11,#0x150]
  001868e4: movs r0,#0x0
  001868e6: blx 0x00077fbc
  001868ea: ldr.w r1,[r11,#0x148]
  001868ee: mov.w r0,#0x3f800000
  001868f2: blx r4
  001868f4: add.w r9,r9,#0x1
  001868f8: ldr r0,[sp,#0x8]
  001868fa: cmp r9,r0
  001868fc: blt.w 0x0018679e
  00186900: ldr r4,[sp,#0x0]
  00186902: ldr r0,[sp,#0x4c]
  00186904: ldr r1,[r4,#0x0]
  00186906: subs r0,r1,r0
  00186908: itttt eq
  0018690a: add.eq sp,#0x50
  0018690c: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  00186910: add.eq sp,#0x4
  00186912: pop.eq.w {r8,r9,r10,r11}
  00186916: it eq
  00186918: pop.eq {r4,r5,r6,r7,pc}
  0018691a: blx 0x0006e824
```
