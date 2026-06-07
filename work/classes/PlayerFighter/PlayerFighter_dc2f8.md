# PlayerFighter::PlayerFighter
elf 0xdc2f8 body 1234
Sig: undefined __thiscall PlayerFighter(PlayerFighter * this, int param_1, int param_2, Player * param_3, AEGeometry * param_4, float param_5, float param_6, float param_7, bool param_8)

## decompile
```c

/* PlayerFighter::PlayerFighter(int, int, Player*, AEGeometry*, float, float, float, bool) */

void __thiscall
PlayerFighter::PlayerFighter
          (PlayerFighter *this,int param_1,int param_2,Player *param_3,AEGeometry *param_4,
          float param_5,float param_6,float param_7,bool param_8)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int *piVar4;
  Route *pRVar5;
  Generator *this_00;
  void *pvVar6;
  Explosion *this_01;
  int __flags;
  void *__arg;
  int *piVar7;
  int *piVar8;
  uint uVar9;
  uint in_fpscr;
  float fVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  float fVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  undefined8 uVar21;
  undefined3 in_stack_00000005;
  char local_d4 [4];
  int local_d0 [13];
  float local_9c [4];
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  int local_6c;
  
  piVar8 = *(int **)(DAT_000ec6a4 + 0xec314);
  local_6c = *piVar8;
  KIPlayer::KIPlayer((KIPlayer *)this,param_1,-1,param_3,param_4,param_5,param_6,param_7,param_8);
  *(int *)this = *(int *)(DAT_000ec6a8 + 0xec34e) + 8;
  *(undefined4 *)(this + 0x200) = 0;
  *(undefined4 *)(this + 0x204) = 0;
  *(undefined4 *)(this + 0x208) = 0;
  __aeabi_memclr4((Vector *)(this + 0x158),0x48);
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x218));
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 600));
  piVar7 = *(int **)(DAT_000ec6ac + 0xec37a);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  fVar14 = (float)VectorSignedToFloat(iVar1 + -30000,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  iVar2 = AbyssEngine::AERandom::nextInt(*piVar7);
  uVar16 = VectorSignedToFloat(iVar2 + 20000,(byte)(in_fpscr >> 0x16) & 3);
  uVar18 = VectorSignedToFloat(iVar1 + -10000,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  uVar19 = VectorSignedToFloat(iVar1 + 5000,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  iVar2 = AbyssEngine::AERandom::nextInt(*piVar7);
  uVar20 = VectorSignedToFloat(iVar2 + 20000,(byte)(in_fpscr >> 0x16) & 3);
  uVar11 = VectorSignedToFloat(iVar1 + -10000,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  uVar12 = VectorSignedToFloat(iVar1 + 5000,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  iVar2 = AbyssEngine::AERandom::nextInt(*piVar7);
  uVar13 = VectorSignedToFloat(iVar2 + 55000,(byte)(in_fpscr >> 0x16) & 3);
  uVar15 = VectorSignedToFloat(iVar1 + -10000,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  uVar17 = VectorSignedToFloat(iVar1 + -30000,(byte)(in_fpscr >> 0x16) & 3);
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  iVar2 = AbyssEngine::AERandom::nextInt(*piVar7);
  local_74 = VectorSignedToFloat(iVar1 + -10000,(byte)(in_fpscr >> 0x16) & 3);
  local_70 = VectorSignedToFloat(iVar2 + 55000,(byte)(in_fpscr >> 0x16) & 3);
  local_9c[0] = fVar14;
  local_9c[1] = (float)uVar18;
  local_9c[2] = (float)uVar16;
  local_9c[3] = (float)uVar19;
  local_8c = uVar11;
  local_88 = uVar20;
  local_84 = uVar12;
  local_80 = uVar15;
  local_7c = uVar13;
  local_78 = uVar17;
  iVar1 = AbyssEngine::AERandom::nextInt(*piVar7);
  uVar9 = iVar1 * 3 + 6;
  local_d4[0] = '\0';
  local_d4[1] = '\0';
  local_d4[2] = '\0';
  local_d4[3] = '\0';
  uVar3 = (uint)((ulonglong)uVar9 * 4);
  if ((int)((ulonglong)uVar9 * 4 >> 0x20) != 0) {
    uVar3 = 0xffffffff;
  }
  piVar4 = operator_new__(uVar3);
  for (iVar1 = 0; iVar1 < (int)uVar9; iVar1 = iVar1 + 3) {
    do {
      iVar2 = AbyssEngine::AERandom::nextInt(*piVar7);
    } while (*(char *)((int)local_d0 + iVar2 + -4) != '\0');
    fVar14 = local_9c[iVar2 * 3];
    *(undefined1 *)((int)local_d0 + iVar2 + -4) = 1;
    fVar10 = local_9c[iVar2 * 3 + 1];
    piVar4[iVar1] = (int)fVar14;
    piVar4[iVar1 + 1] = (int)fVar10;
    piVar4[iVar1 + 2] = (int)local_9c[iVar2 * 3 + 2];
  }
  pRVar5 = operator_new(0x18);
  Route::Route(pRVar5,piVar4,uVar9);
  *(Route **)(this + 0x144) = pRVar5;
  operator_delete__(piVar4);
  piVar7 = *(int **)(DAT_000ec818 + 0xec57e);
  if (*piVar7 == 0) {
    __aeabi_memcpy(local_d0,DAT_000ec81c + 0xec58c,0x30);
    pRVar5 = operator_new(0x18);
    Route::Route(pRVar5,local_d0,0xc);
    *piVar7 = (int)pRVar5;
  }
  uVar11 = DAT_000ec80c;
  *(undefined4 *)(this + 0x130) = 0xffffffff;
  *(undefined4 *)(this + 0x134) = 0xffffffff;
  *(undefined4 *)(this + 0x138) = 0xffffffff;
  *(undefined4 *)(this + 0x128) = 50000;
  *(undefined4 *)(this + 0x1a4) = 0x40000000;
  *(undefined4 *)(this + 0x1a8) = uVar11;
  uVar11 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar13 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x1ac) = 0x40000000;
  *(undefined4 *)(this + 0x1b0) = 0x5dc;
  *(undefined4 *)(this + 0x1b4) = 5;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x140) = 0;
  *(undefined2 *)(this + 0x12d) = 0;
  *(undefined2 *)(this + 0x13d) = 0;
  *(undefined4 *)(this + 0x1f8) = 0;
  *(undefined4 *)(this + 0xd8) = 0;
  this[0x12f] = (PlayerFighter)0x0;
  this[0x1fc] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x1c8) = 0;
  this[0x1e0] = (PlayerFighter)0x0;
  *(undefined2 *)(this + 500) = 0;
  *(int *)(this + 0x28) = param_2;
  *(undefined4 *)(this + 0x1b8) = 0;
  *(undefined4 *)(this + 0x1bc) = uVar11;
  *(undefined4 *)(this + 0x1c0) = uVar12;
  *(undefined4 *)(this + 0x1c4) = uVar13;
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x14c) = uVar11;
  *(undefined4 *)(this + 0x150) = uVar12;
  *(undefined4 *)(this + 0x154) = uVar13;
  *(undefined4 *)(this + 0x1d0) = 0;
  *(undefined4 *)(this + 0x1d4) = uVar11;
  *(undefined4 *)(this + 0x1d8) = uVar12;
  *(undefined4 *)(this + 0x1dc) = uVar13;
  *(undefined4 *)(this + 0x1e4) = 0;
  *(undefined4 *)(this + 0x1e8) = uVar11;
  *(undefined4 *)(this + 0x1ec) = uVar12;
  *(undefined4 *)(this + 0x1f0) = uVar13;
  local_d0[0] = _param_8;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x158),(Vector *)local_d0);
  this[0x13d] = (PlayerFighter)0x1;
  this[0x4c] = (PlayerFighter)0x1;
  *(undefined4 *)(this + 0x1e8) = *(undefined4 *)(this + 0x1ac);
  *(undefined4 *)(this + 0x1f0) = *(undefined4 *)(this + 0x1a4);
  Route::setLoop(SUB41(*(undefined4 *)(this + 0x144),0));
  Route::setLoop(SUB41(*piVar7,0));
  *(undefined4 *)(this + 0x6c) = 0;
  uVar21 = Status::getCurrentCampaignMission();
  pvVar6 = (void *)((ulonglong)uVar21 >> 0x20);
  if ((int)uVar21 != 0x29) {
    if (param_2 == 9) {
      iVar1 = Route::clone((__fn *)*piVar7,pvVar6,__flags,__arg);
    }
    else {
      iVar1 = Route::clone(*(__fn **)(this + 0x144),pvVar6,__flags,__arg);
    }
    *(int *)(this + 0x6c) = iVar1;
  }
  this[0x12d] = (PlayerFighter)0x0;
  if (param_2 == 9) {
    *(undefined4 *)(this + 0x50) = 0;
  }
  else {
    this_00 = operator_new(1);
    Generator::Generator(this_00);
    uVar11 = Generator::getLootList((int)this_00,-1);
    *(undefined4 *)(this + 0x50) = uVar11;
    pvVar6 = (void *)Generator::~Generator(this_00);
    operator_delete(pvVar6);
  }
  iVar1 = Status::inAlienOrbit();
  uVar11 = 50000;
  if (iVar1 != 0) {
    uVar11 = 100000;
  }
  *(undefined4 *)(this + 0x128) = uVar11;
  this_01 = operator_new(0x68);
  Explosion::Explosion(this_01,0);
  *(Explosion **)(this + 0x124) = this_01;
  Explosion::addFireStreaks(this_01);
  this[0x13e] = (PlayerFighter)0x1;
  uVar11 = Player::getHitpoints();
  *(undefined4 *)(this + 0x1d8) = uVar11;
  *(undefined4 *)(this + 0x1dc) = 0;
  this[0x1e0] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0xe4) = 1;
  this[0x25] = (PlayerFighter)0x1;
  *(undefined4 *)(this + 0x1a0) = 0xffffffff;
  iVar1 = Status::getCurrentCampaignMission();
  uVar11 = DAT_000ec810;
  if (iVar1 == 1) {
    uVar12 = 0xffffffff;
  }
  else {
    uVar12 = 0x30;
    if (this[0xdc] == (PlayerFighter)0x0) {
      uVar12 = 0x2e;
    }
  }
  *(undefined4 *)(this + 0xf8) = uVar12;
  uVar12 = DAT_000ec814;
  *(undefined4 *)(this + 0x20c) = 0;
  *(undefined4 *)(this + 0x210) = 0;
  *(undefined4 *)(this + 0x214) = 0;
  *(undefined4 *)(this + 0x294) = uVar12;
  *(undefined4 *)(this + 0x298) = uVar11;
  for (iVar1 = 0x29c; iVar1 != 0x2b0; iVar1 = iVar1 + 4) {
    *(undefined4 *)(this + iVar1) = 0;
  }
  this[0x2b4] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x2b0) = 0;
  this[0x100] = (PlayerFighter)0x0;
  *(undefined2 *)(this + 0x254) = 0;
  *(undefined4 *)(this + 0x2b8) = 0;
  this[0x13c] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x2c4) = 0;
  *(undefined4 *)(this + 0x2c8) = 0;
  *(undefined4 *)(this + 0x2d4) = 0;
  *(undefined4 *)(this + 0x2cd) = 0;
  *(undefined4 *)(this + 0x2c9) = 0;
  this[0x2d8] = (PlayerFighter)0x1;
  *(undefined4 *)(this + 0x2dc) = 0xffffffff;
  this[0x2e4] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x2e8) = 0;
  iVar1 = *piVar8 - local_6c;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  000ec2f8: push {r4,r5,r6,r7,lr}
  000ec2fa: add r7,sp,#0xc
  000ec2fc: push {r7,r8,r9,r10,r11}
  000ec300: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  000ec304: sub sp,#0x90
  000ec306: str r2,[sp,#0x20]
  000ec308: mov r8,r0
  000ec30a: ldr r0,[0x000ec6a4]
  000ec30c: vldr.32 s16,[r7,#0x14]
  000ec310: add r0,pc
  000ec312: ldr r2,[r7,#0x18]
  000ec314: vldr.32 s20,[r7,#0x10]
  000ec318: ldr.w r9,[r0,#0x0]
  000ec31c: vldr.32 s18,[r7,#0xc]
  000ec320: ldr r6,[r7,#0x8]
  000ec322: ldr.w r0,[r9,#0x0]
  000ec326: str r0,[sp,#0x8c]
  000ec328: mov r0,r8
  000ec32a: str r2,[sp,#0x10]
  000ec32c: mov.w r2,#0xffffffff
  000ec330: vstr.32 s18,[sp,#0x4]
  000ec334: vstr.32 s20,[sp,#0x8]
  000ec338: vstr.32 s16,[sp,#0xc]
  000ec33c: str r6,[sp,#0x0]
  000ec33e: blx 0x00075790
  000ec342: ldr r0,[0x000ec6a8]
  000ec344: add.w r6,r8,#0x158
  000ec348: movs r1,#0x0
  000ec34a: add r0,pc
  000ec34c: ldr r0,[r0,#0x0]
  000ec34e: adds r0,#0x8
  000ec350: str.w r0,[r8,#0x0]
  000ec354: strd r1,r1,[r8,#0x200]
  000ec358: mov r0,r6
  000ec35a: str.w r1,[r8,#0x208]
  000ec35e: movs r1,#0x48
  000ec360: blx 0x0006ec14
  000ec364: add.w r0,r8,#0x218
  000ec368: blx 0x0006f118
  000ec36c: add.w r0,r8,#0x258
  000ec370: blx 0x0006f118
  000ec374: ldr r0,[0x000ec6ac]
  000ec376: add r0,pc
  000ec378: ldr r4,[r0,#0x0]
  000ec37a: ldr r0,[r4,#0x0]
  000ec37c: movw r1,#0x61a8
  000ec380: blx 0x00071848
  000ec384: movw r1,#0x7530
  000ec388: subs r0,r0,r1
  000ec38a: vmov s0,r0
  000ec38e: vcvt.f32.s32 s22,s0
  000ec392: ldr r0,[r4,#0x0]
  000ec394: movw r1,#0x2710
  000ec398: blx 0x00071848
  000ec39c: mov r5,r0
  000ec39e: ldr r0,[r4,#0x0]
  000ec3a0: movw r1,#0x61a8
  000ec3a4: blx 0x00071848
  000ec3a8: movw r1,#0x4e20
  000ec3ac: add r0,r1
  000ec3ae: movw r1,#0x2710
  000ec3b2: vmov s0,r0
  000ec3b6: subs r0,r5,r1
  000ec3b8: vcvt.f32.s32 s24,s0
  000ec3bc: vmov s0,r0
  000ec3c0: vcvt.f32.s32 s26,s0
  000ec3c4: ldr r0,[r4,#0x0]
  000ec3c6: movw r1,#0x61a8
  000ec3ca: blx 0x00071848
  000ec3ce: movw r1,#0x1388
  000ec3d2: add r0,r1
  000ec3d4: vmov s0,r0
  000ec3d8: vcvt.f32.s32 s28,s0
  000ec3dc: ldr r0,[r4,#0x0]
  000ec3de: movw r1,#0x2710
  000ec3e2: blx 0x00071848
  000ec3e6: mov r5,r0
  000ec3e8: ldr r0,[r4,#0x0]
  000ec3ea: movw r1,#0x61a8
  000ec3ee: blx 0x00071848
  000ec3f2: movw r1,#0x4e20
  000ec3f6: add r0,r1
  000ec3f8: movw r1,#0x2710
  000ec3fc: vmov s0,r0
  000ec400: subs r0,r5,r1
  000ec402: vcvt.f32.s32 s30,s0
  000ec406: vmov s0,r0
  000ec40a: vcvt.f32.s32 s17,s0
  000ec40e: ldr r0,[r4,#0x0]
  000ec410: movw r1,#0x61a8
  000ec414: blx 0x00071848
  000ec418: movw r1,#0x1388
  000ec41c: add r0,r1
  000ec41e: vmov s0,r0
  000ec422: vcvt.f32.s32 s19,s0
  000ec426: ldr r0,[r4,#0x0]
  000ec428: movw r1,#0x2710
  000ec42c: blx 0x00071848
  000ec430: mov r5,r0
  000ec432: ldr r0,[r4,#0x0]
  000ec434: movw r1,#0x61a8
  000ec438: blx 0x00071848
  000ec43c: movw r1,#0xd6d8
  000ec440: add r0,r1
  000ec442: movw r1,#0x2710
  000ec446: vmov s0,r0
  000ec44a: subs r0,r5,r1
  000ec44c: vcvt.f32.s32 s21,s0
  000ec450: vmov s0,r0
  000ec454: vcvt.f32.s32 s23,s0
  000ec458: ldr r0,[r4,#0x0]
  000ec45a: movw r1,#0x61a8
  000ec45e: blx 0x00071848
  000ec462: movw r1,#0x7530
  000ec466: subs r0,r0,r1
  000ec468: vmov s0,r0
  000ec46c: vcvt.f32.s32 s25,s0
  000ec470: ldr r0,[r4,#0x0]
  000ec472: movw r1,#0x2710
  000ec476: blx 0x00071848
  000ec47a: mov r5,r0
  000ec47c: ldr r0,[r4,#0x0]
  000ec47e: movw r1,#0x61a8
  000ec482: blx 0x00071848
  000ec486: movw r1,#0xd6d8
  000ec48a: add r0,r1
  000ec48c: movw r1,#0x2710
  000ec490: strd r9,r6,[sp,#0x18]
  000ec494: subs r1,r5,r1
  000ec496: vmov s2,r0
  000ec49a: vstr.32 s26,[sp,#0x60]
  000ec49e: vmov s0,r1
  000ec4a2: vstr.32 s22,[sp,#0x5c]
  000ec4a6: vstr.32 s24,[sp,#0x64]
  000ec4aa: vstr.32 s28,[sp,#0x68]
  000ec4ae: vcvt.f32.s32 s0,s0
  000ec4b2: ldr r0,[r4,#0x0]
  000ec4b4: vcvt.f32.s32 s2,s2
  000ec4b8: vstr.32 s17,[sp,#0x6c]
  000ec4bc: vstr.32 s30,[sp,#0x70]
  000ec4c0: vstr.32 s19,[sp,#0x74]
  000ec4c4: vstr.32 s23,[sp,#0x78]
  000ec4c8: vstr.32 s21,[sp,#0x7c]
  000ec4cc: vstr.32 s25,[sp,#0x80]
  000ec4d0: vstr.32 s0,[sp,#0x84]
  000ec4d4: vstr.32 s2,[sp,#0x88]
  000ec4d8: movs r1,#0x3
  000ec4da: blx 0x00071848
  000ec4de: add.w r0,r0,r0, lsl #0x1
  000ec4e2: movs r2,#0x0
  000ec4e4: add.w r10,r0,#0x6
  000ec4e8: movs r0,#0x4
  000ec4ea: str r2,[sp,#0x24]
  000ec4ec: umull r0,r1,r10,r0
  000ec4f0: cmp r1,#0x0
  000ec4f2: it ne
  000ec4f4: mov.ne r1,#0x1
  000ec4f6: cmp r1,#0x0
  000ec4f8: it ne
  000ec4fa: mov.ne.w r0,#0xffffffff
  000ec4fe: blx 0x0006ec08
  000ec502: add r5,sp,#0x24
  000ec504: mov r11,r0
  000ec506: movs r6,#0x0
  000ec508: mov.w r9,#0x1
  000ec50c: b 0x000ec558
  000ec50e: ldr r0,[r4,#0x0]
  000ec510: movs r1,#0x4
  000ec512: blx 0x00071848
  000ec516: ldrb r1,[r5,r0]
  000ec518: cmp r1,#0x0
  000ec51a: bne 0x000ec50e
  000ec51c: add.w r1,r0,r0, lsl #0x1
  000ec520: add r2,sp,#0x5c
  000ec522: add.w r1,r2,r1, lsl #0x2
  000ec526: vldr.32 s0,[r1]
  000ec52a: vcvt.s32.f32 s0,s0
  000ec52e: strb.w r9,[r5,r0]
  000ec532: vmov r0,s0
  000ec536: vldr.32 s0,[r1,#0x4]
  000ec53a: str.w r0,[r11,r6,lsl #0x2]
  000ec53e: vcvt.s32.f32 s0,s0
  000ec542: add.w r0,r11,r6, lsl #0x2
  000ec546: adds r6,#0x3
  000ec548: vstr.32 s0,[r0,#0x4]
  000ec54c: vldr.32 s0,[r1,#0x8]
  000ec550: vcvt.s32.f32 s0,s0
  000ec554: vstr.32 s0,[r0,#0x8]
  000ec558: cmp r6,r10
  000ec55a: blt 0x000ec50e
  000ec55c: movs r0,#0x18
  000ec55e: blx 0x0006eb24
  000ec562: mov r4,r0
  000ec564: mov r1,r11
  000ec566: mov r2,r10
  000ec568: blx 0x00073eb8
  000ec56c: mov r0,r11
  000ec56e: str.w r4,[r8,#0x144]
  000ec572: blx 0x0006ebfc
  000ec576: ldr r0,[0x000ec818]
  000ec578: ldr r4,[sp,#0x1c]
  000ec57a: add r0,pc
  000ec57c: ldr r5,[r0,#0x0]
  000ec57e: ldr r0,[r5,#0x0]
  000ec580: cbnz r0,0x000ec5a0
  000ec582: ldr r1,[0x000ec81c]
  000ec584: add r0,sp,#0x28
  000ec586: movs r2,#0x30
  000ec588: add r1,pc
  000ec58a: blx 0x0006f1e4
  000ec58e: movs r0,#0x18
  000ec590: blx 0x0006eb24
  000ec594: mov r6,r0
  000ec596: add r1,sp,#0x28
  000ec598: movs r2,#0xc
  000ec59a: blx 0x00073eb8
  000ec59e: str r6,[r5,#0x0]
  000ec5a0: ldr r0,[0x000ec80c]
  000ec5a2: mov.w r1,#0xffffffff
  000ec5a6: strd r1,r1,[r8,#0x130]
  000ec5aa: movw r3,#0xc350
  000ec5ae: str.w r1,[r8,#0x138]
  000ec5b2: mov.w r1,#0x40000000
  000ec5b6: str.w r3,[r8,#0x128]
  000ec5ba: movw r2,#0x5dc
  000ec5be: strd r1,r0,[r8,#0x1a4]
  000ec5c2: movs r3,#0x5
  000ec5c4: add.w r0,r8,#0x1ac
  000ec5c8: vmov.i32 q8,#0x0
  000ec5cc: stmia r0!,{r1,r2,r3}
  000ec5ce: movs r0,#0x0
  000ec5d0: str.w r0,[r8,#0x38]
  000ec5d4: str.w r0,[r8,#0x140]
  000ec5d8: strh.w r0,[r8,#0x12d]
  000ec5dc: strh.w r0,[r8,#0x13d]
  000ec5e0: str.w r0,[r8,#0x1f8]
  000ec5e4: str.w r0,[r8,#0xd8]
  000ec5e8: strb.w r0,[r8,#0x12f]
  000ec5ec: strb.w r0,[r8,#0x1fc]
  000ec5f0: str.w r0,[r8,#0x1c8]
  000ec5f4: strb.w r0,[r8,#0x1e0]
  000ec5f8: strh.w r0,[r8,#0x1f4]
  000ec5fc: ldr r0,[sp,#0x20]
  000ec5fe: str.w r0,[r8,#0x28]
  000ec602: add.w r0,r8,#0x1b8
  000ec606: vst1.64 {d16,d17},[r0]
  000ec60a: add.w r0,r8,#0x148
  000ec60e: vst1.64 {d16,d17},[r0]
  000ec612: add.w r0,r8,#0x1d0
  000ec616: vst1.64 {d16,d17},[r0]
  000ec61a: add.w r0,r8,#0x1e4
  000ec61e: vst1.32 {d16,d17},[r0]
  000ec622: vstr.32 s20,[sp,#0x2c]
  000ec626: vstr.32 s18,[sp,#0x28]
  000ec62a: vstr.32 s16,[sp,#0x30]
  000ec62e: add r1,sp,#0x28
  000ec630: mov r0,r4
  000ec632: blx 0x0006eb3c
  000ec636: ldr.w r0,[r8,#0x144]
  000ec63a: movs r3,#0x1
  000ec63c: ldr.w r1,[r8,#0x1a4]
  000ec640: ldr.w r2,[r8,#0x1ac]
  000ec644: strb.w r3,[r8,#0x13d]
  000ec648: strb.w r3,[r8,#0x4c]
  000ec64c: str.w r2,[r8,#0x1e8]
  000ec650: str.w r1,[r8,#0x1f0]
  000ec654: movs r1,#0x1
  000ec656: blx 0x00073f48
  000ec65a: ldr r0,[r5,#0x0]
  000ec65c: movs r1,#0x1
  000ec65e: blx 0x00073f48
  000ec662: ldr r0,[0x000ec820]
  000ec664: add r0,pc
  000ec666: ldr r4,[r0,#0x0]
  000ec668: movs r0,#0x0
  000ec66a: str.w r0,[r8,#0x6c]
  000ec66e: ldr r0,[r4,#0x0]
  000ec670: blx 0x00071770
  000ec674: cmp r0,#0x29
  000ec676: beq 0x000ec692
  000ec678: ldr r0,[sp,#0x20]
  000ec67a: cmp r0,#0x9
  000ec67c: bne 0x000ec686
  000ec67e: ldr r0,[r5,#0x0]
  000ec680: blx 0x00073f3c
  000ec684: b 0x000ec68e
  000ec686: ldr.w r0,[r8,#0x144]
  000ec68a: blx 0x00073f3c
  000ec68e: str.w r0,[r8,#0x6c]
  000ec692: ldr r1,[sp,#0x20]
  000ec694: movs r0,#0x0
  000ec696: strb.w r0,[r8,#0x12d]
  000ec69a: cmp r1,#0x9
  000ec69c: bne 0x000ec6b0
  000ec69e: str.w r0,[r8,#0x50]
  000ec6a2: b 0x000ec6d8
  000ec6b0: movs r0,#0x1
  000ec6b2: blx 0x0006eb24
  000ec6b6: mov r6,r0
  000ec6b8: blx 0x000736cc
  000ec6bc: mov r0,r6
  000ec6be: mov.w r1,#0xffffffff
  000ec6c2: mov.w r2,#0xffffffff
  000ec6c6: blx 0x000740c8
  000ec6ca: str.w r0,[r8,#0x50]
  000ec6ce: mov r0,r6
  000ec6d0: blx 0x000740d4
  000ec6d4: blx 0x0006eb48
  000ec6d8: ldr r0,[r4,#0x0]
  000ec6da: blx 0x000723d0
  000ec6de: movw r1,#0xc350
  000ec6e2: cmp r0,#0x0
  000ec6e4: itt ne
  000ec6e6: movw.ne r1,#0x86a0
  000ec6ea: movt.ne r1,#0x1
  000ec6ee: str.w r1,[r8,#0x128]
  000ec6f2: movs r0,#0x68
  000ec6f4: blx 0x0006eb24
  000ec6f8: mov r6,r0
  000ec6fa: movs r1,#0x0
  000ec6fc: blx 0x000727f0
  000ec700: str.w r6,[r8,#0x124]
  000ec704: mov r0,r6
  000ec706: blx 0x0007579c
  000ec70a: ldr.w r0,[r8,#0x4]
  000ec70e: movs r5,#0x1
  000ec710: strb.w r5,[r8,#0x13e]
  000ec714: blx 0x000724f0
  000ec718: movs r1,#0x0
  000ec71a: strd r0,r1,[r8,#0x1d8]
  000ec71e: mov.w r0,#0xffffffff
  000ec722: strb.w r1,[r8,#0x1e0]
  000ec726: str.w r5,[r8,#0xe4]
  000ec72a: strb.w r5,[r8,#0x25]
  000ec72e: str.w r0,[r8,#0x1a0]
  000ec732: ldr r0,[r4,#0x0]
  000ec734: blx 0x00071770
  000ec738: cmp r0,#0x1
  000ec73a: bne 0x000ec742
  000ec73c: mov.w r1,#0xffffffff
  000ec740: b 0x000ec74e
  000ec742: ldrb.w r0,[r8,#0xdc]
  000ec746: movs r1,#0x30
  000ec748: cmp r0,#0x0
  000ec74a: it eq
  000ec74c: mov.eq r1,#0x2e
  000ec74e: ldr r6,[sp,#0x18]
  000ec750: movs r0,#0x0
  000ec752: ldr r2,[0x000ec810]
  000ec754: str.w r1,[r8,#0xf8]
  000ec758: mov.w r1,#0x29c
  000ec75c: ldr r3,[0x000ec814]
  000ec75e: strd r0,r0,[r8,#0x20c]
  000ec762: str.w r0,[r8,#0x214]
  000ec766: strd r3,r2,[r8,#0x294]
  000ec76a: b 0x000ec772
  000ec76c: str.w r0,[r8,r1]
  000ec770: adds r1,#0x4
  000ec772: cmp.w r1,#0x2b0
  000ec776: bne 0x000ec76c
  000ec778: movs r0,#0x0
  000ec77a: movs r1,#0x1
  000ec77c: strb.w r0,[r8,#0x2b4]
  000ec780: str.w r0,[r8,#0x2b0]
  000ec784: strb.w r0,[r8,#0x100]
  000ec788: strh.w r0,[r8,#0x254]
  000ec78c: str.w r0,[r8,#0x2b8]
  000ec790: strb.w r0,[r8,#0x13c]
  000ec794: strd r0,r0,[r8,#0x2c4]
  000ec798: str.w r0,[r8,#0x2d4]
  000ec79c: str.w r0,[r8,#0x2cd]
  000ec7a0: str.w r0,[r8,#0x2c9]
  000ec7a4: strb.w r1,[r8,#0x2d8]
  000ec7a8: mov.w r1,#0xffffffff
  000ec7ac: str.w r1,[r8,#0x2dc]
  000ec7b0: strb.w r0,[r8,#0x2e4]
  000ec7b4: str.w r0,[r8,#0x2e8]
  000ec7b8: ldr r0,[sp,#0x8c]
  000ec7ba: ldr r1,[r6,#0x0]
  000ec7bc: subs r0,r1,r0
  000ec7be: itttt eq
  000ec7c0: mov.eq r0,r8
  000ec7c2: add.eq sp,#0x90
  000ec7c4: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  000ec7c8: add.eq sp,#0x4
  000ec7ca: itt eq
  000ec7cc: pop.eq.w {r8,r9,r10,r11}
  000ec7d0: pop.eq {r4,r5,r6,r7,pc}
  000ec7d2: blx 0x0006e824
```
