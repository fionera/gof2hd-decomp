# PlayerEgo::calcCollision
elf 0x9f0cc body 932
Sig: undefined __thiscall calcCollision(PlayerEgo * this, Array * param_1)

## decompile
```c

/* PlayerEgo::calcCollision(Array<KIPlayer*>*) */

void __thiscall PlayerEgo::calcCollision(PlayerEgo *this,Array *param_1)

{
  float fVar1;
  float fVar2;
  int *piVar3;
  int **this_00;
  int *piVar4;
  int iVar5;
  float fVar6;
  Matrix *pMVar7;
  SingleLevel *this_01;
  uint uVar8;
  uint *puVar9;
  uint uVar10;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float fVar11;
  float extraout_s1;
  float extraout_s2;
  undefined8 local_88;
  int local_80;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 local_40;
  int local_3c;
  
  fVar2 = DAT_000af47c;
  fVar1 = DAT_000af470;
  piVar3 = *(int **)(DAT_000af480 + 0xaf0e6);
  local_3c = *piVar3;
  if ((param_1 != (Array *)0x0) &&
     ((*(char *)((int)&this[0x17].field_18 + 2) == '\0' || (2 < (int)this[0xc].field_14 - 1U)))) {
    this_00 = &this[6].field_14;
    piVar4 = *(int **)(DAT_000af488 + 0xaf132);
    for (uVar10 = 0; uVar10 < *(uint *)param_1; uVar10 = uVar10 + 1) {
      this_01 = *(SingleLevel **)(*(int *)(param_1 + 4) + uVar10 * 4);
      if (this_01 != (SingleLevel *)0x0) {
        if ((uVar10 == 0) && (iVar5 = Status::inAlienOrbit(), iVar5 == 0)) {
          fVar6 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&this[9].field_4);
          in_fpscr = in_fpscr & 0xfffffff;
          if ((fVar6 < fVar1) && (*(char *)((int)&this_01->field_70 + 1) != '\0')) {
            *(undefined1 *)&this[0xf].field_18 = 1;
          }
        }
        AEGeometry::getPosition();
        iVar5 = (*(code *)this_01->field_0[0x10])(this_01,(Vector *)&local_88);
        if (iVar5 != 0) {
          iVar5 = KIPlayer::getType((KIPlayer *)this_01);
          if ((iVar5 == 0x4262) && (iVar5 = KIPlayer::isVisible((KIPlayer *)this_01), iVar5 != 0)) {
            iVar5 = PlayerWormHole::isShrinking((PlayerWormHole *)this_01);
            if ((iVar5 == 0) && (this[0xd].field_10 == (int *)0x0)) {
              (*(code *)this_01->field_0[10])((Vector *)&local_88,this_01);
              AbyssEngine::AEMath::Vector::operator=((Vector *)this_00,(Vector *)&local_88);
              AEGeometry::getPosition();
              AbyssEngine::AEMath::Vector::operator-=((Vector *)this_00,(Vector *)&local_88);
              fVar6 = (float)AbyssEngine::AEMath::VectorLength((Vector *)this_00);
              uVar8 = 40000 - (int)fVar6;
              iVar5 = FModSound::isPlaying(*piVar4);
              if ((int)uVar8 < 1) {
                if (iVar5 != 0) {
                  FModSound::stop(*piVar4);
                }
              }
              else {
                if (iVar5 == 0) {
                  FModSound::play(*piVar4,(Vector *)0x22,(Vector *)0x0,extraout_s0);
                }
                else {
                  local_88 = 0;
                  local_80 = 0;
                  (*(code *)this_01->field_0[10])(&local_48,this_01);
                  FModSound::updateEvent3DAttributes
                            ((FModSound *)*piVar4,0x22,(Vector *)&local_48,(Vector *)&local_88,false
                            );
                }
                AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_88,(Vector *)this_00);
                AbyssEngine::AEMath::Vector::operator=((Vector *)this_00,(Vector *)&local_88);
                local_88._0_4_ = this[6].field_14;
                local_88._4_4_ = this[6].field_18;
                local_80 = this[6].field_1C;
                AbyssEngine::AEMath::Vector::operator=
                          ((Vector *)&this[6].field_8,(Vector *)&local_88);
                fVar11 = (float)VectorSignedToFloat(uVar8 >> 8,(byte)(in_fpscr >> 0x16) & 3);
                AbyssEngine::AEMath::Vector::operator*=((Vector *)&this[6].field_8,fVar11);
                AEGeometry::translate((Vector *)this->field_8);
                hitCamera(this);
                if ((int)fVar6 < 1000) {
                  *(undefined1 *)((int)&this[1].m_pPlayer + 1) = 1;
                }
                puVar9 = *(uint **)(DAT_000af48c + 0xaf41e);
                pMVar7 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar9);
                AbyssEngine::PaintCanvas::TransformGetLocal(*puVar9);
                AbyssEngine::AEMath::operator*((Matrix *)&local_88,pMVar7);
                AbyssEngine::AEMath::Matrix::operator=
                          ((Matrix *)(this->m_pPlayer + 4),(Matrix *)&local_88);
              }
            }
          }
          else if ((char)this_01->field_3C == '\0') {
            iVar5 = KIPlayer::isVisible((KIPlayer *)this_01);
            if (iVar5 != 0) {
              AEGeometry::getPosition();
              (*(code *)this_01->field_0[0x16])((Vector *)&local_48,this_01,(Vector *)&local_88);
              local_88 = 0;
              local_80 = 0;
              iVar5 = AbyssEngine::AEMath::operator!=((Vector *)&local_48,(Vector *)&local_88);
              if (iVar5 != 0) {
                AEGeometry::setPosition((AEGeometry *)this->field_8,(Vector *)&local_48);
                puVar9 = *(uint **)(DAT_000af490 + 0xaf342);
                pMVar7 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar9);
                AbyssEngine::PaintCanvas::TransformGetLocal(*puVar9);
                AbyssEngine::AEMath::operator*((Matrix *)&local_88,pMVar7);
                AbyssEngine::AEMath::Matrix::operator=
                          ((Matrix *)(this->m_pPlayer + 4),(Matrix *)&local_88);
                if ((uVar10 == 0) && (*(char *)((int)&this_01->field_70 + 1) != '\0')) {
                  *(undefined1 *)&this[0xf].field_18 = 1;
                }
              }
LAB_000af386:
              hitCamera(this);
            }
          }
          else {
            iVar5 = KIPlayer::isDying();
            if (((iVar5 == 0) && (iVar5 = KIPlayer::isDead(), iVar5 == 0)) &&
               (((*(char *)&this[0xc].field_10 == '\0' &&
                 (*(char *)((int)&this[0x17].field_18 + 2) == '\0')) ||
                (this_01 != this[0xc].m_pLevel)))) {
              AEGeometry::getPosition();
              (*(code *)this_01->field_0[0x14])((Vector *)&local_88,this_01,(Vector *)&local_48);
              local_48 = 0;
              uStack_44 = 0;
              local_40 = 0;
              iVar5 = AbyssEngine::AEMath::operator!=((Vector *)&local_88,(Vector *)&local_48);
              if (iVar5 != 0) {
                fVar6 = (float)Player::setHitVector
                                         ((Player *)this_01->field_04,extraout_s0_00,extraout_s1,
                                          extraout_s2);
                Player::setBombForce(fVar6);
                Player::damage((Player *)this_01->field_04,9999);
                Player::damage((Player *)this->m_pPlayer,0x14);
                if (*(char *)&this[0x19].field_14 != '\0') {
                  *(float *)(this->m_pPlayer + 0x60) = *(float *)(this->m_pPlayer + 0x60) + fVar2;
                }
                goto LAB_000af386;
              }
            }
          }
        }
      }
    }
  }
  if (*piVar3 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000af0cc: push {r4,r5,r6,r7,lr}
  000af0ce: add r7,sp,#0xc
  000af0d0: push {r7,r8,r9,r10,r11}
  000af0d4: vpush {d8,d9}
  000af0d8: sub sp,#0x70
  000af0da: mov r5,r0
  000af0dc: ldr r0,[0x000af480]
  000af0de: mov r4,r1
  000af0e0: cmp r1,#0x0
  000af0e2: add r0,pc
  000af0e4: ldr r0,[r0,#0x0]
  000af0e6: str r0,[sp,#0x18]
  000af0e8: ldr r0,[r0,#0x0]
  000af0ea: str r0,[sp,#0x6c]
  000af0ec: beq.w 0x000af452
  000af0f0: ldrb.w r0,[r5,#0x356]
  000af0f4: cbz r0,0x000af102
  000af0f6: ldr.w r0,[r5,#0x1c4]
  000af0fa: subs r0,#0x1
  000af0fc: cmp r0,#0x3
  000af0fe: bcc.w 0x000af452
  000af102: add.w r0,r5,#0xe0
  000af106: str r0,[sp,#0x8]
  000af108: add.w r0,r5,#0xec
  000af10c: str r0,[sp,#0xc]
  000af10e: ldr r0,[0x000af484]
  000af110: add.w r9,r5,#0x148
  000af114: vldr.32 s16,[pc,#0x358]
  000af118: add r6,sp,#0x20
  000af11a: add r0,pc
  000af11c: vldr.32 s18,[pc,#0x35c]
  000af120: mov.w r11,#0x0
  000af124: ldr r0,[r0,#0x0]
  000af126: str r0,[sp,#0x1c]
  000af128: ldr r0,[0x000af488]
  000af12a: str.w r9,[sp,#0x14]
  000af12e: add r0,pc
  000af130: ldr r0,[r0,#0x0]
  000af132: str r0,[sp,#0x10]
  000af134: ldr r0,[r4,#0x0]
  000af136: cmp r11,r0
  000af138: bcs.w 0x000af452
  000af13c: ldr r0,[r4,#0x4]
  000af13e: ldr.w r8,[r0,r11,lsl #0x2]
  000af142: cmp.w r8,#0x0
  000af146: beq.w 0x000af38c
  000af14a: cmp.w r11,#0x0
  000af14e: bne 0x000af17a
  000af150: ldr r0,[sp,#0x1c]
  000af152: ldr r0,[r0,#0x0]
  000af154: blx 0x000723d0
  000af158: cbnz r0,0x000af17a
  000af15a: mov r0,r9
  000af15c: blx 0x0006ec44
  000af160: vmov s0,r0
  000af164: vcmpe.f32 s0,s16
  000af168: vmrs apsr,fpscr
  000af16c: bpl 0x000af17a
  000af16e: ldrb.w r0,[r8,#0x71]
  000af172: cbz r0,0x000af17a
  000af174: movs r0,#0x1
  000af176: strb.w r0,[r5,#0x234]
  000af17a: ldr r1,[r5,#0x8]
  000af17c: mov r0,r6
  000af17e: blx 0x000720b8
  000af182: ldr.w r0,[r8,#0x0]
  000af186: mov r1,r6
  000af188: ldr r2,[r0,#0x40]
  000af18a: mov r0,r8
  000af18c: blx r2
  000af18e: cmp r0,#0x0
  000af190: beq.w 0x000af38c
  000af194: mov r0,r8
  000af196: blx 0x00072010
  000af19a: movw r1,#0x4262
  000af19e: cmp r0,r1
  000af1a0: bne 0x000af244
  000af1a2: mov r0,r8
  000af1a4: blx 0x00072a78
  000af1a8: cmp r0,#0x0
  000af1aa: beq 0x000af244
  000af1ac: mov r0,r8
  000af1ae: blx 0x00072a84
  000af1b2: cmp r0,#0x0
  000af1b4: bne.w 0x000af38c
  000af1b8: ldr.w r0,[r5,#0x1e4]
  000af1bc: cmp r0,#0x0
  000af1be: bne.w 0x000af38c
  000af1c2: ldr.w r0,[r8,#0x0]
  000af1c6: mov r1,r8
  000af1c8: ldr r2,[r0,#0x28]
  000af1ca: mov r0,r6
  000af1cc: blx r2
  000af1ce: ldr.w r9,[sp,#0xc]
  000af1d2: mov r1,r6
  000af1d4: mov r0,r9
  000af1d6: blx 0x0006eb3c
  000af1da: ldr r1,[r5,#0x8]
  000af1dc: mov r0,r6
  000af1de: blx 0x000720b8
  000af1e2: mov r0,r9
  000af1e4: mov r1,r6
  000af1e6: blx 0x00072694
  000af1ea: mov r0,r9
  000af1ec: blx 0x0006ec44
  000af1f0: vmov s0,r0
  000af1f4: movw r1,#0x9c40
  000af1f8: vcvt.s32.f32 s0,s0
  000af1fc: ldr r0,[sp,#0x10]
  000af1fe: ldr r0,[r0,#0x0]
  000af200: vmov r10,s0
  000af204: sub.w r9,r1,r10
  000af208: movs r1,#0x22
  000af20a: blx 0x0007153c
  000af20e: cmp.w r9,#0x1
  000af212: blt.w 0x000af392
  000af216: cmp r0,#0x0
  000af218: beq.w 0x000af3a6
  000af21c: movs r0,#0x0
  000af21e: mov r1,r8
  000af220: strd r0,r0,[sp,#0x20]
  000af224: str r0,[sp,#0x28]
  000af226: ldr.w r0,[r8,#0x0]
  000af22a: ldr r2,[r0,#0x28]
  000af22c: add r0,sp,#0x60
  000af22e: blx r2
  000af230: ldr r0,[sp,#0x10]
  000af232: movs r1,#0x0
  000af234: add r2,sp,#0x60
  000af236: str r1,[sp,#0x0]
  000af238: movs r1,#0x22
  000af23a: mov r3,r6
  000af23c: ldr r0,[r0,#0x0]
  000af23e: blx 0x000726e8
  000af242: b 0x000af3b8
  000af244: ldrb.w r0,[r8,#0x3c]
  000af248: cmp r0,#0x0
  000af24a: beq 0x000af2f6
  000af24c: mov r0,r8
  000af24e: blx 0x00071f50
  000af252: cmp r0,#0x0
  000af254: bne.w 0x000af38c
  000af258: mov r0,r8
  000af25a: blx 0x00071ec0
  000af25e: cmp r0,#0x0
  000af260: bne.w 0x000af38c
  000af264: ldrb.w r0,[r5,#0x1c0]
  000af268: cbnz r0,0x000af270
  000af26a: ldrb.w r0,[r5,#0x356]
  000af26e: cbz r0,0x000af27a
  000af270: ldr.w r0,[r5,#0x1bc]
  000af274: cmp r8,r0
  000af276: beq.w 0x000af38c
  000af27a: add.w r10,sp,#0x60
  000af27e: ldr r1,[r5,#0x8]
  000af280: mov r0,r10
  000af282: blx 0x000720b8
  000af286: ldr.w r0,[r8,#0x0]
  000af28a: mov r1,r8
  000af28c: mov r2,r10
  000af28e: ldr r3,[r0,#0x50]
  000af290: mov r0,r6
  000af292: blx r3
  000af294: movs r0,#0x0
  000af296: mov r1,r10
  000af298: strd r0,r0,[sp,#0x60]
  000af29c: str r0,[sp,#0x68]
  000af29e: mov r0,r6
  000af2a0: blx 0x00072a90
  000af2a4: cmp r0,#0x0
  000af2a6: beq 0x000af38c
  000af2a8: add r3,sp,#0x20
  000af2aa: ldmia r3,{r1,r2,r3}
  000af2ac: ldr.w r0,[r8,#0x4]
  000af2b0: blx 0x00072a9c
  000af2b4: ldrb.w r1,[r5,#0x13c]
  000af2b8: ldr.w r0,[r8,#0x4]
  000af2bc: cmp r1,#0x0
  000af2be: adr r1,[0xaf474]
  000af2c0: it eq
  000af2c2: add.eq r1,#0x4
  000af2c4: ldr r1,[r1,#0x0]
  000af2c6: blx 0x00072aa8
  000af2ca: ldr.w r0,[r8,#0x4]
  000af2ce: movw r1,#0x270f
  000af2d2: blx 0x00072ab4
  000af2d6: ldr r0,[r5,#0x0]
  000af2d8: movs r1,#0x14
  000af2da: blx 0x00072ab4
  000af2de: ldrb.w r0,[r5,#0x398]
  000af2e2: cmp r0,#0x0
  000af2e4: beq 0x000af386
  000af2e6: ldr r0,[r5,#0x0]
  000af2e8: vldr.32 s0,[r0,#0x60]
  000af2ec: vadd.f32 s0,s0,s18
  000af2f0: vstr.32 s0,[r0,#0x60]
  000af2f4: b 0x000af386
  000af2f6: mov r0,r8
  000af2f8: blx 0x00072a78
  000af2fc: cmp r0,#0x0
  000af2fe: beq 0x000af38c
  000af300: ldr r1,[r5,#0x8]
  000af302: mov r0,r6
  000af304: blx 0x000720b8
  000af308: ldr.w r0,[r8,#0x0]
  000af30c: mov r10,r5
  000af30e: mov r5,r6
  000af310: add r6,sp,#0x60
  000af312: mov r1,r8
  000af314: mov r2,r5
  000af316: ldr r3,[r0,#0x58]
  000af318: mov r0,r6
  000af31a: blx r3
  000af31c: movs r0,#0x0
  000af31e: strd r0,r0,[sp,#0x20]
  000af322: str r0,[sp,#0x28]
  000af324: mov r0,r6
  000af326: mov r6,r5
  000af328: mov r5,r10
  000af32a: mov r1,r6
  000af32c: blx 0x00072a90
  000af330: cbz r0,0x000af386
  000af332: ldr r0,[r5,#0x8]
  000af334: add r1,sp,#0x60
  000af336: blx 0x00072148
  000af33a: ldr r0,[0x000af490]
  000af33c: ldr r1,[r5,#0x8]
  000af33e: add r0,pc
  000af340: ldr.w r10,[r0,#0x0]
  000af344: ldr r1,[r1,#0xc]
  000af346: ldr.w r0,[r10,#0x0]
  000af34a: blx 0x000720c4
  000af34e: ldr r1,[r5,#0x4]
  000af350: mov r9,r0
  000af352: ldr.w r0,[r10,#0x0]
  000af356: ldr r1,[r1,#0xc]
  000af358: blx 0x000720c4
  000af35c: mov r2,r0
  000af35e: mov r1,r9
  000af360: mov r0,r6
  000af362: ldr.w r9,[sp,#0x14]
  000af366: blx 0x0006f82c
  000af36a: ldr r0,[r5,#0x0]
  000af36c: mov r1,r6
  000af36e: adds r0,#0x4
  000af370: blx 0x0006f148
  000af374: cmp.w r11,#0x0
  000af378: bne 0x000af386
  000af37a: ldrb.w r0,[r8,#0x71]
  000af37e: cbz r0,0x000af386
  000af380: movs r0,#0x1
  000af382: strb.w r0,[r5,#0x234]
  000af386: mov r0,r5
  000af388: blx 0x000729ac
  000af38c: add.w r11,r11,#0x1
  000af390: b 0x000af134
  000af392: ldr.w r9,[sp,#0x14]
  000af396: cmp r0,#0x0
  000af398: beq 0x000af38c
  000af39a: ldr r0,[sp,#0x10]
  000af39c: movs r1,#0x22
  000af39e: ldr r0,[r0,#0x0]
  000af3a0: blx 0x000724a8
  000af3a4: b 0x000af38c
  000af3a6: ldr r0,[sp,#0x10]
  000af3a8: movs r1,#0x0
  000af3aa: str r1,[sp,#0x0]
  000af3ac: movs r1,#0x22
  000af3ae: movs r2,#0x0
  000af3b0: movs r3,#0x0
  000af3b2: ldr r0,[r0,#0x0]
  000af3b4: blx 0x00071548
  000af3b8: ldr.w r8,[sp,#0xc]
  000af3bc: mov r0,r6
  000af3be: mov r1,r8
  000af3c0: blx 0x0006ec80
  000af3c4: mov r0,r8
  000af3c6: mov r1,r6
  000af3c8: blx 0x0006eb3c
  000af3cc: vldr.64 d16,[r8]
  000af3d0: mov r1,r6
  000af3d2: ldr.w r0,[r8,#0x8]
  000af3d6: ldr.w r8,[sp,#0x8]
  000af3da: str r0,[sp,#0x28]
  000af3dc: vstr.64 d16,[sp,#0x20]
  000af3e0: mov r0,r8
  000af3e2: blx 0x0006eb3c
  000af3e6: lsr.w r0,r9,#0x8
  000af3ea: vmov s0,r0
  000af3ee: mov r0,r8
  000af3f0: vcvt.f32.s32 s0,s0
  000af3f4: vmov r1,s0
  000af3f8: blx 0x00072628
  000af3fc: ldr r0,[r5,#0x8]
  000af3fe: mov r1,r8
  000af400: blx 0x00072460
  000af404: mov r0,r5
  000af406: blx 0x000729ac
  000af40a: cmp.w r10,#0x3e8
  000af40e: itt lt
  000af410: mov.lt r0,#0x1
  000af412: strb.lt.w r0,[r5,#0x25]
  000af416: ldr r0,[0x000af48c]
  000af418: ldr r1,[r5,#0x8]
  000af41a: add r0,pc
  000af41c: ldr.w r9,[r0,#0x0]
  000af420: ldr r1,[r1,#0xc]
  000af422: ldr.w r0,[r9,#0x0]
  000af426: blx 0x000720c4
  000af42a: ldr r1,[r5,#0x4]
  000af42c: mov r8,r0
  000af42e: ldr.w r0,[r9,#0x0]
  000af432: ldr r1,[r1,#0xc]
  000af434: blx 0x000720c4
  000af438: mov r2,r0
  000af43a: mov r0,r6
  000af43c: mov r1,r8
  000af43e: blx 0x0006f82c
  000af442: ldr r0,[r5,#0x0]
  000af444: mov r1,r6
  000af446: adds r0,#0x4
  000af448: blx 0x0006f148
  000af44c: ldr.w r9,[sp,#0x14]
  000af450: b 0x000af38c
  000af452: ldr r0,[sp,#0x6c]
  000af454: ldr r1,[sp,#0x18]
  000af456: ldr r1,[r1,#0x0]
  000af458: subs r0,r1,r0
  000af45a: itttt eq
  000af45c: add.eq sp,#0x70
  000af45e: vpop.eq {d8,d9}
  000af462: add.eq sp,#0x4
  000af464: pop.eq.w {r8,r9,r10,r11}
  000af468: it eq
  000af46a: pop.eq {r4,r5,r6,r7,pc}
  000af46c: blx 0x0006e824
```
