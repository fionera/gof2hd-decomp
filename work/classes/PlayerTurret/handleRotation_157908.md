# PlayerTurret::handleRotation
elf 0x157908 body 668
Sig: undefined __stdcall handleRotation(int param_1, AEGeometry * param_2, AEGeometry * param_3)

## decompile
```c

/* PlayerTurret::handleRotation(int, AEGeometry*, AEGeometry*) */

void PlayerTurret::handleRotation(int param_1,AEGeometry *param_2,AEGeometry *param_3)

{
  uint uVar1;
  byte bVar2;
  bool bVar3;
  Matrix *pMVar4;
  undefined4 in_r3;
  int *piVar5;
  code *pcVar6;
  uint in_fpscr;
  uint uVar7;
  float fVar8;
  float fVar9;
  float fVar11;
  float extraout_s1;
  float extraout_s1_00;
  float fVar12;
  longlong lVar13;
  Vector aVStack_10c [60];
  float local_d0;
  float local_cc;
  Matrix aMStack_94 [12];
  AEMath aAStack_88 [12];
  Vector aVStack_7c [12];
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  int local_34;
  AEGeometry *pAVar10;
  
  piVar5 = *(int **)(DAT_00167bc0 + 0x167926);
  local_34 = *piVar5;
  __aeabi_memcpy(&local_70,*(int *)(param_1 + 0x14c) + 4,0x3c);
  Player::getPosition();
  AbyssEngine::AEMath::MatrixGetDir(aMStack_94);
  fVar8 = (float)AbyssEngine::AEMath::VectorNormalize(aAStack_88,(Vector *)aMStack_94);
  AbyssEngine::AEMath::operator*(aVStack_7c,fVar8);
  AbyssEngine::AEMath::operator+((Vector *)&local_d0,aVStack_10c);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x9c),(Vector *)&local_d0);
  if (*(char *)(param_1 + 0x3f) == '\0') {
    pcVar6 = *(code **)(DAT_00167bc4 + 0x16799c);
    pMVar4 = (Matrix *)(*pcVar6)(*(undefined4 *)(param_1 + 8));
    (*pcVar6)(param_3);
    AbyssEngine::AEMath::operator*((Matrix *)aVStack_10c,pMVar4);
    (*pcVar6)(in_r3);
    pMVar4 = (Matrix *)&local_d0;
    AbyssEngine::AEMath::operator*(pMVar4,(Matrix *)aVStack_10c);
  }
  else {
    pMVar4 = (Matrix *)AEGeometry::getMatrix();
  }
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)&local_70,pMVar4);
  AbyssEngine::AEMath::MatrixInverseTransformVector((Matrix *)aVStack_10c,(Vector *)&local_70);
  AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_d0,aVStack_10c);
  fVar8 = DAT_00167ba8;
  in_fpscr = in_fpscr & 0xfffffff;
  uVar1 = in_fpscr | (uint)(local_d0 < DAT_00167ba8) << 0x1f |
          (uint)(local_d0 == DAT_00167ba8) << 0x1e;
  uVar7 = uVar1 | (uint)(NAN(local_d0) || NAN(DAT_00167ba8)) << 0x1c;
  bVar2 = (byte)(uVar1 >> 0x18);
  pAVar10 = param_2;
  if ((bool)(bVar2 >> 6 & 1) || bVar2 >> 7 != ((byte)(uVar7 >> 0x1c) & 1)) {
    if (local_d0 < DAT_00167bac) {
      uVar7 = in_fpscr;
      pAVar10 = (AEGeometry *)-(int)param_2;
      goto LAB_00167a18;
    }
    bVar3 = true;
    fVar9 = extraout_s1;
  }
  else {
LAB_00167a18:
    in_fpscr = uVar7;
    fVar9 = (float)VectorSignedToFloat(pAVar10,(byte)(in_fpscr >> 0x16) & 3);
    bVar3 = false;
    AEGeometry::rotate(fVar9 * DAT_00167bb0 * DAT_00167bb4,extraout_s1,DAT_00167bb4);
    fVar9 = extraout_s1_00;
  }
  in_fpscr = in_fpscr & 0xfffffff;
  uVar1 = in_fpscr | (uint)(local_cc < fVar8) << 0x1f | (uint)(local_cc == fVar8) << 0x1e;
  bVar2 = (byte)(uVar1 >> 0x18);
  if ((bool)(bVar2 >> 6 & 1) || (bool)(bVar2 >> 7) != (NAN(local_cc) || NAN(fVar8))) {
    if (DAT_00167bac <= local_cc) {
      if (bVar3) {
        Player::shoot(*(undefined4 *)(param_1 + 4),0,param_2,(int)param_2 >> 0x1f,0,local_70,
                      local_6c,local_68,uStack_64,local_60,uStack_5c,local_58,uStack_54,local_50,
                      uStack_4c,uStack_48,local_44,uStack_40,uStack_3c,local_38);
        lVar13 = AbyssEngine::PaintCanvas::TransformGetTransform
                           (**(uint **)(DAT_00167bc8 + 0x167b2e));
        AbyssEngine::Transform::Update(lVar13,SUB41(param_2,0));
      }
      goto LAB_00167b86;
    }
    if ((*(char *)(param_1 + 0x3f) == '\0') && (99 < *(int *)(param_1 + 0x134))) goto LAB_00167aa6;
    fVar8 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x124),(byte)(in_fpscr >> 0x16) & 3
                                      );
    fVar12 = fVar8 * DAT_00167bb0 * DAT_00167bb4;
    fVar11 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x134),(byte)(in_fpscr >> 0x16) & 3);
    fVar8 = fVar8 + fVar11;
  }
  else {
    if ((*(char *)(param_1 + 0x3f) == '\0') && (*(int *)(param_1 + 0x134) < DAT_00167bb8)) {
LAB_00167aa6:
      *(undefined4 *)(param_1 + 0x150) = *(undefined4 *)(param_1 + 0x14c);
      *(AEGeometry **)(param_1 + 0x130) = param_2 + *(int *)(param_1 + 0x130);
      goto LAB_00167b86;
    }
    fVar11 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x124),(byte)(uVar1 >> 0x16) & 3);
    fVar12 = fVar11 * DAT_00167bb0 * DAT_00167bbc;
    fVar8 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x134),(byte)(uVar1 >> 0x16) & 3);
    fVar8 = fVar8 - fVar11;
  }
  *(int *)(param_1 + 0x134) = (int)fVar8;
  AEGeometry::rotate((float)(int)fVar8,fVar9,fVar12);
  *(undefined4 *)(param_1 + 0x150) = 0;
LAB_00167b86:
  if (*piVar5 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00167908: push {r4,r5,r6,r7,lr}
  0016790a: add r7,sp,#0xc
  0016790c: push {r7,r8,r9,r10,r11}
  00167910: vpush {d8}
  00167914: sub sp,#0x138
  00167916: str r3,[sp,#0x58]
  00167918: mov r6,r0
  0016791a: ldr r0,[0x00167bc0]
  0016791c: add r4,sp,#0xf8
  0016791e: mov r9,r1
  00167920: mov r10,r2
  00167922: add r0,pc
  00167924: movs r2,#0x3c
  00167926: ldr.w r8,[r0,#0x0]
  0016792a: ldr.w r0,[r8,#0x0]
  0016792e: str r0,[sp,#0x134]
  00167930: mov r0,r4
  00167932: ldr.w r5,[r6,#0x14c]
  00167936: adds r1,r5,#0x4
  00167938: blx 0x0006f1e4
  0016793c: add.w r11,sp,#0x5c
  00167940: mov r1,r5
  00167942: mov r0,r11
  00167944: blx 0x00072964
  00167948: add r5,sp,#0xd4
  0016794a: mov r1,r4
  0016794c: mov r0,r5
  0016794e: blx 0x0006f4c0
  00167952: add r4,sp,#0xe0
  00167954: mov r1,r5
  00167956: mov r0,r4
  00167958: blx 0x0006ec80
  0016795c: add r5,sp,#0xec
  0016795e: ldr r2,[0x00167ba4]
  00167960: mov r1,r4
  00167962: mov r0,r5
  00167964: blx 0x0006ec74
  00167968: add r4,sp,#0x98
  0016796a: mov r1,r11
  0016796c: mov r2,r5
  0016796e: mov r0,r4
  00167970: blx 0x0006f1cc
  00167974: add.w r5,r6,#0x9c
  00167978: mov r1,r4
  0016797a: mov r0,r5
  0016797c: blx 0x0006eb3c
  00167980: ldrb.w r0,[r6,#0x3f]
  00167984: cbz r0,0x00167994
  00167986: ldr r0,[r6,#0x8]
  00167988: mov r11,r10
  0016798a: blx 0x000721cc
  0016798e: mov r1,r0
  00167990: add r0,sp,#0xf8
  00167992: b 0x001679d2
  00167994: ldr r1,[0x00167bc4]
  00167996: ldr r0,[r6,#0x8]
  00167998: add r1,pc
  0016799a: str.w r9,[sp,#0x54]
  0016799e: ldr.w r9,[r1,#0x0]
  001679a2: blx r9
  001679a4: mov r4,r0
  001679a6: mov r0,r10
  001679a8: mov r11,r10
  001679aa: blx r9
  001679ac: add.w r10,sp,#0x5c
  001679b0: mov r2,r0
  001679b2: mov r1,r4
  001679b4: mov r0,r10
  001679b6: blx 0x0006f82c
  001679ba: ldr r0,[sp,#0x58]
  001679bc: blx r9
  001679be: add r4,sp,#0x98
  001679c0: mov r2,r0
  001679c2: mov r1,r10
  001679c4: ldr.w r9,[sp,#0x54]
  001679c8: mov r0,r4
  001679ca: blx 0x0006f82c
  001679ce: add r0,sp,#0xf8
  001679d0: mov r1,r4
  001679d2: blx 0x0006f148
  001679d6: add r4,sp,#0x5c
  001679d8: add r1,sp,#0xf8
  001679da: mov r2,r5
  001679dc: mov r0,r4
  001679de: blx 0x0006f4b4
  001679e2: add r0,sp,#0x98
  001679e4: mov r1,r4
  001679e6: blx 0x0006ec80
  001679ea: vldr.32 s16,[pc,#0x1bc]
  001679ee: vldr.32 s0,[sp,#0x98]
  001679f2: vcmpe.f32 s0,s16
  001679f6: vmrs apsr,fpscr
  001679fa: ble 0x00167a02
  001679fc: vmov s0,r9
  00167a00: b 0x00167a18
  00167a02: vldr.32 s2,[pc,#0x1a8]
  00167a06: vcmpe.f32 s0,s2
  00167a0a: vmrs apsr,fpscr
  00167a0e: bpl 0x00167a3e
  00167a10: rsb.w r0,r9,#0x0
  00167a14: vmov s0,r0
  00167a18: vcvt.f32.s32 s0,s0
  00167a1c: vldr.32 s2,[pc,#0x190]
  00167a20: mov r0,r11
  00167a22: movs r1,#0x0
  00167a24: movs r3,#0x0
  00167a26: movs r4,#0x0
  00167a28: vmul.f32 s0,s0,s2
  00167a2c: vldr.32 s2,[pc,#0x184]
  00167a30: vmul.f32 s0,s0,s2
  00167a34: vmov r2,s0
  00167a38: blx 0x00072160
  00167a3c: b 0x00167a40
  00167a3e: movs r4,#0x1
  00167a40: vldr.32 s0,[sp,#0x9c]
  00167a44: vcmpe.f32 s0,s16
  00167a48: vmrs apsr,fpscr
  00167a4c: ble 0x00167a88
  00167a4e: ldrb.w r1,[r6,#0x3f]
  00167a52: ldr.w r0,[r6,#0x134]
  00167a56: cbnz r1,0x00167a5e
  00167a58: ldr r1,[0x00167bb8]
  00167a5a: cmp r0,r1
  00167a5c: blt 0x00167aa6
  00167a5e: vldr.32 s0,[r6,#0x124]
  00167a62: vldr.32 s2,[pc,#0x14c]
  00167a66: vcvt.f32.s32 s0,s0
  00167a6a: vldr.32 s4,[pc,#0x150]
  00167a6e: vmul.f32 s2,s0,s2
  00167a72: vmul.f32 s2,s2,s4
  00167a76: vmov s4,r0
  00167a7a: vcvt.f32.s32 s4,s4
  00167a7e: vmov r1,s2
  00167a82: vsub.f32 s0,s4,s0
  00167a86: b 0x00167b6e
  00167a88: vldr.32 s2,[pc,#0x120]
  00167a8c: vcmpe.f32 s0,s2
  00167a90: vmrs apsr,fpscr
  00167a94: bpl 0x00167aba
  00167a96: ldrb.w r1,[r6,#0x3f]
  00167a9a: ldr.w r0,[r6,#0x134]
  00167a9e: cmp r1,#0x0
  00167aa0: bne 0x00167b46
  00167aa2: cmp r0,#0x63
  00167aa4: ble 0x00167b46
  00167aa6: ldr.w r0,[r6,#0x130]
  00167aaa: ldr.w r1,[r6,#0x14c]
  00167aae: str.w r1,[r6,#0x150]
  00167ab2: add r0,r9
  00167ab4: str.w r0,[r6,#0x130]
  00167ab8: b 0x00167b86
  00167aba: cmp r4,#0x0
  00167abc: beq 0x00167b86
  00167abe: ldr r0,[sp,#0xf8]
  00167ac0: add r4,sp,#0x118
  00167ac2: strd r0,r8,[sp,#0x50]
  00167ac6: add.w lr,sp,#0x124
  00167aca: ldr r0,[sp,#0xfc]
  00167acc: mov r10,r9
  00167ace: str r0,[sp,#0x4c]
  00167ad0: ldrd r8,r0,[sp,#0x100]
  00167ad4: str r0,[sp,#0x48]
  00167ad6: ldrd r11,r0,[sp,#0x108]
  00167ada: ldr.w r9,[r6,#0x4]
  00167ade: str r0,[sp,#0x44]
  00167ae0: ldrd r6,r1,[sp,#0x110]
  00167ae4: ldmia r4,{r2,r3,r4}
  00167ae6: ldm.w lr,{r0,r12,lr}
  00167aea: str r1,[sp,#0x20]
  00167aec: add r1,sp,#0x30
  00167aee: str r2,[sp,#0x24]
  00167af0: mov r2,r10
  00167af2: str r4,[sp,#0x2c]
  00167af4: asr.w r4,r10, asr #0x1f
  00167af8: str r3,[sp,#0x28]
  00167afa: mov r3,r4
  00167afc: ldr r5,[sp,#0x130]
  00167afe: stm r1,{r0,r12,lr}
  00167b02: movs r1,#0x0
  00167b04: ldr r0,[sp,#0x50]
  00167b06: str r5,[sp,#0x3c]
  00167b08: movs r5,#0x0
  00167b0a: str r0,[sp,#0x4]
  00167b0c: ldr r0,[sp,#0x4c]
  00167b0e: str r5,[sp,#0x0]
  00167b10: strd r0,r8,[sp,#0x8]
  00167b14: ldr r0,[sp,#0x48]
  00167b16: strd r0,r11,[sp,#0x10]
  00167b1a: ldr r0,[sp,#0x44]
  00167b1c: strd r0,r6,[sp,#0x18]
  00167b20: mov r0,r9
  00167b22: blx 0x000724fc
  00167b26: ldr r0,[0x00167bc8]
  00167b28: ldr r1,[sp,#0x58]
  00167b2a: add r0,pc
  00167b2c: ldr.w r8,[sp,#0x54]
  00167b30: ldr r0,[r0,#0x0]
  00167b32: ldr r1,[r1,#0xc]
  00167b34: ldr r0,[r0,#0x0]
  00167b36: blx 0x00072088
  00167b3a: mov r2,r10
  00167b3c: mov r3,r4
  00167b3e: str r5,[sp,#0x0]
  00167b40: blx 0x0006f7cc
  00167b44: b 0x00167b86
  00167b46: vldr.32 s0,[r6,#0x124]
  00167b4a: vldr.32 s2,[pc,#0x64]
  00167b4e: vcvt.f32.s32 s0,s0
  00167b52: vldr.32 s4,[pc,#0x60]
  00167b56: vmul.f32 s2,s0,s2
  00167b5a: vmul.f32 s2,s2,s4
  00167b5e: vmov s4,r0
  00167b62: vcvt.f32.s32 s4,s4
  00167b66: vmov r1,s2
  00167b6a: vadd.f32 s0,s0,s4
  00167b6e: vcvt.s32.f32 s0,s0
  00167b72: movs r2,#0x0
  00167b74: movs r3,#0x0
  00167b76: movs r4,#0x0
  00167b78: vstr.32 s0,[r6,#0x134]
  00167b7c: ldr r0,[sp,#0x58]
  00167b7e: blx 0x00072160
  00167b82: str.w r4,[r6,#0x150]
  00167b86: ldr r0,[sp,#0x134]
  00167b88: ldr.w r1,[r8,#0x0]
  00167b8c: subs r0,r1,r0
  00167b8e: itttt eq
  00167b90: add.eq sp,#0x138
  00167b92: vpop.eq {d8}
  00167b96: add.eq sp,#0x4
  00167b98: pop.eq.w {r8,r9,r10,r11}
  00167b9c: it eq
  00167b9e: pop.eq {r4,r5,r6,r7,pc}
  00167ba0: blx 0x0006e824
```
