# PlayerEgo::down
elf 0xa0b90 body 560
Sig: undefined __stdcall down(int param_1, float param_2)

## decompile
```c

/* PlayerEgo::down(int, float) */

float PlayerEgo::down(int param_1,float param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 in_r1;
  float in_r2;
  uint in_fpscr;
  uint uVar3;
  float fVar4;
  float in_s1;
  float fVar5;
  float fVar6;
  
  fVar6 = DAT_000b0dc0;
  if (*(int *)(param_1 + 0x1e4) != 0) {
    if (*(char *)(*(int *)(DAT_000b0dec + 0xb0bb0) + 0x10) == '\0') {
      fVar6 = (float)(*(code *)(DAT_001abb44 + 0x1abb48))();
      return fVar6;
    }
    fVar6 = (float)(*(code *)(DAT_001abb24 + 0x1abb28))(*(int *)(param_1 + 0x1e4),-in_r2);
    return fVar6;
  }
  if (*(char *)(param_1 + 0x1a0) == '\0') {
    if (*(int *)(param_1 + 0x194) == 0) {
      if ((*(char *)(param_1 + 0x158) == '\0') &&
         ((*(char *)(param_1 + 0x356) == '\0' || (*(int *)(param_1 + 0x1c4) == 1)))) {
        *(undefined4 *)(param_1 + 0x100) = 0xffffffff;
        if (*(char *)(param_1 + 0x235) == '\0') {
          fVar6 = *(float *)(param_1 + 0x154);
        }
        else {
          Status::getShip();
          uVar1 = Ship::getCurrentLoad();
          fVar4 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
          Status::getShip();
          uVar1 = Ship::getMaxLoad();
          fVar6 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
          fVar6 = *(float *)(param_1 + 0x154) * (1.0 - fVar4 / fVar6) * DAT_000b0dd4 +
                  *(float *)(param_1 + 0x154) * DAT_000b0dd8;
        }
        uVar1 = __aeabi_idiv((int)(in_r2 * DAT_000b0ddc * fVar6),0x3f);
        param_2 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(param_1 + 0x270) = in_r2;
        *(float *)(param_1 + 600) = -fVar6;
        if (*(float *)(param_1 + 0x278) < param_2) {
          fVar4 = DAT_000b0de0;
          if (**(int **)(DAT_000b0df4 + 0xb0d5e) == 0) {
            iVar2 = *(int *)(DAT_000b0df8 + 0xb0d72);
            fVar4 = *(float *)(iVar2 + 0x14);
            if (*(char *)(iVar2 + 0x11) == '\0') {
              fVar4 = *(float *)(iVar2 + 0x18) * DAT_000b0de4;
            }
            fVar4 = (DAT_000b0de8 - fVar4) * 20.0;
          }
          fVar5 = (float)VectorSignedToFloat(in_r1,(byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
          fVar6 = *(float *)(param_1 + 0x278) + (fVar6 * fVar5) / fVar4;
          if (param_2 < fVar6) {
            fVar6 = param_2;
          }
          *(float *)(param_1 + 0x278) = fVar6;
        }
      }
    }
    else {
      param_2 = in_r2 * DAT_000b0dd0 * *(float *)(*(int *)(param_1 + 0x194) + 0x50);
      *(float *)(param_1 + 0x7c) = param_2;
    }
  }
  else {
    uVar3 = in_fpscr & 0xfffffff;
    if (*(float *)(param_1 + 0x1a8) < DAT_000b0dc0) {
      fVar5 = (float)VectorSignedToFloat(in_r1,(byte)(uVar3 >> 0x16) & 3);
      fVar4 = fVar5 * in_r2 + *(float *)(param_1 + 0x1a8);
      *(float *)(param_1 + 0x1a8) = fVar4;
      AEGeometry::rotate(fVar4,in_s1,fVar5 * in_r2);
    }
    param_2 = *(float *)(param_1 + 0x1a4);
    if (param_2 < fVar6) {
      fVar6 = (float)VectorSignedToFloat(in_r1,(byte)((uVar3 & 0xfffffff) >> 0x16) & 3);
      fVar6 = fVar6 * in_r2 * 0.5;
      fVar4 = fVar6 * DAT_000b0dc4 * DAT_000b0dcc;
      *(float *)(param_1 + 0x1a4) = fVar6 + param_2;
      fVar6 = (float)(*(code *)(DAT_001abb34 + 0x1abb38))
                               (*(undefined4 *)(param_1 + 0x19c),fVar4,0,0);
      return fVar6;
    }
  }
  return param_2;
}

```

## target disasm
```
  000b0b90: push {r4,r5,r6,r7,lr}
  000b0b92: add r7,sp,#0xc
  000b0b94: push.w r11
  000b0b98: vpush {d8,d9}
  000b0b9c: mov r4,r0
  000b0b9e: ldr.w r0,[r0,#0x1e4]
  000b0ba2: vmov s16,r2
  000b0ba6: mov r5,r1
  000b0ba8: cbz r0,0x000b0bca
  000b0baa: ldr r1,[0x000b0dec]
  000b0bac: add r1,pc
  000b0bae: ldr r1,[r1,#0x0]
  000b0bb0: ldrb r1,[r1,#0x10]
  000b0bb2: cmp r1,#0x0
  000b0bb4: beq 0x000b0c6c
  000b0bb6: eor r1,r2,#0x80000000
  000b0bba: vpop {d8,d9}
  000b0bbe: pop.w r11
  000b0bc2: pop.w {r4,r5,r6,r7,lr}
  000b0bc6: b.w 0x001abb18
  000b0bca: ldrb.w r0,[r4,#0x1a0]
  000b0bce: cmp r0,#0x0
  000b0bd0: beq 0x000b0c7e
  000b0bd2: vldr.32 s18,[pc,#0x1ec]
  000b0bd6: vldr.32 s0,[r4,#0x1a8]
  000b0bda: vcmpe.f32 s0,s18
  000b0bde: vmrs apsr,fpscr
  000b0be2: bpl 0x000b0c16
  000b0be4: vmov s2,r5
  000b0be8: vldr.32 s4,[pc,#0x1d8]
  000b0bec: vldr.32 s6,[pc,#0x1d8]
  000b0bf0: movs r2,#0x0
  000b0bf2: vcvt.f32.s32 s2,s2
  000b0bf6: ldr r0,[r4,#0x28]
  000b0bf8: movs r3,#0x0
  000b0bfa: vmul.f32 s2,s2,s16
  000b0bfe: vmul.f32 s4,s2,s4
  000b0c02: vadd.f32 s0,s2,s0
  000b0c06: vmul.f32 s4,s4,s6
  000b0c0a: vstr.32 s0,[r4,#0x1a8]
  000b0c0e: vmov r1,s4
  000b0c12: blx 0x00072160
  000b0c16: vldr.32 s0,[r4,#0x1a4]
  000b0c1a: vcmpe.f32 s0,s18
  000b0c1e: vmrs apsr,fpscr
  000b0c22: bpl 0x000b0ca0
  000b0c24: vmov s2,r5
  000b0c28: vldr.32 s6,[pc,#0x1a0]
  000b0c2c: vmov.f32 s4,0x3f000000
  000b0c30: movs r2,#0x0
  000b0c32: vcvt.f32.s32 s2,s2
  000b0c36: ldr.w r0,[r4,#0x19c]
  000b0c3a: movs r3,#0x0
  000b0c3c: vmul.f32 s2,s2,s16
  000b0c40: vmul.f32 s2,s2,s4
  000b0c44: vldr.32 s4,[pc,#0x17c]
  000b0c48: vmul.f32 s4,s2,s4
  000b0c4c: vadd.f32 s0,s2,s0
  000b0c50: vmul.f32 s4,s4,s6
  000b0c54: vstr.32 s0,[r4,#0x1a4]
  000b0c58: vmov r1,s4
  000b0c5c: vpop {d8,d9}
  000b0c60: pop.w r11
  000b0c64: pop.w {r4,r5,r6,r7,lr}
  000b0c68: b.w 0x001abb28
  000b0c6c: mov r1,r2
  000b0c6e: vpop {d8,d9}
  000b0c72: pop.w r11
  000b0c76: pop.w {r4,r5,r6,r7,lr}
  000b0c7a: b.w 0x001abb38
  000b0c7e: ldr.w r0,[r4,#0x194]
  000b0c82: cbz r0,0x000b0c9a
  000b0c84: vldr.32 s0,[pc,#0x148]
  000b0c88: vldr.32 s2,[r0,#0x50]
  000b0c8c: vmul.f32 s0,s16,s0
  000b0c90: vmul.f32 s0,s0,s2
  000b0c94: vstr.32 s0,[r4,#0x7c]
  000b0c98: b 0x000b0ca0
  000b0c9a: ldrb.w r0,[r4,#0x158]
  000b0c9e: cbz r0,0x000b0caa
  000b0ca0: vpop {d8,d9}
  000b0ca4: pop.w r11
  000b0ca8: pop {r4,r5,r6,r7,pc}
  000b0caa: ldrb.w r0,[r4,#0x356]
  000b0cae: cbz r0,0x000b0cb8
  000b0cb0: ldr.w r0,[r4,#0x1c4]
  000b0cb4: cmp r0,#0x1
  000b0cb6: bne 0x000b0ca0
  000b0cb8: ldrb.w r0,[r4,#0x235]
  000b0cbc: mov.w r1,#0xffffffff
  000b0cc0: str.w r1,[r4,#0x100]
  000b0cc4: cbz r0,0x000b0d18
  000b0cc6: ldr r0,[0x000b0df0]
  000b0cc8: add r0,pc
  000b0cca: ldr r6,[r0,#0x0]
  000b0ccc: ldr r0,[r6,#0x0]
  000b0cce: blx 0x00071a58
  000b0cd2: blx 0x00072bf8
  000b0cd6: ldr r1,[r6,#0x0]
  000b0cd8: vmov s0,r0
  000b0cdc: vcvt.f32.s32 s18,s0
  000b0ce0: mov r0,r1
  000b0ce2: blx 0x00071a58
  000b0ce6: blx 0x00072c04
  000b0cea: vmov s0,r0
  000b0cee: vldr.32 s4,[pc,#0xe4]
  000b0cf2: vmov.f32 s2,0x3f800000
  000b0cf6: vcvt.f32.s32 s0,s0
  000b0cfa: vdiv.f32 s0,s18,s0
  000b0cfe: vsub.f32 s0,s2,s0
  000b0d02: vldr.32 s2,[r4,#0x154]
  000b0d06: vmul.f32 s0,s2,s0
  000b0d0a: vmul.f32 s18,s0,s4
  000b0d0e: vldr.32 s0,[pc,#0xc8]
  000b0d12: vmla.f32 s18,s2,s0
  000b0d16: b 0x000b0d1c
  000b0d18: vldr.32 s18,[r4,#0x154]
  000b0d1c: vldr.32 s0,[pc,#0xbc]
  000b0d20: movs r1,#0x3f
  000b0d22: vmul.f32 s0,s16,s0
  000b0d26: vmul.f32 s0,s0,s18
  000b0d2a: vcvt.s32.f32 s0,s0
  000b0d2e: vmov r0,s0
  000b0d32: blx 0x0007198c
  000b0d36: vmov s0,r0
  000b0d3a: vneg.f32 s4,s18
  000b0d3e: vcvt.f32.s32 s0,s0
  000b0d42: vldr.32 s2,[r4,#0x278]
  000b0d46: vstr.32 s16,[r4,#0x270]
  000b0d4a: vstr.32 s4,[r4,#0x258]
  000b0d4e: vcmpe.f32 s2,s0
  000b0d52: vmrs apsr,fpscr
  000b0d56: bpl 0x000b0ca0
  000b0d58: ldr r0,[0x000b0df4]
  000b0d5a: add r0,pc
  000b0d5c: ldr r0,[r0,#0x0]
  000b0d5e: ldr r0,[r0,#0x0]
  000b0d60: cbz r0,0x000b0d68
  000b0d62: vldr.32 s4,[pc,#0x7c]
  000b0d66: b 0x000b0d98
  000b0d68: ldr r0,[0x000b0df8]
  000b0d6a: vldr.32 s4,[pc,#0x78]
  000b0d6e: add r0,pc
  000b0d70: ldr r0,[r0,#0x0]
  000b0d72: vldr.32 s8,[r0,#0x18]
  000b0d76: vldr.32 s6,[r0,#0x14]
  000b0d7a: vmul.f32 s4,s8,s4
  000b0d7e: ldrb r0,[r0,#0x11]
  000b0d80: vmov.f32 s8,0x41a00000
  000b0d84: cmp r0,#0x0
  000b0d86: it eq
  000b0d88: vmov.eq.f32 s6,s4
  000b0d8c: vldr.32 s4,[pc,#0x58]
  000b0d90: vsub.f32 s4,s4,s6
  000b0d94: vmul.f32 s4,s4,s8
  000b0d98: vmov s6,r5
  000b0d9c: vcvt.f32.s32 s6,s6
  000b0da0: vmul.f32 s6,s18,s6
  000b0da4: vdiv.f32 s4,s6,s4
  000b0da8: vadd.f32 s2,s2,s4
  000b0dac: vcmpe.f32 s2,s0
  000b0db0: vmrs apsr,fpscr
  000b0db4: it gt
  000b0db6: vmov.gt.f32 s2,s0
  000b0dba: vstr.32 s2,[r4,#0x278]
  000b0dbe: b 0x000b0ca0
```
