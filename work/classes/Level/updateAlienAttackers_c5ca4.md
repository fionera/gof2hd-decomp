# Level::updateAlienAttackers
elf 0xc5ca4 body 642
Sig: undefined __stdcall updateAlienAttackers(int param_1)

## decompile
```c

/* Level::updateAlienAttackers(int) */

undefined4 Level::updateAlienAttackers(int param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  Mission *this;
  int iVar6;
  uint *puVar7;
  Station *this_00;
  undefined4 uVar8;
  undefined4 uVar9;
  float *pfVar10;
  int in_r1;
  int iVar11;
  code *pcVar12;
  undefined4 *puVar13;
  code *pcVar14;
  int iVar15;
  code *pcVar16;
  int *piVar17;
  undefined4 uVar18;
  uint uVar19;
  int *piVar20;
  uint in_fpscr;
  undefined4 in_s0;
  float fVar21;
  float fVar22;
  undefined8 in_d4;
  undefined8 in_d5;
  undefined8 in_d6;
  undefined4 in_s14;
  float fVar23;
  float fVar24;
  undefined4 local_a8;
  undefined1 auStack_90 [8];
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined4 uStack_70;
  int local_6c;
  
  piVar20 = *(int **)(DAT_000d5f40 + 0xd5cba);
  uVar18 = *(undefined4 *)(DAT_000d5f44 + 0xd5cbc);
  _uStack_70 = CONCAT44(*piVar20,in_s14);
  *(int *)(param_1 + 0x170) = *(int *)(param_1 + 0x170) + in_r1;
  local_88 = in_d4;
  local_80 = in_d5;
  local_78 = in_d6;
  this = (Mission *)Status::getMission();
  if (((this == (Mission *)0x0) || (iVar6 = Mission::isCampaignMission(this), iVar6 == 0)) ||
     ((iVar6 = Status::getCurrentCampaignMission(), local_a8 = in_s0, iVar6 != 0x28 &&
      ((iVar6 = Status::getCurrentCampaignMission(), local_a8 = in_s0, iVar6 != 0x93 &&
       (iVar6 = Status::getCurrentCampaignMission(), local_a8 = in_s0, iVar6 != 0x9a)))))) {
    iVar15 = *(int *)(param_1 + 0x170);
    iVar6 = Status::getCurrentCampaignMission();
    iVar11 = 45000;
    if (iVar6 == 0x29) {
      iVar11 = 10000;
    }
    local_a8 = in_s0;
    if (iVar11 < iVar15) {
      puVar7 = *(uint **)(param_1 + 0xf8);
      *(undefined4 *)(param_1 + 0x170) = 0;
      fVar3 = DAT_000d5f34;
      fVar2 = DAT_000d5f30;
      fVar1 = DAT_000d5f28;
      local_a8 = in_s0;
      if (puVar7 != (uint *)0x0) {
        pcVar12 = *(code **)(DAT_000d5f48 + 0xd5d48);
        puVar13 = *(undefined4 **)(DAT_000d5f4c + 0xd5d5a);
        pcVar14 = *(code **)(DAT_000d5f50 + 0xd5d60);
        for (uVar19 = 0; local_a8 = uVar18, uVar19 < *puVar7; uVar19 = uVar19 + 1) {
          piVar17 = *(int **)(puVar7[1] + uVar19 * 4);
          if (((piVar17[10] == 9) && (iVar6 = KIPlayer::isDead(), iVar6 != 0)) &&
             (iVar6 = Player::isActive(), iVar6 == 0)) {
            (**(code **)(*piVar17 + 0x18))(piVar17);
            iVar6 = Status::inAlienOrbit();
            if (iVar6 == 0) {
              this_00 = (Station *)Status::getStation();
              iVar6 = Station::isAttackedByAliens(this_00);
              if (iVar6 == 0) goto LAB_000d5e58;
              (**(code **)(**(int **)(*(int *)(*(int *)(param_1 + 0x100) + 4) + 0xc) + 0x28))
                        (&local_78);
              fVar5 = (float)local_78;
              uVar8 = (*pcVar14)(*puVar13,20000);
              (**(code **)(**(int **)(*(int *)(*(int *)(param_1 + 0x100) + 4) + 0xc) + 0x28))
                        ((int)&local_88 + 4);
              fVar4 = (float)local_80;
              uVar9 = (*pcVar14)(*puVar13,20000);
              (**(code **)(**(int **)(*(int *)(*(int *)(param_1 + 0x100) + 4) + 0xc) + 0x28))
                        (auStack_90);
              fVar23 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
              fVar21 = (float)local_88;
              fVar24 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
              uVar8 = (*pcVar14)(*puVar13,20000);
              fVar22 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
              pcVar16 = *(code **)(*piVar17 + 0x48);
              fVar23 = fVar5 + fVar1 + fVar23;
              fVar24 = fVar4 + fVar1 + fVar24;
              fVar22 = fVar21 + fVar1 + fVar22;
            }
            else {
LAB_000d5e58:
              (*pcVar12)(&local_78,*(undefined4 *)(param_1 + 0xf0));
              fVar4 = (float)local_78;
              uVar8 = (*pcVar14)(*puVar13,DAT_000d5f2c);
              (*pcVar12)((int)&local_88 + 4,*(undefined4 *)(param_1 + 0xf0));
              fVar21 = (float)local_80;
              uVar9 = (*pcVar14)(*puVar13,60000);
              (*pcVar12)(auStack_90,*(undefined4 *)(param_1 + 0xf0));
              fVar23 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
              fVar24 = (float)VectorSignedToFloat(uVar9,(byte)(in_fpscr >> 0x16) & 3);
              fVar22 = (float)local_88;
              fVar23 = fVar4 + fVar2 + fVar23;
              fVar24 = fVar21 + fVar3 + fVar24;
              uVar8 = (*pcVar14)(*puVar13,2);
              fVar21 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
              pcVar16 = *(code **)(*piVar17 + 0x48);
              pfVar10 = (float *)&DAT_000d5f38;
              in_fpscr = in_fpscr & 0xfffffff | (uint)(fVar22 + fVar21 == 0.0) << 0x1e;
              if ((byte)(in_fpscr >> 0x1e) != 0) {
                pfVar10 = (float *)&DAT_000d5f3c;
              }
              fVar22 = *pfVar10;
            }
            (*pcVar16)(piVar17,fVar23,fVar24,fVar22);
          }
          puVar7 = *(uint **)(param_1 + 0xf8);
        }
      }
    }
  }
  if (*piVar20 != local_6c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return local_a8;
}

```
## target disasm
```
  000d5ca4: push {r4,r5,r6,r7,lr}
  000d5ca6: add r7,sp,#0xc
  000d5ca8: push {r7,r8,r9,r10,r11}
  000d5cac: vpush {d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15}
  000d5cb0: mov r4,r0
  000d5cb2: ldr r0,[0x000d5f40]
  000d5cb4: ldr r2,[0x000d5f44]
  000d5cb6: add r0,pc
  000d5cb8: add r2,pc
  000d5cba: ldr.w r11,[r0,#0x0]
  000d5cbe: ldr.w r9,[r2,#0x0]
  000d5cc2: ldr.w r0,[r11,#0x0]
  000d5cc6: str r0,[sp,#0x3c]
  000d5cc8: ldr.w r0,[r4,#0x170]
  000d5ccc: add r0,r1
  000d5cce: str.w r0,[r4,#0x170]
  000d5cd2: ldr.w r0,[r9,#0x0]
  000d5cd6: blx 0x00072850
  000d5cda: cbz r0,0x000d5d0c
  000d5cdc: blx 0x0007372c
  000d5ce0: cbz r0,0x000d5d0c
  000d5ce2: ldr.w r0,[r9,#0x0]
  000d5ce6: blx 0x00071770
  000d5cea: cmp r0,#0x28
  000d5cec: beq.w 0x000d5f0c
  000d5cf0: ldr.w r0,[r9,#0x0]
  000d5cf4: blx 0x00071770
  000d5cf8: cmp r0,#0x93
  000d5cfa: beq.w 0x000d5f0c
  000d5cfe: ldr.w r0,[r9,#0x0]
  000d5d02: blx 0x00071770
  000d5d06: cmp r0,#0x9a
  000d5d08: beq.w 0x000d5f0c
  000d5d0c: ldr.w r0,[r9,#0x0]
  000d5d10: ldr.w r5,[r4,#0x170]
  000d5d14: blx 0x00071770
  000d5d18: movw r1,#0xafc8
  000d5d1c: cmp r0,#0x29
  000d5d1e: it eq
  000d5d20: movw.eq r1,#0x2710
  000d5d24: cmp r5,r1
  000d5d26: ble.w 0x000d5f0c
  000d5d2a: ldr.w r0,[r4,#0xf8]
  000d5d2e: movs r1,#0x0
  000d5d30: str.w r1,[r4,#0x170]
  000d5d34: cmp r0,#0x0
  000d5d36: beq.w 0x000d5f0c
  000d5d3a: ldr r1,[0x000d5f48]
  000d5d3c: mov.w r10,#0x0
  000d5d40: vldr.32 s16,[pc,#0x1ec]
  000d5d44: add r1,pc
  000d5d46: vldr.32 s18,[pc,#0x1ec]
  000d5d4a: vldr.32 s20,[pc,#0x1dc]
  000d5d4e: ldr r1,[r1,#0x0]
  000d5d50: str r1,[sp,#0x4]
  000d5d52: ldr r1,[0x000d5f4c]
  000d5d54: ldr r2,[0x000d5f50]
  000d5d56: add r1,pc
  000d5d58: str.w r11,[sp,#0x10]
  000d5d5c: add r2,pc
  000d5d5e: str.w r9,[sp,#0x0]
  000d5d62: ldr r1,[r1,#0x0]
  000d5d64: str r1,[sp,#0xc]
  000d5d66: ldr r1,[r2,#0x0]
  000d5d68: str r1,[sp,#0x8]
  000d5d6a: b 0x000d5f04
  000d5d6c: ldr r0,[r0,#0x4]
  000d5d6e: ldr.w r6,[r0,r10,lsl #0x2]
  000d5d72: ldr r0,[r6,#0x28]
  000d5d74: cmp r0,#0x9
  000d5d76: bne.w 0x000d5efc
  000d5d7a: mov r0,r6
  000d5d7c: blx 0x00071ec0
  000d5d80: cmp r0,#0x0
  000d5d82: beq.w 0x000d5efc
  000d5d86: ldr r0,[r6,#0x4]
  000d5d88: blx 0x00071f14
  000d5d8c: cmp r0,#0x0
  000d5d8e: bne.w 0x000d5efc
  000d5d92: ldr r0,[r6,#0x0]
  000d5d94: ldr r1,[r0,#0x18]
  000d5d96: mov r0,r6
  000d5d98: blx r1
  000d5d9a: ldr.w r0,[r9,#0x0]
  000d5d9e: blx 0x000723d0
  000d5da2: cmp r0,#0x0
  000d5da4: bne 0x000d5e58
  000d5da6: ldr.w r0,[r9,#0x0]
  000d5daa: blx 0x00071c14
  000d5dae: blx 0x0007321c
  000d5db2: cmp r0,#0x0
  000d5db4: beq 0x000d5e58
  000d5db6: ldr.w r0,[r4,#0x100]
  000d5dba: ldr r0,[r0,#0x4]
  000d5dbc: ldr r1,[r0,#0xc]
  000d5dbe: ldr r0,[r1,#0x0]
  000d5dc0: ldr r2,[r0,#0x28]
  000d5dc2: add r0,sp,#0x30
  000d5dc4: blx r2
  000d5dc6: ldr r5,[sp,#0xc]
  000d5dc8: movw r1,#0x4e20
  000d5dcc: ldr.w r11,[sp,#0x8]
  000d5dd0: vldr.32 s22,[sp,#0x30]
  000d5dd4: ldr r0,[r5,#0x0]
  000d5dd6: blx r11
  000d5dd8: str r0,[sp,#0x14]
  000d5dda: ldr.w r0,[r4,#0x100]
  000d5dde: ldr r0,[r0,#0x4]
  000d5de0: ldr r1,[r0,#0xc]
  000d5de2: ldr r0,[r1,#0x0]
  000d5de4: ldr r2,[r0,#0x28]
  000d5de6: add r0,sp,#0x24
  000d5de8: blx r2
  000d5dea: ldr r0,[r5,#0x0]
  000d5dec: movw r1,#0x4e20
  000d5df0: vldr.32 s24,[sp,#0x28]
  000d5df4: blx r11
  000d5df6: mov r8,r0
  000d5df8: ldr.w r0,[r4,#0x100]
  000d5dfc: ldr r0,[r0,#0x4]
  000d5dfe: ldr r1,[r0,#0xc]
  000d5e00: ldr r0,[r1,#0x0]
  000d5e02: ldr r2,[r0,#0x28]
  000d5e04: add r0,sp,#0x18
  000d5e06: blx r2
  000d5e08: ldr r0,[sp,#0x14]
  000d5e0a: movw r1,#0x4e20
  000d5e0e: vmov s0,r0
  000d5e12: ldr r0,[r5,#0x0]
  000d5e14: vcvt.f32.s32 s26,s0
  000d5e18: vldr.32 s28,[sp,#0x20]
  000d5e1c: vmov s0,r8
  000d5e20: vcvt.f32.s32 s30,s0
  000d5e24: blx r11
  000d5e26: vmov s6,r0
  000d5e2a: vadd.f32 s0,s22,s20
  000d5e2e: vadd.f32 s2,s24,s20
  000d5e32: vcvt.f32.s32 s6,s6
  000d5e36: ldr r0,[r6,#0x0]
  000d5e38: vadd.f32 s4,s28,s20
  000d5e3c: ldr r5,[r0,#0x48]
  000d5e3e: vadd.f32 s0,s0,s26
  000d5e42: vadd.f32 s2,s2,s30
  000d5e46: vadd.f32 s4,s4,s6
  000d5e4a: vmov r1,s0
  000d5e4e: vmov r2,s2
  000d5e52: vmov r3,s4
  000d5e56: b 0x000d5ef4
  000d5e58: ldr.w r1,[r4,#0xf0]
  000d5e5c: add r0,sp,#0x30
  000d5e5e: ldr.w r8,[sp,#0x4]
  000d5e62: blx r8
  000d5e64: ldr.w r9,[sp,#0xc]
  000d5e68: ldr r1,[0x000d5f2c]
  000d5e6a: ldr.w r11,[sp,#0x8]
  000d5e6e: ldr.w r0,[r9,#0x0]
  000d5e72: vldr.32 s22,[sp,#0x30]
  000d5e76: blx r11
  000d5e78: ldr.w r1,[r4,#0xf0]
  000d5e7c: str r0,[sp,#0x14]
  000d5e7e: add r0,sp,#0x24
  000d5e80: blx r8
  000d5e82: ldr.w r0,[r9,#0x0]
  000d5e86: movw r1,#0xea60
  000d5e8a: vldr.32 s24,[sp,#0x28]
  000d5e8e: blx r11
  000d5e90: ldr.w r1,[r4,#0xf0]
  000d5e94: mov r5,r0
  000d5e96: add r0,sp,#0x18
  000d5e98: blx r8
  000d5e9a: ldr r0,[sp,#0x14]
  000d5e9c: vmov s6,r5
  000d5ea0: vadd.f32 s0,s22,s16
  000d5ea4: movs r1,#0x2
  000d5ea6: vadd.f32 s4,s24,s18
  000d5eaa: vmov s2,r0
  000d5eae: vcvt.f32.s32 s2,s2
  000d5eb2: vcvt.f32.s32 s6,s6
  000d5eb6: ldr.w r0,[r9,#0x0]
  000d5eba: ldr.w r9,[sp,#0x0]
  000d5ebe: vldr.32 s26,[sp,#0x20]
  000d5ec2: vadd.f32 s22,s0,s2
  000d5ec6: vadd.f32 s24,s4,s6
  000d5eca: blx r11
  000d5ecc: vmov s0,r0
  000d5ed0: vmov r1,s22
  000d5ed4: vcvt.f32.s32 s0,s0
  000d5ed8: ldr r0,[r6,#0x0]
  000d5eda: vmov r2,s24
  000d5ede: ldr r5,[r0,#0x48]
  000d5ee0: adr r0,[0xd5f38]
  000d5ee2: vadd.f32 s0,s26,s0
  000d5ee6: vcmpe.f32 s0,#0
  000d5eea: vmrs apsr,fpscr
  000d5eee: it eq
  000d5ef0: add.eq r0,#0x4
  000d5ef2: ldr r3,[r0,#0x0]
  000d5ef4: mov r0,r6
  000d5ef6: blx r5
  000d5ef8: ldr.w r11,[sp,#0x10]
  000d5efc: ldr.w r0,[r4,#0xf8]
  000d5f00: add.w r10,r10,#0x1
  000d5f04: ldr r1,[r0,#0x0]
  000d5f06: cmp r10,r1
  000d5f08: bcc.w 0x000d5d6c
  000d5f0c: ldr r0,[sp,#0x3c]
  000d5f0e: ldr.w r1,[r11,#0x0]
  000d5f12: subs r0,r1,r0
  000d5f14: itttt eq
  000d5f16: vpop.eq {d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15}
  000d5f1a: add.eq sp,#0x4
  000d5f1c: pop.eq.w {r8,r9,r10,r11}
  000d5f20: pop.eq {r4,r5,r6,r7,pc}
  000d5f22: blx 0x0006e824
```
