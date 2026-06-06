# Player::shoot
elf 0xa3b20 body 402
Sig: undefined __stdcall shoot(int param_1, longlong param_2, bool param_3, Matrix param_4)

## decompile
```c

/* Player::shoot(int, long long, bool, AbyssEngine::AEMath::Matrix) */

void Player::shoot(int *param_1,uint param_2,undefined4 param_3_00,undefined4 param_4,
                  undefined4 param_3,undefined4 param_6,undefined4 param_7,undefined4 param_8,
                  undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
                  undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16,
                  undefined4 param_17,undefined4 param_18,undefined4 param_19,undefined4 param_20)

{
  float fVar1;
  uint *puVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  Matrix aMStack_7c [12];
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  int local_34;
  
  fVar1 = DAT_000b3cb4;
  piVar4 = *(int **)(DAT_000b3cb8 + 0xb3b42);
  local_34 = *piVar4;
  local_70 = param_6;
  local_6c = param_7;
  local_68 = param_8;
  local_64 = param_9;
  local_60 = param_10;
  local_5c = param_11;
  local_58 = param_12;
  local_54 = param_13;
  local_50 = param_14;
  local_4c = param_15;
  uStack_48 = param_16;
  local_44 = param_17;
  local_40 = param_18;
  uStack_3c = param_19;
  local_38 = param_20;
  puVar2 = (uint *)*param_1;
  if ((((puVar2 != (uint *)0x0) && (*(char *)((int)param_1 + 0xc3) != '\0')) && (-1 < (int)param_2))
     && ((param_2 < *puVar2 && (puVar2 = *(uint **)(puVar2[1] + param_2 * 4), puVar2 != (uint *)0x0)
         ))) {
    for (uVar5 = 0; uVar5 < *puVar2; uVar5 = uVar5 + 1) {
      iVar3 = *(int *)(puVar2[1] + uVar5 * 4);
      if (*(int *)(iVar3 + 0x48) < *(int *)(iVar3 + 0x6c)) {
        iVar3 = Gun::shootAt(iVar3,local_70,local_6c,local_68,local_64,local_60,local_5c,local_58,
                             local_54,local_50,local_4c,uStack_48,local_44,local_40,uStack_3c,
                             local_38,param_3_00,param_1,param_3);
        if (iVar3 != 0) {
          param_1[0x18] = (int)((float)param_1[0x18] + fVar1);
          iVar3 = *(int *)(*(int *)(*(int *)(*(int *)(*param_1 + 4) + param_2 * 4) + 4) + uVar5 * 4)
          ;
          *(undefined4 *)(iVar3 + 0x6c) = 0;
          if (((char)param_1[0x1c] != '\0') && (*(char *)(iVar3 + 0x89) != '\0')) {
            AbyssEngine::AEMath::MatrixGetPosition(aMStack_7c);
            iVar3 = *(int *)(*(int *)(*(int *)(*(int *)(*param_1 + 4) + param_2 * 4) + 4) +
                            uVar5 * 4);
            playShootSound((int)param_1,*(int *)(iVar3 + 0x58),*(Vector **)(iVar3 + 0x5c),
                           *(float *)(iVar3 + 0xb0));
          }
        }
      }
      puVar2 = *(uint **)(*(int *)(*param_1 + 4) + param_2 * 4);
    }
  }
  iVar3 = *piVar4 - local_34;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```
## target disasm
```
  000b3b20: push {r4,r5,r6,r7,lr}
  000b3b22: add r7,sp,#0xc
  000b3b24: push {r7,r8,r9,r10,r11}
  000b3b28: vpush {d8}
  000b3b2c: sub sp,#0xa8
  000b3b2e: str r2,[sp,#0x44]
  000b3b30: mov r6,r0
  000b3b32: ldr r0,[0x000b3cb8]
  000b3b34: add.w lr,r7,#0xc
  000b3b38: mov r5,r1
  000b3b3a: ldr.w r12,[r7,#0x20]
  000b3b3e: add r0,pc
  000b3b40: ldr.w r8,[r7,#0x24]
  000b3b44: ldr.w r11,[r7,#0x28]
  000b3b48: add.w r9,sp,#0x88
  000b3b4c: ldr r4,[r0,#0x0]
  000b3b4e: ldm.w lr,{r0,r1,r2,r3,lr}
  000b3b52: str r4,[sp,#0x3c]
  000b3b54: ldr r4,[r4,#0x0]
  000b3b56: str r0,[sp,#0x68]
  000b3b58: str r1,[sp,#0x6c]
  000b3b5a: str r2,[sp,#0x70]
  000b3b5c: str r3,[sp,#0x74]
  000b3b5e: str.w lr,[sp,#0x78]
  000b3b62: str.w r12,[sp,#0x7c]
  000b3b66: str.w r8,[sp,#0x80]
  000b3b6a: ldr r2,[r7,#0x30]
  000b3b6c: ldr r3,[r7,#0x34]
  000b3b6e: ldr.w lr,[r7,#0x38]
  000b3b72: ldr.w r8,[r7,#0x40]
  000b3b76: ldr r1,[r7,#0x2c]
  000b3b78: ldr.w r12,[r7,#0x3c]
  000b3b7c: ldr r0,[r7,#0x44]
  000b3b7e: str.w r11,[sp,#0x84]
  000b3b82: stm.w r9!,{r1,r2,r3,lr}
  000b3b86: strd r12,r8,[sp,#0x98]
  000b3b8a: str r0,[sp,#0xa0]
  000b3b8c: str r4,[sp,#0xa4]
  000b3b8e: ldr r0,[r6,#0x0]
  000b3b90: cmp r0,#0x0
  000b3b92: beq 0x000b3c92
  000b3b94: ldrb.w r1,[r6,#0xc3]
  000b3b98: cmp r1,#0x0
  000b3b9a: beq 0x000b3c92
  000b3b9c: cmp r5,#0x0
  000b3b9e: blt 0x000b3c92
  000b3ba0: ldr r1,[r0,#0x0]
  000b3ba2: cmp r1,r5
  000b3ba4: bls 0x000b3c92
  000b3ba6: ldr r0,[r0,#0x4]
  000b3ba8: ldr.w r0,[r0,r5,lsl #0x2]
  000b3bac: cmp r0,#0x0
  000b3bae: beq 0x000b3c92
  000b3bb0: vldr.32 s16,[pc,#0x100]
  000b3bb4: mov.w r8,#0x0
  000b3bb8: str r5,[sp,#0x40]
  000b3bba: b 0x000b3c8c
  000b3bbc: ldr r0,[r0,#0x4]
  000b3bbe: ldr.w r0,[r0,r8,lsl #0x2]
  000b3bc2: ldr r1,[r0,#0x48]
  000b3bc4: ldr r2,[r0,#0x6c]
  000b3bc6: cmp r2,r1
  000b3bc8: ble 0x000b3c80
  000b3bca: ldr r1,[sp,#0x68]
  000b3bcc: add.w r12,sp,#0x94
  000b3bd0: str r1,[sp,#0x58]
  000b3bd2: ldr r1,[sp,#0x6c]
  000b3bd4: str r1,[sp,#0x54]
  000b3bd6: ldr r1,[sp,#0x70]
  000b3bd8: str r1,[sp,#0x50]
  000b3bda: ldrd r4,r5,[sp,#0x74]
  000b3bde: ldr r1,[sp,#0x7c]
  000b3be0: str r1,[sp,#0x4c]
  000b3be2: ldr r1,[sp,#0x80]
  000b3be4: str r1,[sp,#0x48]
  000b3be6: ldrd lr,r11,[sp,#0x84]
  000b3bea: ldrd r10,r9,[sp,#0x8c]
  000b3bee: ldm.w r12,{r1,r2,r3,r12}
  000b3bf2: str r1,[sp,#0x20]
  000b3bf4: ldr r1,[sp,#0x44]
  000b3bf6: str r2,[sp,#0x24]
  000b3bf8: str r3,[sp,#0x28]
  000b3bfa: str.w r12,[sp,#0x2c]
  000b3bfe: strd r1,r6,[sp,#0x30]
  000b3c02: ldr r1,[r7,#0x8]
  000b3c04: str r1,[sp,#0x38]
  000b3c06: strd r4,r5,[sp,#0x0]
  000b3c0a: ldr r1,[sp,#0x4c]
  000b3c0c: str r1,[sp,#0x8]
  000b3c0e: ldr r1,[sp,#0x48]
  000b3c10: ldr r5,[sp,#0x40]
  000b3c12: strd r1,lr,[sp,#0xc]
  000b3c16: strd r11,r10,[sp,#0x14]
  000b3c1a: str.w r9,[sp,#0x1c]
  000b3c1e: ldr r1,[sp,#0x58]
  000b3c20: ldrd r3,r2,[sp,#0x50]
  000b3c24: blx 0x00072fb8
  000b3c28: cbz r0,0x000b3c80
  000b3c2a: vldr.32 s0,[r6,#0x60]
  000b3c2e: movs r1,#0x0
  000b3c30: ldr r0,[r6,#0x0]
  000b3c32: vadd.f32 s0,s0,s16
  000b3c36: vstr.32 s0,[r6,#0x60]
  000b3c3a: ldr r0,[r0,#0x4]
  000b3c3c: ldr.w r0,[r0,r5,lsl #0x2]
  000b3c40: ldr r0,[r0,#0x4]
  000b3c42: ldr.w r0,[r0,r8,lsl #0x2]
  000b3c46: str r1,[r0,#0x6c]
  000b3c48: ldrb.w r1,[r6,#0x70]
  000b3c4c: cbz r1,0x000b3c80
  000b3c4e: ldrb.w r0,[r0,#0x89]
  000b3c52: cbz r0,0x000b3c80
  000b3c54: add r4,sp,#0x5c
  000b3c56: add r1,sp,#0x68
  000b3c58: mov r0,r4
  000b3c5a: blx 0x0006f16c
  000b3c5e: ldr r0,[r6,#0x0]
  000b3c60: mov r3,r4
  000b3c62: ldr r0,[r0,#0x4]
  000b3c64: ldr.w r0,[r0,r5,lsl #0x2]
  000b3c68: ldr r0,[r0,#0x4]
  000b3c6a: ldr.w r0,[r0,r8,lsl #0x2]
  000b3c6e: ldrd r1,r2,[r0,#0x58]
  000b3c72: vldr.32 s0,[r0,#0xb0]
  000b3c76: mov r0,r6
  000b3c78: vstr.32 s0,[sp]
  000b3c7c: blx 0x00072fc4
  000b3c80: ldr r0,[r6,#0x0]
  000b3c82: add.w r8,r8,#0x1
  000b3c86: ldr r0,[r0,#0x4]
  000b3c88: ldr.w r0,[r0,r5,lsl #0x2]
  000b3c8c: ldr r1,[r0,#0x0]
  000b3c8e: cmp r8,r1
  000b3c90: bcc 0x000b3bbc
  000b3c92: ldr r0,[sp,#0xa4]
  000b3c94: ldr r1,[sp,#0x3c]
  000b3c96: ldr r1,[r1,#0x0]
  000b3c98: subs r0,r1,r0
  000b3c9a: itttt eq
  000b3c9c: mov.eq r0,#0x1
  000b3c9e: add.eq sp,#0xa8
  000b3ca0: vpop.eq {d8}
  000b3ca4: add.eq sp,#0x4
  000b3ca6: itt eq
  000b3ca8: pop.eq.w {r8,r9,r10,r11}
  000b3cac: pop.eq {r4,r5,r6,r7,pc}
  000b3cae: blx 0x0006e824
```
