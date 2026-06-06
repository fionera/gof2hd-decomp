# Level::update
elf 0xc5f58 body 726
Sig: undefined __stdcall update(longlong param_1, bool param_2)

## decompile
```c

/* Level::update(long long, bool) */

void Level::update(longlong param_1,bool param_2)

{
  undefined4 uVar1;
  undefined1 auVar2 [16];
  byte bVar3;
  Level *this;
  uint uVar4;
  int iVar5;
  Station *pSVar6;
  uint *puVar7;
  int *piVar8;
  undefined4 extraout_r1;
  int extraout_r1_00;
  int extraout_r1_01;
  int extraout_r1_02;
  undefined4 extraout_r1_03;
  int extraout_r1_04;
  int extraout_r1_05;
  int extraout_r1_06;
  int extraout_r1_07;
  int extraout_r1_08;
  int extraout_r1_09;
  uint uVar9;
  int iVar10;
  int *piVar11;
  int *piVar12;
  uint in_fpscr;
  uint uVar13;
  float fVar14;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  undefined4 in_s1;
  float fVar15;
  undefined4 in_s14;
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  int in_stack_00000000;
  Vector aVStack_40 [8];
  undefined4 uStack_38;
  int local_34;
  
  this = (Level *)param_1;
  uVar9 = (uint)param_2;
  piVar11 = *(int **)(DAT_000d6238 + 0xd5f70);
  _uStack_38 = CONCAT44(*piVar11,in_s14);
  if (this[0x158] != (Level)0x0) {
    uVar4 = *(int *)(this + 0x150) - uVar9;
    *(uint *)(this + 0x150) = uVar4;
    if (0x7fffffff < uVar4) {
      this[0x158] = (Level)0x0;
    }
    fVar15 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
    fVar14 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x154),(byte)(in_fpscr >> 0x16) & 3);
    uVar1 = VectorGetElement(CONCAT44(in_s1,fVar15 / fVar14),0,4,0);
    auVar16 = VectorMultiply(*(undefined1 (*) [16])(this + 0x140),uVar1,4);
    auVar17._4_4_ = **(undefined4 **)(DAT_000d6248 + 0xd5fbe);
    auVar17._0_4_ = **(undefined4 **)(DAT_000d6244 + 0xd5fba);
    auVar17._8_4_ = **(undefined4 **)(DAT_000d623c + 0xd5fb2);
    auVar17._12_4_ = **(undefined4 **)(DAT_000d6240 + 0xd5fb4);
    auVar17 = FloatVectorCompareGreaterThan(auVar17,auVar16,2);
    auVar2._4_4_ = **(undefined4 **)(DAT_000d6248 + 0xd5fbe);
    auVar2._0_4_ = **(undefined4 **)(DAT_000d6244 + 0xd5fba);
    auVar2._8_4_ = **(undefined4 **)(DAT_000d623c + 0xd5fb2);
    auVar2._12_4_ = **(undefined4 **)(DAT_000d6240 + 0xd5fb4);
    auVar17 = VectorBitwiseSelect(auVar17,auVar2,auVar16);
    *(longlong *)(this + 0x140) = auVar17._0_8_;
    *(longlong *)(this + 0x148) = auVar17._8_8_;
  }
  piVar12 = *(int **)(DAT_000d624c + 0xd5fee);
  iVar5 = Status::getMission();
  if (iVar5 != 0) {
    Status::getMission();
    iVar5 = Mission::isEmpty();
    if (iVar5 == 0) {
      updateMissionOrbit(this,uVar9);
      goto LAB_000d601a;
    }
  }
  updateOrbit(this,uVar9);
LAB_000d601a:
  pSVar6 = (Station *)Status::getStation();
  iVar5 = Station::isAttackedByAliens(pSVar6);
  if ((iVar5 != 0) || (iVar5 = Status::inAlienOrbit(), iVar5 != 0)) {
    updateAlienAttackers((int)this);
  }
  puVar7 = *(uint **)(this + 0xe4);
  if (puVar7 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar7; uVar4 = uVar4 + 1) {
      piVar8 = *(int **)(puVar7[1] + uVar4 * 4);
      (**(code **)(*piVar8 + 0x10))(piVar8,uVar9);
      puVar7 = *(uint **)(this + 0xe4);
    }
  }
  puVar7 = *(uint **)(this + 0xe8);
  if (puVar7 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar7; uVar4 = uVar4 + 1) {
      piVar8 = *(int **)(puVar7[1] + uVar4 * 4);
      (**(code **)(*piVar8 + 0x10))(piVar8,uVar9);
      puVar7 = *(uint **)(this + 0xe8);
    }
  }
  iVar10 = *piVar12;
  pSVar6 = (Station *)Status::getStation();
  iVar5 = Station::getIndex(pSVar6);
  Status::getCurrentCampaignMission();
  fVar14 = (float)Status::getGammaRayDamagePerSecond(iVar10,iVar5);
  iVar5 = *(int *)(this + 0xf0);
  uVar4 = in_fpscr & 0xfffffff | (uint)(fVar14 < 0.0) << 0x1f | (uint)(fVar14 == 0.0) << 0x1e;
  uVar13 = uVar4 | (uint)NAN(fVar14) << 0x1c;
  bVar3 = (byte)(uVar4 >> 0x18);
  if ((!(bool)(bVar3 >> 6 & 1) && bVar3 >> 7 == ((byte)(uVar13 >> 0x1c) & 1)) && (iVar5 != 0)) {
    iVar5 = Status::getShip();
    iVar5 = Ship::getFirstEquipmentOfSort(iVar5);
    fVar14 = extraout_s0;
    if ((iVar5 != 0) && (iVar5 = Item::getAttribute(iVar5), fVar14 = extraout_s0_00, 0 < iVar5)) {
      fVar14 = (float)VectorSignedToFloat(iVar5,(byte)(uVar13 >> 0x16) & 3);
      fVar14 = (DAT_000d6230 - fVar14) / DAT_000d6230;
    }
    __aeabi_l2f(fVar14,uVar9);
    iVar5 = Player::getGammaHP();
    Player::damageGamma((Player *)**(undefined4 **)(this + 0xf0),extraout_s0_01);
    if ((0xe < iVar5) && (iVar5 = Player::getGammaHP(), iVar5 < 0xf)) {
      iVar5 = PlayerEgo::getHUD(*(PlayerEgo **)(this + 0xf0));
      Hud::hudEvent(iVar5,(PlayerEgo *)0x2c,*(int *)(this + 0xf0));
    }
    iVar5 = *(int *)(this + 0xf0);
  }
  if (iVar5 != 0) {
    if (*(int *)(this + 0x80) != 0) {
      PlayerEgo::getPosition();
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0xb4),aVStack_40);
      ParticleSystemManager::update(CONCAT44(extraout_r1,*(undefined4 *)(this + 0x80)));
      iVar5 = extraout_r1_00;
    }
    if (*(int *)(this + 0x74) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x74)));
      iVar5 = extraout_r1_01;
    }
    if (*(int *)(this + 0x78) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x78)));
      iVar5 = extraout_r1_02;
    }
    if (*(int *)(this + 0x88) != 0) {
      PlayerEgo::getPosition();
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0xb4),aVStack_40);
      ParticleSystemManager::update(CONCAT44(extraout_r1_03,*(undefined4 *)(this + 0x88)));
      iVar5 = extraout_r1_04;
    }
    if (*(int *)(this + 0x7c) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x7c)));
      iVar5 = extraout_r1_05;
    }
    if (*(int *)(this + 0x84) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x84)));
      iVar5 = extraout_r1_06;
    }
    if (*(int *)(this + 0x8c) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x8c)));
      iVar5 = extraout_r1_07;
    }
    if (*(int *)(this + 0x98) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x98)));
      iVar5 = extraout_r1_08;
    }
    if (*(int *)(this + 0x94) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x94)));
      iVar5 = extraout_r1_09;
    }
    if (*(int *)(this + 0x9c) != 0) {
      ParticleSystemManager::update(CONCAT44(iVar5,*(int *)(this + 0x9c)));
    }
  }
  if (in_stack_00000000 == 0) {
    LODManager::update(*(LODManager **)this,uVar9);
  }
  if (*piVar11 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000d5f58: push {r4,r5,r6,r7,lr}
  000d5f5a: add r7,sp,#0xc
  000d5f5c: push {r7,r8,r9,r10,r11}
  000d5f60: vpush {d6,d7,d8}
  000d5f64: mov r5,r0
  000d5f66: ldr r0,[0x000d6238]
  000d5f68: mov r8,r3
  000d5f6a: mov r4,r2
  000d5f6c: add r0,pc
  000d5f6e: ldr.w r10,[r0,#0x0]
  000d5f72: ldr.w r0,[r10,#0x0]
  000d5f76: str r0,[sp,#0xc]
  000d5f78: ldrb.w r0,[r5,#0x158]
  000d5f7c: cbz r0,0x000d5fe8
  000d5f7e: ldr.w r0,[r5,#0x150]
  000d5f82: add.w r6,r5,#0x140
  000d5f86: subs r0,r0,r4
  000d5f88: str.w r0,[r5,#0x150]
  000d5f8c: cmp.w r0,#0xffffffff
  000d5f90: itt le
  000d5f92: mov.le r1,#0x0
  000d5f94: strb.le.w r1,[r5,#0x158]
  000d5f98: vldr.32 s0,[r5,#0x154]
  000d5f9c: vmov s2,r0
  000d5fa0: vcvt.f32.s32 s2,s2
  000d5fa4: vcvt.f32.s32 s0,s0
  000d5fa8: ldr r0,[0x000d623c]
  000d5faa: ldr r1,[0x000d6240]
  000d5fac: ldr r2,[0x000d6244]
  000d5fae: add r0,pc
  000d5fb0: add r1,pc
  000d5fb2: ldr r3,[0x000d6248]
  000d5fb4: ldr r0,[r0,#0x0]
  000d5fb6: add r2,pc
  000d5fb8: ldr r1,[r1,#0x0]
  000d5fba: add r3,pc
  000d5fbc: vld1.32 {d16,d17},[r6]
  000d5fc0: vdiv.f32 s0,s2,s0
  000d5fc4: ldr r3,[r3,#0x0]
  000d5fc6: ldr r2,[r2,#0x0]
  000d5fc8: vmul.F32 q8,q8,d0[0x0]
  000d5fcc: vldr.32 s3,[r1]
  000d5fd0: vldr.32 s2,[r0]
  000d5fd4: vldr.32 s1,[r3]
  000d5fd8: vldr.32 s0,[r2]
  000d5fdc: vcgt.f32 q9,q0,q8
  000d5fe0: vbsl q9,q0,q8
  000d5fe4: vst1.32 {d18,d19},[r6]
  000d5fe8: ldr r0,[0x000d624c]
  000d5fea: add r0,pc
  000d5fec: ldr.w r11,[r0,#0x0]
  000d5ff0: ldr.w r0,[r11,#0x0]
  000d5ff4: blx 0x00072850
  000d5ff8: cbz r0,0x000d6008
  000d5ffa: ldr.w r0,[r11,#0x0]
  000d5ffe: blx 0x00072850
  000d6002: blx 0x00072868
  000d6006: cbz r0,0x000d6012
  000d6008: mov r0,r5
  000d600a: mov r1,r4
  000d600c: blx 0x00074494
  000d6010: b 0x000d601a
  000d6012: mov r0,r5
  000d6014: mov r1,r4
  000d6016: blx 0x000744a0
  000d601a: ldr.w r0,[r11,#0x0]
  000d601e: blx 0x00071c14
  000d6022: blx 0x0007321c
  000d6026: cbnz r0,0x000d6032
  000d6028: ldr.w r0,[r11,#0x0]
  000d602c: blx 0x000723d0
  000d6030: cbz r0,0x000d603a
  000d6032: mov r0,r5
  000d6034: mov r1,r4
  000d6036: blx 0x000744ac
  000d603a: ldr.w r0,[r5,#0xe4]
  000d603e: cbz r0,0x000d605e
  000d6040: movs r6,#0x0
  000d6042: b 0x000d6058
  000d6044: ldr r0,[r0,#0x4]
  000d6046: ldr.w r0,[r0,r6,lsl #0x2]
  000d604a: ldr r1,[r0,#0x0]
  000d604c: ldr r2,[r1,#0x10]
  000d604e: mov r1,r4
  000d6050: blx r2
  000d6052: ldr.w r0,[r5,#0xe4]
  000d6056: adds r6,#0x1
  000d6058: ldr r1,[r0,#0x0]
  000d605a: cmp r6,r1
  000d605c: bcc 0x000d6044
  000d605e: ldr.w r0,[r5,#0xe8]
  000d6062: cbz r0,0x000d6082
  000d6064: movs r6,#0x0
  000d6066: b 0x000d607c
  000d6068: ldr r0,[r0,#0x4]
  000d606a: ldr.w r0,[r0,r6,lsl #0x2]
  000d606e: ldr r1,[r0,#0x0]
  000d6070: ldr r2,[r1,#0x10]
  000d6072: mov r1,r4
  000d6074: blx r2
  000d6076: ldr.w r0,[r5,#0xe8]
  000d607a: adds r6,#0x1
  000d607c: ldr r1,[r0,#0x0]
  000d607e: cmp r6,r1
  000d6080: bcc 0x000d6068
  000d6082: ldr.w r6,[r11,#0x0]
  000d6086: mov r0,r6
  000d6088: blx 0x00071c14
  000d608c: blx 0x00071824
  000d6090: mov r9,r0
  000d6092: ldr.w r0,[r11,#0x0]
  000d6096: blx 0x00071770
  000d609a: mov r2,r0
  000d609c: mov r0,r6
  000d609e: mov r1,r9
  000d60a0: blx 0x000723dc
  000d60a4: vmov s16,r0
  000d60a8: ldr.w r1,[r5,#0xf0]
  000d60ac: vcmpe.f32 s16,#0
  000d60b0: vmrs apsr,fpscr
  000d60b4: ble 0x000d6148
  000d60b6: cmp r1,#0x0
  000d60b8: beq 0x000d6148
  000d60ba: ldr.w r0,[r11,#0x0]
  000d60be: blx 0x00071a58
  000d60c2: movs r1,#0x26
  000d60c4: blx 0x0007228c
  000d60c8: cbz r0,0x000d60ec
  000d60ca: movs r1,#0x34
  000d60cc: blx 0x000718e4
  000d60d0: cmp r0,#0x1
  000d60d2: blt 0x000d60ec
  000d60d4: vmov s0,r0
  000d60d8: vldr.32 s2,[pc,#0x154]
  000d60dc: vcvt.f32.s32 s0,s0
  000d60e0: vsub.f32 s0,s2,s0
  000d60e4: vdiv.f32 s0,s0,s2
  000d60e8: vmul.f32 s16,s16,s0
  000d60ec: mov r0,r4
  000d60ee: mov r1,r8
  000d60f0: blx 0x0006f7d8
  000d60f4: vmov s0,r0
  000d60f8: ldr.w r0,[r5,#0xf0]
  000d60fc: vldr.32 s2,[pc,#0x134]
  000d6100: vmul.f32 s0,s16,s0
  000d6104: ldr r0,[r0,#0x0]
  000d6106: vdiv.f32 s16,s0,s2
  000d610a: blx 0x00072814
  000d610e: vmov r1,s16
  000d6112: mov r6,r0
  000d6114: ldr.w r0,[r5,#0xf0]
  000d6118: ldr r0,[r0,#0x0]
  000d611a: blx 0x000744b8
  000d611e: cmp r6,#0xf
  000d6120: blt 0x000d6144
  000d6122: ldr.w r0,[r5,#0xf0]
  000d6126: ldr r0,[r0,#0x0]
  000d6128: blx 0x00072814
  000d612c: cmp r0,#0xe
  000d612e: bgt 0x000d6144
  000d6130: ldr.w r0,[r5,#0xf0]
  000d6134: blx 0x00072f10
  000d6138: ldr.w r2,[r5,#0xf0]
  000d613c: movs r1,#0x2c
  000d613e: movs r3,#0x0
  000d6140: blx 0x00072418
  000d6144: ldr.w r1,[r5,#0xf0]
  000d6148: ldr.w r9,[r7,#0x8]
  000d614c: cmp r1,#0x0
  000d614e: beq 0x000d6206
  000d6150: ldr.w r0,[r5,#0x80]
  000d6154: cbz r0,0x000d6174
  000d6156: mov r6,sp
  000d6158: mov r0,r6
  000d615a: blx 0x0007264c
  000d615e: add.w r0,r5,#0xb4
  000d6162: mov r1,r6
  000d6164: blx 0x0006eb3c
  000d6168: ldr.w r0,[r5,#0x80]
  000d616c: mov r2,r4
  000d616e: mov r3,r8
  000d6170: blx 0x000744c4
  000d6174: ldr r0,[r5,#0x74]
  000d6176: cbz r0,0x000d6180
  000d6178: mov r2,r4
  000d617a: mov r3,r8
  000d617c: blx 0x000744c4
  000d6180: ldr r0,[r5,#0x78]
  000d6182: cbz r0,0x000d618c
  000d6184: mov r2,r4
  000d6186: mov r3,r8
  000d6188: blx 0x000744c4
  000d618c: ldr.w r0,[r5,#0x88]
  000d6190: cbz r0,0x000d61b4
  000d6192: ldr.w r1,[r5,#0xf0]
  000d6196: mov r6,sp
  000d6198: mov r0,r6
  000d619a: blx 0x0007264c
  000d619e: add.w r0,r5,#0xb4
  000d61a2: mov r1,r6
  000d61a4: blx 0x0006eb3c
  000d61a8: ldr.w r0,[r5,#0x88]
  000d61ac: mov r2,r4
  000d61ae: mov r3,r8
  000d61b0: blx 0x000744c4
  000d61b4: ldr r0,[r5,#0x7c]
  000d61b6: cbz r0,0x000d61c0
  000d61b8: mov r2,r4
  000d61ba: mov r3,r8
  000d61bc: blx 0x000744c4
  000d61c0: ldr.w r0,[r5,#0x84]
  000d61c4: cbz r0,0x000d61ce
  000d61c6: mov r2,r4
  000d61c8: mov r3,r8
  000d61ca: blx 0x000744c4
  000d61ce: ldr.w r0,[r5,#0x8c]
  000d61d2: cbz r0,0x000d61dc
  000d61d4: mov r2,r4
  000d61d6: mov r3,r8
  000d61d8: blx 0x000744c4
  000d61dc: ldr.w r0,[r5,#0x98]
  000d61e0: cbz r0,0x000d61ea
  000d61e2: mov r2,r4
  000d61e4: mov r3,r8
  000d61e6: blx 0x000744c4
  000d61ea: ldr.w r0,[r5,#0x94]
  000d61ee: cbz r0,0x000d61f8
  000d61f0: mov r2,r4
  000d61f2: mov r3,r8
  000d61f4: blx 0x000744c4
  000d61f8: ldr.w r0,[r5,#0x9c]
  000d61fc: cbz r0,0x000d6206
  000d61fe: mov r2,r4
  000d6200: mov r3,r8
  000d6202: blx 0x000744c4
  000d6206: cmp.w r9,#0x0
  000d620a: bne 0x000d6214
  000d620c: ldr r0,[r5,#0x0]
  000d620e: mov r1,r4
  000d6210: blx 0x000744d0
  000d6214: ldr r0,[sp,#0xc]
  000d6216: ldr.w r1,[r10,#0x0]
  000d621a: subs r0,r1,r0
  000d621c: itttt eq
  000d621e: vpop.eq {d6,d7,d8}
  000d6222: add.eq sp,#0x4
  000d6224: pop.eq.w {r8,r9,r10,r11}
  000d6228: pop.eq {r4,r5,r6,r7,pc}
  000d622a: blx 0x0006e824
```
