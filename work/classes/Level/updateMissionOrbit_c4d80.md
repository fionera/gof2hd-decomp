# Level::updateMissionOrbit
elf 0xc4d80 body 1124
Sig: undefined __thiscall updateMissionOrbit(Level * this, int param_1)

## decompile
```c

/* Level::updateMissionOrbit(int) */

void __thiscall Level::updateMissionOrbit(Level *this,int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  float unaff_r4;
  float unaff_r5;
  uint uVar6;
  float unaff_r6;
  int iVar7;
  code *pcVar8;
  undefined4 *puVar9;
  uint in_fpscr;
  float fVar10;
  
  piVar1 = *(int **)(DAT_000d50dc + 0xd4d92);
  iVar2 = *piVar1;
  if (this[0x288] != (Level)0x0) {
    Status::getMission();
    iVar3 = Mission::isEmpty();
    if ((iVar3 == 0) &&
       (iVar3 = *(int *)(this + 0x174), *(int *)(this + 0x174) = iVar3 + param_1,
       0x57e4 < iVar3 + param_1)) {
      iVar7 = 0;
      *(undefined4 *)(this + 0x174) = 0;
      for (iVar3 = 0; iVar3 != 4; iVar3 = iVar3 + 1) {
        uVar6 = KIPlayer::isDead();
        iVar7 = iVar7 + (uVar6 ^ 1);
      }
      if (iVar7 != 0) {
        puVar9 = *(undefined4 **)(DAT_000d50e4 + 0xd4e02);
        pcVar8 = *(code **)(DAT_000d50e8 + 0xd4e04);
        for (uVar6 = 4; uVar6 < **(uint **)(this + 0xf8); uVar6 = uVar6 + 1) {
          piVar5 = *(int **)((*(uint **)(this + 0xf8))[1] + uVar6 * 4);
          iVar3 = KIPlayer::isDead();
          if ((iVar3 != 0) && (iVar3 = Player::isActive(), iVar3 == 0)) {
            (**(code **)(*piVar5 + 0x18))(piVar5);
            PlayerEgo::getPosition();
            iVar3 = (*pcVar8)(*puVar9,40000);
            iVar7 = (*pcVar8)(*puVar9,2);
            iVar4 = -1;
            if (iVar7 == 0) {
              iVar4 = 1;
            }
            fVar10 = (float)VectorSignedToFloat(iVar4 * (iVar3 + 40000),(byte)(in_fpscr >> 0x16) & 3
                                               );
            unaff_r4 = unaff_r4 + fVar10;
            iVar3 = (*pcVar8)(*puVar9,5000);
            iVar7 = (*pcVar8)(*puVar9,2);
            iVar4 = -1;
            if (iVar7 == 0) {
              iVar4 = 1;
            }
            fVar10 = (float)VectorSignedToFloat((iVar3 + 5000) * iVar4,(byte)(in_fpscr >> 0x16) & 3)
            ;
            unaff_r5 = unaff_r5 + fVar10;
            iVar3 = (*pcVar8)(*puVar9,40000);
            iVar7 = (*pcVar8)(*puVar9,2);
            iVar4 = -1;
            if (iVar7 == 0) {
              iVar4 = 1;
            }
            fVar10 = (float)VectorSignedToFloat((iVar3 + 40000) * iVar4,(byte)(in_fpscr >> 0x16) & 3
                                               );
            unaff_r6 = unaff_r6 + fVar10;
            (**(code **)(*piVar5 + 0x48))(piVar5,unaff_r4,unaff_r5,unaff_r6);
          }
        }
      }
    }
  }
  iVar3 = Status::getMission();
  if (iVar3 != 0) {
    Status::getMission();
    iVar3 = Mission::getType();
    if ((iVar3 == 0xb7) &&
       (iVar3 = *(int *)(this + 0x174), *(int *)(this + 0x174) = iVar3 + param_1,
       0x1d4c < iVar3 + param_1)) {
      *(undefined4 *)(this + 0x174) = 0;
      puVar9 = *(undefined4 **)(DAT_000d51f8 + 0xd4f54);
      pcVar8 = *(code **)(DAT_000d51fc + 0xd4f56);
      for (uVar6 = 0; uVar6 < **(uint **)(this + 0xf8); uVar6 = uVar6 + 1) {
        piVar5 = *(int **)((*(uint **)(this + 0xf8))[1] + uVar6 * 4);
        iVar3 = KIPlayer::isDead();
        if (((iVar3 != 0) && (iVar3 = Player::isActive(), iVar3 == 0)) && (piVar5[0x1f] != 0x33)) {
          (**(code **)(*piVar5 + 0x18))(piVar5);
          PlayerEgo::getPosition();
          iVar3 = (*pcVar8)(*puVar9,30000);
          iVar7 = (*pcVar8)(*puVar9,2);
          iVar4 = -1;
          if (iVar7 == 0) {
            iVar4 = 1;
          }
          fVar10 = (float)VectorSignedToFloat((iVar3 + 50000) * iVar4,(byte)(in_fpscr >> 0x16) & 3);
          unaff_r4 = unaff_r4 + fVar10;
          iVar3 = (*pcVar8)(*puVar9,10000);
          iVar7 = (*pcVar8)(*puVar9,2);
          iVar4 = -1;
          if (iVar7 == 0) {
            iVar4 = 1;
          }
          fVar10 = (float)VectorSignedToFloat((iVar3 + 10000) * iVar4,(byte)(in_fpscr >> 0x16) & 3);
          unaff_r5 = unaff_r5 + fVar10;
          iVar3 = (*pcVar8)(*puVar9,30000);
          iVar7 = (*pcVar8)(*puVar9,2);
          iVar4 = -1;
          if (iVar7 == 0) {
            iVar4 = 1;
          }
          fVar10 = (float)VectorSignedToFloat((iVar3 + 50000) * iVar4,(byte)(in_fpscr >> 0x16) & 3);
          unaff_r6 = unaff_r6 + fVar10;
          (**(code **)(*piVar5 + 0x48))(piVar5,unaff_r4,unaff_r5,unaff_r6);
          piVar5[0x14] = 0;
        }
      }
    }
  }
  Status::getMission();
  iVar3 = Mission::isEmpty();
  if (iVar3 == 0) {
    Status::getMission();
    iVar3 = Mission::getType();
    if ((iVar3 == 0xf) &&
       (iVar3 = *(int *)(this + 0x174), *(int *)(this + 0x174) = iVar3 + param_1,
       50000 < iVar3 + param_1)) {
      uVar6 = 0;
      iVar3 = 1;
      *(undefined4 *)(this + 0x174) = 0;
      while( true ) {
        piVar5 = *(int **)(this + 0xf8);
        iVar7 = *piVar5;
        if ((iVar3 * -0x80000000 == 0) || (iVar7 - 1U <= uVar6)) break;
        iVar3 = KIPlayer::isDead();
        uVar6 = uVar6 + 1;
      }
      if (iVar3 * -0x80000000 != 0) {
        puVar9 = *(undefined4 **)(DAT_000d5200 + 0xd50d2);
        pcVar8 = *(code **)(DAT_000d5204 + 0xd50d4);
        for (uVar6 = 0; uVar6 < iVar7 - 1U; uVar6 = uVar6 + 1) {
          piVar5 = *(int **)(piVar5[1] + uVar6 * 4);
          iVar3 = KIPlayer::isDead();
          if ((iVar3 != 0) && (iVar3 = Player::isActive(), iVar3 == 0)) {
            (**(code **)(*piVar5 + 0x18))(piVar5);
            PlayerEgo::getPosition();
            iVar3 = (*pcVar8)(*puVar9,40000);
            iVar7 = (*pcVar8)(*puVar9,2);
            iVar4 = -1;
            if (iVar7 == 0) {
              iVar4 = 1;
            }
            fVar10 = (float)VectorSignedToFloat((iVar3 + 40000) * iVar4,(byte)(in_fpscr >> 0x16) & 3
                                               );
            unaff_r4 = unaff_r4 + fVar10;
            iVar3 = (*pcVar8)(*puVar9,5000);
            iVar7 = (*pcVar8)(*puVar9,2);
            iVar4 = -1;
            if (iVar7 == 0) {
              iVar4 = 1;
            }
            fVar10 = (float)VectorSignedToFloat((iVar3 + 5000) * iVar4,(byte)(in_fpscr >> 0x16) & 3)
            ;
            unaff_r5 = unaff_r5 + fVar10;
            iVar3 = (*pcVar8)(*puVar9,40000);
            iVar7 = (*pcVar8)(*puVar9,2);
            iVar4 = -1;
            if (iVar7 == 0) {
              iVar4 = 1;
            }
            fVar10 = (float)VectorSignedToFloat((iVar3 + 40000) * iVar4,(byte)(in_fpscr >> 0x16) & 3
                                               );
            unaff_r6 = unaff_r6 + fVar10;
            (**(code **)(*piVar5 + 0x48))(piVar5,unaff_r4,unaff_r5,unaff_r6);
          }
          piVar5 = *(int **)(this + 0xf8);
          iVar7 = *piVar5;
        }
      }
    }
  }
  if (*piVar1 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000d4d80: push {r4,r5,r6,r7,lr}
  000d4d82: add r7,sp,#0xc
  000d4d84: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000d4d88: str r1,[sp,#0x4]
  000d4d8a: mov r11,r0
  000d4d8c: ldr r0,[0x000d50dc]
  000d4d8e: add r0,pc
  000d4d90: ldr r0,[r0,#0x0]
  000d4d92: str r0,[sp,#0x8]
  000d4d94: ldr r0,[r0,#0x0]
  000d4d96: str r0,[sp,#0x18]
  000d4d98: ldrb.w r0,[r11,#0x288]
  000d4d9c: cmp r0,#0x0
  000d4d9e: beq.w 0x000d4f04
  000d4da2: ldr r0,[0x000d50e0]
  000d4da4: add r0,pc
  000d4da6: ldr r0,[r0,#0x0]
  000d4da8: ldr r0,[r0,#0x0]
  000d4daa: blx 0x00072850
  000d4dae: blx 0x00072868
  000d4db2: cmp r0,#0x0
  000d4db4: bne.w 0x000d4f04
  000d4db8: ldr.w r0,[r11,#0x174]
  000d4dbc: ldr r1,[sp,#0x4]
  000d4dbe: add r0,r1
  000d4dc0: movw r1,#0x57e5
  000d4dc4: cmp r0,r1
  000d4dc6: str.w r0,[r11,#0x174]
  000d4dca: blt.w 0x000d4f04
  000d4dce: movs r5,#0x0
  000d4dd0: movs r6,#0x0
  000d4dd2: str.w r5,[r11,#0x174]
  000d4dd6: b 0x000d4dee
  000d4dd8: ldr.w r0,[r11,#0xf8]
  000d4ddc: ldr r0,[r0,#0x4]
  000d4dde: ldr.w r0,[r0,r5,lsl #0x2]
  000d4de2: blx 0x00071ec0
  000d4de6: eor r0,r0,#0x1
  000d4dea: adds r5,#0x1
  000d4dec: add r6,r0
  000d4dee: cmp r5,#0x4
  000d4df0: bne 0x000d4dd8
  000d4df2: cmp r6,#0x0
  000d4df4: beq.w 0x000d4f04
  000d4df8: ldr r0,[0x000d50e4]
  000d4dfa: movs r5,#0x4
  000d4dfc: ldr r1,[0x000d50e8]
  000d4dfe: add r0,pc
  000d4e00: add r1,pc
  000d4e02: ldr.w r9,[r0,#0x0]
  000d4e06: ldr.w r8,[r1,#0x0]
  000d4e0a: b 0x000d4efa
  000d4e0c: ldr r0,[r0,#0x4]
  000d4e0e: ldr.w r6,[r0,r5,lsl #0x2]
  000d4e12: mov r0,r6
  000d4e14: blx 0x00071ec0
  000d4e18: cmp r0,#0x0
  000d4e1a: beq 0x000d4ef8
  000d4e1c: ldr r0,[r6,#0x4]
  000d4e1e: blx 0x00071f14
  000d4e22: cmp r0,#0x0
  000d4e24: bne 0x000d4ef8
  000d4e26: ldr r0,[r6,#0x0]
  000d4e28: ldr r1,[r0,#0x18]
  000d4e2a: mov r0,r6
  000d4e2c: blx r1
  000d4e2e: ldr.w r1,[r11,#0xf0]
  000d4e32: add r0,sp,#0xc
  000d4e34: blx 0x0007264c
  000d4e38: ldr.w r0,[r9,#0x0]
  000d4e3c: movw r1,#0x9c40
  000d4e40: blx r8
  000d4e42: ldr.w r1,[r9,#0x0]
  000d4e46: movw r2,#0x9c40
  000d4e4a: add.w r10,r0,r2
  000d4e4e: mov r0,r1
  000d4e50: movs r1,#0x2
  000d4e52: blx r8
  000d4e54: cmp r0,#0x0
  000d4e56: mov.w r0,#0xffffffff
  000d4e5a: it eq
  000d4e5c: mov.eq r0,#0x1
  000d4e5e: movw r1,#0x1388
  000d4e62: mul r0,r0,r10
  000d4e66: vmov s0,r0
  000d4e6a: vcvt.f32.s32 s0,s0
  000d4e6e: vldr.32 s2,[sp,#0xc]
  000d4e72: ldr.w r0,[r9,#0x0]
  000d4e76: vadd.f32 s0,s2,s0
  000d4e7a: vstr.32 s0,[sp,#0xc]
  000d4e7e: blx r8
  000d4e80: movw r1,#0x1388
  000d4e84: adds r4,r0,r1
  000d4e86: ldr.w r0,[r9,#0x0]
  000d4e8a: movs r1,#0x2
  000d4e8c: blx r8
  000d4e8e: cmp r0,#0x0
  000d4e90: mov.w r0,#0xffffffff
  000d4e94: it eq
  000d4e96: mov.eq r0,#0x1
  000d4e98: movw r1,#0x9c40
  000d4e9c: muls r0,r4
  000d4e9e: vmov s0,r0
  000d4ea2: vcvt.f32.s32 s0,s0
  000d4ea6: vldr.32 s2,[sp,#0x10]
  000d4eaa: ldr.w r0,[r9,#0x0]
  000d4eae: vadd.f32 s0,s2,s0
  000d4eb2: vstr.32 s0,[sp,#0x10]
  000d4eb6: blx r8
  000d4eb8: ldr.w r1,[r9,#0x0]
  000d4ebc: movw r2,#0x9c40
  000d4ec0: adds r4,r0,r2
  000d4ec2: mov r0,r1
  000d4ec4: movs r1,#0x2
  000d4ec6: blx r8
  000d4ec8: cmp r0,#0x0
  000d4eca: mov.w r0,#0xffffffff
  000d4ece: it eq
  000d4ed0: mov.eq r0,#0x1
  000d4ed2: muls r0,r4
  000d4ed4: vmov s0,r0
  000d4ed8: vcvt.f32.s32 s0,s0
  000d4edc: vldr.32 s2,[sp,#0x14]
  000d4ee0: vadd.f32 s0,s2,s0
  000d4ee4: vmov r3,s0
  000d4ee8: vstr.32 s0,[sp,#0x14]
  000d4eec: ldr r0,[r6,#0x0]
  000d4eee: ldrd r1,r2,[sp,#0xc]
  000d4ef2: ldr r4,[r0,#0x48]
  000d4ef4: mov r0,r6
  000d4ef6: blx r4
  000d4ef8: adds r5,#0x1
  000d4efa: ldr.w r0,[r11,#0xf8]
  000d4efe: ldr r1,[r0,#0x0]
  000d4f00: cmp r5,r1
  000d4f02: bcc 0x000d4e0c
  000d4f04: ldr r0,[0x000d51f4]
  000d4f06: add r0,pc
  000d4f08: ldr.w r10,[r0,#0x0]
  000d4f0c: ldr.w r0,[r10,#0x0]
  000d4f10: blx 0x00072850
  000d4f14: cmp r0,#0x0
  000d4f16: beq.w 0x000d505e
  000d4f1a: ldr.w r0,[r10,#0x0]
  000d4f1e: blx 0x00072850
  000d4f22: blx 0x00072874
  000d4f26: cmp r0,#0xb7
  000d4f28: bne.w 0x000d505e
  000d4f2c: ldr.w r0,[r11,#0x174]
  000d4f30: ldr r1,[sp,#0x4]
  000d4f32: add r0,r1
  000d4f34: movw r1,#0x1d4d
  000d4f38: cmp r0,r1
  000d4f3a: str.w r0,[r11,#0x174]
  000d4f3e: blt.w 0x000d505e
  000d4f42: movs r0,#0x0
  000d4f44: mov.w r9,#0x0
  000d4f48: str.w r0,[r11,#0x174]
  000d4f4c: ldr r0,[0x000d51f8]
  000d4f4e: ldr r1,[0x000d51fc]
  000d4f50: add r0,pc
  000d4f52: add r1,pc
  000d4f54: ldr.w r8,[r0,#0x0]
  000d4f58: ldr r4,[r1,#0x0]
  000d4f5a: b 0x000d5052
  000d4f5c: ldr r0,[r0,#0x4]
  000d4f5e: ldr.w r6,[r0,r9,lsl #0x2]
  000d4f62: mov r0,r6
  000d4f64: blx 0x00071ec0
  000d4f68: cmp r0,#0x0
  000d4f6a: beq 0x000d504e
  000d4f6c: ldr r0,[r6,#0x4]
  000d4f6e: blx 0x00071f14
  000d4f72: cmp r0,#0x0
  000d4f74: bne 0x000d504e
  000d4f76: ldr r0,[r6,#0x7c]
  000d4f78: cmp r0,#0x33
  000d4f7a: beq 0x000d504e
  000d4f7c: ldr r0,[r6,#0x0]
  000d4f7e: ldr r1,[r0,#0x18]
  000d4f80: mov r0,r6
  000d4f82: blx r1
  000d4f84: ldr.w r1,[r11,#0xf0]
  000d4f88: add r0,sp,#0xc
  000d4f8a: blx 0x0007264c
  000d4f8e: ldr.w r0,[r8,#0x0]
  000d4f92: movw r1,#0x7530
  000d4f96: blx r4
  000d4f98: ldr.w r1,[r8,#0x0]
  000d4f9c: movw r2,#0xc350
  000d4fa0: adds r5,r0,r2
  000d4fa2: mov r0,r1
  000d4fa4: movs r1,#0x2
  000d4fa6: blx r4
  000d4fa8: cmp r0,#0x0
  000d4faa: mov.w r0,#0xffffffff
  000d4fae: it eq
  000d4fb0: mov.eq r0,#0x1
  000d4fb2: movw r1,#0x2710
  000d4fb6: muls r0,r5
  000d4fb8: vmov s0,r0
  000d4fbc: vcvt.f32.s32 s0,s0
  000d4fc0: vldr.32 s2,[sp,#0xc]
  000d4fc4: ldr.w r0,[r8,#0x0]
  000d4fc8: vadd.f32 s0,s2,s0
  000d4fcc: vstr.32 s0,[sp,#0xc]
  000d4fd0: blx r4
  000d4fd2: movw r1,#0x2710
  000d4fd6: adds r5,r0,r1
  000d4fd8: ldr.w r0,[r8,#0x0]
  000d4fdc: movs r1,#0x2
  000d4fde: blx r4
  000d4fe0: cmp r0,#0x0
  000d4fe2: mov.w r0,#0xffffffff
  000d4fe6: it eq
  000d4fe8: mov.eq r0,#0x1
  000d4fea: movw r1,#0x7530
  000d4fee: muls r0,r5
  000d4ff0: vmov s0,r0
  000d4ff4: vcvt.f32.s32 s0,s0
  000d4ff8: vldr.32 s2,[sp,#0x10]
  000d4ffc: ldr.w r0,[r8,#0x0]
  000d5000: vadd.f32 s0,s2,s0
  000d5004: vstr.32 s0,[sp,#0x10]
  000d5008: blx r4
  000d500a: ldr.w r1,[r8,#0x0]
  000d500e: movw r2,#0xc350
  000d5012: adds r5,r0,r2
  000d5014: mov r0,r1
  000d5016: movs r1,#0x2
  000d5018: blx r4
  000d501a: cmp r0,#0x0
  000d501c: mov.w r0,#0xffffffff
  000d5020: it eq
  000d5022: mov.eq r0,#0x1
  000d5024: muls r0,r5
  000d5026: vmov s0,r0
  000d502a: vcvt.f32.s32 s0,s0
  000d502e: vldr.32 s2,[sp,#0x14]
  000d5032: vadd.f32 s0,s2,s0
  000d5036: vmov r3,s0
  000d503a: vstr.32 s0,[sp,#0x14]
  000d503e: ldr r0,[r6,#0x0]
  000d5040: ldrd r1,r2,[sp,#0xc]
  000d5044: ldr r5,[r0,#0x48]
  000d5046: mov r0,r6
  000d5048: blx r5
  000d504a: movs r0,#0x0
  000d504c: str r0,[r6,#0x50]
  000d504e: add.w r9,r9,#0x1
  000d5052: ldr.w r0,[r11,#0xf8]
  000d5056: ldr r1,[r0,#0x0]
  000d5058: cmp r9,r1
  000d505a: bcc.w 0x000d4f5c
  000d505e: ldr.w r0,[r10,#0x0]
  000d5062: blx 0x00072850
  000d5066: blx 0x00072868
  000d506a: cmp r0,#0x0
  000d506c: bne.w 0x000d51de
  000d5070: ldr.w r0,[r10,#0x0]
  000d5074: blx 0x00072850
  000d5078: blx 0x00072874
  000d507c: cmp r0,#0xf
  000d507e: bne.w 0x000d51de
  000d5082: ldr.w r0,[r11,#0x174]
  000d5086: ldr r1,[sp,#0x4]
  000d5088: add r0,r1
  000d508a: movw r1,#0xc351
  000d508e: cmp r0,r1
  000d5090: str.w r0,[r11,#0x174]
  000d5094: blt.w 0x000d51de
  000d5098: movs r5,#0x0
  000d509a: movs r0,#0x1
  000d509c: str.w r5,[r11,#0x174]
  000d50a0: b 0x000d50b4
  000d50a2: subs r3,r2,#0x1
  000d50a4: cmp r5,r3
  000d50a6: bcs 0x000d50be
  000d50a8: ldr r0,[r1,#0x4]
  000d50aa: ldr.w r0,[r0,r5,lsl #0x2]
  000d50ae: blx 0x00071ec0
  000d50b2: adds r5,#0x1
  000d50b4: ldr.w r1,[r11,#0xf8]
  000d50b8: lsls r0,r0,#0x1f
  000d50ba: ldr r2,[r1,#0x0]
  000d50bc: bne 0x000d50a2
  000d50be: cmp r0,#0x0
  000d50c0: beq.w 0x000d51de
  000d50c4: ldr r0,[0x000d5200]
  000d50c6: movs r5,#0x0
  000d50c8: ldr r3,[0x000d5204]
  000d50ca: movw r8,#0x9c40
  000d50ce: add r0,pc
  000d50d0: add r3,pc
  000d50d2: ldr.w r9,[r0,#0x0]
  000d50d6: ldr.w r10,[r3,#0x0]
  000d50da: b 0x000d51d8
  000d50ec: ldr r0,[r1,#0x4]
  000d50ee: ldr.w r6,[r0,r5,lsl #0x2]
  000d50f2: mov r0,r6
  000d50f4: blx 0x00071ec0
  000d50f8: cmp r0,#0x0
  000d50fa: beq 0x000d51d0
  000d50fc: ldr r0,[r6,#0x4]
  000d50fe: blx 0x00071f14
  000d5102: cmp r0,#0x0
  000d5104: bne 0x000d51d0
  000d5106: ldr r0,[r6,#0x0]
  000d5108: ldr r1,[r0,#0x18]
  000d510a: mov r0,r6
  000d510c: blx r1
  000d510e: ldr.w r1,[r11,#0xf0]
  000d5112: add r0,sp,#0xc
  000d5114: blx 0x0007264c
  000d5118: ldr.w r0,[r9,#0x0]
  000d511c: movw r1,#0x9c40
  000d5120: blx r10
  000d5122: ldr.w r1,[r9,#0x0]
  000d5126: add.w r4,r0,r8
  000d512a: mov r0,r1
  000d512c: movs r1,#0x2
  000d512e: blx r10
  000d5130: cmp r0,#0x0
  000d5132: mov.w r0,#0xffffffff
  000d5136: it eq
  000d5138: mov.eq r0,#0x1
  000d513a: movw r1,#0x1388
  000d513e: muls r0,r4
  000d5140: vmov s0,r0
  000d5144: vcvt.f32.s32 s0,s0
  000d5148: vldr.32 s2,[sp,#0xc]
  000d514c: ldr.w r0,[r9,#0x0]
  000d5150: vadd.f32 s0,s2,s0
  000d5154: vstr.32 s0,[sp,#0xc]
  000d5158: blx r10
  000d515a: movw r1,#0x1388
  000d515e: adds r4,r0,r1
  000d5160: ldr.w r0,[r9,#0x0]
  000d5164: movs r1,#0x2
  000d5166: blx r10
  000d5168: cmp r0,#0x0
  000d516a: mov.w r0,#0xffffffff
  000d516e: it eq
  000d5170: mov.eq r0,#0x1
  000d5172: movw r1,#0x9c40
  000d5176: muls r0,r4
  000d5178: vmov s0,r0
  000d517c: vcvt.f32.s32 s0,s0
  000d5180: vldr.32 s2,[sp,#0x10]
  000d5184: ldr.w r0,[r9,#0x0]
  000d5188: vadd.f32 s0,s2,s0
  000d518c: vstr.32 s0,[sp,#0x10]
  000d5190: blx r10
  000d5192: ldr.w r1,[r9,#0x0]
  000d5196: add.w r4,r0,r8
  000d519a: mov r0,r1
  000d519c: movs r1,#0x2
  000d519e: blx r10
  000d51a0: cmp r0,#0x0
  000d51a2: mov.w r0,#0xffffffff
  000d51a6: it eq
  000d51a8: mov.eq r0,#0x1
  000d51aa: muls r0,r4
  000d51ac: vmov s0,r0
  000d51b0: vcvt.f32.s32 s0,s0
  000d51b4: vldr.32 s2,[sp,#0x14]
  000d51b8: vadd.f32 s0,s2,s0
  000d51bc: vmov r3,s0
  000d51c0: vstr.32 s0,[sp,#0x14]
  000d51c4: ldr r0,[r6,#0x0]
  000d51c6: ldrd r1,r2,[sp,#0xc]
  000d51ca: ldr r4,[r0,#0x48]
  000d51cc: mov r0,r6
  000d51ce: blx r4
  000d51d0: ldr.w r1,[r11,#0xf8]
  000d51d4: adds r5,#0x1
  000d51d6: ldr r2,[r1,#0x0]
  000d51d8: subs r0,r2,#0x1
  000d51da: cmp r5,r0
  000d51dc: bcc 0x000d50ec
  000d51de: ldr r0,[sp,#0x18]
  000d51e0: ldr r1,[sp,#0x8]
  000d51e2: ldr r1,[r1,#0x0]
  000d51e4: subs r0,r1,r0
  000d51e6: ittt eq
  000d51e8: add.eq sp,#0x1c
  000d51ea: pop.eq.w {r8,r9,r10,r11}
  000d51ee: pop.eq {r4,r5,r6,r7,pc}
  000d51f0: blx 0x0006e824
```
