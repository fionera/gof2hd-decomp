# Player::shoot
elf 0xa3d5c body 456
Sig: undefined __stdcall shoot(int param_1, int param_2, longlong param_3, bool param_4, Matrix param_5)

## decompile
```c

/* Player::shoot(int, int, long long, bool, AbyssEngine::AEMath::Matrix) */

void Player::shoot(int *param_1,uint param_2,int param_3,undefined4 param_4_00,undefined4 param_4,
                  undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9,
                  undefined4 param_10,undefined4 param_11,undefined4 param_12,undefined4 param_13,
                  undefined4 param_14,undefined4 param_15,undefined4 param_16,undefined4 param_17,
                  undefined4 param_18,undefined4 param_19,undefined4 param_20,undefined4 param_21,
                  undefined4 param_22)

{
  uint uVar1;
  int *piVar2;
  uint *puVar3;
  uint uVar4;
  int iVar5;
  Gun *this;
  uint uVar6;
  Matrix aMStack_70 [12];
  undefined4 local_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  
  uVar1 = DAT_000b3f24;
  piVar2 = *(int **)(DAT_000b3f2c + 0xb3d76);
  local_28 = *piVar2;
  local_2c = param_22;
  local_30 = param_21;
  local_34 = param_20;
  local_38 = param_19;
  local_3c = param_18;
  local_40 = param_17;
  local_44 = param_16;
  local_48 = param_15;
  local_64 = param_8;
  uStack_60 = param_9;
  local_5c = param_10;
  uStack_58 = param_11;
  uStack_54 = param_12;
  uStack_50 = param_13;
  uStack_4c = param_14;
  puVar3 = (uint *)*param_1;
  if ((((puVar3 != (uint *)0x0) && (*(char *)((int)param_1 + 0xc3) != '\0')) && (-1 < (int)param_2))
     && ((param_2 < *puVar3 && (puVar3 = *(uint **)(puVar3[1] + param_2 * 4), puVar3 != (uint *)0x0)
         ))) {
    for (uVar6 = 0; uVar6 < *puVar3; uVar6 = uVar6 + 1) {
      this = *(Gun **)(puVar3[1] + uVar6 * 4);
      uVar4 = *(int *)(this + 0x5c) - 6;
      if (((uVar4 < 0x1d) && ((1 << (uVar4 & 0xff) & uVar1) != 0)) && (-1 < **(int **)(this + 0x3c))
         ) {
        Gun::ignite(this);
      }
      else if ((*(int *)(this + 0x58) == param_3) && (*(int *)(this + 0x48) < *(int *)(this + 0x6c))
              ) {
        if ((uVar4 < 0x1d) && ((1 << (uVar4 & 0xff) & uVar1) != 0)) {
          *(undefined1 *)(*(int *)(this + 0x38) + 0x69) = 1;
        }
        iVar5 = Gun::shoot(this,param_8,param_9,param_10,param_11,param_12,param_13,param_14,
                           param_15,param_16,param_17,param_18,param_19,param_20,param_21,param_22,
                           param_4,param_7);
        if (iVar5 != 0) {
          param_1[0x18] = (int)((float)param_1[0x18] + DAT_000b3f28);
          if ((char)param_1[0x1c] != '\0') {
            AbyssEngine::AEMath::MatrixGetPosition(aMStack_70);
            iVar5 = *(int *)(*(int *)(*(int *)(*(int *)(*param_1 + 4) + param_2 * 4) + 4) +
                            uVar6 * 4);
            playShootSound((int)param_1,*(int *)(iVar5 + 0x58),*(Vector **)(iVar5 + 0x5c),
                           *(float *)(iVar5 + 0xb0));
          }
          *(undefined4 *)(this + 0x6c) = 0;
          break;
        }
      }
      puVar3 = *(uint **)(*(int *)(*param_1 + 4) + param_2 * 4);
    }
  }
  iVar5 = *piVar2 - local_28;
  if (iVar5 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar5);
  }
  return;
}

```
## target disasm
```
  000b3d5c: push {r4,r5,r6,r7,lr}
  000b3d5e: add r7,sp,#0xc
  000b3d60: push {r8,r9,r10,r11}
  000b3d64: sub sp,#0x94
  000b3d66: str r2,[sp,#0x40]
  000b3d68: mov r10,r0
  000b3d6a: ldr r0,[0x000b3f2c]
  000b3d6c: mov r9,r1
  000b3d6e: ldr.w r12,[r7,#0x4c]
  000b3d72: add r0,pc
  000b3d74: ldr r6,[r7,#0x28]
  000b3d76: ldr r4,[r7,#0x18]
  000b3d78: ldr r0,[r0,#0x0]
  000b3d7a: str r0,[sp,#0x44]
  000b3d7c: ldr r5,[r7,#0x14]
  000b3d7e: ldr r0,[r0,#0x0]
  000b3d80: ldrd r8,lr,[r7,#0x44]
  000b3d84: ldrd r1,r11,[r7,#0x3c]
  000b3d88: ldrd r3,r2,[r7,#0x34]
  000b3d8c: str r0,[sp,#0x90]
  000b3d8e: str.w r12,[sp,#0x8c]
  000b3d92: str.w lr,[sp,#0x88]
  000b3d96: str.w r8,[sp,#0x84]
  000b3d9a: str.w r11,[sp,#0x80]
  000b3d9e: str r1,[sp,#0x7c]
  000b3da0: str r2,[sp,#0x78]
  000b3da2: str r3,[sp,#0x74]
  000b3da4: ldr.w r12,[r7,#0x2c]
  000b3da8: ldr r2,[r7,#0x20]
  000b3daa: ldr r3,[r7,#0x24]
  000b3dac: ldr r0,[r7,#0x30]
  000b3dae: ldr r1,[r7,#0x1c]
  000b3db0: str r0,[sp,#0x70]
  000b3db2: add r0,sp,#0x5c
  000b3db4: strd r5,r4,[sp,#0x54]
  000b3db8: stm r0,{r1,r2,r3,r6,r12}
  000b3dbc: ldr.w r0,[r10,#0x0]
  000b3dc0: cmp r0,#0x0
  000b3dc2: beq 0x000b3eba
  000b3dc4: ldrb.w r1,[r10,#0xc3]
  000b3dc8: cmp r1,#0x0
  000b3dca: beq 0x000b3eba
  000b3dcc: movs r6,#0x1
  000b3dce: cmp.w r9,#0x0
  000b3dd2: blt 0x000b3ebc
  000b3dd4: ldr r1,[r0,#0x0]
  000b3dd6: cmp r1,r9
  000b3dd8: bls 0x000b3ebc
  000b3dda: ldr r0,[r0,#0x4]
  000b3ddc: movs r6,#0x1
  000b3dde: ldr.w r0,[r0,r9,lsl #0x2]
  000b3de2: cmp r0,#0x0
  000b3de4: beq 0x000b3ebc
  000b3de6: ldr r5,[0x000b3f24]
  000b3de8: movs r1,#0x1
  000b3dea: ldr r4,[sp,#0x40]
  000b3dec: mov.w r11,#0x0
  000b3df0: str r1,[sp,#0x3c]
  000b3df2: b 0x000b3eac
  000b3df4: ldr r0,[r0,#0x4]
  000b3df6: ldr.w r8,[r0,r11,lsl #0x2]
  000b3dfa: ldr.w r0,[r8,#0x5c]
  000b3dfe: subs r0,#0x6
  000b3e00: cmp r0,#0x1c
  000b3e02: bhi 0x000b3e20
  000b3e04: lsl.w r1,r6,r0
  000b3e08: tst r1,r5
  000b3e0a: mov r2,r5
  000b3e0c: beq 0x000b3e20
  000b3e0e: ldr.w r1,[r8,#0x3c]
  000b3e12: ldr r1,[r1,#0x0]
  000b3e14: cmp r1,#0x0
  000b3e16: blt 0x000b3e20
  000b3e18: mov r0,r8
  000b3e1a: blx 0x00072fdc
  000b3e1e: b 0x000b3e9e
  000b3e20: ldr.w r1,[r8,#0x58]
  000b3e24: cmp r1,r4
  000b3e26: bne 0x000b3e9e
  000b3e28: ldr.w r1,[r8,#0x48]
  000b3e2c: ldr.w r2,[r8,#0x6c]
  000b3e30: cmp r2,r1
  000b3e32: ble 0x000b3e9e
  000b3e34: ldrd r2,r1,[r7,#0x48]
  000b3e38: cmp r0,#0x1c
  000b3e3a: bhi 0x000b3e4e
  000b3e3c: lsl.w r0,r6,r0
  000b3e40: tst r0,r5
  000b3e42: itt ne
  000b3e44: ldr.ne.w r0,[r8,#0x38]
  000b3e48: strb.ne.w r6,[r0,#0x69]
  000b3e4c: mov r3,r5
  000b3e4e: ldr r0,[r7,#0x20]
  000b3e50: add.w r3,r7,#0x14
  000b3e54: str r0,[sp,#0x0]
  000b3e56: ldr r0,[r7,#0x24]
  000b3e58: str r0,[sp,#0x4]
  000b3e5a: ldr r0,[r7,#0x28]
  000b3e5c: str r0,[sp,#0x8]
  000b3e5e: ldr r0,[r7,#0x2c]
  000b3e60: str r0,[sp,#0xc]
  000b3e62: ldr r0,[r7,#0x30]
  000b3e64: str r0,[sp,#0x10]
  000b3e66: ldr r0,[r7,#0x34]
  000b3e68: str r0,[sp,#0x14]
  000b3e6a: ldr r0,[r7,#0x38]
  000b3e6c: str r0,[sp,#0x18]
  000b3e6e: ldr r0,[r7,#0x3c]
  000b3e70: str r0,[sp,#0x1c]
  000b3e72: ldr r0,[r7,#0x40]
  000b3e74: str r0,[sp,#0x20]
  000b3e76: ldr r0,[r7,#0x44]
  000b3e78: strd r0,r2,[sp,#0x24]
  000b3e7c: ldr r0,[r7,#0x8]
  000b3e7e: str r0,[sp,#0x30]
  000b3e80: ldr r0,[r7,#0x10]
  000b3e82: str r0,[sp,#0x34]
  000b3e84: mov r0,r8
  000b3e86: str r1,[sp,#0x2c]
  000b3e88: ldmia r3,{r1,r2,r3}
  000b3e8a: blx 0x00072fe8
  000b3e8e: cbnz r0,0x000b3ed4
  000b3e90: ldr.w r0,[r8,#0x74]
  000b3e94: cmp r0,#0x1
  000b3e96: ldr r0,[sp,#0x3c]
  000b3e98: it lt
  000b3e9a: mov.lt r0,#0x0
  000b3e9c: str r0,[sp,#0x3c]
  000b3e9e: ldr.w r0,[r10,#0x0]
  000b3ea2: add.w r11,r11,#0x1
  000b3ea6: ldr r0,[r0,#0x4]
  000b3ea8: ldr.w r0,[r0,r9,lsl #0x2]
  000b3eac: ldr r1,[r0,#0x0]
  000b3eae: cmp r11,r1
  000b3eb0: bcc 0x000b3df4
  000b3eb2: ldr r5,[sp,#0x3c]
  000b3eb4: and r6,r5,#0x1
  000b3eb8: b 0x000b3ebc
  000b3eba: movs r6,#0x1
  000b3ebc: ldr r0,[sp,#0x90]
  000b3ebe: ldr r1,[sp,#0x44]
  000b3ec0: ldr r1,[r1,#0x0]
  000b3ec2: subs r0,r1,r0
  000b3ec4: itttt eq
  000b3ec6: mov.eq r0,r6
  000b3ec8: add.eq sp,#0x94
  000b3eca: pop.eq.w {r8,r9,r10,r11}
  000b3ece: pop.eq {r4,r5,r6,r7,pc}
  000b3ed0: blx 0x0006e824
  000b3ed4: vldr.32 s0,[pc,#0x50]
  000b3ed8: vldr.32 s2,[r10,#0x60]
  000b3edc: ldrb.w r0,[r10,#0x70]
  000b3ee0: vadd.f32 s0,s2,s0
  000b3ee4: cmp r0,#0x0
  000b3ee6: vstr.32 s0,[r10,#0x60]
  000b3eea: beq 0x000b3f1a
  000b3eec: add r6,sp,#0x48
  000b3eee: add r1,sp,#0x54
  000b3ef0: mov r0,r6
  000b3ef2: blx 0x0006f16c
  000b3ef6: ldr.w r0,[r10,#0x0]
  000b3efa: mov r3,r6
  000b3efc: ldr r0,[r0,#0x4]
  000b3efe: ldr.w r0,[r0,r9,lsl #0x2]
  000b3f02: ldr r0,[r0,#0x4]
  000b3f04: ldr.w r0,[r0,r11,lsl #0x2]
  000b3f08: ldrd r1,r2,[r0,#0x58]
  000b3f0c: vldr.32 s0,[r0,#0xb0]
  000b3f10: mov r0,r10
  000b3f12: vstr.32 s0,[sp]
  000b3f16: blx 0x00072fc4
  000b3f1a: movs r0,#0x0
  000b3f1c: movs r5,#0x1
  000b3f1e: str.w r0,[r8,#0x6c]
  000b3f22: b 0x000b3eb4
```
