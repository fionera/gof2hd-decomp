# TractorBeam::update
elf 0x15122c body 818
Sig: undefined __thiscall update(TractorBeam * this, int param_1, Radar * param_2, Level * param_3, Hud * param_4)

## decompile
```c

/* TractorBeam::update(int, Radar*, Level*, Hud*) */

void __thiscall
TractorBeam::update(TractorBeam *this,int param_1,Radar *param_2,Level *param_3,Hud *param_4)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  float fVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int *piVar7;
  int *piVar8;
  int iVar9;
  AEGeometry *this_00;
  Vector *pVVar10;
  uint in_fpscr;
  float fVar11;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float fVar12;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar13;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  Vector aVStack_58 [12];
  Vector aVStack_4c [12];
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  int local_34;
  
  piVar7 = *(int **)(DAT_00161568 + 0x161246);
  local_34 = *piVar7;
  iVar2 = *(int *)(this + 0xc);
  iVar9 = *(int *)(param_2 + 0x1c);
  if (iVar9 != 0 || iVar2 != 0) {
    if (iVar2 == 0) {
      Level::getPlayer();
      iVar2 = PlayerEgo::isInTurretMode();
      if (iVar2 == 0) {
        iVar2 = KIPlayer::cargoAvailable();
        if (iVar2 == 0) {
          this[0x10] = (TractorBeam)0x0;
          *(undefined4 *)(param_2 + 0x1c) = 0;
        }
        else {
          if (*(int *)(iVar9 + 0x78) == 0) {
            KIPlayer::createCrate(iVar9);
          }
          *(int *)(this + 0xc) = iVar9;
          uVar6 = Player::getHitpoints();
          *(undefined4 *)(this + 0x18) = uVar6;
          this[0x10] = (TractorBeam)0x1;
        }
        goto LAB_00161540;
      }
      iVar2 = *(int *)(this + 0xc);
      if (iVar2 == 0) goto LAB_0016131a;
    }
    if ((*(int *)(iVar2 + 0x78) == 0) ||
       ((*(char *)(iVar2 + 0x3c) == '\0' && (iVar2 = Player::isActive(), iVar2 == 0)))) {
LAB_0016131a:
      iVar2 = DAT_0016156c;
      *(undefined4 *)(param_2 + 8) = 0;
      *(undefined4 *)(param_2 + 0x1c) = 0;
      *(undefined4 *)(this + 0xc) = 0;
      puVar5 = *(undefined4 **)(iVar2 + 0x161326);
      *(undefined2 *)(this + 0x10) = 0;
      if (*piVar7 - local_34 == 0) {
        (*(code *)(DAT_001abd24 + 0x1abd28))();
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(*puVar5,*piVar7 - local_34);
    }
    uVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_00161570 + 0x16127e));
    AbyssEngine::Transform::Update((ulonglong)uVar3,SUB41(param_1,0));
    AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator=((Vector *)this,(Vector *)&local_40);
    Level::getPlayer();
    PlayerEgo::getPosition();
    AbyssEngine::AEMath::Vector::operator-=((Vector *)this,(Vector *)&local_40);
    fVar4 = (float)AbyssEngine::AEMath::VectorLength((Vector *)this);
    Status::getShip();
    iVar2 = Ship::getIndex();
    if (iVar2 == 0x2c) {
      Level::getPlayer();
      fVar11 = (float)PlayerEgo::GetDirVector();
      AbyssEngine::AEMath::operator*((Vector *)&local_40,fVar11);
      fVar11 = extraout_s0;
      fVar12 = extraout_s1;
      fVar13 = extraout_s2;
    }
    else {
      Status::getShip();
      iVar2 = Ship::getIndex();
      if (iVar2 == 0x31) {
        Level::getPlayer();
        fVar11 = (float)PlayerEgo::GetDirVector();
        AbyssEngine::AEMath::operator*(aVStack_4c,fVar11);
        Level::getPlayer();
        fVar11 = (float)PlayerEgo::GetUpVector();
        AbyssEngine::AEMath::operator*((Vector *)&local_64,fVar11);
        AbyssEngine::AEMath::operator+((Vector *)&local_40,aVStack_4c);
        fVar11 = extraout_s0_01;
        fVar12 = extraout_s1_01;
        fVar13 = extraout_s2_01;
      }
      else {
        local_40 = 0;
        uStack_3c = 0;
        local_38 = 0;
        fVar11 = extraout_s0_00;
        fVar12 = extraout_s1_00;
        fVar13 = extraout_s2_00;
      }
    }
    AEGeometry::setScaling(fVar11,fVar12,fVar13);
    pVVar10 = *(Vector **)(this + 0x14);
    AbyssEngine::AEMath::operator-(aVStack_58,(Vector *)this);
    AbyssEngine::AEMath::VectorNormalize((AEMath *)aVStack_4c,aVStack_58);
    local_60 = 0x3f800000;
    local_64 = 0;
    local_5c = 0;
    AEGeometry::setDirection(pVVar10,aVStack_4c);
    this_00 = *(AEGeometry **)(this + 0x14);
    Level::getPlayer();
    PlayerEgo::getPosition();
    AbyssEngine::AEMath::operator+(aVStack_4c,aVStack_58);
    AEGeometry::setPosition(this_00,aVStack_4c);
    uVar3 = in_fpscr & 0xfffffff | (uint)(fVar4 == DAT_00161564) << 0x1e |
            (uint)(DAT_00161564 <= fVar4) << 0x1d;
    bVar1 = (byte)(uVar3 >> 0x18);
    if (!(bool)(bVar1 >> 5 & 1) || (bool)(bVar1 >> 6)) {
      KIPlayer::captureCrate(*(KIPlayer **)(this + 0xc),param_4);
      iVar2 = DAT_00161578;
      *(undefined4 *)(this + 0xc) = 0;
      this[0x10] = (TractorBeam)0x0;
      *(undefined4 *)(param_2 + 8) = 0;
      piVar8 = *(int **)(iVar2 + 0x16150e);
      *(undefined4 *)(param_2 + 0x1c) = 0;
      this[0x11] = (TractorBeam)0x0;
      fVar4 = (float)FModSound::stop(*piVar8);
      FModSound::play(*piVar8,(Vector *)&DAT_00000004,(Vector *)0x0,fVar4);
    }
    else {
      *(undefined4 *)(param_2 + 0x1c) = 0;
      AbyssEngine::AEMath::VectorNormalize((AEMath *)aVStack_4c,(Vector *)this);
      AbyssEngine::AEMath::Vector::operator=((Vector *)this,aVStack_4c);
      fVar4 = (float)VectorSignedToFloat(param_1 * 10,(byte)(uVar3 >> 0x16) & 3);
      AbyssEngine::AEMath::Vector::operator*=((Vector *)this,fVar4);
      pVVar10 = *(Vector **)(*(int *)(this + 0xc) + 0x78);
      AbyssEngine::AEMath::operator-((AEMath *)aVStack_4c,(Vector *)this);
      AEGeometry::translate(pVVar10);
      iVar2 = KIPlayer::isDead();
      fVar4 = extraout_s0_02;
      if ((((iVar2 != 0) || (iVar2 = KIPlayer::isDying(), fVar4 = extraout_s0_03, iVar2 != 0)) &&
          (piVar8 = *(int **)(this + 0xc), (char)piVar8[0x10] == '\0')) &&
         (*(char *)((int)piVar8 + 0x41) == '\0')) {
        AbyssEngine::AEMath::operator-((AEMath *)aVStack_4c,(Vector *)this);
        fVar4 = (float)(**(code **)(*piVar8 + 0x20))(piVar8,(AEMath *)aVStack_4c);
      }
      if (this[0x11] == (TractorBeam)0x0) {
        FModSound::play(**(int **)(DAT_0016157c + 0x1614ea),(Vector *)0x0,(Vector *)0x0,fVar4);
        this[0x11] = (TractorBeam)0x1;
      }
    }
  }
LAB_00161540:
  if (*piVar7 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0016122c: push {r4,r5,r6,r7,lr}
  0016122e: add r7,sp,#0xc
  00161230: push {r7,r8,r9,r10,r11}
  00161234: vpush {d8}
  00161238: sub sp,#0x58
  0016123a: mov r4,r0
  0016123c: ldr r0,[0x00161568]
  0016123e: mov r10,r3
  00161240: mov r9,r2
  00161242: add r0,pc
  00161244: mov r8,r1
  00161246: ldr r5,[r0,#0x0]
  00161248: ldr r0,[r5,#0x0]
  0016124a: str r0,[sp,#0x54]
  0016124c: ldr r0,[r4,#0xc]
  0016124e: ldr r6,[r2,#0x1c]
  00161250: orrs.w r1,r6,r0
  00161254: beq.w 0x00161540
  00161258: ldr.w r11,[r7,#0x8]
  0016125c: cmp r0,#0x0
  0016125e: beq 0x00161306
  00161260: ldr r1,[r0,#0x78]
  00161262: cmp r1,#0x0
  00161264: beq 0x0016131a
  00161266: ldrb.w r1,[r0,#0x3c]
  0016126a: cbnz r1,0x00161276
  0016126c: ldr r0,[r0,#0x4]
  0016126e: blx 0x00071f14
  00161272: cmp r0,#0x0
  00161274: beq 0x0016131a
  00161276: ldr r0,[0x00161570]
  00161278: ldr r1,[r4,#0x14]
  0016127a: add r0,pc
  0016127c: ldr r0,[r0,#0x0]
  0016127e: ldr r1,[r1,#0xc]
  00161280: ldr r0,[r0,#0x0]
  00161282: blx 0x00072088
  00161286: movs r1,#0x0
  00161288: asr.w r3,r8, asr #0x1f
  0016128c: mov r2,r8
  0016128e: str r1,[sp,#0x0]
  00161290: blx 0x0006f7cc
  00161294: ldr r0,[r4,#0xc]
  00161296: add r6,sp,#0x48
  00161298: ldr r1,[r0,#0x78]
  0016129a: mov r0,r6
  0016129c: blx 0x000720b8
  001612a0: mov r0,r4
  001612a2: mov r1,r6
  001612a4: blx 0x0006eb3c
  001612a8: mov r0,r10
  001612aa: blx 0x00072034
  001612ae: add r6,sp,#0x48
  001612b0: mov r1,r0
  001612b2: mov r0,r6
  001612b4: blx 0x0007264c
  001612b8: mov r0,r4
  001612ba: mov r1,r6
  001612bc: blx 0x00072694
  001612c0: mov r0,r4
  001612c2: blx 0x0006ec44
  001612c6: mov r1,r0
  001612c8: ldr r0,[0x00161574]
  001612ca: str r1,[sp,#0x10]
  001612cc: vmov s16,r1
  001612d0: add r0,pc
  001612d2: ldr r6,[r0,#0x0]
  001612d4: ldr r0,[r6,#0x0]
  001612d6: blx 0x00071a58
  001612da: blx 0x000719c8
  001612de: cmp r0,#0x2c
  001612e0: str r5,[sp,#0x14]
  001612e2: strd r11,r8,[sp,#0x8]
  001612e6: bne 0x00161354
  001612e8: mov r0,r10
  001612ea: mov r5,r10
  001612ec: blx 0x00072034
  001612f0: add r6,sp,#0x3c
  001612f2: mov r1,r0
  001612f4: mov r0,r6
  001612f6: blx 0x00073540
  001612fa: ldr r2,[0x0016155c]
  001612fc: add r0,sp,#0x48
  001612fe: mov r1,r6
  00161300: blx 0x0006ec74
  00161304: b 0x001613e2
  00161306: mov r0,r10
  00161308: blx 0x00072034
  0016130c: blx 0x0007678c
  00161310: cmp r0,#0x0
  00161312: beq 0x001613b0
  00161314: ldr r0,[r4,#0xc]
  00161316: cmp r0,#0x0
  00161318: bne 0x00161260
  0016131a: ldr r0,[0x0016156c]
  0016131c: movs r1,#0x0
  0016131e: str.w r1,[r9,#0x8]
  00161322: add r0,pc
  00161324: str.w r1,[r9,#0x1c]
  00161328: str r1,[r4,#0xc]
  0016132a: ldr r0,[r0,#0x0]
  0016132c: strh r1,[r4,#0x10]
  0016132e: ldr r0,[r0,#0x0]
  00161330: ldr r1,[sp,#0x54]
  00161332: ldr r2,[r5,#0x0]
  00161334: subs r1,r2,r1
  00161336: itttt eq
  00161338: mov.eq r1,#0x0
  0016133a: add.eq sp,#0x58
  0016133c: vpop.eq {d8}
  00161340: add.eq sp,#0x4
  00161342: ittt eq
  00161344: pop.eq.w {r8,r9,r10,r11}
  00161348: pop.eq.w {r4,r5,r6,r7,lr}
  0016134c: b.eq.w 0x001abd18
  00161350: blx 0x0006e824
  00161354: ldr r0,[r6,#0x0]
  00161356: str.w r10,[sp,#0x4]
  0016135a: blx 0x00071a58
  0016135e: blx 0x000719c8
  00161362: cmp r0,#0x31
  00161364: bne 0x001613d8
  00161366: ldr r5,[sp,#0x4]
  00161368: mov r0,r5
  0016136a: blx 0x00072034
  0016136e: add r6,sp,#0x30
  00161370: mov r1,r0
  00161372: mov r0,r6
  00161374: blx 0x00073540
  00161378: add.w r10,sp,#0x3c
  0016137c: ldr r2,[0x0016155c]
  0016137e: mov r1,r6
  00161380: mov r0,r10
  00161382: blx 0x0006ec74
  00161386: mov r0,r5
  00161388: blx 0x00072034
  0016138c: add r6,sp,#0x18
  0016138e: mov r1,r0
  00161390: mov r0,r6
  00161392: blx 0x0007354c
  00161396: add.w r8,sp,#0x24
  0016139a: ldr r2,[0x00161560]
  0016139c: mov r1,r6
  0016139e: mov r0,r8
  001613a0: blx 0x0006ec74
  001613a4: add r0,sp,#0x48
  001613a6: mov r1,r10
  001613a8: mov r2,r8
  001613aa: blx 0x0006f1cc
  001613ae: b 0x001613e2
  001613b0: mov r0,r6
  001613b2: blx 0x000758f8
  001613b6: cmp r0,#0x0
  001613b8: beq.w 0x00161538
  001613bc: ldr r0,[r6,#0x78]
  001613be: cbnz r0,0x001613c8
  001613c0: mov r0,r6
  001613c2: movs r1,#0x0
  001613c4: blx 0x00075904
  001613c8: str r6,[r4,#0xc]
  001613ca: ldr r0,[r6,#0x4]
  001613cc: blx 0x000724f0
  001613d0: str r0,[r4,#0x18]
  001613d2: movs r0,#0x1
  001613d4: strb r0,[r4,#0x10]
  001613d6: b 0x00161540
  001613d8: movs r0,#0x0
  001613da: strd r0,r0,[sp,#0x48]
  001613de: str r0,[sp,#0x50]
  001613e0: ldr r5,[sp,#0x4]
  001613e2: ldr r3,[sp,#0x10]
  001613e4: mov.w r1,#0x3f000000
  001613e8: ldr r0,[r4,#0x14]
  001613ea: mov.w r2,#0x3f000000
  001613ee: blx 0x000727b4
  001613f2: add r6,sp,#0x30
  001613f4: add r2,sp,#0x48
  001613f6: mov r1,r4
  001613f8: ldr.w r10,[r4,#0x14]
  001613fc: mov r0,r6
  001613fe: blx 0x0006ec38
  00161402: add.w r8,sp,#0x3c
  00161406: mov r1,r6
  00161408: mov r0,r8
  0016140a: blx 0x0006ec80
  0016140e: mov.w r0,#0x3f800000
  00161412: add r2,sp,#0x24
  00161414: str r0,[sp,#0x28]
  00161416: mov.w r11,#0x0
  0016141a: mov r0,r10
  0016141c: mov r1,r8
  0016141e: str.w r11,[sp,#0x24]
  00161422: str.w r11,[sp,#0x2c]
  00161426: blx 0x000726ac
  0016142a: mov r0,r5
  0016142c: ldr.w r8,[r4,#0x14]
  00161430: blx 0x00072034
  00161434: add r5,sp,#0x30
  00161436: mov r1,r0
  00161438: mov r0,r5
  0016143a: blx 0x0007264c
  0016143e: add r6,sp,#0x3c
  00161440: add r2,sp,#0x48
  00161442: mov r1,r5
  00161444: mov r0,r6
  00161446: blx 0x0006f1cc
  0016144a: mov r0,r8
  0016144c: mov r1,r6
  0016144e: blx 0x00072148
  00161452: vldr.32 s0,[pc,#0x110]
  00161456: vcmpe.f32 s16,s0
  0016145a: vmrs apsr,fpscr
  0016145e: bls 0x001614fa
  00161460: add r5,sp,#0x3c
  00161462: mov r1,r4
  00161464: str.w r11,[r9,#0x1c]
  00161468: mov r0,r5
  0016146a: blx 0x0006ec80
  0016146e: mov r0,r4
  00161470: mov r1,r5
  00161472: blx 0x0006eb3c
  00161476: ldr r0,[sp,#0xc]
  00161478: add.w r0,r0,r0, lsl #0x2
  0016147c: lsls r0,r0,#0x1
  0016147e: vmov s0,r0
  00161482: mov r0,r4
  00161484: vcvt.f32.s32 s0,s0
  00161488: vmov r1,s0
  0016148c: blx 0x00072628
  00161490: ldr r0,[r4,#0xc]
  00161492: add r6,sp,#0x3c
  00161494: mov r1,r4
  00161496: ldr r5,[r0,#0x78]
  00161498: mov r0,r6
  0016149a: blx 0x0006f4cc
  0016149e: mov r0,r5
  001614a0: mov r1,r6
  001614a2: blx 0x00072460
  001614a6: ldr r0,[r4,#0xc]
  001614a8: blx 0x00071ec0
  001614ac: cbnz r0,0x001614b6
  001614ae: ldr r0,[r4,#0xc]
  001614b0: blx 0x00071f50
  001614b4: cbz r0,0x001614d8
  001614b6: ldr r5,[r4,#0xc]
  001614b8: ldrb.w r0,[r5,#0x40]
  001614bc: cbnz r0,0x001614d8
  001614be: ldrb.w r0,[r5,#0x41]
  001614c2: cbnz r0,0x001614d8
  001614c4: add r6,sp,#0x3c
  001614c6: mov r1,r4
  001614c8: mov r0,r6
  001614ca: blx 0x0006f4cc
  001614ce: ldr r0,[r5,#0x0]
  001614d0: mov r1,r6
  001614d2: ldr r2,[r0,#0x20]
  001614d4: mov r0,r5
  001614d6: blx r2
  001614d8: ldrb r0,[r4,#0x11]
  001614da: ldr r5,[sp,#0x14]
  001614dc: cbnz r0,0x00161540
  001614de: ldr r0,[0x0016157c]
  001614e0: movs r1,#0x0
  001614e2: movs r2,#0x0
  001614e4: movs r3,#0x0
  001614e6: add r0,pc
  001614e8: ldr r0,[r0,#0x0]
  001614ea: ldr r0,[r0,#0x0]
  001614ec: str r1,[sp,#0x0]
  001614ee: movs r1,#0x0
  001614f0: blx 0x00071548
  001614f4: movs r0,#0x1
  001614f6: strb r0,[r4,#0x11]
  001614f8: b 0x00161540
  001614fa: ldr r0,[r4,#0xc]
  001614fc: ldr r1,[sp,#0x8]
  001614fe: blx 0x000772fc
  00161502: ldr r0,[0x00161578]
  00161504: movs r1,#0x0
  00161506: str.w r11,[r4,#0xc]
  0016150a: add r0,pc
  0016150c: strb.w r11,[r4,#0x10]
  00161510: str.w r11,[r9,#0x8]
  00161514: ldr r5,[r0,#0x0]
  00161516: str.w r11,[r9,#0x1c]
  0016151a: strb.w r11,[r4,#0x11]
  0016151e: ldr r0,[r5,#0x0]
  00161520: blx 0x000724a8
  00161524: ldr r0,[r5,#0x0]
  00161526: movs r1,#0x4
  00161528: movs r2,#0x0
  0016152a: movs r3,#0x0
  0016152c: str.w r11,[sp,#0x0]
  00161530: blx 0x00071548
  00161534: ldr r5,[sp,#0x14]
  00161536: b 0x00161540
  00161538: movs r0,#0x0
  0016153a: strb r0,[r4,#0x10]
  0016153c: str.w r0,[r9,#0x1c]
  00161540: ldr r0,[sp,#0x54]
  00161542: ldr r1,[r5,#0x0]
  00161544: subs r0,r1,r0
  00161546: itttt eq
  00161548: add.eq sp,#0x58
  0016154a: vpop.eq {d8}
  0016154e: add.eq sp,#0x4
  00161550: pop.eq.w {r8,r9,r10,r11}
  00161554: it eq
  00161556: pop.eq {r4,r5,r6,r7,pc}
  00161558: blx 0x0006e824
  001abd18: bx pc
```
