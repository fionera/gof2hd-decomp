# PlayerGasCloud::update
elf 0x1769b4 body 1062
Sig: undefined __thiscall update(PlayerGasCloud * this, int param_1)

## decompile
```c

/* PlayerGasCloud::update(int) */

void __thiscall PlayerGasCloud::update(PlayerGasCloud *this,int param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  undefined4 uVar6;
  uint *puVar7;
  float *pfVar8;
  int iVar9;
  PlayerEgo *pPVar10;
  int iVar11;
  Item *pIVar12;
  int *piVar13;
  int *piVar14;
  Vector *pVVar15;
  int *piVar16;
  AEGeometry *this_00;
  uint uVar17;
  uint in_fpscr;
  uint uVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  Vector aVStack_a8 [12];
  Vector aVStack_9c [12];
  Vector aVStack_90 [12];
  Vector aVStack_84 [12];
  Vector aVStack_78 [12];
  int local_6c;
  
  piVar16 = *(int **)(DAT_00186d74 + 0x1869ce);
  local_6c = *piVar16;
  if (param_1 != 0) {
    if (((this[0x154] == (PlayerGasCloud)0x0) || (this[0x15c] != (PlayerGasCloud)0x0)) ||
       (puVar7 = *(uint **)(this + 0x138), puVar7 == (uint *)0x0)) {
      uVar6 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_00186d78 + 0x1869ea));
      AbyssEngine::Transform::Update(CONCAT44(1,uVar6),SUB41(param_1,0));
    }
    else {
      fVar21 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
      this[0x15c] = (PlayerGasCloud)0x1;
      *(int *)(this + 0x158) = *(int *)(this + 0x158) + param_1;
      fVar5 = DAT_00186dfc;
      fVar4 = DAT_00186df8;
      fVar3 = DAT_00186df4;
      fVar2 = DAT_00186df0;
      fVar1 = DAT_00186de8;
      uVar17 = 0;
      fVar22 = fVar21 * DAT_00186de4;
      piVar13 = *(int **)(DAT_00186e08 + 0x186a6e);
      piVar14 = *(int **)(DAT_00186e0c + 0x186a7e);
      while (uVar17 < *puVar7) {
        *(int *)(*(int *)(*(int *)(this + 0x14c) + 4) + uVar17 * 4) =
             *(int *)(*(int *)(*(int *)(this + 0x14c) + 4) + uVar17 * 4) - param_1;
        pfVar8 = (float *)(*(int *)(*(int *)(this + 0x140) + 4) + uVar17 * 4);
        fVar20 = *pfVar8 - fVar22;
        *pfVar8 = fVar20;
        fVar19 = *(float *)(*(int *)(*(int *)(this + 0x144) + 4) + uVar17 * 4);
        uVar18 = in_fpscr & 0xfffffff;
        if (fVar20 < fVar19) {
          *pfVar8 = fVar19;
        }
        Level::getPlayer();
        PlayerEgo::getTurretPosition();
        AEGeometry::getPosition();
        AbyssEngine::AEMath::operator-(aVStack_90,aVStack_78);
        fVar19 = (float)AbyssEngine::AEMath::VectorLength(aVStack_90);
        in_fpscr = uVar18 & 0xfffffff;
        if ((fVar1 <= fVar19) || (*(int *)(this + 0x158) < 2000)) {
LAB_00186bfa:
          iVar9 = *(int *)(*(int *)(*(int *)(this + 0x14c) + 4) + uVar17 * 4);
          if (iVar9 < 1) {
            fVar19 = (float)VectorSignedToFloat(iVar9,(byte)(in_fpscr >> 0x16) & 3);
            pfVar8 = (float *)(*(int *)(*(int *)(this + 0x148) + 4) + uVar17 * 4);
            fVar19 = fVar19 / fVar5 + 1.0;
            *pfVar8 = fVar19;
            if (fVar19 < 0.0) {
              *pfVar8 = 0.0;
            }
          }
          else if (fVar19 < fVar2) {
            *(float *)(*(int *)(*(int *)(this + 0x148) + 4) + uVar17 * 4) = (fVar19 + fVar3) / fVar4
            ;
          }
          in_fpscr = in_fpscr & 0xfffffff;
          uVar6 = AbyssEngine::PaintCanvas::TransformGetTransform
                            (**(uint **)(DAT_00186e14 + 0x186c60));
          AbyssEngine::Transform::Update(CONCAT44(1,uVar6),SUB41(param_1,0));
        }
        else {
          Level::getPlayer();
          iVar9 = PlayerEgo::isInTurretMode();
          if ((iVar9 == 0) ||
             (*(int *)(*(int *)(*(int *)(this + 0x14c) + 4) + uVar17 * 4) < DAT_00186dec))
          goto LAB_00186bfa;
          *(undefined4 *)(*(int *)(*(int *)(this + 0x14c) + 4) + uVar17 * 4) = DAT_00186e00;
          Status::getShip();
          iVar9 = Ship::getFreeSpace();
          if (iVar9 < 1) {
            iVar9 = Level::getPlayer();
            if (iVar9 != 0) {
              fVar19 = (float)FModSound::stop(*piVar14);
              FModSound::play(*piVar14,(Vector *)0x8d0,(Vector *)0x0,fVar19);
              pPVar10 = (PlayerEgo *)Level::getPlayer();
              iVar9 = PlayerEgo::getHUD(pPVar10);
              Hud::catchCargo(iVar9,*(int *)(this + 0x160),false,true,false,true,false);
            }
          }
          else {
            Item::makeItem(*(int *)(*(int *)(*piVar13 + 4) + *(int *)(this + 0x160) * 4));
            iVar9 = Level::getPlayer();
            if (iVar9 != 0) {
              pPVar10 = (PlayerEgo *)Level::getPlayer();
              iVar9 = PlayerEgo::getHUD(pPVar10);
              Hud::catchCargo(iVar9,*(int *)(this + 0x160),true,false,false,false,false);
              iVar9 = *(int *)(DAT_00186e10 + 0x186b94);
              if ((*(char *)(iVar9 + 0x2d) == '\0') &&
                 (iVar11 = Status::getCurrentCampaignMission(), 0x8e < iVar11)) {
                Status::getMission();
                iVar11 = Mission::isEmpty();
                if ((iVar11 != 0) && (*(int *)(this + 0x160) == 0xcc)) {
                  *(undefined1 *)(iVar9 + 0x2d) = 1;
                  Level::createRadioMessage(*(int *)(this + 0x54),0x1a);
                }
              }
            }
            fVar19 = (float)FModSound::stop(*piVar14);
            FModSound::play(*piVar14,(Vector *)0x8d0,(Vector *)0x0,fVar19);
            pIVar12 = (Item *)Status::getShip();
            Ship::addCargo(pIVar12);
          }
          *(undefined4 *)(*(int *)(*(int *)(this + 0x148) + 4) + uVar17 * 4) = 0;
        }
        if ((*(char *)(*(int *)(*(int *)(this + 0x150) + 4) + uVar17) == '\0') ||
           (*(int *)(this + 0x158) < 2000)) {
LAB_00186d12:
          this_00 = *(AEGeometry **)(*(int *)(*(int *)(this + 0x138) + 4) + uVar17 * 4);
          AbyssEngine::AEMath::operator*
                    (*(float *)(*(int *)(*(int *)(this + 0x140) + 4) + uVar17 * 4) * fVar21,
                     aVStack_9c);
          AbyssEngine::AEMath::operator+(aVStack_90,aVStack_84);
          pVVar15 = aVStack_90;
        }
        else {
          iVar9 = Status::getShip();
          iVar9 = Ship::getFirstEquipmentOfSort(iVar9);
          if (iVar9 == 0) goto LAB_00186d12;
          AbyssEngine::AEMath::operator-(aVStack_9c,aVStack_78);
          AbyssEngine::AEMath::VectorNormalize((AEMath *)aVStack_90,aVStack_9c);
          AbyssEngine::AEMath::Vector::operator=
                    (*(Vector **)(*(int *)(*(int *)(this + 0x13c) + 4) + uVar17 * 4),aVStack_90);
          this_00 = *(AEGeometry **)(*(int *)(*(int *)(this + 0x138) + 4) + uVar17 * 4);
          iVar9 = Status::getShip();
          iVar9 = Ship::getFirstEquipmentOfSort(iVar9);
          iVar9 = Item::getAttribute(iVar9);
          fVar19 = (float)VectorSignedToFloat(iVar9 * param_1,(byte)(in_fpscr >> 0x16) & 3);
          AbyssEngine::AEMath::operator*(fVar19,aVStack_a8);
          AbyssEngine::AEMath::operator+(aVStack_9c,aVStack_84);
          pVVar15 = aVStack_9c;
        }
        AEGeometry::setPosition(this_00,pVVar15);
        iVar9 = uVar17 * 4;
        uVar17 = uVar17 + 1;
        if (DAT_00186dec <= *(int *)(*(int *)(*(int *)(this + 0x14c) + 4) + iVar9)) {
          this[0x15c] = (PlayerGasCloud)0x0;
        }
        puVar7 = *(uint **)(this + 0x138);
      }
      if (this[0x15c] != (PlayerGasCloud)0x0) {
        this[0xf5] = (PlayerGasCloud)0x0;
      }
    }
  }
  if (*piVar16 == local_6c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001869b4: push {r4,r5,r6,r7,lr}
  001869b6: add r7,sp,#0xc
  001869b8: push {r7,r8,r9,r10,r11}
  001869bc: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  001869c0: sub sp,#0x60
  001869c2: mov r5,r0
  001869c4: ldr r0,[0x00186d74]
  001869c6: mov r9,r1
  001869c8: cmp r1,#0x0
  001869ca: add r0,pc
  001869cc: ldr r6,[r0,#0x0]
  001869ce: ldr r0,[r6,#0x0]
  001869d0: str r0,[sp,#0x5c]
  001869d2: beq 0x00186a00
  001869d4: ldrb.w r0,[r5,#0x154]
  001869d8: cbz r0,0x001869e0
  001869da: ldrb.w r0,[r5,#0x15c]
  001869de: cbz r0,0x00186a1c
  001869e0: ldr r0,[0x00186d78]
  001869e2: ldr.w r1,[r5,#0x134]
  001869e6: add r0,pc
  001869e8: ldr r0,[r0,#0x0]
  001869ea: ldr r1,[r1,#0xc]
  001869ec: ldr r0,[r0,#0x0]
  001869ee: blx 0x00072088
  001869f2: movs r1,#0x1
  001869f4: asr.w r3,r9, asr #0x1f
  001869f8: mov r2,r9
  001869fa: str r1,[sp,#0x0]
  001869fc: blx 0x0006f7cc
  00186a00: ldr r0,[sp,#0x5c]
  00186a02: ldr r1,[r6,#0x0]
  00186a04: subs r0,r1,r0
  00186a06: itttt eq
  00186a08: add.eq sp,#0x60
  00186a0a: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  00186a0e: add.eq sp,#0x4
  00186a10: pop.eq.w {r8,r9,r10,r11}
  00186a14: it eq
  00186a16: pop.eq {r4,r5,r6,r7,pc}
  00186a18: blx 0x0006e824
  00186a1c: ldr.w r0,[r5,#0x138]
  00186a20: cmp r0,#0x0
  00186a22: beq 0x001869e0
  00186a24: vmov s0,r9
  00186a28: str r6,[sp,#0x10]
  00186a2a: movs r2,#0x1
  00186a2c: vmov.f32 s24,0x3f800000
  00186a30: vcvt.f32.s32 s16,s0
  00186a34: ldr.w r1,[r5,#0x158]
  00186a38: strb.w r2,[r5,#0x15c]
  00186a3c: add.w r11,sp,#0x38
  00186a40: add r1,r9
  00186a42: str.w r1,[r5,#0x158]
  00186a46: ldr r1,[0x00186e04]
  00186a48: movs r6,#0x0
  00186a4a: vldr.32 s0,[pc,#0x398]
  00186a4e: mov.w r8,#0x0
  00186a52: add r1,pc
  00186a54: vldr.32 s20,[pc,#0x390]
  00186a58: vldr.32 s22,[pc,#0x3a0]
  00186a5c: ldr r1,[r1,#0x0]
  00186a5e: vmul.f32 s18,s16,s0
  00186a62: str r1,[sp,#0x1c]
  00186a64: ldr r1,[0x00186e08]
  00186a66: vldr.32 s26,[pc,#0x388]
  00186a6a: add r1,pc
  00186a6c: vldr.32 s28,[pc,#0x384]
  00186a70: vldr.32 s30,[pc,#0x384]
  00186a74: ldr r1,[r1,#0x0]
  00186a76: str r1,[sp,#0x14]
  00186a78: ldr r1,[0x00186e0c]
  00186a7a: add r1,pc
  00186a7c: ldr r1,[r1,#0x0]
  00186a7e: str r1,[sp,#0x18]
  00186a80: ldr r0,[r0,#0x0]
  00186a82: cmp r8,r0
  00186a84: bcs.w 0x00186dd2
  00186a88: ldr.w r0,[r5,#0x14c]
  00186a8c: ldr r0,[r0,#0x4]
  00186a8e: ldr.w r1,[r0,r8,lsl #0x2]
  00186a92: sub.w r1,r1,r9
  00186a96: str.w r1,[r0,r8,lsl #0x2]
  00186a9a: ldr.w r0,[r5,#0x140]
  00186a9e: ldr r0,[r0,#0x4]
  00186aa0: add.w r0,r0,r8, lsl #0x2
  00186aa4: vldr.32 s0,[r0]
  00186aa8: vsub.f32 s2,s0,s18
  00186aac: vstr.32 s2,[r0]
  00186ab0: ldr.w r1,[r5,#0x144]
  00186ab4: ldr r1,[r1,#0x4]
  00186ab6: add.w r1,r1,r8, lsl #0x2
  00186aba: vldr.32 s0,[r1]
  00186abe: vcmpe.f32 s2,s0
  00186ac2: vmrs apsr,fpscr
  00186ac6: it mi
  00186ac8: vstr.mi.32 s0,[r0]
  00186acc: ldr r0,[r5,#0x54]
  00186ace: blx 0x00072034
  00186ad2: add.w r10,sp,#0x50
  00186ad6: mov r1,r0
  00186ad8: mov r0,r10
  00186ada: blx 0x00077fc8
  00186ade: ldr.w r0,[r5,#0x138]
  00186ae2: add r4,sp,#0x44
  00186ae4: ldr r0,[r0,#0x4]
  00186ae6: ldr.w r1,[r0,r8,lsl #0x2]
  00186aea: mov r0,r4
  00186aec: blx 0x000720b8
  00186af0: mov r0,r11
  00186af2: mov r1,r10
  00186af4: mov r2,r4
  00186af6: blx 0x0006ec38
  00186afa: mov r0,r11
  00186afc: blx 0x0006ec44
  00186b00: vmov s17,r0
  00186b04: vcmpe.f32 s17,s20
  00186b08: vmrs apsr,fpscr
  00186b0c: bpl 0x00186bfa
  00186b0e: ldr.w r0,[r5,#0x158]
  00186b12: cmp.w r0,#0x7d0
  00186b16: blt 0x00186bfa
  00186b18: ldr r0,[r5,#0x54]
  00186b1a: blx 0x00072034
  00186b1e: blx 0x0007678c
  00186b22: cmp r0,#0x0
  00186b24: beq 0x00186bfa
  00186b26: ldr.w r0,[r5,#0x14c]
  00186b2a: ldr r2,[0x00186dec]
  00186b2c: ldr r0,[r0,#0x4]
  00186b2e: ldr.w r1,[r0,r8,lsl #0x2]
  00186b32: cmp r1,r2
  00186b34: blt 0x00186bfa
  00186b36: ldr r1,[0x00186e00]
  00186b38: str.w r1,[r0,r8,lsl #0x2]
  00186b3c: ldr r0,[sp,#0x1c]
  00186b3e: ldr r0,[r0,#0x0]
  00186b40: blx 0x00071a58
  00186b44: blx 0x000722ec
  00186b48: cmp r0,#0x1
  00186b4a: blt.w 0x00186d7c
  00186b4e: ldr r0,[sp,#0x14]
  00186b50: ldr.w r1,[r5,#0x160]
  00186b54: ldr r0,[r0,#0x0]
  00186b56: ldr r0,[r0,#0x4]
  00186b58: ldr.w r0,[r0,r1,lsl #0x2]
  00186b5c: movs r1,#0x1
  00186b5e: blx 0x000718c0
  00186b62: mov r4,r0
  00186b64: ldr r0,[r5,#0x54]
  00186b66: blx 0x00072034
  00186b6a: cbz r0,0x00186bcc
  00186b6c: ldr r0,[r5,#0x54]
  00186b6e: blx 0x00072034
  00186b72: blx 0x00072f10
  00186b76: ldr.w r1,[r5,#0x160]
  00186b7a: movs r2,#0x0
  00186b7c: strd r2,r2,[sp,#0x0]
  00186b80: movs r3,#0x0
  00186b82: str r2,[sp,#0x8]
  00186b84: movs r2,#0x1
  00186b86: str r2,[sp,#0xc]
  00186b88: movs r2,#0x1
  00186b8a: blx 0x00072ce8
  00186b8e: ldr r0,[0x00186e10]
  00186b90: add r0,pc
  00186b92: ldr r6,[r0,#0x0]
  00186b94: ldrb.w r0,[r6,#0x2d]
  00186b98: cbnz r0,0x00186bcc
  00186b9a: ldr r0,[sp,#0x1c]
  00186b9c: ldr r0,[r0,#0x0]
  00186b9e: blx 0x00071770
  00186ba2: cmp r0,#0x8f
  00186ba4: blt 0x00186bcc
  00186ba6: ldr r0,[sp,#0x1c]
  00186ba8: ldr r0,[r0,#0x0]
  00186baa: blx 0x00072850
  00186bae: blx 0x00072868
  00186bb2: cbz r0,0x00186bcc
  00186bb4: ldr.w r0,[r5,#0x160]
  00186bb8: cmp r0,#0xcc
  00186bba: bne 0x00186bcc
  00186bbc: movs r0,#0x1
  00186bbe: movs r1,#0x1a
  00186bc0: strb.w r0,[r6,#0x2d]
  00186bc4: movs r2,#0x0
  00186bc6: ldr r0,[r5,#0x54]
  00186bc8: blx 0x00072754
  00186bcc: ldr r6,[sp,#0x18]
  00186bce: mov.w r1,#0x8d0
  00186bd2: ldr r0,[r6,#0x0]
  00186bd4: blx 0x000724a8
  00186bd8: ldr r0,[r6,#0x0]
  00186bda: movs r6,#0x0
  00186bdc: mov.w r1,#0x8d0
  00186be0: movs r2,#0x0
  00186be2: movs r3,#0x0
  00186be4: str r6,[sp,#0x0]
  00186be6: blx 0x00071548
  00186bea: ldr r0,[sp,#0x1c]
  00186bec: ldr r0,[r0,#0x0]
  00186bee: blx 0x00071a58
  00186bf2: mov r1,r4
  00186bf4: blx 0x00072cdc
  00186bf8: b 0x00186dc6
  00186bfa: ldr.w r0,[r5,#0x14c]
  00186bfe: ldr r0,[r0,#0x4]
  00186c00: ldr.w r0,[r0,r8,lsl #0x2]
  00186c04: cmp r0,#0x0
  00186c06: ble 0x00186c2a
  00186c08: vcmpe.f32 s17,s26
  00186c0c: vmrs apsr,fpscr
  00186c10: bpl 0x00186c54
  00186c12: vadd.f32 s0,s17,s28
  00186c16: ldr.w r0,[r5,#0x148]
  00186c1a: ldr r0,[r0,#0x4]
  00186c1c: add.w r0,r0,r8, lsl #0x2
  00186c20: vdiv.f32 s0,s0,s30
  00186c24: vstr.32 s0,[r0]
  00186c28: b 0x00186c54
  00186c2a: vmov s0,r0
  00186c2e: vcvt.f32.s32 s0,s0
  00186c32: ldr.w r0,[r5,#0x148]
  00186c36: ldr r0,[r0,#0x4]
  00186c38: add.w r0,r0,r8, lsl #0x2
  00186c3c: vdiv.f32 s0,s0,s22
  00186c40: vadd.f32 s0,s0,s24
  00186c44: vcmpe.f32 s0,#0
  00186c48: vstr.32 s0,[r0]
  00186c4c: vmrs apsr,fpscr
  00186c50: it mi
  00186c52: str.mi r6,[r0,#0x0]
  00186c54: ldr.w r0,[r5,#0x138]
  00186c58: ldr r1,[0x00186e14]
  00186c5a: ldr r0,[r0,#0x4]
  00186c5c: add r1,pc
  00186c5e: ldr r1,[r1,#0x0]
  00186c60: ldr.w r2,[r0,r8,lsl #0x2]
  00186c64: ldr r0,[r1,#0x0]
  00186c66: ldr r1,[r2,#0xc]
  00186c68: blx 0x00072088
  00186c6c: movs r1,#0x1
  00186c6e: asr.w r3,r9, asr #0x1f
  00186c72: mov r2,r9
  00186c74: str r1,[sp,#0x0]
  00186c76: blx 0x0006f7cc
  00186c7a: ldr.w r0,[r5,#0x150]
  00186c7e: ldr r0,[r0,#0x4]
  00186c80: ldrb.w r0,[r0,r8]
  00186c84: cmp r0,#0x0
  00186c86: beq 0x00186d12
  00186c88: ldr.w r0,[r5,#0x158]
  00186c8c: cmp.w r0,#0x7d0
  00186c90: blt 0x00186d12
  00186c92: ldr r0,[sp,#0x1c]
  00186c94: ldr r0,[r0,#0x0]
  00186c96: blx 0x00071a58
  00186c9a: movs r1,#0x23
  00186c9c: blx 0x0007228c
  00186ca0: cbz r0,0x00186d12
  00186ca2: add.w r10,sp,#0x2c
  00186ca6: add r4,sp,#0x44
  00186ca8: add r1,sp,#0x50
  00186caa: mov r0,r10
  00186cac: mov r2,r4
  00186cae: blx 0x0006ec38
  00186cb2: mov r0,r11
  00186cb4: mov r1,r10
  00186cb6: blx 0x0006ec80
  00186cba: ldr.w r0,[r5,#0x13c]
  00186cbe: mov r1,r11
  00186cc0: ldr r0,[r0,#0x4]
  00186cc2: ldr.w r0,[r0,r8,lsl #0x2]
  00186cc6: blx 0x0006eb3c
  00186cca: ldr.w r0,[r5,#0x138]
  00186cce: ldr r0,[r0,#0x4]
  00186cd0: ldr.w r6,[r0,r8,lsl #0x2]
  00186cd4: ldr r0,[sp,#0x1c]
  00186cd6: ldr r0,[r0,#0x0]
  00186cd8: blx 0x00071a58
  00186cdc: movs r1,#0x23
  00186cde: blx 0x0007228c
  00186ce2: movs r1,#0x31
  00186ce4: blx 0x000718e4
  00186ce8: mul r0,r0,r9
  00186cec: add r4,sp,#0x20
  00186cee: mov r2,r11
  00186cf0: vmov s0,r0
  00186cf4: mov r0,r4
  00186cf6: vcvt.f32.s32 s0,s0
  00186cfa: vmov r1,s0
  00186cfe: blx 0x00072658
  00186d02: add r1,sp,#0x44
  00186d04: mov r0,r10
  00186d06: mov r2,r4
  00186d08: blx 0x0006f1cc
  00186d0c: mov r0,r6
  00186d0e: mov r1,r10
  00186d10: b 0x00186d4e
  00186d12: ldrd r0,r2,[r5,#0x138]
  00186d16: add r4,sp,#0x2c
  00186d18: ldr.w r1,[r5,#0x140]
  00186d1c: ldr r0,[r0,#0x4]
  00186d1e: ldr.w r6,[r0,r8,lsl #0x2]
  00186d22: ldr r0,[r1,#0x4]
  00186d24: add.w r0,r0,r8, lsl #0x2
  00186d28: vldr.32 s0,[r0]
  00186d2c: ldr r0,[r2,#0x4]
  00186d2e: vmul.f32 s0,s0,s16
  00186d32: ldr.w r2,[r0,r8,lsl #0x2]
  00186d36: mov r0,r4
  00186d38: vmov r1,s0
  00186d3c: blx 0x00072658
  00186d40: add r1,sp,#0x44
  00186d42: mov r0,r11
  00186d44: mov r2,r4
  00186d46: blx 0x0006f1cc
  00186d4a: mov r0,r6
  00186d4c: mov r1,r11
  00186d4e: blx 0x00072148
  00186d52: ldr.w r0,[r5,#0x14c]
  00186d56: movs r6,#0x0
  00186d58: ldr r1,[0x00186dec]
  00186d5a: ldr r0,[r0,#0x4]
  00186d5c: ldr.w r0,[r0,r8,lsl #0x2]
  00186d60: add.w r8,r8,#0x1
  00186d64: cmp r0,r1
  00186d66: it ge
  00186d68: strb.ge.w r6,[r5,#0x15c]
  00186d6c: ldr.w r0,[r5,#0x138]
  00186d70: b 0x00186a80
  00186d7c: ldr r0,[r5,#0x54]
  00186d7e: blx 0x00072034
  00186d82: cbz r0,0x00186dc6
  00186d84: ldr.w r10,[sp,#0x18]
  00186d88: mov.w r1,#0x8d0
  00186d8c: ldr.w r0,[r10,#0x0]
  00186d90: blx 0x000724a8
  00186d94: ldr.w r0,[r10,#0x0]
  00186d98: mov.w r1,#0x8d0
  00186d9c: movs r2,#0x0
  00186d9e: movs r3,#0x0
  00186da0: str r6,[sp,#0x0]
  00186da2: blx 0x00071548
  00186da6: ldr r0,[r5,#0x54]
  00186da8: blx 0x00072034
  00186dac: blx 0x00072f10
  00186db0: ldr.w r1,[r5,#0x160]
  00186db4: movs r2,#0x1
  00186db6: str r6,[sp,#0x0]
  00186db8: movs r3,#0x1
  00186dba: strd r2,r6,[sp,#0x4]
  00186dbe: movs r2,#0x0
  00186dc0: str r6,[sp,#0xc]
  00186dc2: blx 0x00072ce8
  00186dc6: ldr.w r0,[r5,#0x148]
  00186dca: ldr r0,[r0,#0x4]
  00186dcc: str.w r6,[r0,r8,lsl #0x2]
  00186dd0: b 0x00186c7a
  00186dd2: ldr r6,[sp,#0x10]
  00186dd4: ldrb.w r0,[r5,#0x15c]
  00186dd8: cmp r0,#0x0
  00186dda: itt ne
  00186ddc: mov.ne r0,#0x0
  00186dde: strb.ne.w r0,[r5,#0xf5]
  00186de2: b 0x00186a00
```
