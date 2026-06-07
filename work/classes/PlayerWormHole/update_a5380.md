# PlayerWormHole::update
elf 0xa5380 body 996
Sig: undefined __thiscall update(PlayerWormHole * this, int param_1)

## decompile
```c

/* PlayerWormHole::update(int) */

void __thiscall PlayerWormHole::update(PlayerWormHole *this,int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  Station *this_00;
  int iVar5;
  int iVar6;
  PlayerEgo *pPVar7;
  KIPlayer *pKVar8;
  uint *puVar9;
  Vector *this_01;
  undefined4 *puVar10;
  int *piVar11;
  PlayerWormHole *pPVar12;
  code *pcVar13;
  uint in_fpscr;
  float fVar14;
  undefined4 uVar15;
  float extraout_s1;
  float fVar16;
  undefined4 uVar17;
  float fVar18;
  undefined4 uVar19;
  int local_3c;
  int local_38;
  
  piVar11 = *(int **)(DAT_000b562c + 0xb5394);
  puVar9 = *(uint **)(DAT_000b5630 + 0xb5396);
  iVar1 = *piVar11;
  uVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar9);
  AbyssEngine::Transform::Update((ulonglong)uVar2,SUB41(param_1,0));
  if (this[0xf5] == (PlayerWormHole)0x0) goto LAB_000b5758;
  iVar3 = *(int *)(this + 0x150) + param_1;
  *(int *)(this + 0x150) = iVar3;
  if (iVar3 < 0) {
    fVar14 = (float)VectorSignedToFloat(-iVar3,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(this + 0x154) = 0x1000 - (int)((fVar14 / DAT_000b5770) * DAT_000b5774);
  }
  else if (60000 < iVar3) {
    iVar3 = Status::getCurrentCampaignMission();
    if (this[0x15c] == (PlayerWormHole)0x0) {
LAB_000b5444:
      iVar4 = *(int *)(this + 0x150);
    }
    else {
      if (iVar3 == 0x2a) {
        iVar4 = Status::inAlienOrbit();
        if (iVar4 == 0) goto LAB_000b5444;
      }
      else if ((iVar3 != 0x28) || (iVar4 = Status::inAlienOrbit(), iVar4 != 0)) goto LAB_000b5444;
      iVar4 = 60000;
      *(undefined4 *)(this + 0x150) = 60000;
    }
    fVar14 = (float)VectorSignedToFloat(iVar4 + -60000,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(this + 0x154) = 0x1000 - (int)((fVar14 / DAT_000b5770) * DAT_000b5774);
    if (63000 < iVar4) {
      iVar4 = Status::inAlienOrbit();
      if (iVar4 == 0) {
        this_00 = (Station *)Status::getStation();
        iVar4 = Station::isAttackedByAliens(this_00);
        if (iVar4 != 0) goto LAB_000b5496;
      }
      else {
LAB_000b5496:
        if ((this[0x15c] == (PlayerWormHole)0x0) ||
           (iVar4 = Status::inAlienOrbit(), iVar3 != 0x2a || iVar4 != 0)) {
          *(undefined4 *)(this + 0x150) = DAT_000b5778;
          iVar4 = Status::inAlienOrbit();
          puVar10 = *(undefined4 **)(DAT_000b578c + 0xb54c2);
          if (iVar4 == 0) {
            pcVar13 = *(code **)(DAT_000b5794 + 0xb5530);
            iVar4 = (*pcVar13)(*puVar10,40000);
            iVar5 = (*pcVar13)(*puVar10,2);
            local_38 = -1;
            if (iVar5 == 0) {
              local_38 = 1;
            }
            local_38 = (iVar4 + 20000) * local_38;
            iVar4 = (*pcVar13)(*puVar10,40000);
            iVar5 = (*pcVar13)(*puVar10,2);
            local_3c = -1;
            if (iVar5 == 0) {
              local_3c = 1;
            }
            local_3c = (iVar4 + 20000) * local_3c;
            iVar5 = (*pcVar13)(*puVar10,40000);
            iVar6 = (*pcVar13)(*puVar10,2);
            iVar4 = -1;
            if (iVar6 == 0) {
              iVar4 = 1;
            }
            iVar4 = iVar4 * (iVar5 + 20000);
          }
          else {
            pcVar13 = *(code **)(DAT_000b5790 + 0xb54cc);
            iVar4 = (*pcVar13)(*puVar10,60000);
            iVar5 = (*pcVar13)(*puVar10,2);
            local_38 = -1;
            if (iVar5 == 0) {
              local_38 = 1;
            }
            local_38 = local_38 * (iVar4 + 30000);
            local_3c = (*pcVar13)(*puVar10,40000);
            local_3c = local_3c + 20000;
            iVar4 = (*pcVar13)(*puVar10,40000);
            iVar4 = DAT_000b577c - iVar4;
          }
          if (iVar3 == 0x1d || iVar3 == 0x29) {
            Level::getPlayer();
            PlayerEgo::getPosition();
            AbyssEngine::AEMath::Vector::operator=
                      ((Vector *)(this + 0x90),(Vector *)&stack0xffffffcc);
            fVar14 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
            fVar16 = (float)VectorSignedToFloat(local_3c,(byte)(in_fpscr >> 0x16) & 3);
            fVar18 = (float)VectorSignedToFloat(local_38,(byte)(in_fpscr >> 0x16) & 3);
            iVar4 = (int)(*(float *)(this + 0x98) + fVar14 * DAT_000b5780 + fVar14);
            local_3c = (int)(*(float *)(this + 0x94) + fVar16 * DAT_000b5780 + fVar16);
            local_38 = (int)(*(float *)(this + 0x90) + fVar18 * DAT_000b5780 + fVar18);
          }
          pPVar12 = this + 0x54;
          uVar15 = VectorSignedToFloat(local_38,(byte)(in_fpscr >> 0x16) & 3);
          uVar17 = VectorSignedToFloat(local_3c,(byte)(in_fpscr >> 0x16) & 3);
          uVar19 = VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
          (**(code **)(*(int *)this + 0x48))(this,uVar15,uVar17,uVar19);
          pPVar7 = (PlayerEgo *)Level::getPlayer();
          iVar3 = PlayerEgo::goingToWormhole(pPVar7);
          if (iVar3 != 0) {
            pcVar13 = *(code **)(DAT_000b5798 + 0xb5682);
            pPVar7 = (PlayerEgo *)(*pcVar13)(*(undefined4 *)pPVar12);
            iVar3 = PlayerEgo::getHUD(pPVar7);
            iVar4 = (*pcVar13)(*(undefined4 *)pPVar12);
            Hud::hudEvent(iVar3,(PlayerEgo *)0x6,iVar4);
            pKVar8 = (KIPlayer *)(*pcVar13)(*(undefined4 *)pPVar12);
            PlayerEgo::setAutoPilot(pKVar8);
          }
          goto LAB_000b56a8;
        }
      }
      this[0xf5] = (PlayerWormHole)0x0;
      AEGeometry::setVisible(SUB41(*(undefined4 *)(this + 8),0));
    }
  }
LAB_000b56a8:
  PlayerStaticFar::update((int)this);
  uVar2 = *puVar9;
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  AbyssEngine::PaintCanvas::CameraGetLocal(uVar2);
  AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&stack0xffffffcc);
  this_01 = (Vector *)(this + 0x90);
  pcVar13 = *(code **)(DAT_000b579c + 0xb56d6);
  (*pcVar13)(this_01,(Matrix *)&stack0xffffffcc);
  fVar14 = (float)VectorSignedToFloat(*(int *)(this + 0x154) << 4,(byte)(in_fpscr >> 0x16) & 3);
  AEGeometry::setScaling(fVar14 * DAT_000b5784,extraout_s1,DAT_000b5784);
  AEGeometry::getPosition();
  (*pcVar13)((Vector *)(this + 0x134),&stack0xffffffcc);
  AbyssEngine::AEMath::Vector::operator-=(this_01,(Vector *)(this + 0x134));
  AbyssEngine::AEMath::VectorNormalize((AEMath *)&stack0xffffffcc,this_01);
  (*pcVar13)(this_01,(AEMath *)&stack0xffffffcc);
  *(float *)(this + 0x90) = *(float *)(this + 0x90) + 0.5;
  AEGeometry::setDirection(*(Vector **)(this + 8),this_01);
LAB_000b5758:
  if (*piVar11 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b5380: push {r4,r5,r6,r7,lr}
  000b5382: add r7,sp,#0xc
  000b5384: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000b5388: mov r8,r0
  000b538a: ldr r0,[0x000b562c]
  000b538c: mov r5,r1
  000b538e: ldr r1,[0x000b5630]
  000b5390: add r0,pc
  000b5392: add r1,pc
  000b5394: ldr.w r9,[r0,#0x0]
  000b5398: ldr r4,[r1,#0x0]
  000b539a: ldr.w r0,[r9,#0x0]
  000b539e: str r0,[sp,#0x18]
  000b53a0: ldr.w r1,[r8,#0x8]
  000b53a4: ldr r0,[r4,#0x0]
  000b53a6: ldr r1,[r1,#0xc]
  000b53a8: blx 0x00072088
  000b53ac: movs r1,#0x0
  000b53ae: asrs r3,r5,#0x1f
  000b53b0: mov r2,r5
  000b53b2: str r1,[sp,#0x0]
  000b53b4: blx 0x0006f7cc
  000b53b8: ldrb.w r0,[r8,#0xf5]
  000b53bc: cmp r0,#0x0
  000b53be: beq.w 0x000b5758
  000b53c2: ldr.w r0,[r8,#0x150]
  000b53c6: add r0,r5
  000b53c8: str.w r0,[r8,#0x150]
  000b53cc: cmp.w r0,#0xffffffff
  000b53d0: ble 0x000b540e
  000b53d2: movw r1,#0xea60
  000b53d6: cmp r0,r1
  000b53d8: ble.w 0x000b56a8
  000b53dc: ldr r0,[0x000b5788]
  000b53de: add r0,pc
  000b53e0: ldr r6,[r0,#0x0]
  000b53e2: ldr r0,[r6,#0x0]
  000b53e4: blx 0x00071770
  000b53e8: mov r10,r0
  000b53ea: ldrb.w r0,[r8,#0x15c]
  000b53ee: cbz r0,0x000b5444
  000b53f0: cmp.w r10,#0x2a
  000b53f4: beq 0x000b543a
  000b53f6: cmp.w r10,#0x28
  000b53fa: bne 0x000b5444
  000b53fc: ldr r0,[r6,#0x0]
  000b53fe: blx 0x000723d0
  000b5402: cbnz r0,0x000b5444
  000b5404: movw r0,#0xea60
  000b5408: str.w r0,[r8,#0x150]
  000b540c: b 0x000b5448
  000b540e: rsbs r0,r0
  000b5410: vldr.32 s2,[pc,#0x35c]
  000b5414: vmov s0,r0
  000b5418: vcvt.f32.s32 s0,s0
  000b541c: vdiv.f32 s0,s0,s2
  000b5420: vldr.32 s2,[pc,#0x350]
  000b5424: vmul.f32 s0,s0,s2
  000b5428: vcvt.s32.f32 s0,s0
  000b542c: vmov r0,s0
  000b5430: rsb.w r0,r0,#0x1000
  000b5434: str.w r0,[r8,#0x154]
  000b5438: b 0x000b56a8
  000b543a: ldr r0,[r6,#0x0]
  000b543c: blx 0x000723d0
  000b5440: cmp r0,#0x0
  000b5442: bne 0x000b5404
  000b5444: ldr.w r0,[r8,#0x150]
  000b5448: movw r1,#0xea60
  000b544c: vldr.32 s2,[pc,#0x320]
  000b5450: subs r1,r0,r1
  000b5452: vmov s0,r1
  000b5456: vcvt.f32.s32 s0,s0
  000b545a: vdiv.f32 s0,s0,s2
  000b545e: vldr.32 s2,[pc,#0x314]
  000b5462: vmul.f32 s0,s0,s2
  000b5466: vcvt.s32.f32 s0,s0
  000b546a: vmov r1,s0
  000b546e: rsb.w r1,r1,#0x1000
  000b5472: str.w r1,[r8,#0x154]
  000b5476: movw r1,#0xf619
  000b547a: cmp r0,r1
  000b547c: blt.w 0x000b56a8
  000b5480: ldr r0,[r6,#0x0]
  000b5482: blx 0x000723d0
  000b5486: cbnz r0,0x000b5496
  000b5488: ldr r0,[r6,#0x0]
  000b548a: blx 0x00071c14
  000b548e: blx 0x0007321c
  000b5492: cmp r0,#0x0
  000b5494: beq 0x000b5518
  000b5496: ldrb.w r0,[r8,#0x15c]
  000b549a: cbz r0,0x000b54ac
  000b549c: ldr r0,[r6,#0x0]
  000b549e: blx 0x000723d0
  000b54a2: cmp.w r10,#0x2a
  000b54a6: it eq
  000b54a8: cmp.eq r0,#0x0
  000b54aa: beq 0x000b5518
  000b54ac: ldr r0,[0x000b5778]
  000b54ae: str.w r0,[r8,#0x150]
  000b54b2: ldr r0,[r6,#0x0]
  000b54b4: blx 0x000723d0
  000b54b8: mov r1,r0
  000b54ba: ldr r0,[0x000b578c]
  000b54bc: cmp r1,#0x0
  000b54be: add r0,pc
  000b54c0: ldr r6,[r0,#0x0]
  000b54c2: ldr r0,[r6,#0x0]
  000b54c4: beq 0x000b552a
  000b54c6: ldr r1,[0x000b5790]
  000b54c8: add r1,pc
  000b54ca: ldr.w r11,[r1,#0x0]
  000b54ce: movw r1,#0xea60
  000b54d2: blx r11
  000b54d4: ldr r1,[r6,#0x0]
  000b54d6: movw r2,#0x7530
  000b54da: add r0,r2
  000b54dc: str r0,[sp,#0x8]
  000b54de: mov r0,r1
  000b54e0: movs r1,#0x2
  000b54e2: blx r11
  000b54e4: mov.w r2,#0xffffffff
  000b54e8: ldr r1,[r6,#0x0]
  000b54ea: cmp r0,#0x0
  000b54ec: it eq
  000b54ee: mov.eq r2,#0x1
  000b54f0: ldr r0,[sp,#0x8]
  000b54f2: muls r0,r2
  000b54f4: str r0,[sp,#0x8]
  000b54f6: mov r0,r1
  000b54f8: movw r1,#0x9c40
  000b54fc: blx r11
  000b54fe: ldr r1,[r6,#0x0]
  000b5500: movw r2,#0x4e20
  000b5504: add r0,r2
  000b5506: str r0,[sp,#0x4]
  000b5508: mov r0,r1
  000b550a: movw r1,#0x9c40
  000b550e: blx r11
  000b5510: ldr r1,[0x000b577c]
  000b5512: sub.w r11,r1,r0
  000b5516: b 0x000b55a2
  000b5518: ldr.w r0,[r8,#0x8]
  000b551c: movs r1,#0x0
  000b551e: strb.w r1,[r8,#0xf5]
  000b5522: movs r1,#0x0
  000b5524: blx 0x00072d24
  000b5528: b 0x000b56a8
  000b552a: ldr r1,[0x000b5794]
  000b552c: add r1,pc
  000b552e: ldr.w r11,[r1,#0x0]
  000b5532: movw r1,#0x9c40
  000b5536: blx r11
  000b5538: ldr r1,[r6,#0x0]
  000b553a: movw r2,#0x4e20
  000b553e: add r0,r2
  000b5540: str r0,[sp,#0x8]
  000b5542: mov r0,r1
  000b5544: movs r1,#0x2
  000b5546: blx r11
  000b5548: cmp r0,#0x0
  000b554a: mov.w r0,#0xffffffff
  000b554e: it eq
  000b5550: mov.eq r0,#0x1
  000b5552: ldr r1,[sp,#0x8]
  000b5554: muls r0,r1
  000b5556: movw r1,#0x9c40
  000b555a: str r0,[sp,#0x8]
  000b555c: ldr r0,[r6,#0x0]
  000b555e: blx r11
  000b5560: movw r1,#0x4e20
  000b5564: add r0,r1
  000b5566: str r0,[sp,#0x4]
  000b5568: movs r1,#0x2
  000b556a: ldr r0,[r6,#0x0]
  000b556c: blx r11
  000b556e: cmp r0,#0x0
  000b5570: mov.w r0,#0xffffffff
  000b5574: it eq
  000b5576: mov.eq r0,#0x1
  000b5578: ldr r1,[sp,#0x4]
  000b557a: muls r0,r1
  000b557c: movw r1,#0x9c40
  000b5580: str r0,[sp,#0x4]
  000b5582: ldr r0,[r6,#0x0]
  000b5584: blx r11
  000b5586: ldr r1,[r6,#0x0]
  000b5588: movw r2,#0x4e20
  000b558c: adds r6,r0,r2
  000b558e: mov r0,r1
  000b5590: movs r1,#0x2
  000b5592: blx r11
  000b5594: cmp r0,#0x0
  000b5596: mov.w r0,#0xffffffff
  000b559a: it eq
  000b559c: mov.eq r0,#0x1
  000b559e: mul r11,r0,r6
  000b55a2: cmp.w r10,#0x1d
  000b55a6: it ne
  000b55a8: cmp.ne.w r10,#0x29
  000b55ac: bne 0x000b5634
  000b55ae: mov r10,r8
  000b55b0: ldr.w r0,[r10,#0x54]!
  000b55b4: blx 0x00072034
  000b55b8: add r6,sp,#0xc
  000b55ba: mov r1,r0
  000b55bc: mov r0,r6
  000b55be: blx 0x0007264c
  000b55c2: add.w r0,r10,#0x3c
  000b55c6: mov r1,r6
  000b55c8: blx 0x0006eb3c
  000b55cc: vmov s0,r11
  000b55d0: vldr.32 s6,[pc,#0x1ac]
  000b55d4: vcvt.f32.s32 s0,s0
  000b55d8: ldr r0,[sp,#0x4]
  000b55da: vmov s2,r0
  000b55de: vcvt.f32.s32 s2,s2
  000b55e2: ldr r0,[sp,#0x8]
  000b55e4: vmov s4,r0
  000b55e8: vcvt.f32.s32 s4,s4
  000b55ec: vldr.32 s8,[r10,#0x3c]
  000b55f0: vldr.32 s10,[r10,#0x40]
  000b55f4: vldr.32 s12,[r10,#0x44]
  000b55f8: vmla.f32 s10,s2,s6
  000b55fc: vmla.f32 s12,s0,s6
  000b5600: vmla.f32 s8,s4,s6
  000b5604: vadd.f32 s0,s12,s0
  000b5608: vadd.f32 s2,s10,s2
  000b560c: vadd.f32 s4,s8,s4
  000b5610: vcvt.s32.f32 s0,s0
  000b5614: vcvt.s32.f32 s2,s2
  000b5618: vcvt.s32.f32 s4,s4
  000b561c: vmov r11,s0
  000b5620: vmov r1,s2
  000b5624: vmov r0,s4
  000b5628: b 0x000b563c
  000b5634: add.w r10,r8,#0x54
  000b5638: ldrd r1,r0,[sp,#0x4]
  000b563c: vmov s0,r0
  000b5640: vmov s2,r1
  000b5644: vmov s4,r11
  000b5648: vcvt.f32.s32 s0,s0
  000b564c: vcvt.f32.s32 s2,s2
  000b5650: vcvt.f32.s32 s4,s4
  000b5654: ldr.w r0,[r8,#0x0]
  000b5658: ldr r6,[r0,#0x48]
  000b565a: mov r0,r8
  000b565c: vmov r1,s0
  000b5660: vmov r2,s2
  000b5664: vmov r3,s4
  000b5668: blx r6
  000b566a: ldr.w r0,[r8,#0x54]
  000b566e: blx 0x00072034
  000b5672: blx 0x00073228
  000b5676: cbz r0,0x000b56a8
  000b5678: ldr r1,[0x000b5798]
  000b567a: ldr.w r0,[r10,#0x0]
  000b567e: add r1,pc
  000b5680: ldr r6,[r1,#0x0]
  000b5682: blx r6
  000b5684: blx 0x00072f10
  000b5688: mov r11,r0
  000b568a: ldr.w r0,[r10,#0x0]
  000b568e: blx r6
  000b5690: mov r2,r0
  000b5692: mov r0,r11
  000b5694: movs r1,#0x6
  000b5696: movs r3,#0x0
  000b5698: blx 0x00072418
  000b569c: ldr.w r0,[r10,#0x0]
  000b56a0: blx r6
  000b56a2: movs r1,#0x0
  000b56a4: blx 0x000728d4
  000b56a8: mov r0,r8
  000b56aa: mov r1,r5
  000b56ac: blx 0x00073234
  000b56b0: ldr r5,[r4,#0x0]
  000b56b2: mov r0,r5
  000b56b4: blx 0x000717f4
  000b56b8: mov r1,r0
  000b56ba: mov r0,r5
  000b56bc: blx 0x0006ff1c
  000b56c0: add r4,sp,#0xc
  000b56c2: mov r1,r0
  000b56c4: mov r0,r4
  000b56c6: blx 0x0006f16c
  000b56ca: ldr r0,[0x000b579c]
  000b56cc: add.w r5,r8,#0x90
  000b56d0: mov r1,r4
  000b56d2: add r0,pc
  000b56d4: ldr r6,[r0,#0x0]
  000b56d6: mov r0,r5
  000b56d8: blx r6
  000b56da: ldr.w r1,[r8,#0x154]
  000b56de: ldr.w r0,[r8,#0x8]
  000b56e2: vldr.32 s2,[pc,#0xa0]
  000b56e6: lsls r1,r1,#0x4
  000b56e8: vmov s0,r1
  000b56ec: vcvt.f32.s32 s0,s0
  000b56f0: vmul.f32 s0,s0,s2
  000b56f4: vmov r1,s0
  000b56f8: mov r2,r1
  000b56fa: mov r3,r1
  000b56fc: blx 0x000727b4
  000b5700: add.w r10,sp,#0xc
  000b5704: ldr.w r1,[r8,#0x8]
  000b5708: mov r0,r10
  000b570a: blx 0x000720b8
  000b570e: add.w r4,r8,#0x134
  000b5712: mov r1,r10
  000b5714: mov r0,r4
  000b5716: blx r6
  000b5718: mov r0,r5
  000b571a: mov r1,r4
  000b571c: blx 0x00072694
  000b5720: add r4,sp,#0xc
  000b5722: mov r1,r5
  000b5724: mov r0,r4
  000b5726: blx 0x0006ec80
  000b572a: mov r0,r5
  000b572c: mov r1,r4
  000b572e: blx r6
  000b5730: vmov.f32 s0,0x3f000000
  000b5734: vldr.32 s2,[r8,#0x90]
  000b5738: ldr.w r0,[r8,#0x8]
  000b573c: mov.w r1,#0x3f800000
  000b5740: add r2,sp,#0xc
  000b5742: vadd.f32 s0,s2,s0
  000b5746: vstr.32 s0,[r8,#0x90]
  000b574a: str r1,[sp,#0x10]
  000b574c: movs r1,#0x0
  000b574e: str r1,[sp,#0xc]
  000b5750: str r1,[sp,#0x14]
  000b5752: mov r1,r5
  000b5754: blx 0x000726ac
  000b5758: ldr r0,[sp,#0x18]
  000b575a: ldr.w r1,[r9,#0x0]
  000b575e: subs r0,r1,r0
  000b5760: ittt eq
  000b5762: add.eq sp,#0x1c
  000b5764: pop.eq.w {r8,r9,r10,r11}
  000b5768: pop.eq {r4,r5,r6,r7,pc}
  000b576a: blx 0x0006e824
```
