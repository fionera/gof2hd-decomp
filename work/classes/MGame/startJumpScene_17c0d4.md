# MGame::startJumpScene
elf 0x17c0d4 body 814
Sig: undefined __thiscall startJumpScene(MGame * this)

## decompile
```c

/* MGame::startJumpScene() */

void __thiscall MGame::startJumpScene(MGame *this)

{
  int iVar1;
  undefined4 *puVar2;
  PlayerEgo *pPVar3;
  Engine *this_00;
  AEGeometry *this_01;
  undefined4 uVar4;
  float *pfVar5;
  uint uVar6;
  uint *puVar7;
  int *this_02;
  code *pcVar8;
  int *piVar9;
  float fVar10;
  float extraout_s0;
  float fVar11;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar12;
  float extraout_s2_02;
  undefined8 in_d4;
  undefined8 in_d5;
  undefined4 in_s14;
  undefined8 local_50;
  undefined8 local_48;
  Vector aVStack_40 [8];
  undefined4 uStack_38;
  int local_34;
  
  local_50 = CONCAT44((int)((ulonglong)in_d4 >> 0x20),*(undefined4 **)(DAT_0018c43c + 0x18c0ea));
  _uStack_38 = CONCAT44(**(undefined4 **)(DAT_0018c43c + 0x18c0ea),in_s14);
  local_48 = in_d5;
  Player::setVulnerable(SUB41(this->m_pPlayerEgo->m_pPlayer,0));
  Level::enableFog((Level *)this->field_78,false);
  iVar1 = PlayerEgo::isDockedToDockingPoint();
  if (iVar1 != 0) {
    PlayerEgo::dockToDockingPoint((KIPlayer *)this->m_pPlayerEgo,(Radar *)0x0);
    TargetFollowCamera::setActive((TargetFollowCamera *)this[1].field_2C,true);
    TargetFollowCamera::setLookAtCam(SUB41(this[1].field_2C,0));
    fVar10 = (float)TargetFollowCamera::useTargetsUpVector(SUB41(this[1].field_2C,0));
    PlayerEgo::setSpeed(this->m_pPlayerEgo,fVar10);
    PlayerEgo::setDockingState((int)this->m_pPlayerEgo);
  }
  iVar1 = PlayerEgo::isInTurretMode();
  if (iVar1 != 0) {
    PlayerEgo::setTurretMode(SUB41(this->m_pPlayerEgo,0));
  }
  piVar9 = *(int **)(DAT_0018c440 + 0x18c15a);
  FModSound::stop(*piVar9);
  switchCamera((int)this);
  this->field_70 = DAT_0018c410;
  Hud::releaseAllKeys();
  puVar2 = (undefined4 *)(DAT_0018c444 + 0x18c182);
  *(undefined1 *)&this[1].field_48 = 0;
  *(undefined1 *)&this->field_5C = 0;
  puVar7 = (uint *)*puVar2;
  uVar6 = *puVar7;
  iVar1 = Status::inAlienOrbit();
  fVar10 = DAT_0018c414;
  fVar11 = extraout_s1;
  fVar12 = extraout_s2;
  if (iVar1 != 0) {
    iVar1 = Status::getCurrentCampaignMission();
    pfVar5 = (float *)&DAT_0018c418;
    if (iVar1 < 0x50) {
      pfVar5 = (float *)&DAT_0018c41c;
    }
    fVar10 = *pfVar5;
    fVar11 = extraout_s1_00;
    fVar12 = extraout_s2_00;
  }
  AbyssEngine::PaintCanvas::CameraSetPerspective(uVar6,fVar10,fVar11,fVar12);
  PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
  *(undefined1 *)((int)&this->field_5C + 1) = 0;
  *(undefined1 *)((int)&this[1].field_0C + 2) = 0;
  pPVar3 = this->m_pPlayerEgo;
  *(undefined1 *)&this[1].field_14 = 1;
  *(undefined1 *)((int)&this->field_5C + 3) = 1;
  PlayerEgo::setCollide(pPVar3,false);
  TargetFollowCamera::setLookAtCam(SUB41(this[1].field_2C,0));
  PlayerEgo::stopBoost();
  this_00 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
  AbyssEngine::Engine::SetPostEffect(this_00,DAT_0018c424,false);
  if (*(char *)((int)&this[1].field_14 + 1) == '\0') {
    iVar1 = Level::getLandmarks();
    (**(code **)(**(int **)(*(int *)(iVar1 + 4) + 4) + 0x28))(aVStack_40);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&this[1].field_1C,aVStack_40);
    pPVar3 = this->m_pPlayerEgo;
    fVar10 = (float)this[1].field_24 + DAT_0018c434;
    this[1].field_24 = (int)fVar10;
    PlayerEgo::setPosition(pPVar3,this[1].field_1C,this[1].field_20,fVar10);
    PlayerEgo::setComputerControlled(this->m_pPlayerEgo,true);
    AEGeometry::setRotation
              ((AEGeometry *)this->m_pPlayerEgo->field_8,extraout_s0_00,extraout_s1_03,
               extraout_s2_02);
    fVar11 = (float)this[1].field_1C + DAT_0018c42c;
    fVar12 = (float)this[1].field_20 + DAT_0018c430;
    fVar10 = (float)this[1].field_24 + DAT_0018c438;
    this[1].field_1C = (int)fVar11;
    this[1].field_20 = (int)fVar12;
    this[1].field_24 = (int)fVar10;
    fVar10 = extraout_s1_04;
  }
  else {
    PlayerEgo::resetMovement(this->m_pPlayerEgo);
    PlayerEgo::setComputerControlled(this->m_pPlayerEgo,true);
    this_01 = operator_new(0xc0);
    AEGeometry::AEGeometry(this_01,0x3ab2,(PaintCanvas *)*puVar7,false);
    this[1].field_4C = (int)this_01;
    uVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar7);
    AbyssEngine::Transform::SetAnimationState(uVar4,1,0);
    PlayerEgo::getPosition();
    this_02 = &this[1].field_1C;
    pcVar8 = *(code **)(DAT_0018c44c + 0x18c268);
    (*pcVar8)(this_02,aVStack_40);
    fVar10 = (float)AEGeometry::getDirection();
    AbyssEngine::AEMath::operator*(aVStack_40,fVar10);
    AbyssEngine::AEMath::Vector::operator+=((Vector *)this_02,aVStack_40);
    AEGeometry::setPosition((AEGeometry *)this[1].field_4C,(Vector *)this_02);
    AEGeometry::getDirection();
    AEGeometry::setScaling(extraout_s0,extraout_s1_01,extraout_s2_01);
    local_50 = local_50 & 0xffffffff;
    local_48 = 0x3f800000;
    AEGeometry::setDirection((Vector *)this[1].field_4C,aVStack_40);
    local_50 = CONCAT44(DAT_0018c42c,(int *)local_50);
    local_48 = CONCAT44(DAT_0018c42c,DAT_0018c430);
    (*pcVar8)(aVStack_40,(int)&local_50 + 4);
    AbyssEngine::AEMath::MatrixRotateVector
              ((Matrix *)((int)&local_50 + 4),(Vector *)(this->m_pPlayerEgo->m_pPlayer + 4));
    (*pcVar8)(aVStack_40,(Matrix *)((int)&local_50 + 4));
    AbyssEngine::AEMath::Vector::operator+=((Vector *)this_02,aVStack_40);
    pcVar8 = *(code **)(DAT_0018c450 + 0x18c30e);
    (*pcVar8)(*piVar9,this->m_pPlayerEgo->field_1C);
    (*pcVar8)(*piVar9,0x23);
    (*pcVar8)(*piVar9,0x8d5);
    fVar10 = (float)(*pcVar8)(*piVar9,0x8d4);
    FModSound::play(*piVar9,(Vector *)0x20,(Vector *)0x0,fVar10);
    fVar11 = (float)this[1].field_1C;
    fVar12 = (float)this[1].field_20;
    fVar10 = extraout_s1_02;
  }
  TargetFollowCamera::setPosition((TargetFollowCamera *)this[1].field_2C,fVar11,fVar10,fVar12);
  if (*(int *)local_50 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0018c0d4: push {r4,r5,r6,r7,lr}
  0018c0d6: add r7,sp,#0xc
  0018c0d8: push {r7,r8,r9,r10,r11}
  0018c0dc: vpush {d3,d4,d5,d6,d7,d8}
  0018c0e0: mov r4,r0
  0018c0e2: ldr r0,[0x0018c43c]
  0018c0e4: movs r1,#0x0
  0018c0e6: add r0,pc
  0018c0e8: ldr r0,[r0,#0x0]
  0018c0ea: str r0,[sp,#0x8]
  0018c0ec: ldr r0,[r0,#0x0]
  0018c0ee: str r0,[sp,#0x24]
  0018c0f0: ldr r0,[r4,#0x58]
  0018c0f2: ldr r0,[r0,#0x0]
  0018c0f4: blx 0x0007294c
  0018c0f8: ldr r0,[r4,#0x78]
  0018c0fa: movs r1,#0x0
  0018c0fc: blx 0x0007843c
  0018c100: ldr r0,[r4,#0x58]
  0018c102: blx 0x00075928
  0018c106: cbz r0,0x0018c142
  0018c108: ldr r0,[r4,#0x58]
  0018c10a: movs r1,#0x0
  0018c10c: ldr.w r2,[r4,#0x80]
  0018c110: blx 0x00072838
  0018c114: ldr.w r0,[r4,#0xf4]
  0018c118: movs r1,#0x1
  0018c11a: blx 0x00072acc
  0018c11e: ldr.w r0,[r4,#0xf4]
  0018c122: movs r1,#0x0
  0018c124: blx 0x00072670
  0018c128: ldr.w r0,[r4,#0xf4]
  0018c12c: movs r1,#0x0
  0018c12e: blx 0x0007267c
  0018c132: ldr r0,[r4,#0x58]
  0018c134: movs r1,#0x0
  0018c136: blx 0x00076b40
  0018c13a: ldr r0,[r4,#0x58]
  0018c13c: movs r1,#0x0
  0018c13e: blx 0x00076ca8
  0018c142: ldr r0,[r4,#0x58]
  0018c144: blx 0x0007678c
  0018c148: cbz r0,0x0018c152
  0018c14a: ldr r0,[r4,#0x58]
  0018c14c: movs r1,#0x0
  0018c14e: blx 0x00072b08
  0018c152: ldr r0,[0x0018c440]
  0018c154: movs r1,#0x23
  0018c156: add r0,pc
  0018c158: ldr.w r11,[r0,#0x0]
  0018c15c: ldr.w r0,[r11,#0x0]
  0018c160: blx 0x000724a8
  0018c164: mov r0,r4
  0018c166: movs r1,#0x0
  0018c168: mov.w r9,#0x0
  0018c16c: blx 0x00078388
  0018c170: ldr r0,[r4,#0x74]
  0018c172: ldr r1,[0x0018c410]
  0018c174: str r1,[r4,#0x70]
  0018c176: blx 0x00077a1c
  0018c17a: ldr r0,[0x0018c444]
  0018c17c: ldr r1,[0x0018c448]
  0018c17e: add r0,pc
  0018c180: ldr.w r10,[r4,#0xf0]
  0018c184: add r1,pc
  0018c186: vldr.32 s16,[r4,#0x70]
  0018c18a: strb.w r9,[r4,#0x110]
  0018c18e: ldr r5,[r1,#0x0]
  0018c190: strb.w r9,[r4,#0x5c]
  0018c194: ldr.w r8,[r0,#0x0]
  0018c198: ldr r0,[r5,#0x0]
  0018c19a: ldr.w r6,[r8,#0x0]
  0018c19e: blx 0x000723d0
  0018c1a2: cbz r0,0x0018c1b8
  0018c1a4: ldr r0,[r5,#0x0]
  0018c1a6: blx 0x00071770
  0018c1aa: adr r1,[0x18c418]
  0018c1ac: cmp r0,#0x50
  0018c1ae: it lt
  0018c1b0: add.lt r1,#0x4
  0018c1b2: vldr.32 s0,[r1]
  0018c1b6: b 0x0018c1bc
  0018c1b8: vldr.32 s0,[pc,#0x258]
  0018c1bc: vmov r2,s16
  0018c1c0: ldr r3,[0x0018c420]
  0018c1c2: mov r0,r6
  0018c1c4: mov r1,r10
  0018c1c6: vstr.32 s0,[sp]
  0018c1ca: blx 0x00072004
  0018c1ce: ldr r0,[r4,#0x58]
  0018c1d0: movs r1,#0x0
  0018c1d2: blx 0x000728d4
  0018c1d6: strb.w r9,[r4,#0x5d]
  0018c1da: movs r1,#0x1
  0018c1dc: strb.w r9,[r4,#0xd6]
  0018c1e0: ldr r0,[r4,#0x58]
  0018c1e2: strb.w r1,[r4,#0xdc]
  0018c1e6: strb.w r1,[r4,#0x5f]
  0018c1ea: movs r1,#0x0
  0018c1ec: blx 0x00076a80
  0018c1f0: ldr.w r0,[r4,#0xf4]
  0018c1f4: movs r1,#0x1
  0018c1f6: blx 0x00072670
  0018c1fa: ldr r0,[r4,#0x58]
  0018c1fc: blx 0x00072cac
  0018c200: ldr r0,[r4,#0x8]
  0018c202: blx 0x0007258c
  0018c206: ldr r1,[0x0018c424]
  0018c208: movs r2,#0x0
  0018c20a: blx 0x000707f8
  0018c20e: ldrb.w r0,[r4,#0xdd]
  0018c212: cmp r0,#0x0
  0018c214: beq.w 0x0018c352
  0018c218: ldr r0,[r4,#0x58]
  0018c21a: blx 0x00078448
  0018c21e: ldr r0,[r4,#0x58]
  0018c220: movs r1,#0x1
  0018c222: blx 0x00076aa4
  0018c226: movs r0,#0xc0
  0018c228: blx 0x0006eb24
  0018c22c: mov r5,r0
  0018c22e: ldr.w r2,[r8,#0x0]
  0018c232: movw r1,#0x3ab2
  0018c236: movs r3,#0x0
  0018c238: blx 0x0007207c
  0018c23c: str.w r5,[r4,#0x114]
  0018c240: ldr r1,[r5,#0xc]
  0018c242: ldr.w r0,[r8,#0x0]
  0018c246: blx 0x00072088
  0018c24a: movs r1,#0x1
  0018c24c: movs r2,#0x0
  0018c24e: blx 0x0006fd18
  0018c252: add r6,sp,#0x18
  0018c254: ldr r1,[r4,#0x58]
  0018c256: mov r0,r6
  0018c258: blx 0x0007264c
  0018c25c: ldr r0,[0x0018c44c]
  0018c25e: add.w r9,r4,#0xe4
  0018c262: mov r1,r6
  0018c264: add r0,pc
  0018c266: ldr.w r10,[r0,#0x0]
  0018c26a: mov r0,r9
  0018c26c: blx r10
  0018c26e: ldr r0,[r4,#0x58]
  0018c270: add.w r8,sp,#0xc
  0018c274: ldr r1,[r0,#0x8]
  0018c276: mov r0,r8
  0018c278: blx 0x00072574
  0018c27c: add r6,sp,#0x18
  0018c27e: ldr r2,[0x0018c428]
  0018c280: mov r1,r8
  0018c282: mov r0,r6
  0018c284: blx 0x0006ec74
  0018c288: mov r0,r9
  0018c28a: mov r1,r6
  0018c28c: blx 0x00073534
  0018c290: ldr.w r0,[r4,#0x114]
  0018c294: mov r1,r9
  0018c296: blx 0x00072148
  0018c29a: ldr r0,[r4,#0x58]
  0018c29c: add r6,sp,#0x18
  0018c29e: ldr r1,[r0,#0x8]
  0018c2a0: mov r0,r6
  0018c2a2: blx 0x00072574
  0018c2a6: ldr.w r0,[r4,#0x114]
  0018c2aa: mov.w r1,#0x40000000
  0018c2ae: mov.w r2,#0x40000000
  0018c2b2: mov.w r3,#0x40000000
  0018c2b6: blx 0x000727b4
  0018c2ba: ldr.w r0,[r4,#0x114]
  0018c2be: mov.w r1,#0x3f800000
  0018c2c2: add r2,sp,#0xc
  0018c2c4: str r1,[sp,#0x10]
  0018c2c6: mov.w r8,#0x0
  0018c2ca: mov r1,r6
  0018c2cc: str.w r8,[sp,#0xc]
  0018c2d0: str.w r8,[sp,#0x14]
  0018c2d4: blx 0x000726ac
  0018c2d8: ldr r1,[0x0018c430]
  0018c2da: ldr r0,[0x0018c42c]
  0018c2dc: strd r0,r1,[sp,#0xc]
  0018c2e0: add r1,sp,#0xc
  0018c2e2: str r0,[sp,#0x14]
  0018c2e4: mov r0,r6
  0018c2e6: blx r10
  0018c2e8: ldr r0,[r4,#0x58]
  0018c2ea: add r5,sp,#0xc
  0018c2ec: mov r2,r6
  0018c2ee: ldr r0,[r0,#0x0]
  0018c2f0: adds r1,r0,#0x4
  0018c2f2: mov r0,r5
  0018c2f4: blx 0x0006f694
  0018c2f8: mov r0,r6
  0018c2fa: mov r1,r5
  0018c2fc: blx r10
  0018c2fe: mov r0,r9
  0018c300: mov r1,r6
  0018c302: blx 0x00073534
  0018c306: ldr r2,[0x0018c450]
  0018c308: ldr r1,[r4,#0x58]
  0018c30a: add r2,pc
  0018c30c: ldr.w r0,[r11,#0x0]
  0018c310: ldr r1,[r1,#0x1c]
  0018c312: ldr r5,[r2,#0x0]
  0018c314: blx r5
  0018c316: ldr.w r0,[r11,#0x0]
  0018c31a: movs r1,#0x23
  0018c31c: blx r5
  0018c31e: ldr.w r0,[r11,#0x0]
  0018c322: movw r1,#0x8d5
  0018c326: blx r5
  0018c328: ldr.w r0,[r11,#0x0]
  0018c32c: movw r1,#0x8d4
  0018c330: blx r5
  0018c332: ldr.w r0,[r11,#0x0]
  0018c336: movs r1,#0x20
  0018c338: movs r2,#0x0
  0018c33a: movs r3,#0x0
  0018c33c: str.w r8,[sp,#0x0]
  0018c340: blx 0x00071548
  0018c344: vldr.32 s0,[r4,#0xe4]
  0018c348: vldr.32 s2,[r4,#0xe8]
  0018c34c: vldr.32 s4,[r4,#0xec]
  0018c350: b 0x0018c3d4
  0018c352: ldr r0,[r4,#0x78]
  0018c354: blx 0x000725b0
  0018c358: ldr r0,[r0,#0x4]
  0018c35a: add r5,sp,#0x18
  0018c35c: ldr r1,[r0,#0x4]
  0018c35e: ldr r0,[r1,#0x0]
  0018c360: ldr r2,[r0,#0x28]
  0018c362: mov r0,r5
  0018c364: blx r2
  0018c366: add.w r0,r4,#0xe4
  0018c36a: mov r1,r5
  0018c36c: blx 0x0006eb3c
  0018c370: vldr.32 s0,[pc,#0xc0]
  0018c374: vldr.32 s2,[r4,#0xec]
  0018c378: ldr r0,[r4,#0x58]
  0018c37a: vadd.f32 s0,s2,s0
  0018c37e: ldrd r1,r2,[r4,#0xe4]
  0018c382: vmov r3,s0
  0018c386: vstr.32 s0,[r4,#0xec]
  0018c38a: blx 0x00072cb8
  0018c38e: ldr r0,[r4,#0x58]
  0018c390: movs r1,#0x1
  0018c392: blx 0x00076aa4
  0018c396: ldr r0,[r4,#0x58]
  0018c398: movs r1,#0x0
  0018c39a: movs r2,#0x0
  0018c39c: movs r3,#0x0
  0018c39e: ldr r0,[r0,#0x8]
  0018c3a0: blx 0x00073054
  0018c3a4: vldr.32 s0,[pc,#0x84]
  0018c3a8: vldr.32 s2,[r4,#0xe4]
  0018c3ac: vldr.32 s4,[r4,#0xe8]
  0018c3b0: vldr.32 s6,[r4,#0xec]
  0018c3b4: vadd.f32 s0,s2,s0
  0018c3b8: vldr.32 s8,[pc,#0x74]
  0018c3bc: vldr.32 s10,[pc,#0x78]
  0018c3c0: vadd.f32 s2,s4,s8
  0018c3c4: vadd.f32 s4,s6,s10
  0018c3c8: vstr.32 s0,[r4,#0xe4]
  0018c3cc: vstr.32 s2,[r4,#0xe8]
  0018c3d0: vstr.32 s4,[r4,#0xec]
  0018c3d4: vmov r1,s0
  0018c3d8: ldr.w r0,[r4,#0xf4]
  0018c3dc: vmov r2,s2
  0018c3e0: vmov r3,s4
  0018c3e4: blx 0x00076abc
  0018c3e8: ldr r0,[sp,#0x24]
  0018c3ea: ldr r1,[sp,#0x8]
  0018c3ec: ldr r1,[r1,#0x0]
  0018c3ee: subs r0,r1,r0
  0018c3f0: itttt eq
  0018c3f2: vpop.eq {d3,d4,d5,d6,d7,d8}
  0018c3f6: add.eq sp,#0x4
  0018c3f8: pop.eq.w {r8,r9,r10,r11}
  0018c3fc: pop.eq {r4,r5,r6,r7,pc}
  0018c3fe: blx 0x0006e824
```
