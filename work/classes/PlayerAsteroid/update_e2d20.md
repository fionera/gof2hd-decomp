# PlayerAsteroid::update
elf 0xe2d20 body 736
Sig: undefined __thiscall update(PlayerAsteroid * this, int param_1)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* PlayerAsteroid::update(int) */

void __thiscall PlayerAsteroid::update(PlayerAsteroid *this,int param_1)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  Array *pAVar4;
  undefined4 uVar5;
  int iVar6;
  Vector *pVVar7;
  int *piVar8;
  uint in_fpscr;
  uint uVar9;
  float fVar10;
  float extraout_s0;
  float fVar11;
  float fVar12;
  float fVar13;
  Vector aVStack_38 [12];
  int local_2c;
  
  piVar8 = *(int **)(DAT_000f3024 + 0xf2d38);
  local_2c = *piVar8;
  *(int *)(this + 0x134) = param_1;
  if (param_1 == 0) goto LAB_000f2f6e;
  iVar3 = Player::isActive();
  if ((iVar3 == 0) && (*(int *)(this + 0x88) == 4)) {
    this[0x124] = (PlayerAsteroid)0x0;
    goto LAB_000f2f6e;
  }
  iVar3 = Player::getHitpoints();
  iVar6 = *(int *)(this + 0x88);
  if ((0 < iVar3) || (iVar6 != 0)) {
    if (iVar6 == 3) {
      Explosion::update(*(Explosion **)(this + 300),param_1,(TargetFollowCamera *)0x0);
      iVar3 = Explosion::isPlaying();
      if (iVar3 == 0) {
        *(undefined4 *)(this + 0x88) = 4;
        fVar10 = extraout_s0;
        goto LAB_000f2e2a;
      }
    }
    else if (iVar6 == 4) {
      fVar10 = (float)KIPlayer::setActive(SUB41(this,0));
LAB_000f2e2a:
      Player::setBombForce(fVar10);
    }
    iVar6 = *(int *)(this + 0x158);
    iVar3 = Player::getHitpoints();
    fVar10 = _LAB_000f3008;
    if (iVar3 < iVar6) {
      *(undefined4 *)(this + 0x15c) = _LAB_000f3004;
      *(float *)(this + 0x160) = fVar10;
      uVar5 = Player::getHitpoints();
      *(undefined4 *)(this + 0x158) = uVar5;
    }
    if (this[0x14c] != (PlayerAsteroid)0x0) {
      fVar10 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
      pVVar7 = *(Vector **)(this + 8);
      AbyssEngine::AEMath::operator*(aVStack_38,fVar10 * _LAB_000f3008);
      AEGeometry::rotate(pVVar7);
    }
    fVar10 = (float)Player::getBombForce();
    uVar1 = in_fpscr & 0xfffffff | (uint)(fVar10 < 0.0) << 0x1f | (uint)(fVar10 == 0.0) << 0x1e;
    uVar9 = uVar1 | (uint)NAN(fVar10) << 0x1c;
    bVar2 = (byte)(uVar1 >> 0x18);
    if ((!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar9 >> 0x1c) & 1)) &&
       (*(int *)(this + 0x88) == 3)) {
      Player::getHitVector();
      pVVar7 = (Vector *)(this + 0x90);
      AbyssEngine::AEMath::Vector::operator=(pVVar7,aVStack_38);
      fVar11 = *(float *)(this + 0x138);
      fVar12 = _LAB_000f300c;
      if (1.0 < fVar11) {
        fVar12 = 1.0;
      }
      fVar13 = 1.0;
      if (fVar11 < _LAB_000f300c) {
        fVar13 = fVar12;
      }
      fVar12 = fVar11;
      if (1.0 < fVar11) {
        fVar12 = fVar13;
      }
      if (fVar11 < _LAB_000f300c) {
        fVar12 = fVar13;
      }
      fVar12 = (float)VectorSignedToFloat((int)(fVar10 * _LAB_000f3014 *
                                               (fVar12 / _LAB_000f3010 + 1.0)),
                                          (byte)((uVar9 & 0xfffffff) >> 0x16) & 3);
      AbyssEngine::AEMath::Vector::operator*=(pVVar7,fVar12);
      (**(code **)(*(int *)this + 0x20))(this,pVVar7);
      Explosion::translate(*(Vector **)(this + 300));
      if (*(Vector **)(this + 0x78) != (Vector *)0x0) {
        AEGeometry::translate(*(Vector **)(this + 0x78));
      }
      fVar12 = fVar10 * _LAB_000f3018;
      if ((int)((uint)(fVar10 * _LAB_000f3018 < DAT_000f301c) << 0x1f) < 0) {
        fVar12 = DAT_000f3020;
      }
      Player::setBombForce(fVar12);
    }
LAB_000f2f6e:
    if (*piVar8 == local_2c) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  *(undefined4 *)(this + 0x88) = 3;
  Level::asteroidDied();
  *(int *)(**(int **)(DAT_000f3028 + 0xf2d82) + 0xd8) =
       *(int *)(**(int **)(DAT_000f3028 + 0xf2d82) + 0xd8) + 1;
  if (this[0x4c] == (PlayerAsteroid)0x0) {
LAB_000f2e02:
    this[0x4c] = (PlayerAsteroid)0x0;
    *(undefined4 *)(this + 0x50) = 0;
  }
  else {
    iVar3 = *(int *)(this + 0x150);
    if (iVar3 == 7) {
      iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f302c + 0xf2da4));
      if (3 < iVar3) {
        iVar3 = *(int *)(this + 0x150);
        goto LAB_000f2db2;
      }
    }
    else {
LAB_000f2db2:
      if ((6 < iVar3) ||
         (iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f3030 + 0xf2dbe)), 0x13 < iVar3))
      goto LAB_000f2e02;
    }
    pAVar4 = operator_new(0xc);
    Array<int>::Array();
    iVar3 = *(int *)(this + 0x128);
    *(Array **)(this + 0x50) = pAVar4;
    if (iVar3 == 0xd9) {
      iVar3 = 0xd9;
      if (*(int *)(this + 0x150) == 7) {
        iVar3 = 0xda;
      }
    }
    else if (*(int *)(this + 0x150) == 7) {
      iVar3 = iVar3 + 0xb;
    }
    ArrayAdd<int>(iVar3,pAVar4);
    iVar3 = 1;
    if (*(int *)(this + 0x150) != 7) {
      iVar3 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f3034 + 0xf2fa8));
      iVar3 = iVar3 + 1;
    }
    ArrayAdd<int>(iVar3,*(Array **)(this + 0x50));
    KIPlayer::createCrate((int)this);
  }
  uVar5 = AEGeometry::getMatrix();
  if (*piVar8 - local_2c == 0) {
    (*(code *)(DAT_001ac2a4 + 0x1ac2a8))();
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(*piVar8 - local_2c,uVar5);
}

```

## target disasm
```
  000f2d20: push {r4,r5,r6,r7,lr}
  000f2d22: add r7,sp,#0xc
  000f2d24: push {r8,r9,r11}
  000f2d28: vpush {d6,d7,d8}
  000f2d2c: mov r4,r0
  000f2d2e: ldr r0,[0x000f3024]
  000f2d30: mov r5,r1
  000f2d32: cmp r1,#0x0
  000f2d34: add r0,pc
  000f2d36: ldr.w r9,[r0,#0x0]
  000f2d3a: ldr.w r0,[r9,#0x0]
  000f2d3e: str r0,[sp,#0xc]
  000f2d40: str.w r1,[r4,#0x134]
  000f2d44: beq.w 0x000f2f6e
  000f2d48: ldr r0,[r4,#0x4]
  000f2d4a: blx 0x00071f14
  000f2d4e: cbnz r0,0x000f2d60
  000f2d50: ldr.w r0,[r4,#0x88]
  000f2d54: cmp r0,#0x4
  000f2d56: bne 0x000f2d60
  000f2d58: movs r0,#0x0
  000f2d5a: strb.w r0,[r4,#0x124]
  000f2d5e: b 0x000f2f6e
  000f2d60: ldr r0,[r4,#0x4]
  000f2d62: blx 0x000724f0
  000f2d66: ldr.w r1,[r4,#0x88]
  000f2d6a: cmp r0,#0x0
  000f2d6c: bgt 0x000f2dee
  000f2d6e: cbnz r1,0x000f2dee
  000f2d70: ldr r0,[r4,#0x54]
  000f2d72: movs r1,#0x3
  000f2d74: str.w r1,[r4,#0x88]
  000f2d78: blx 0x00075bf8
  000f2d7c: ldr r0,[0x000f3028]
  000f2d7e: add r0,pc
  000f2d80: ldr r0,[r0,#0x0]
  000f2d82: ldr r0,[r0,#0x0]
  000f2d84: ldr.w r1,[r0,#0xd8]
  000f2d88: adds r1,#0x1
  000f2d8a: str.w r1,[r0,#0xd8]
  000f2d8e: ldrb.w r0,[r4,#0x4c]
  000f2d92: cbz r0,0x000f2e02
  000f2d94: ldr.w r0,[r4,#0x150]
  000f2d98: cmp r0,#0x7
  000f2d9a: bne 0x000f2db2
  000f2d9c: ldr r0,[0x000f302c]
  000f2d9e: movs r1,#0x64
  000f2da0: add r0,pc
  000f2da2: ldr r0,[r0,#0x0]
  000f2da4: ldr r0,[r0,#0x0]
  000f2da6: blx 0x00071848
  000f2daa: cmp r0,#0x4
  000f2dac: blt 0x000f2dc8
  000f2dae: ldr.w r0,[r4,#0x150]
  000f2db2: cmp r0,#0x6
  000f2db4: bgt 0x000f2e02
  000f2db6: ldr r0,[0x000f3030]
  000f2db8: movs r1,#0x64
  000f2dba: add r0,pc
  000f2dbc: ldr r0,[r0,#0x0]
  000f2dbe: ldr r0,[r0,#0x0]
  000f2dc0: blx 0x00071848
  000f2dc4: cmp r0,#0x13
  000f2dc6: bgt 0x000f2e02
  000f2dc8: movs r0,#0xc
  000f2dca: blx 0x0006eb24
  000f2dce: mov r5,r0
  000f2dd0: blx 0x000701f8
  000f2dd4: ldr.w r0,[r4,#0x128]
  000f2dd8: ldr.w r1,[r4,#0x150]
  000f2ddc: cmp r0,#0xd9
  000f2dde: str r5,[r4,#0x50]
  000f2de0: bne.w 0x000f2f86
  000f2de4: movs r0,#0xd9
  000f2de6: cmp r1,#0x7
  000f2de8: it eq
  000f2dea: mov.eq r0,#0xda
  000f2dec: b 0x000f2f8c
  000f2dee: cmp r1,#0x3
  000f2df0: beq 0x000f2e0c
  000f2df2: cmp r1,#0x4
  000f2df4: bne 0x000f2e30
  000f2df6: mov r0,r4
  000f2df8: movs r1,#0x0
  000f2dfa: blx 0x000732f4
  000f2dfe: ldr r0,[r4,#0x4]
  000f2e00: b 0x000f2e2a
  000f2e02: movs r0,#0x0
  000f2e04: strb.w r0,[r4,#0x4c]
  000f2e08: str r0,[r4,#0x50]
  000f2e0a: b 0x000f2fc8
  000f2e0c: ldr.w r0,[r4,#0x12c]
  000f2e10: mov r1,r5
  000f2e12: movs r2,#0x0
  000f2e14: blx 0x00075934
  000f2e18: ldr.w r0,[r4,#0x12c]
  000f2e1c: blx 0x000757fc
  000f2e20: cbnz r0,0x000f2e30
  000f2e22: ldr r0,[r4,#0x4]
  000f2e24: movs r1,#0x4
  000f2e26: str.w r1,[r4,#0x88]
  000f2e2a: movs r1,#0x0
  000f2e2c: blx 0x00072aa8
  000f2e30: ldr r0,[r4,#0x4]
  000f2e32: ldr.w r6,[r4,#0x158]
  000f2e36: blx 0x000724f0
  000f2e3a: cmp r6,r0
  000f2e3c: ble 0x000f2e50
  000f2e3e: ldr r0,[r4,#0x4]
  000f2e40: ldr r2,[0x000f3008]
  000f2e42: ldr r1,[0x000f3004]
  000f2e44: strd r1,r2,[r4,#0x15c]
  000f2e48: blx 0x000724f0
  000f2e4c: str.w r0,[r4,#0x158]
  000f2e50: ldrb.w r0,[r4,#0x14c]
  000f2e54: cbz r0,0x000f2e80
  000f2e56: vmov s0,r5
  000f2e5a: vldr.32 s2,[pc,#0x1ac]
  000f2e5e: mov r8,sp
  000f2e60: add.w r1,r4,#0x140
  000f2e64: vcvt.f32.s32 s0,s0
  000f2e68: mov r0,r8
  000f2e6a: ldr r5,[r4,#0x8]
  000f2e6c: vmul.f32 s0,s0,s2
  000f2e70: vmov r2,s0
  000f2e74: blx 0x0006ec74
  000f2e78: mov r0,r5
  000f2e7a: mov r1,r8
  000f2e7c: blx 0x00072478
  000f2e80: ldr r0,[r4,#0x4]
  000f2e82: blx 0x00075940
  000f2e86: vmov s16,r0
  000f2e8a: vcmpe.f32 s16,#0
  000f2e8e: vmrs apsr,fpscr
  000f2e92: ble 0x000f2f6e
  000f2e94: ldr.w r0,[r4,#0x88]
  000f2e98: cmp r0,#0x3
  000f2e9a: bne 0x000f2f6e
  000f2e9c: ldr r1,[r4,#0x4]
  000f2e9e: mov r8,sp
  000f2ea0: mov r0,r8
  000f2ea2: blx 0x000729b8
  000f2ea6: add.w r5,r4,#0x90
  000f2eaa: mov r1,r8
  000f2eac: mov r0,r5
  000f2eae: blx 0x0006eb3c
  000f2eb2: vmov.f32 s0,0x3f800000
  000f2eb6: vldr.32 s4,[r4,#0x138]
  000f2eba: vldr.32 s2,[pc,#0x150]
  000f2ebe: mov r0,r5
  000f2ec0: vmov.f32 s6,s2
  000f2ec4: vcmpe.f32 s4,s0
  000f2ec8: vmrs apsr,fpscr
  000f2ecc: vcmpe.f32 s4,s2
  000f2ed0: vmov.f32 s8,s0
  000f2ed4: it gt
  000f2ed6: vmov.gt.f32 s6,s0
  000f2eda: vmrs apsr,fpscr
  000f2ede: vcmpe.f32 s4,s0
  000f2ee2: it mi
  000f2ee4: vmov.mi.f32 s8,s6
  000f2ee8: vmrs apsr,fpscr
  000f2eec: vcmpe.f32 s4,s2
  000f2ef0: vldr.32 s2,[pc,#0x11c]
  000f2ef4: it gt
  000f2ef6: vmov.gt.f32 s4,s8
  000f2efa: vmrs apsr,fpscr
  000f2efe: it mi
  000f2f00: vmov.mi.f32 s4,s8
  000f2f04: vdiv.f32 s2,s4,s2
  000f2f08: vldr.32 s4,[pc,#0x108]
  000f2f0c: vadd.f32 s0,s2,s0
  000f2f10: vmul.f32 s4,s16,s4
  000f2f14: vmul.f32 s0,s4,s0
  000f2f18: vcvt.s32.f32 s0,s0
  000f2f1c: vcvt.f32.s32 s0,s0
  000f2f20: vmov r1,s0
  000f2f24: blx 0x00072628
  000f2f28: ldr r0,[r4,#0x0]
  000f2f2a: mov r1,r5
  000f2f2c: ldr r2,[r0,#0x20]
  000f2f2e: mov r0,r4
  000f2f30: blx r2
  000f2f32: ldr.w r0,[r4,#0x12c]
  000f2f36: mov r1,r5
  000f2f38: blx 0x00075c04
  000f2f3c: ldr r0,[r4,#0x78]
  000f2f3e: cbz r0,0x000f2f46
  000f2f40: mov r1,r5
  000f2f42: blx 0x00072460
  000f2f46: vldr.32 s0,[pc,#0xd0]
  000f2f4a: vldr.32 s2,[pc,#0xd0]
  000f2f4e: vmul.f32 s0,s16,s0
  000f2f52: vldr.32 s4,[pc,#0xcc]
  000f2f56: vcmpe.f32 s0,s2
  000f2f5a: vmrs apsr,fpscr
  000f2f5e: it mi
  000f2f60: vmov.mi.f32 s0,s4
  000f2f64: ldr r0,[r4,#0x4]
  000f2f66: vmov r1,s0
  000f2f6a: blx 0x00072aa8
  000f2f6e: ldr r0,[sp,#0xc]
  000f2f70: ldr.w r1,[r9,#0x0]
  000f2f74: subs r0,r1,r0
  000f2f76: ittt eq
  000f2f78: vpop.eq {d6,d7,d8}
  000f2f7c: pop.eq.w {r8,r9,r11}
  000f2f80: pop.eq {r4,r5,r6,r7,pc}
  000f2f82: blx 0x0006e824
  000f2f86: cmp r1,#0x7
  000f2f88: it eq
  000f2f8a: add.eq r0,#0xb
  000f2f8c: mov r1,r5
  000f2f8e: blx 0x0007021c
  000f2f92: ldr.w r0,[r4,#0x150]
  000f2f96: movs r5,#0x1
  000f2f98: cmp r0,#0x7
  000f2f9a: mov.w r0,#0x1
  000f2f9e: beq 0x000f2fb0
  000f2fa0: ldr r0,[0x000f3034]
  000f2fa2: movs r1,#0x3
  000f2fa4: add r0,pc
  000f2fa6: ldr r0,[r0,#0x0]
  000f2fa8: ldr r0,[r0,#0x0]
  000f2faa: blx 0x00071848
  000f2fae: adds r0,#0x1
  000f2fb0: ldr r1,[r4,#0x50]
  000f2fb2: blx 0x0007021c
  000f2fb6: ldr.w r0,[r4,#0x128]
  000f2fba: cmp r0,#0xa4
  000f2fbc: it eq
  000f2fbe: mov.eq r5,#0x2
  000f2fc0: mov r0,r4
  000f2fc2: mov r1,r5
  000f2fc4: blx 0x00075904
  000f2fc8: ldr r0,[r4,#0x8]
  000f2fca: ldr.w r4,[r4,#0x12c]
  000f2fce: blx 0x000721cc
  000f2fd2: mov r1,r0
  000f2fd4: ldr r0,[sp,#0xc]
  000f2fd6: ldr.w r2,[r9,#0x0]
  000f2fda: subs r0,r2,r0
  000f2fdc: itttt eq
  000f2fde: mov.eq r0,r4
  000f2fe0: vpop.eq {d6,d7,d8}
  000f2fe4: pop.eq.w {r8,r9,r11}
  000f2fe8: pop.eq.w {r4,r5,r6,r7,lr}
  000f2fec: it eq
  000f2fee: b.eq.w 0x001ac298
  000f2ff2: blx 0x0006e824
  001ac298: bx pc
  001ac29c: ldr r12,[0x1ac2a4]
  001ac2a0: add pc,r12,pc
```
