# PlayerEgo::updateManeuver
elf 0x9c1f8 body 1150
Sig: undefined __thiscall updateManeuver(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::updateManeuver() */

void __thiscall PlayerEgo::updateManeuver(PlayerEgo *this)

{
  undefined4 *puVar1;
  Matrix *pMVar2;
  undefined8 *puVar3;
  Level *pLVar4;
  code *pcVar5;
  Matrix *pMVar6;
  uint *puVar7;
  int *piVar8;
  uint in_fpscr;
  undefined4 uVar9;
  float fVar10;
  float fVar11;
  float extraout_s0;
  float fVar12;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s2;
  float extraout_s2_00;
  float fVar13;
  Matrix local_114 [60];
  undefined4 local_d8 [5];
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined4 local_a0;
  Vector aVStack_98 [12];
  Vector aVStack_8c [12];
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  float local_68;
  undefined4 uStack_64;
  undefined4 local_60;
  float local_58 [2];
  float local_50;
  int local_4c;
  
  piVar8 = *(int **)(DAT_000ac468 + 0xac20e);
  local_4c = *piVar8;
  if (this[0x16].field_1C - 1U < 2) {
    this[0x17].field_14 = (int *)((int)this[8].field_14 + (int)this[0x17].field_14);
    AEGeometry::getRightVector();
    uVar9 = VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    pcVar5 = *(code **)(DAT_000ac46c + 0xac24e);
    (*pcVar5)(local_114,&local_68,uVar9);
    (*pcVar5)(local_d8,local_114,0x40800000);
    fVar13 = 1.0;
    fVar10 = (float)VectorSignedToFloat(this[0x17].field_14,(byte)(in_fpscr >> 0x16) & 3);
    (*pcVar5)((Vector *)local_58,local_d8,fVar10 / DAT_000ac470 + 1.0);
    fVar10 = DAT_000ac474;
    fVar11 = (float)AbyssEngine::AEMath::Vector::operator*=
                              ((Vector *)local_58,(float)this[9].field_10 * DAT_000ac474);
    if (this[0x16].field_1C == 2) {
      AbyssEngine::AEMath::Vector::operator*=((Vector *)local_58,fVar11);
    }
    fVar11 = (float)VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::moveForward((float)this[5].field_8 * fVar11 * (float)this[5].field_4);
    AEGeometry::translate(extraout_s0,extraout_s1,extraout_s2);
    uStack_a8 = DAT_000ac698;
    uStack_b0 = DAT_000ac690;
    uStack_bc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_b8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_b4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar1 = (undefined4 *)((uint)local_d8 | 4);
    *puVar1 = 0;
    puVar1[1] = uStack_bc;
    puVar1[2] = uStack_b8;
    puVar1[3] = uStack_b4;
    local_c0 = 0;
    local_d8[0] = 0x3f800000;
    local_c4 = 0x3f800000;
    local_a0 = 0x3f800000;
    fVar12 = (float)VectorSignedToFloat(this[0x17].field_14,(byte)(in_fpscr >> 0x16) & 3);
    fVar11 = 1.0;
    if (this[0x16].field_1C == 1) {
      fVar11 = -1.0;
    }
    AbyssEngine::AEMath::MatrixSetRotation
              (local_114,fVar11 * (1.0 - fVar12 / DAT_000ac6a0) * DAT_000ac6a4,extraout_s1_00,
               DAT_000ac6a4);
    pMVar6 = (Matrix *)this->field_8;
    pMVar2 = (Matrix *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::operator*(local_114,pMVar2);
    AEGeometry::setMatrix(pMVar6);
    VectorSignedToFloat(this[0x17].field_14,(byte)(in_fpscr >> 0x16) & 3);
    fVar10 = (float)this[9].field_10 * fVar10 * DAT_000ac6ac;
    fVar11 = fVar10 * DAT_000ac6b0;
    fVar10 = (float)AbyssEngine::AEMath::Sinf(fVar10);
    if (this[0x16].field_1C == 1) {
      fVar13 = -1.0;
    }
    this[0x11].field_18 = (int *)(fVar10 * fVar11 * fVar13);
    puVar3 = (undefined8 *)
             TargetFollowCamera::getTargetOffset((TargetFollowCamera *)this[3].field_1C);
    local_60 = *(undefined4 *)(puVar3 + 1);
    _local_68 = CONCAT44((int)((ulonglong)*puVar3 >> 0x20),(float)this[0x11].field_18 * -3.0);
    TargetFollowCamera::setTargetOffset((Vector *)this[3].field_1C);
    puVar7 = *(uint **)(DAT_000ac6d0 + 0xac3f2);
    pMVar2 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar7);
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar7);
    AbyssEngine::AEMath::operator*(local_114,pMVar2);
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this->m_pPlayer + 4),local_114);
    TargetFollowCamera::translateNoUpdate
              ((TargetFollowCamera *)this[3].field_1C,local_50 * DAT_000ac6b4,extraout_s1_01,
               local_58[0] * DAT_000ac6b4);
    if (0x4af < (int)this[0x17].field_14) {
      pLVar4 = (Level *)this->field_10;
      this[0x16].field_1C = 0;
      LevelScript::resetCamera(pLVar4);
    }
  }
  else {
    if (this[0x16].field_1C != 3) {
      uVar9 = 0;
      goto LAB_000ac66a;
    }
    this[0x17].field_14 = (int *)((int)this[8].field_14 + (int)this[0x17].field_14);
    AEGeometry::getDirection();
    AEGeometry::getRightVector();
    AEGeometry::getUpVector();
    fVar10 = (float)getPosition();
    AbyssEngine::AEMath::operator*(fVar10,aVStack_8c);
    fVar10 = (float)AbyssEngine::AEMath::operator+((Vector *)local_d8,(Vector *)local_114);
    AbyssEngine::AEMath::operator*(fVar10,aVStack_98);
    AbyssEngine::AEMath::operator+((Vector *)&local_80,(Vector *)local_d8);
    if (900 < (int)this[0x17].field_14) {
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_80,(Vector *)&this[0x16].field_20);
    }
    uVar9 = DAT_000ac6bc;
    moveToPosition(this,local_80,uStack_7c,uStack_78,1,DAT_000ac6bc);
    uStack_a8 = DAT_000ac698;
    uStack_b0 = DAT_000ac690;
    uStack_bc = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_b8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_b4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar1 = (undefined4 *)((uint)local_d8 | 4);
    *puVar1 = 0;
    puVar1[1] = uStack_bc;
    puVar1[2] = uStack_b8;
    puVar1[3] = uStack_b4;
    fVar10 = DAT_000ac6c0;
    local_c0 = 0;
    local_d8[0] = 0x3f800000;
    local_c4 = 0x3f800000;
    local_a0 = 0x3f800000;
    fVar11 = (float)VectorSignedToFloat(this[0x17].field_14,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::MatrixSetRotation
              (local_114,(fVar11 / DAT_000ac6c0) * DAT_000ac6c4,extraout_s1_02,DAT_000ac6c4);
    pMVar6 = (Matrix *)this->field_8;
    pMVar2 = (Matrix *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::operator*(local_114,pMVar2);
    AEGeometry::setMatrix(pMVar6);
    puVar7 = *(uint **)(DAT_000ac6c8 + 0xac58c);
    pMVar2 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar7);
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar7);
    AbyssEngine::AEMath::operator*(local_114,pMVar2);
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this->m_pPlayer + 4),local_114);
    TargetFollowCamera::setLookAtCam(SUB41(this[3].field_1C,0));
    TargetFollowCamera::useTargetsUpVector(SUB41(this[3].field_1C,0));
    pcVar5 = *(code **)(DAT_000ac6cc + 0xac5dc);
    (*pcVar5)(aVStack_98,&this[0x17].field_8,uVar9);
    uVar9 = VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    (*pcVar5)(aVStack_8c,aVStack_98,uVar9);
    fVar11 = (float)VectorSignedToFloat(this[0x17].field_14,(byte)(in_fpscr >> 0x16) & 3);
    (*pcVar5)(local_114,aVStack_8c,1.0 - fVar11 / fVar10);
    TargetFollowCamera::translate
              ((TargetFollowCamera *)this[3].field_1C,extraout_s0_00,extraout_s1_03,extraout_s2_00);
    fVar10 = (float)VectorSignedToFloat(this[8].field_14,(byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::moveForward((float)this[5].field_8 * fVar10 * (float)this[5].field_4);
    if (2999 < (int)this[0x17].field_14) {
      this[0x16].field_1C = 0;
      TargetFollowCamera::setLookAtCam(SUB41(this[3].field_1C,0));
      TargetFollowCamera::useTargetsUpVector(SUB41(this[3].field_1C,0));
    }
  }
  uVar9 = 1;
LAB_000ac66a:
  if (*piVar8 != local_4c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar9);
  }
  return;
}

```

## target disasm
```
  000ac1f8: push {r4,r5,r6,r7,lr}
  000ac1fa: add r7,sp,#0xc
  000ac1fc: push {r7,r8,r9,r10,r11}
  000ac200: vpush {d8,d9,d10,d11}
  000ac204: sub sp,#0xd8
  000ac206: mov r4,r0
  000ac208: ldr r0,[0x000ac468]
  000ac20a: add r0,pc
  000ac20c: ldr.w r10,[r0,#0x0]
  000ac210: ldr.w r0,[r10,#0x0]
  000ac214: str r0,[sp,#0xd4]
  000ac216: ldr.w r0,[r4,#0x334]
  000ac21a: subs r1,r0,#0x1
  000ac21c: cmp r1,#0x2
  000ac21e: bcs.w 0x000ac478
  000ac222: ldr.w r0,[r4,#0x134]
  000ac226: add.w r8,sp,#0xb8
  000ac22a: ldr.w r1,[r4,#0x350]
  000ac22e: add r0,r1
  000ac230: str.w r0,[r4,#0x350]
  000ac234: ldr r1,[r4,#0x8]
  000ac236: mov r0,r8
  000ac238: blx 0x0007261c
  000ac23c: vldr.32 s0,[r4,#0x134]
  000ac240: add r6,sp,#0xc
  000ac242: mov r1,r8
  000ac244: vcvt.f32.s32 s0,s0
  000ac248: ldr r0,[0x000ac46c]
  000ac24a: add r0,pc
  000ac24c: ldr r5,[r0,#0x0]
  000ac24e: mov r0,r6
  000ac250: vmov r2,s0
  000ac254: blx r5
  000ac256: add.w r8,sp,#0x48
  000ac25a: mov r1,r6
  000ac25c: mov.w r2,#0x40800000
  000ac260: mov r0,r8
  000ac262: blx r5
  000ac264: vldr.32 s0,[r4,#0x350]
  000ac268: vmov.f32 s16,0x3f800000
  000ac26c: vldr.32 s2,[pc,#0x200]
  000ac270: add r6,sp,#0xc8
  000ac272: vcvt.f32.s32 s0,s0
  000ac276: mov r1,r8
  000ac278: mov r0,r6
  000ac27a: vdiv.f32 s0,s0,s2
  000ac27e: vadd.f32 s0,s0,s16
  000ac282: vmov r2,s0
  000ac286: blx r5
  000ac288: vldr.32 s18,[pc,#0x1e8]
  000ac28c: mov r0,r6
  000ac28e: vldr.32 s0,[r4,#0x154]
  000ac292: vmul.f32 s0,s0,s18
  000ac296: vmov r1,s0
  000ac29a: blx 0x00072628
  000ac29e: ldr.w r0,[r4,#0x334]
  000ac2a2: cmp r0,#0x2
  000ac2a4: bne 0x000ac2ae
  000ac2a6: ldr r1,[0x000ac688]
  000ac2a8: add r0,sp,#0xc8
  000ac2aa: blx 0x00072628
  000ac2ae: vldr.32 s0,[r4,#0x134]
  000ac2b2: vcvt.f32.s32 s0,s0
  000ac2b6: vldr.32 s4,[r4,#0xbc]
  000ac2ba: vldr.32 s2,[r4,#0xb8]
  000ac2be: ldr r0,[r4,#0x8]
  000ac2c0: vmul.f32 s0,s4,s0
  000ac2c4: vmul.f32 s0,s0,s2
  000ac2c8: vmov r1,s0
  000ac2cc: blx 0x000725c8
  000ac2d0: add r3,sp,#0xc8
  000ac2d2: ldmia r3,{r1,r2,r3}
  000ac2d4: ldr r0,[r4,#0x8]
  000ac2d6: blx 0x00072154
  000ac2da: adr r0,[0xac690]
  000ac2dc: vmov.i32 q8,#0x0
  000ac2e0: vld1.64 {d18,d19},[r0]
  000ac2e4: orr r0,r8,#0x4
  000ac2e8: vmov.f32 s22,0xbf800000
  000ac2ec: mov.w r9,#0x0
  000ac2f0: vst1.32 {d16,d17},[r0]
  000ac2f4: add.w r0,r8,#0x18
  000ac2f8: vmov.f32 s4,s16
  000ac2fc: mov r1,r8
  000ac2fe: vst1.64 {d16,d17},[r0]
  000ac302: add.w r0,r8,#0x28
  000ac306: movs r2,#0x0
  000ac308: vst1.64 {d18,d19},[r0]
  000ac30c: mov.w r0,#0x3f800000
  000ac310: str r0,[sp,#0x48]
  000ac312: str r0,[sp,#0x5c]
  000ac314: str r0,[sp,#0x80]
  000ac316: vldr.32 s0,[r4,#0x350]
  000ac31a: vldr.32 s20,[pc,#0x384]
  000ac31e: vcvt.f32.s32 s0,s0
  000ac322: vldr.32 s2,[pc,#0x380]
  000ac326: ldr.w r0,[r4,#0x334]
  000ac32a: cmp r0,#0x1
  000ac32c: it eq
  000ac32e: vmov.eq.f32 s4,s22
  000ac332: add r0,sp,#0xc
  000ac334: str.w r9,[sp,#0x0]
  000ac338: vdiv.f32 s0,s0,s20
  000ac33c: vsub.f32 s0,s16,s0
  000ac340: vmul.f32 s0,s0,s2
  000ac344: vmul.f32 s0,s4,s0
  000ac348: vmov r3,s0
  000ac34c: blx 0x00072094
  000ac350: ldr r6,[r4,#0x8]
  000ac352: mov r0,r6
  000ac354: blx 0x000721cc
  000ac358: add r5,sp,#0xc
  000ac35a: mov r1,r0
  000ac35c: mov r2,r8
  000ac35e: mov r0,r5
  000ac360: blx 0x0006f82c
  000ac364: mov r0,r6
  000ac366: mov r1,r5
  000ac368: blx 0x000721e4
  000ac36c: vldr.32 s2,[r4,#0x350]
  000ac370: vldr.32 s0,[r4,#0x154]
  000ac374: vcvt.f32.s32 s2,s2
  000ac378: vldr.32 s4,[pc,#0x32c]
  000ac37c: vmul.f32 s0,s0,s18
  000ac380: vdiv.f32 s2,s2,s20
  000ac384: vmul.f32 s2,s2,s4
  000ac388: vmov r0,s2
  000ac38c: vldr.32 s2,[pc,#0x31c]
  000ac390: vmul.f32 s0,s0,s2
  000ac394: vldr.32 s2,[pc,#0x318]
  000ac398: vmul.f32 s18,s0,s2
  000ac39c: blx 0x0006f1a8
  000ac3a0: vmov s0,r0
  000ac3a4: ldr.w r1,[r4,#0x334]
  000ac3a8: vmul.f32 s0,s0,s18
  000ac3ac: cmp r1,#0x1
  000ac3ae: it eq
  000ac3b0: vmov.eq.f32 s16,s22
  000ac3b4: vmul.f32 s0,s0,s16
  000ac3b8: vstr.32 s0,[r4,#0x27c]
  000ac3bc: ldr.w r0,[r4,#0x88]
  000ac3c0: blx 0x00072634
  000ac3c4: vmov.f32 s0,0xc0400000
  000ac3c8: vldr.64 d16,[r0]
  000ac3cc: ldr r0,[r0,#0x8]
  000ac3ce: add r1,sp,#0xb8
  000ac3d0: str r0,[sp,#0xc0]
  000ac3d2: vstr.64 d16,[sp,#0xb8]
  000ac3d6: vldr.32 s2,[r4,#0x27c]
  000ac3da: vmul.f32 s0,s2,s0
  000ac3de: vstr.32 s0,[sp,#0xb8]
  000ac3e2: ldr.w r0,[r4,#0x88]
  000ac3e6: blx 0x00072640
  000ac3ea: ldr r0,[0x000ac6d0]
  000ac3ec: ldr r1,[r4,#0x8]
  000ac3ee: add r0,pc
  000ac3f0: ldr r6,[r0,#0x0]
  000ac3f2: ldr r1,[r1,#0xc]
  000ac3f4: ldr r0,[r6,#0x0]
  000ac3f6: blx 0x000720c4
  000ac3fa: ldr r1,[r4,#0x4]
  000ac3fc: mov r5,r0
  000ac3fe: ldr r0,[r6,#0x0]
  000ac400: ldr r1,[r1,#0xc]
  000ac402: blx 0x000720c4
  000ac406: add r6,sp,#0xc
  000ac408: mov r2,r0
  000ac40a: mov r1,r5
  000ac40c: mov r0,r6
  000ac40e: blx 0x0006f82c
  000ac412: ldr r0,[r4,#0x0]
  000ac414: mov r1,r6
  000ac416: adds r0,#0x4
  000ac418: blx 0x0006f148
  000ac41c: vldr.32 s0,[pc,#0x294]
  000ac420: vldr.32 s2,[sp,#0xc8]
  000ac424: vldr.32 s4,[sp,#0xcc]
  000ac428: vldr.32 s6,[sp,#0xd0]
  000ac42c: vmul.f32 s2,s2,s0
  000ac430: vmul.f32 s4,s4,s0
  000ac434: ldr.w r0,[r4,#0x88]
  000ac438: vmul.f32 s0,s6,s0
  000ac43c: vmov r1,s2
  000ac440: vmov r2,s4
  000ac444: vmov r3,s0
  000ac448: blx 0x00072610
  000ac44c: ldr.w r0,[r4,#0x350]
  000ac450: cmp.w r0,#0x4b0
  000ac454: blt.w 0x000ac664
  000ac458: ldrd r1,r0,[r4,#0xc]
  000ac45c: str.w r9,[r4,#0x334]
  000ac460: blx 0x0007246c
  000ac464: b 0x000ac664
  000ac478: cmp r0,#0x3
  000ac47a: bne.w 0x000ac668
  000ac47e: ldr.w r0,[r4,#0x134]
  000ac482: add.w r9,sp,#0xc8
  000ac486: ldr.w r1,[r4,#0x350]
  000ac48a: add r0,r1
  000ac48c: str.w r0,[r4,#0x350]
  000ac490: ldr r1,[r4,#0x8]
  000ac492: mov r0,r9
  000ac494: blx 0x00072574
  000ac498: ldr r1,[r4,#0x8]
  000ac49a: add r0,sp,#0xb8
  000ac49c: blx 0x0007261c
  000ac4a0: add.w r8,sp,#0xac
  000ac4a4: ldr r1,[r4,#0x8]
  000ac4a6: mov r0,r8
  000ac4a8: blx 0x00072568
  000ac4ac: add r5,sp,#0xc
  000ac4ae: mov r1,r4
  000ac4b0: mov r0,r5
  000ac4b2: blx 0x0007264c
  000ac4b6: ldr r1,[0x000ac6b8]
  000ac4b8: add r6,sp,#0x94
  000ac4ba: mov r2,r9
  000ac4bc: mov r0,r6
  000ac4be: mov r11,r1
  000ac4c0: blx 0x00072658
  000ac4c4: add.w r9,sp,#0x48
  000ac4c8: mov r1,r5
  000ac4ca: mov r2,r6
  000ac4cc: mov r0,r9
  000ac4ce: blx 0x0006f1cc
  000ac4d2: add r5,sp,#0x88
  000ac4d4: mov r1,r11
  000ac4d6: mov r2,r8
  000ac4d8: mov r0,r5
  000ac4da: blx 0x00072658
  000ac4de: add r0,sp,#0xa0
  000ac4e0: mov r1,r9
  000ac4e2: mov r2,r5
  000ac4e4: blx 0x0006f1cc
  000ac4e8: ldr.w r0,[r4,#0x350]
  000ac4ec: cmp.w r0,#0x384
  000ac4f0: ble 0x000ac4fc
  000ac4f2: add.w r1,r4,#0x338
  000ac4f6: add r0,sp,#0xa0
  000ac4f8: blx 0x0006eb3c
  000ac4fc: add r3,sp,#0xa0
  000ac4fe: ldr r5,[0x000ac6bc]
  000ac500: movs r0,#0x1
  000ac502: ldmia r3,{r1,r2,r3}
  000ac504: strd r0,r5,[sp,#0x0]
  000ac508: mov r0,r4
  000ac50a: blx 0x00072664
  000ac50e: adr r0,[0xac690]
  000ac510: vmov.i32 q8,#0x0
  000ac514: vld1.64 {d18,d19},[r0]
  000ac518: orr r0,r9,#0x4
  000ac51c: mov r1,r9
  000ac51e: movs r2,#0x0
  000ac520: vst1.32 {d16,d17},[r0]
  000ac524: add.w r0,r9,#0x18
  000ac528: movs r3,#0x0
  000ac52a: mov.w r11,#0x0
  000ac52e: vst1.64 {d16,d17},[r0]
  000ac532: add.w r0,r9,#0x28
  000ac536: vst1.64 {d18,d19},[r0]
  000ac53a: mov.w r0,#0x3f800000
  000ac53e: str r0,[sp,#0x48]
  000ac540: str r0,[sp,#0x5c]
  000ac542: str r0,[sp,#0x80]
  000ac544: add r0,sp,#0xc
  000ac546: vldr.32 s0,[r4,#0x350]
  000ac54a: vldr.32 s16,[pc,#0x174]
  000ac54e: vcvt.f32.s32 s0,s0
  000ac552: vldr.32 s2,[pc,#0x170]
  000ac556: vdiv.f32 s0,s0,s16
  000ac55a: vmul.f32 s0,s0,s2
  000ac55e: vstr.32 s0,[sp]
  000ac562: blx 0x00072094
  000ac566: ldr r6,[r4,#0x8]
  000ac568: mov r0,r6
  000ac56a: blx 0x000721cc
  000ac56e: add.w r8,sp,#0xc
  000ac572: mov r1,r0
  000ac574: mov r2,r9
  000ac576: mov r0,r8
  000ac578: blx 0x0006f82c
  000ac57c: mov r0,r6
  000ac57e: mov r1,r8
  000ac580: blx 0x000721e4
  000ac584: ldr r0,[0x000ac6c8]
  000ac586: ldr r1,[r4,#0x8]
  000ac588: add r0,pc
  000ac58a: ldr r6,[r0,#0x0]
  000ac58c: ldr r1,[r1,#0xc]
  000ac58e: ldr r0,[r6,#0x0]
  000ac590: blx 0x000720c4
  000ac594: ldr r1,[r4,#0x4]
  000ac596: mov r9,r0
  000ac598: ldr r0,[r6,#0x0]
  000ac59a: ldr r1,[r1,#0xc]
  000ac59c: blx 0x000720c4
  000ac5a0: add.w r8,sp,#0xc
  000ac5a4: mov r2,r0
  000ac5a6: mov r1,r9
  000ac5a8: mov r0,r8
  000ac5aa: blx 0x0006f82c
  000ac5ae: ldr r0,[r4,#0x0]
  000ac5b0: mov r1,r8
  000ac5b2: adds r0,#0x4
  000ac5b4: blx 0x0006f148
  000ac5b8: ldr.w r0,[r4,#0x88]
  000ac5bc: movs r1,#0x1
  000ac5be: blx 0x00072670
  000ac5c2: ldr.w r0,[r4,#0x88]
  000ac5c6: movs r1,#0x0
  000ac5c8: blx 0x0007267c
  000ac5cc: ldr r0,[0x000ac6cc]
  000ac5ce: add.w r8,sp,#0x88
  000ac5d2: add.w r1,r4,#0x344
  000ac5d6: mov r2,r5
  000ac5d8: add r0,pc
  000ac5da: ldr r6,[r0,#0x0]
  000ac5dc: mov r0,r8
  000ac5de: blx r6
  000ac5e0: vldr.32 s0,[r4,#0x134]
  000ac5e4: add r5,sp,#0x94
  000ac5e6: mov r1,r8
  000ac5e8: vcvt.f32.s32 s0,s0
  000ac5ec: mov r0,r5
  000ac5ee: vmov r2,s0
  000ac5f2: blx r6
  000ac5f4: vldr.32 s0,[r4,#0x350]
  000ac5f8: vmov.f32 s2,0x3f800000
  000ac5fc: add r0,sp,#0xc
  000ac5fe: mov r1,r5
  000ac600: vcvt.f32.s32 s0,s0
  000ac604: vdiv.f32 s0,s0,s16
  000ac608: vsub.f32 s0,s2,s0
  000ac60c: vmov r2,s0
  000ac610: blx r6
  000ac612: add r3,sp,#0xc
  000ac614: ldmia r3,{r1,r2,r3}
  000ac616: ldr.w r0,[r4,#0x88]
  000ac61a: blx 0x00072688
  000ac61e: vldr.32 s0,[r4,#0x134]
  000ac622: vcvt.f32.s32 s0,s0
  000ac626: vldr.32 s4,[r4,#0xbc]
  000ac62a: vldr.32 s2,[r4,#0xb8]
  000ac62e: ldr r0,[r4,#0x8]
  000ac630: vmul.f32 s0,s4,s0
  000ac634: vmul.f32 s0,s0,s2
  000ac638: vmov r1,s0
  000ac63c: blx 0x000725c8
  000ac640: ldr.w r0,[r4,#0x350]
  000ac644: movw r1,#0xbb8
  000ac648: cmp r0,r1
  000ac64a: blt 0x000ac664
  000ac64c: ldr.w r0,[r4,#0x88]
  000ac650: movs r1,#0x0
  000ac652: str.w r11,[r4,#0x334]
  000ac656: blx 0x00072670
  000ac65a: ldr.w r0,[r4,#0x88]
  000ac65e: movs r1,#0x1
  000ac660: blx 0x0007267c
  000ac664: movs r0,#0x1
  000ac666: b 0x000ac66a
  000ac668: movs r0,#0x0
  000ac66a: ldr r1,[sp,#0xd4]
  000ac66c: ldr.w r2,[r10,#0x0]
  000ac670: subs r1,r2,r1
  000ac672: itttt eq
  000ac674: add.eq sp,#0xd8
  000ac676: vpop.eq {d8,d9,d10,d11}
  000ac67a: add.eq sp,#0x4
  000ac67c: pop.eq.w {r8,r9,r10,r11}
  000ac680: it eq
  000ac682: pop.eq {r4,r5,r6,r7,pc}
  000ac684: blx 0x0006e824
```
