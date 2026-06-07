# PlayerCreature::update
elf 0x11cd68 body 562
Sig: undefined __thiscall update(PlayerCreature * this, int param_1)

## decompile
```c

/* PlayerCreature::update(int) */

void __thiscall PlayerCreature::update(PlayerCreature *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  Matrix *pMVar3;
  int *piVar4;
  int iVar5;
  Matrix *pMVar6;
  int *piVar7;
  uint in_fpscr;
  float fVar8;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s1;
  float fVar9;
  float fVar10;
  undefined4 uStack_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  int local_34;
  
  piVar7 = *(int **)(DAT_0012cfac + 0x12cd80);
  local_34 = *piVar7;
  iVar5 = *(int *)(this + 0x13c);
  *(int *)(this + 0x124) = param_1;
  iVar1 = Player::getHitpoints();
  if ((iVar1 < iVar5) && (*(int *)(this + 0x88) != 4)) {
    this[0x128] = (PlayerCreature)0x1;
  }
  uVar2 = Player::getHitpoints();
  *(undefined4 *)(this + 0x13c) = uVar2;
  fVar9 = DAT_0012cf9c;
  if (this[0x128] != (PlayerCreature)0x0) {
    fVar8 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
    iVar1 = *(int *)(this + 0x130) + param_1;
    *(int *)(this + 0x130) = iVar1;
    iVar5 = *(int *)(this + 0x138) - param_1;
    fVar9 = *(float *)(this + 300) + fVar8 * fVar9;
    param_1 = (int)(fVar9 * fVar8);
    *(int *)(this + 0x138) = iVar5;
    *(float *)(this + 300) = fVar9;
    if (iVar1 < 4000) {
      pMVar6 = *(Matrix **)(this + 8);
      pMVar3 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::MatrixMultiply((Matrix *)&uStack_70,pMVar3);
      AEGeometry::setMatrix(pMVar6);
    }
    else {
      fVar8 = (float)VectorSignedToFloat(param_1 >> 1,(byte)(in_fpscr >> 0x16) & 3);
      piVar4 = *(int **)(DAT_0012cfb0 + 0x12ce2e);
      uVar2 = AbyssEngine::AERandom::nextInt(*piVar4);
      fVar10 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
      uVar2 = AbyssEngine::AERandom::nextInt(*piVar4);
      fVar9 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
      fVar10 = (fVar8 * -0.5 + fVar10) * DAT_0012cfa0;
      fVar9 = (fVar8 * -0.5 + fVar9) * DAT_0012cfa0;
      AbyssEngine::AEMath::MatrixSetRotation
                ((Matrix *)&uStack_70,(fVar9 + fVar9) * DAT_0012cfa4,extraout_s1,
                 (fVar10 + fVar10) * DAT_0012cfa4);
      *(undefined4 *)(this + 0x130) = 0;
    }
    if (*(int *)(this + 0x138) < 1) {
      if (this[0x129] == (PlayerCreature)0x0) {
        *(undefined4 *)(this + 0x130) = 0;
        *(undefined2 *)(this + 0x128) = 0;
        *(undefined4 *)(this + 0x138) = *(undefined4 *)(this + 0x134);
      }
      else {
        this[0x128] = (PlayerCreature)0x0;
        this[0x12a] = (PlayerCreature)0x1;
      }
    }
  }
  iVar5 = Player::getHitpoints();
  iVar1 = DAT_0012cfb4;
  if (iVar5 < 1) {
    iVar5 = *(int *)(this + 0x88);
    if (iVar5 - 3U < 2) goto LAB_0012cf4a;
    *(undefined4 *)(this + 0x88) = 3;
    FModSound::play(**(int **)(iVar1 + 0x12ceee),(Vector *)0x16,(Vector *)0x0,extraout_s0);
    Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
    iVar1 = Level::getPlayer();
    fVar9 = extraout_s0_00;
    if (*(PlayerCreature **)(*(int *)(iVar1 + 0x14) + 0x1c) == this) {
      iVar1 = Level::getPlayer();
      *(undefined4 *)(*(int *)(iVar1 + 0x14) + 0x1c) = 0;
      fVar9 = extraout_s0_01;
    }
    uStack_70 = *(undefined4 *)(this + 0x58);
    local_6c = *(undefined4 *)(this + 0x5c);
    uStack_68 = *(undefined4 *)(this + 0x60);
    ParticleSystemManager::emitManual
              (*(int *)(*(int *)(this + 0x54) + 0x74),*(Vector **)(*(int *)(this + 0x54) + 0x34),
               (int)&uStack_70,(Vector *)0x0,fVar9);
  }
  iVar5 = *(int *)(this + 0x88);
LAB_0012cf4a:
  if (iVar5 == 3) {
    *(undefined4 *)(this + 0x88) = 4;
  }
  else if ((iVar5 == 0) && (this[0x12a] == (PlayerCreature)0x0)) {
    fVar9 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::moveForward(fVar9);
  }
  pMVar3 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(this + 4) + 4),pMVar3);
  if (*piVar7 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012cd68: push {r4,r5,r6,r7,lr}
  0012cd6a: add r7,sp,#0xc
  0012cd6c: push {r8,r9,r10}
  0012cd70: vpush {d8,d9}
  0012cd74: sub sp,#0x58
  0012cd76: mov r4,r0
  0012cd78: ldr r0,[0x0012cfac]
  0012cd7a: mov r10,r1
  0012cd7c: add r0,pc
  0012cd7e: ldr.w r9,[r0,#0x0]
  0012cd82: ldr.w r0,[r9,#0x0]
  0012cd86: str r0,[sp,#0x54]
  0012cd88: ldr r0,[r4,#0x4]
  0012cd8a: ldr.w r6,[r4,#0x13c]
  0012cd8e: str.w r1,[r4,#0x124]
  0012cd92: blx 0x000724f0
  0012cd96: cmp r6,r0
  0012cd98: ble 0x0012cda8
  0012cd9a: ldr.w r0,[r4,#0x88]
  0012cd9e: cmp r0,#0x4
  0012cda0: itt ne
  0012cda2: mov.ne r0,#0x1
  0012cda4: strb.ne.w r0,[r4,#0x128]
  0012cda8: ldr r0,[r4,#0x4]
  0012cdaa: blx 0x000724f0
  0012cdae: ldrb.w r1,[r4,#0x128]
  0012cdb2: str.w r0,[r4,#0x13c]
  0012cdb6: cmp r1,#0x0
  0012cdb8: beq.w 0x0012ceca
  0012cdbc: vmov s0,r10
  0012cdc0: vldr.32 s2,[pc,#0x1d8]
  0012cdc4: vcvt.f32.s32 s0,s0
  0012cdc8: vldr.32 s4,[r4,#0x12c]
  0012cdcc: ldr.w r0,[r4,#0x130]
  0012cdd0: ldr.w r1,[r4,#0x138]
  0012cdd4: add r0,r10
  0012cdd6: str.w r0,[r4,#0x130]
  0012cdda: sub.w r1,r1,r10
  0012cdde: cmp.w r0,#0xfa0
  0012cde2: vmla.f32 s4,s0,s2
  0012cde6: vmul.f32 s0,s4,s0
  0012cdea: vcvt.s32.f32 s0,s0
  0012cdee: str.w r1,[r4,#0x138]
  0012cdf2: vstr.32 s4,[r4,#0x12c]
  0012cdf6: vmov r10,s0
  0012cdfa: bge 0x0012ce1e
  0012cdfc: ldr r6,[r4,#0x8]
  0012cdfe: mov r0,r6
  0012ce00: blx 0x000721cc
  0012ce04: add.w r8,sp,#0x18
  0012ce08: add.w r2,r4,#0x144
  0012ce0c: mov r1,r0
  0012ce0e: mov r0,r8
  0012ce10: blx 0x00076090
  0012ce14: mov r0,r6
  0012ce16: mov r1,r8
  0012ce18: blx 0x000721e4
  0012ce1c: b 0x0012ce9c
  0012ce1e: ldr r0,[0x0012cfb0]
  0012ce20: asr.w r6,r10, asr #0x1
  0012ce24: vmov s2,r6
  0012ce28: mov r1,r6
  0012ce2a: add r0,pc
  0012ce2c: vmov.f32 s0,0xbf000000
  0012ce30: vcvt.f32.s32 s2,s2
  0012ce34: ldr r5,[r0,#0x0]
  0012ce36: ldr r0,[r5,#0x0]
  0012ce38: vmul.f32 s16,s2,s0
  0012ce3c: blx 0x00071848
  0012ce40: ldr r1,[r5,#0x0]
  0012ce42: vmov s0,r0
  0012ce46: vcvt.f32.s32 s18,s0
  0012ce4a: mov r0,r1
  0012ce4c: mov r1,r6
  0012ce4e: blx 0x00071848
  0012ce52: vmov s0,r0
  0012ce56: vldr.32 s4,[pc,#0x148]
  0012ce5a: vadd.f32 s2,s16,s18
  0012ce5e: add.w r1,r4,#0x144
  0012ce62: vcvt.f32.s32 s0,s0
  0012ce66: add r0,sp,#0x18
  0012ce68: movs r5,#0x0
  0012ce6a: str r5,[sp,#0x0]
  0012ce6c: vmul.f32 s2,s2,s4
  0012ce70: vadd.f32 s0,s16,s0
  0012ce74: vadd.f32 s2,s2,s2
  0012ce78: vmul.f32 s0,s0,s4
  0012ce7c: vldr.32 s4,[pc,#0x124]
  0012ce80: vmul.f32 s2,s2,s4
  0012ce84: vadd.f32 s0,s0,s0
  0012ce88: vmov r2,s2
  0012ce8c: vmul.f32 s0,s0,s4
  0012ce90: vmov r3,s0
  0012ce94: blx 0x00072094
  0012ce98: str.w r5,[r4,#0x130]
  0012ce9c: ldr.w r0,[r4,#0x138]
  0012cea0: cmp r0,#0x0
  0012cea2: bgt 0x0012ceca
  0012cea4: ldrb.w r0,[r4,#0x129]
  0012cea8: cbz r0,0x0012ceb8
  0012ceaa: movs r0,#0x0
  0012ceac: strb.w r0,[r4,#0x128]
  0012ceb0: movs r0,#0x1
  0012ceb2: strb.w r0,[r4,#0x12a]
  0012ceb6: b 0x0012ceca
  0012ceb8: ldr.w r0,[r4,#0x134]
  0012cebc: movs r1,#0x0
  0012cebe: str.w r1,[r4,#0x130]
  0012cec2: strh.w r1,[r4,#0x128]
  0012cec6: str.w r0,[r4,#0x138]
  0012ceca: ldr r0,[r4,#0x4]
  0012cecc: blx 0x000724f0
  0012ced0: cmp r0,#0x0
  0012ced2: bgt 0x0012cf46
  0012ced4: ldr.w r0,[r4,#0x88]
  0012ced8: subs r1,r0,#0x3
  0012ceda: cmp r1,#0x2
  0012cedc: bcc 0x0012cf4a
  0012cede: ldr r0,[0x0012cfb4]
  0012cee0: movs r1,#0x3
  0012cee2: str.w r1,[r4,#0x88]
  0012cee6: mov.w r8,#0x0
  0012ceea: add r0,pc
  0012ceec: movs r1,#0x16
  0012ceee: movs r2,#0x0
  0012cef0: movs r3,#0x0
  0012cef2: ldr r0,[r0,#0x0]
  0012cef4: str.w r8,[sp,#0x0]
  0012cef8: ldr r0,[r0,#0x0]
  0012cefa: blx 0x00071548
  0012cefe: ldr r0,[r4,#0x4]
  0012cf00: movs r1,#0x0
  0012cf02: blx 0x00072580
  0012cf06: ldr r0,[r4,#0x54]
  0012cf08: blx 0x00072034
  0012cf0c: ldr r0,[r0,#0x14]
  0012cf0e: ldr r0,[r0,#0x1c]
  0012cf10: cmp r0,r4
  0012cf12: bne 0x0012cf20
  0012cf14: ldr r0,[r4,#0x54]
  0012cf16: blx 0x00072034
  0012cf1a: ldr r0,[r0,#0x14]
  0012cf1c: str.w r8,[r0,#0x1c]
  0012cf20: add.w r5,r4,#0x54
  0012cf24: ldr r6,[0x0012cfa8]
  0012cf26: ldmia r5,{r0,r2,r3,r5}
  0012cf28: ldr r1,[r0,#0x34]
  0012cf2a: ldr r0,[r0,#0x74]
  0012cf2c: strd r3,r5,[sp,#0x1c]
  0012cf30: movs r3,#0x0
  0012cf32: strd r8,r8,[sp,#0xc]
  0012cf36: strd r8,r2,[sp,#0x14]
  0012cf3a: add r2,sp,#0xc
  0012cf3c: strd r2,r6,[sp,#0x0]
  0012cf40: add r2,sp,#0x18
  0012cf42: blx 0x00072a30
  0012cf46: ldr.w r0,[r4,#0x88]
  0012cf4a: cmp r0,#0x3
  0012cf4c: beq 0x0012cf6a
  0012cf4e: cbnz r0,0x0012cf70
  0012cf50: ldrb.w r0,[r4,#0x12a]
  0012cf54: cbnz r0,0x0012cf70
  0012cf56: vmov s0,r10
  0012cf5a: vcvt.f32.s32 s0,s0
  0012cf5e: ldr r0,[r4,#0x8]
  0012cf60: vmov r1,s0
  0012cf64: blx 0x000725c8
  0012cf68: b 0x0012cf70
  0012cf6a: movs r0,#0x4
  0012cf6c: str.w r0,[r4,#0x88]
  0012cf70: ldr r0,[r4,#0x8]
  0012cf72: blx 0x000721cc
  0012cf76: mov r1,r0
  0012cf78: ldr r0,[r4,#0x4]
  0012cf7a: adds r0,#0x4
  0012cf7c: blx 0x0006f148
  0012cf80: ldr r0,[sp,#0x54]
  0012cf82: ldr.w r1,[r9,#0x0]
  0012cf86: subs r0,r1,r0
  0012cf88: itttt eq
  0012cf8a: add.eq sp,#0x58
  0012cf8c: vpop.eq {d8,d9}
  0012cf90: pop.eq.w {r8,r9,r10}
  0012cf94: pop.eq {r4,r5,r6,r7,pc}
  0012cf96: blx 0x0006e824
```
