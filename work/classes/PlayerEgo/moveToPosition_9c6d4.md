# PlayerEgo::moveToPosition
elf 0x9c6d4 body 894
Sig: undefined __stdcall moveToPosition(Vector param_1, bool param_2, float param_3)

## decompile
```c

/* PlayerEgo::moveToPosition(AbyssEngine::AEMath::Vector, bool, float) */

void PlayerEgo::moveToPosition
               (PlayerEgo *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               int param_5,float param_6)

{
  uint uVar1;
  char cVar2;
  byte bVar3;
  int *piVar4;
  int *piVar5;
  undefined4 uVar6;
  Matrix *pMVar7;
  int iVar8;
  int iVar9;
  int **this;
  int **ppiVar10;
  int *piVar11;
  code *pcVar12;
  code *pcVar13;
  uint in_fpscr;
  float fVar14;
  float fVar15;
  float extraout_s1;
  int *piVar16;
  Matrix aMStack_8c [12];
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  float local_74;
  float local_64;
  float local_54;
  undefined8 local_40;
  int *local_38;
  int local_34;
  
  piVar4 = *(int **)(DAT_000aca6c + 0xac6ec);
  pcVar13 = *(code **)(DAT_000aca70 + 0xac6ee);
  this = &param_1[6].field_14;
  local_34 = *piVar4;
  local_80 = param_2;
  local_7c = param_3;
  local_78 = param_4;
  (*pcVar13)(this,&local_80);
  AEGeometry::getPosition();
  AbyssEngine::AEMath::Vector::operator-=((Vector *)this,(Vector *)&local_80);
  pcVar12 = *(code **)(DAT_000aca74 + 0xac722);
  (*pcVar12)(&local_80,this);
  (*pcVar13)(this,&local_80);
  AEGeometry::getDirection();
  ppiVar10 = &param_1[6].field_8;
  (*pcVar13)(ppiVar10,&local_80);
  (*pcVar12)(&local_80,ppiVar10);
  (*pcVar13)(ppiVar10,&local_80);
  AbyssEngine::AEMath::Vector::operator-=((Vector *)this,(Vector *)ppiVar10);
  fVar14 = (float)VectorSignedToFloat(param_1[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
  fVar14 = (float)VectorSignedToFloat((int)(fVar14 * param_6),(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::AEMath::Vector::operator*=((Vector *)this,fVar14 * DAT_000aca54);
  AbyssEngine::AEMath::operator+((Vector *)&local_80,(Vector *)ppiVar10);
  piVar11 = &param_1[9].field_20;
  (*pcVar13)(piVar11,(Vector *)&local_80);
  (*pcVar12)(&local_80,piVar11);
  (*pcVar13)(piVar11,&local_80);
  if (param_5 == 0) goto LAB_000ac904;
  AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_80,(Vector *)ppiVar10);
  fVar14 = (float)AbyssEngine::AEMath::VectorDot((Vector *)&local_80,(Vector *)piVar11);
  in_fpscr = in_fpscr & 0xfffffff;
  piVar5 = DAT_000aca58;
  if ((fVar14 < 1.0) &&
     (uVar1 = in_fpscr | (uint)(fVar14 < -1.0) << 0x1f | (uint)(fVar14 == -1.0) << 0x1e,
     in_fpscr = uVar1 | (uint)NAN(fVar14) << 0x1c, bVar3 = (byte)(uVar1 >> 0x18),
     !(bool)(bVar3 >> 6 & 1) && bVar3 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1))) {
    piVar5 = (int *)AbyssEngine::AEMath::ACosf((float)DAT_000aca58);
  }
  piVar16 = (int *)-(float)piVar5;
  if (0.0 < (float)piVar5) {
    piVar16 = piVar5;
  }
  in_fpscr = in_fpscr & 0xfffffff | (uint)((float)piVar16 == 0.0) << 0x1e;
  if ((byte)(in_fpscr >> 0x1e) == 0) {
    AEGeometry::getRightVector();
    local_40 = *(ulonglong *)piVar11;
    local_38 = param_1[10].field_4;
    fVar14 = (float)AbyssEngine::AEMath::VectorDot((Vector *)&local_80,(Vector *)&local_40);
    fVar14 = (float)AbyssEngine::AEMath::ACosf(fVar14);
    in_fpscr = in_fpscr & 0xfffffff;
    if (fVar14 < DAT_000aca5c) {
      piVar16 = (int *)-(float)piVar16;
    }
  }
  iVar8 = param_1[0x12].field_1C;
  (&param_1[0x12].field_8)[iVar8] = piVar16;
  cVar2 = (char)param_1[0x12].field_20;
  if (cVar2 == '\0') {
    if (iVar8 != 0) {
      for (iVar9 = 0; iVar9 < iVar8; iVar9 = iVar9 + 1) {
      }
      VectorSignedToFloat(iVar8,(byte)(in_fpscr >> 0x16) & 3);
      goto LAB_000ac8aa;
    }
    param_1[0x12].field_1C = 1;
  }
  else {
    for (iVar9 = 0; iVar9 != 0x14; iVar9 = iVar9 + 4) {
    }
LAB_000ac8aa:
    param_1[0x12].field_1C = iVar8 + 1;
    if ((3 < iVar8) && (param_1[0x12].field_1C = 0, cVar2 == '\0')) {
      *(undefined1 *)&param_1[0x12].field_20 = 1;
    }
  }
  in_fpscr = in_fpscr & 0xfffffff;
LAB_000ac904:
  local_7c = 0x3f800000;
  local_80 = 0;
  local_78 = 0;
  AEGeometry::setDirection((Vector *)param_1->field_8,(Vector *)piVar11);
  fVar14 = (float)VectorSignedToFloat(param_1[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
  AEGeometry::moveForward((float)param_1[5].field_8 * fVar14 * (float)param_1[5].field_4);
  roll(param_1,(int)param_1[8].field_14);
  fVar15 = (float)param_1[0x18].field_1C;
  fVar14 = -fVar15;
  if (0.0 < fVar15) {
    fVar14 = fVar15;
  }
  uVar1 = in_fpscr & 0xfffffff | (uint)(fVar14 < DAT_000aca60) << 0x1f |
          (uint)(fVar14 == DAT_000aca60) << 0x1e;
  bVar3 = (byte)(uVar1 >> 0x18);
  if ((bool)(bVar3 >> 6 & 1) || (bool)(bVar3 >> 7) != (NAN(fVar14) || NAN(DAT_000aca60))) {
    param_1[0x18].field_20 = DAT_000aca64;
  }
  else {
    fVar14 = (float)VectorSignedToFloat(param_1[8].field_14,(byte)(uVar1 >> 0x16) & 3);
    local_38 = (int *)0x0;
    local_40 = (ulonglong)(uint)(fVar15 * fVar14);
    uVar6 = AEGeometry::getMatrix();
    __aeabi_memcpy((Vector *)&local_80,uVar6,0x3c);
    AbyssEngine::AEMath::MatrixRotateVector(aMStack_8c,(Vector *)&local_80);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&local_40,(Vector *)aMStack_8c);
    local_74 = (float)local_40 + local_74;
    local_64 = local_40._4_4_ + local_64;
    local_54 = (float)local_38 + local_54;
    fVar14 = (float)param_1[0x18].field_1C * DAT_000aca68;
    param_1[0x18].field_1C = (int)fVar14;
    TargetFollowCamera::translateNoUpdate
              ((TargetFollowCamera *)param_1[3].field_1C,fVar14,extraout_s1,local_64);
    AEGeometry::setMatrix((Matrix *)param_1->field_8);
  }
  pMVar7 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(param_1->m_pPlayer + 4),pMVar7);
  AEGeometry::getPosition();
  AbyssEngine::AEMath::Vector::operator=((Vector *)&param_1[9].field_4,(Vector *)&local_80);
  iVar8 = *piVar4 - local_34;
  if (iVar8 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar8);
}

```

## target disasm
```
  000ac6d4: push {r4,r5,r6,r7,lr}
  000ac6d6: add r7,sp,#0xc
  000ac6d8: push {r7,r8,r9,r10,r11}
  000ac6dc: vpush {d8}
  000ac6e0: sub sp,#0x60
  000ac6e2: mov r4,r0
  000ac6e4: ldr r0,[0x000aca6c]
  000ac6e6: ldr r5,[0x000aca70]
  000ac6e8: add r0,pc
  000ac6ea: add r5,pc
  000ac6ec: ldr r0,[r0,#0x0]
  000ac6ee: str r0,[sp,#0x0]
  000ac6f0: ldr.w r11,[r5,#0x0]
  000ac6f4: add.w r5,r4,#0xec
  000ac6f8: ldr r0,[r0,#0x0]
  000ac6fa: str r0,[sp,#0x5c]
  000ac6fc: add r0,sp,#0x10
  000ac6fe: stmia r0!,{r1,r2,r3}
  000ac700: add r1,sp,#0x10
  000ac702: mov r0,r5
  000ac704: blx r11
  000ac706: add r6,sp,#0x10
  000ac708: ldr r1,[r4,#0x8]
  000ac70a: mov r0,r6
  000ac70c: blx 0x000720b8
  000ac710: mov r0,r5
  000ac712: mov r1,r6
  000ac714: blx 0x00072694
  000ac718: ldr r0,[0x000aca74]
  000ac71a: add r6,sp,#0x10
  000ac71c: mov r1,r5
  000ac71e: add r0,pc
  000ac720: ldr.w r10,[r0,#0x0]
  000ac724: mov r0,r6
  000ac726: blx r10
  000ac728: mov r0,r5
  000ac72a: mov r1,r6
  000ac72c: blx r11
  000ac72e: add.w r8,sp,#0x10
  000ac732: ldr r1,[r4,#0x8]
  000ac734: mov r0,r8
  000ac736: blx 0x00072574
  000ac73a: add.w r6,r4,#0xe0
  000ac73e: mov r1,r8
  000ac740: mov r0,r6
  000ac742: blx r11
  000ac744: add.w r8,sp,#0x10
  000ac748: mov r1,r6
  000ac74a: mov r0,r8
  000ac74c: blx r10
  000ac74e: mov r0,r6
  000ac750: mov r1,r8
  000ac752: blx r11
  000ac754: mov r0,r5
  000ac756: mov r1,r6
  000ac758: blx 0x00072694
  000ac75c: vldr.32 s0,[r4,#0x134]
  000ac760: mov r0,r5
  000ac762: vldr.32 s2,[r7,#0xc]
  000ac766: vcvt.f32.s32 s0,s0
  000ac76a: vmul.f32 s0,s0,s2
  000ac76e: vldr.32 s2,[pc,#0x2e4]
  000ac772: vcvt.s32.f32 s0,s0
  000ac776: vcvt.f32.s32 s0,s0
  000ac77a: vmul.f32 s0,s0,s2
  000ac77e: vmov r1,s0
  000ac782: blx 0x00072628
  000ac786: add.w r8,sp,#0x10
  000ac78a: mov r1,r6
  000ac78c: mov r2,r5
  000ac78e: mov r0,r8
  000ac790: blx 0x0006f1cc
  000ac794: add.w r9,r4,#0x164
  000ac798: mov r1,r8
  000ac79a: mov r0,r9
  000ac79c: blx r11
  000ac79e: add r5,sp,#0x10
  000ac7a0: mov r1,r9
  000ac7a2: mov r0,r5
  000ac7a4: blx r10
  000ac7a6: mov r0,r9
  000ac7a8: mov r1,r5
  000ac7aa: blx r11
  000ac7ac: ldr r0,[r7,#0x8]
  000ac7ae: cmp r0,#0x0
  000ac7b0: beq 0x000ac87e
  000ac7b2: add r5,sp,#0x10
  000ac7b4: mov r1,r6
  000ac7b6: mov r0,r5
  000ac7b8: blx 0x0006ec80
  000ac7bc: mov r0,r5
  000ac7be: mov r1,r9
  000ac7c0: blx 0x0006ec68
  000ac7c4: vmov.f32 s4,0x3f800000
  000ac7c8: vldr.32 s0,[pc,#0x28c]
  000ac7cc: vmov s2,r0
  000ac7d0: vcmpe.f32 s2,s4
  000ac7d4: vmrs apsr,fpscr
  000ac7d8: bpl 0x000ac7f0
  000ac7da: vmov.f32 s4,0xbf800000
  000ac7de: vcmpe.f32 s2,s4
  000ac7e2: vmrs apsr,fpscr
  000ac7e6: ble 0x000ac7f0
  000ac7e8: blx 0x000726a0
  000ac7ec: vmov s0,r0
  000ac7f0: vcmpe.f32 s0,#0
  000ac7f4: vmrs apsr,fpscr
  000ac7f8: vneg.f32 s16,s0
  000ac7fc: it gt
  000ac7fe: vmov.gt.f32 s16,s0
  000ac802: vcmpe.f32 s16,#0
  000ac806: vmrs apsr,fpscr
  000ac80a: beq 0x000ac84a
  000ac80c: add r5,sp,#0x10
  000ac80e: ldr r1,[r4,#0x8]
  000ac810: mov r0,r5
  000ac812: blx 0x0007261c
  000ac816: vldr.64 d16,[r9]
  000ac81a: add r1,sp,#0x50
  000ac81c: ldr.w r0,[r9,#0x8]
  000ac820: str r0,[sp,#0x58]
  000ac822: mov r0,r5
  000ac824: vstr.64 d16,[sp,#0x50]
  000ac828: blx 0x0006ec68
  000ac82c: blx 0x000726a0
  000ac830: vldr.32 s0,[pc,#0x228]
  000ac834: vmov s2,r0
  000ac838: vneg.f32 s4,s16
  000ac83c: vcmpe.f32 s2,s0
  000ac840: vmrs apsr,fpscr
  000ac844: it mi
  000ac846: vmov.mi.f32 s16,s4
  000ac84a: ldr.w r1,[r4,#0x2a4]
  000ac84e: add.w r0,r4,r1, lsl #0x2
  000ac852: vstr.32 s16,[r0,#0x290]
  000ac856: ldrb.w r0,[r4,#0x2a8]
  000ac85a: cbz r0,0x000ac884
  000ac85c: vldr.32 s0,[pc,#0x1f8]
  000ac860: add.w r2,r4,#0x290
  000ac864: movs r3,#0x0
  000ac866: b 0x000ac874
  000ac868: adds r6,r2,r3
  000ac86a: adds r3,#0x4
  000ac86c: vldr.32 s2,[r6]
  000ac870: vadd.f32 s0,s0,s2
  000ac874: cmp r3,#0x14
  000ac876: bne 0x000ac868
  000ac878: vmov.f32 s2,0x40a00000
  000ac87c: b 0x000ac8aa
  000ac87e: vldr.32 s16,[pc,#0x1d8]
  000ac882: b 0x000ac904
  000ac884: cbz r1,0x000ac8cc
  000ac886: vldr.32 s0,[pc,#0x1d0]
  000ac88a: add.w r2,r4,#0x290
  000ac88e: movs r3,#0x0
  000ac890: b 0x000ac89e
  000ac892: vldr.32 s2,[r2]
  000ac896: adds r2,#0x4
  000ac898: adds r3,#0x1
  000ac89a: vadd.f32 s0,s0,s2
  000ac89e: cmp r3,r1
  000ac8a0: blt 0x000ac892
  000ac8a2: vmov s2,r1
  000ac8a6: vcvt.f32.s32 s2,s2
  000ac8aa: vdiv.f32 s16,s0,s2
  000ac8ae: cmp r1,#0x4
  000ac8b0: add.w r2,r1,#0x1
  000ac8b4: str.w r2,[r4,#0x2a4]
  000ac8b8: blt 0x000ac8d2
  000ac8ba: movs r1,#0x0
  000ac8bc: cmp r0,#0x0
  000ac8be: str.w r1,[r4,#0x2a4]
  000ac8c2: bne 0x000ac8d2
  000ac8c4: movs r0,#0x1
  000ac8c6: strb.w r0,[r4,#0x2a8]
  000ac8ca: b 0x000ac8d2
  000ac8cc: movs r0,#0x1
  000ac8ce: str.w r0,[r4,#0x2a4]
  000ac8d2: vldr.32 s0,[r4,#0x284]
  000ac8d6: vldr.32 s2,[r4,#0x288]
  000ac8da: vmul.f32 s4,s16,s0
  000ac8de: vnmul.f32 s16,s4,s2
  000ac8e2: vcmpe.f32 s0,s16
  000ac8e6: vmrs apsr,fpscr
  000ac8ea: bpl 0x000ac8f2
  000ac8ec: vmov.f32 s16,s0
  000ac8f0: b 0x000ac904
  000ac8f2: vmul.f32 s2,s4,s2
  000ac8f6: vcmpe.f32 s2,s0
  000ac8fa: vmrs apsr,fpscr
  000ac8fe: it gt
  000ac900: vneg.gt.f32 s16,s0
  000ac904: ldr r0,[r4,#0x8]
  000ac906: mov.w r1,#0x3f800000
  000ac90a: add r2,sp,#0x10
  000ac90c: str r1,[sp,#0x14]
  000ac90e: movs r5,#0x0
  000ac910: mov r1,r9
  000ac912: str r5,[sp,#0x10]
  000ac914: str r5,[sp,#0x18]
  000ac916: blx 0x000726ac
  000ac91a: vldr.32 s0,[r4,#0x134]
  000ac91e: vcvt.f32.s32 s0,s0
  000ac922: vldr.32 s4,[r4,#0xbc]
  000ac926: vldr.32 s2,[r4,#0xb8]
  000ac92a: ldr r0,[r4,#0x8]
  000ac92c: vmul.f32 s0,s4,s0
  000ac930: vmul.f32 s0,s0,s2
  000ac934: vmov r1,s0
  000ac938: blx 0x000725c8
  000ac93c: ldr.w r1,[r4,#0x134]
  000ac940: mov r0,r4
  000ac942: blx 0x000725ec
  000ac946: vldr.32 s0,[r4,#0x37c]
  000ac94a: vldr.32 s4,[pc,#0x114]
  000ac94e: vcmpe.f32 s0,#0
  000ac952: vmrs apsr,fpscr
  000ac956: vneg.f32 s2,s0
  000ac95a: it gt
  000ac95c: vmov.gt.f32 s2,s0
  000ac960: vcmpe.f32 s2,s4
  000ac964: vmrs apsr,fpscr
  000ac968: ble 0x000aca06
  000ac96a: vldr.32 s2,[r4,#0x134]
  000ac96e: vcvt.f32.s32 s2,s2
  000ac972: strd r5,r5,[sp,#0x54]
  000ac976: vmul.f32 s0,s0,s2
  000ac97a: vstr.32 s0,[sp,#0x50]
  000ac97e: ldr r0,[r4,#0x8]
  000ac980: blx 0x000721cc
  000ac984: add.w r8,sp,#0x10
  000ac988: mov r1,r0
  000ac98a: movs r2,#0x3c
  000ac98c: mov r0,r8
  000ac98e: blx 0x0006f1e4
  000ac992: add r6,sp,#0x4
  000ac994: add r5,sp,#0x50
  000ac996: mov r1,r8
  000ac998: mov r0,r6
  000ac99a: mov r2,r5
  000ac99c: blx 0x0006f694
  000ac9a0: mov r0,r5
  000ac9a2: mov r1,r6
  000ac9a4: blx 0x0006eb3c
  000ac9a8: vldr.32 s6,[sp,#0x50]
  000ac9ac: vldr.32 s8,[sp,#0x54]
  000ac9b0: vldr.32 s10,[sp,#0x58]
  000ac9b4: vmov r1,s6
  000ac9b8: vmov r2,s8
  000ac9bc: vldr.32 s0,[sp,#0x1c]
  000ac9c0: vmov r3,s10
  000ac9c4: vldr.32 s2,[sp,#0x2c]
  000ac9c8: vldr.32 s4,[sp,#0x3c]
  000ac9cc: vadd.f32 s0,s6,s0
  000ac9d0: vadd.f32 s2,s8,s2
  000ac9d4: vldr.32 s6,[pc,#0x90]
  000ac9d8: vadd.f32 s4,s10,s4
  000ac9dc: vstr.32 s0,[sp,#0x1c]
  000ac9e0: vstr.32 s2,[sp,#0x2c]
  000ac9e4: vstr.32 s4,[sp,#0x3c]
  000ac9e8: vldr.32 s0,[r4,#0x37c]
  000ac9ec: ldr.w r0,[r4,#0x88]
  000ac9f0: vmul.f32 s0,s0,s6
  000ac9f4: vstr.32 s0,[r4,#0x37c]
  000ac9f8: blx 0x00072610
  000ac9fc: ldr r0,[r4,#0x8]
  000ac9fe: mov r1,r8
  000aca00: blx 0x000721e4
  000aca04: b 0x000aca0c
  000aca06: ldr r0,[0x000aca64]
  000aca08: str.w r0,[r4,#0x380]
  000aca0c: ldr r0,[r4,#0x8]
  000aca0e: blx 0x000721cc
  000aca12: mov r1,r0
  000aca14: ldr r0,[r4,#0x0]
  000aca16: adds r0,#0x4
  000aca18: blx 0x0006f148
  000aca1c: add r5,sp,#0x10
  000aca1e: ldr r1,[r4,#0x8]
  000aca20: mov r0,r5
  000aca22: blx 0x000720b8
  000aca26: add.w r0,r4,#0x148
  000aca2a: mov r1,r5
  000aca2c: blx 0x0006eb3c
  000aca30: ldr r0,[sp,#0x5c]
  000aca32: ldr r1,[sp,#0x0]
  000aca34: ldr r1,[r1,#0x0]
  000aca36: subs r0,r1,r0
  000aca38: itttt eq
  000aca3a: vmov.eq r0,s16
  000aca3e: add.eq sp,#0x60
  000aca40: vpop.eq {d8}
  000aca44: add.eq sp,#0x4
  000aca46: itt eq
  000aca48: pop.eq.w {r8,r9,r10,r11}
  000aca4c: pop.eq {r4,r5,r6,r7,pc}
  000aca4e: blx 0x0006e824
```
