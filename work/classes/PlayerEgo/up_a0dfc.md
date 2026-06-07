# PlayerEgo::up
elf 0xa0dfc body 566
Sig: undefined __stdcall up(int param_1, float param_2)

## decompile
```c

/* PlayerEgo::up(int, float) */

float PlayerEgo::up(int param_1,float param_2)

{
  uint uVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 in_r1;
  float in_r2;
  uint in_fpscr;
  uint uVar5;
  float fVar6;
  float in_s1;
  float fVar7;
  float fVar8;
  float fVar9;
  
  if (*(int *)(param_1 + 0x1e4) != 0) {
    if (*(char *)(*(int *)(DAT_000b105c + 0xb0e1c) + 0x10) == '\0') {
      fVar6 = (float)(*(code *)(DAT_001abb24 + 0x1abb28))(*(int *)(param_1 + 0x1e4),-in_r2);
      return fVar6;
    }
    fVar6 = (float)(*(code *)(DAT_001abb44 + 0x1abb48))();
    return fVar6;
  }
  if (*(char *)(param_1 + 0x1a0) == '\0') {
    if (*(int *)(param_1 + 0x194) == 0) {
      if ((*(char *)(param_1 + 0x158) == '\0') &&
         ((*(char *)(param_1 + 0x356) == '\0' || (*(int *)(param_1 + 0x1c4) == 1)))) {
        *(undefined4 *)(param_1 + 0x100) = 1;
        if (*(char *)(param_1 + 0x235) == '\0') {
          fVar6 = *(float *)(param_1 + 0x154);
        }
        else {
          Status::getShip();
          uVar3 = Ship::getCurrentLoad();
          fVar7 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
          Status::getShip();
          uVar3 = Ship::getMaxLoad();
          fVar6 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
          fVar6 = *(float *)(param_1 + 0x154) * (1.0 - fVar7 / fVar6) * DAT_000b1048 +
                  *(float *)(param_1 + 0x154) * DAT_000b104c;
        }
        uVar3 = __aeabi_idiv((int)(in_r2 * DAT_000b1050 * fVar6),0x3f);
        param_2 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
        fVar7 = *(float *)(param_1 + 0x278);
        *(float *)(param_1 + 600) = fVar6;
        *(float *)(param_1 + 0x270) = -in_r2;
        uVar1 = in_fpscr & 0xfffffff | (uint)(fVar7 < param_2) << 0x1f |
                (uint)(fVar7 == param_2) << 0x1e;
        bVar2 = (byte)(uVar1 >> 0x18);
        if (!(bool)(bVar2 >> 6 & 1) && (bool)(bVar2 >> 7) == (NAN(fVar7) || NAN(param_2))) {
          fVar8 = DAT_000b1054;
          if (**(int **)(DAT_000b1064 + 0xb0fcc) == 0) {
            iVar4 = *(int *)(DAT_000b1068 + 0xb0fe0);
            fVar8 = *(float *)(iVar4 + 0x14);
            if (*(char *)(iVar4 + 0x11) == '\0') {
              fVar8 = *(float *)(iVar4 + 0x18) * 1.25;
            }
            fVar8 = (DAT_000b1058 - fVar8) * 20.0;
          }
          fVar9 = (float)VectorSignedToFloat(in_r1,(byte)(uVar1 >> 0x16) & 3);
          fVar7 = fVar7 - (fVar6 * fVar9) / fVar8;
          if ((int)((uint)(fVar7 < param_2) << 0x1f) < 0) {
            fVar7 = param_2;
          }
          *(float *)(param_1 + 0x278) = fVar7;
        }
      }
    }
    else {
      param_2 = in_r2 * DAT_000b1044 * *(float *)(*(int *)(param_1 + 0x194) + 0x50);
      *(float *)(param_1 + 0x7c) = param_2;
    }
  }
  else {
    fVar6 = *(float *)(param_1 + 0x1a8);
    uVar1 = in_fpscr & 0xfffffff | (uint)(fVar6 < DAT_000b1030) << 0x1f |
            (uint)(fVar6 == DAT_000b1030) << 0x1e;
    uVar5 = uVar1 | (uint)(NAN(fVar6) || NAN(DAT_000b1030)) << 0x1c;
    bVar2 = (byte)(uVar1 >> 0x18);
    if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar5 >> 0x1c) & 1)) {
      fVar7 = (float)VectorSignedToFloat(in_r1,(byte)(uVar5 >> 0x16) & 3);
      fVar6 = fVar6 - fVar7 * in_r2;
      *(float *)(param_1 + 0x1a8) = fVar6;
      AEGeometry::rotate(fVar6,in_s1,fVar7 * in_r2);
    }
    param_2 = *(float *)(param_1 + 0x1a4);
    uVar1 = uVar5 & 0xfffffff | (uint)(param_2 < DAT_000b103c) << 0x1f |
            (uint)(param_2 == DAT_000b103c) << 0x1e;
    bVar2 = (byte)(uVar1 >> 0x18);
    if (!(bool)(bVar2 >> 6 & 1) && (bool)(bVar2 >> 7) == (NAN(param_2) || NAN(DAT_000b103c))) {
      fVar6 = (float)VectorSignedToFloat(in_r1,(byte)(uVar1 >> 0x16) & 3);
      fVar6 = fVar6 * in_r2 * 0.5;
      fVar7 = fVar6 * DAT_000b1034 * DAT_000b1040;
      *(float *)(param_1 + 0x1a4) = param_2 - fVar6;
      fVar6 = (float)(*(code *)(DAT_001abb34 + 0x1abb38))
                               (*(undefined4 *)(param_1 + 0x19c),fVar7,0,0);
      return fVar6;
    }
  }
  return param_2;
}

```

## target disasm
```
  000b0dfc: push {r4,r5,r6,r7,lr}
  000b0dfe: add r7,sp,#0xc
  000b0e00: push.w r11
  000b0e04: vpush {d8,d9}
  000b0e08: mov r4,r0
  000b0e0a: ldr.w r0,[r0,#0x1e4]
  000b0e0e: vmov s16,r2
  000b0e12: mov r5,r1
  000b0e14: cbz r0,0x000b0e34
  000b0e16: ldr r1,[0x000b105c]
  000b0e18: add r1,pc
  000b0e1a: ldr r1,[r1,#0x0]
  000b0e1c: ldrb r1,[r1,#0x10]
  000b0e1e: cmp r1,#0x0
  000b0e20: beq 0x000b0eda
  000b0e22: mov r1,r2
  000b0e24: vpop {d8,d9}
  000b0e28: pop.w r11
  000b0e2c: pop.w {r4,r5,r6,r7,lr}
  000b0e30: b.w 0x001abb38
  000b0e34: ldrb.w r0,[r4,#0x1a0]
  000b0e38: cmp r0,#0x0
  000b0e3a: beq 0x000b0eee
  000b0e3c: vldr.32 s2,[pc,#0x1f0]
  000b0e40: vldr.32 s0,[r4,#0x1a8]
  000b0e44: vcmpe.f32 s0,s2
  000b0e48: vmrs apsr,fpscr
  000b0e4c: ble 0x000b0e80
  000b0e4e: vmov s2,r5
  000b0e52: vldr.32 s4,[pc,#0x1e0]
  000b0e56: vldr.32 s6,[pc,#0x1e0]
  000b0e5a: movs r2,#0x0
  000b0e5c: vcvt.f32.s32 s2,s2
  000b0e60: ldr r0,[r4,#0x28]
  000b0e62: movs r3,#0x0
  000b0e64: vmul.f32 s2,s2,s16
  000b0e68: vmul.f32 s4,s2,s4
  000b0e6c: vsub.f32 s0,s0,s2
  000b0e70: vmul.f32 s4,s4,s6
  000b0e74: vstr.32 s0,[r4,#0x1a8]
  000b0e78: vmov r1,s4
  000b0e7c: blx 0x00072160
  000b0e80: vldr.32 s2,[pc,#0x1b8]
  000b0e84: vldr.32 s0,[r4,#0x1a4]
  000b0e88: vcmpe.f32 s0,s2
  000b0e8c: vmrs apsr,fpscr
  000b0e90: ble 0x000b0f10
  000b0e92: vmov s2,r5
  000b0e96: vldr.32 s6,[pc,#0x1a8]
  000b0e9a: vmov.f32 s4,0x3f000000
  000b0e9e: movs r2,#0x0
  000b0ea0: vcvt.f32.s32 s2,s2
  000b0ea4: ldr.w r0,[r4,#0x19c]
  000b0ea8: movs r3,#0x0
  000b0eaa: vmul.f32 s2,s2,s16
  000b0eae: vmul.f32 s2,s2,s4
  000b0eb2: vldr.32 s4,[pc,#0x180]
  000b0eb6: vmul.f32 s4,s2,s4
  000b0eba: vsub.f32 s0,s0,s2
  000b0ebe: vmul.f32 s4,s4,s6
  000b0ec2: vstr.32 s0,[r4,#0x1a4]
  000b0ec6: vmov r1,s4
  000b0eca: vpop {d8,d9}
  000b0ece: pop.w r11
  000b0ed2: pop.w {r4,r5,r6,r7,lr}
  000b0ed6: b.w 0x001abb28
  000b0eda: eor r1,r2,#0x80000000
  000b0ede: vpop {d8,d9}
  000b0ee2: pop.w r11
  000b0ee6: pop.w {r4,r5,r6,r7,lr}
  000b0eea: b.w 0x001abb18
  000b0eee: ldr.w r0,[r4,#0x194]
  000b0ef2: cbz r0,0x000b0f0a
  000b0ef4: vldr.32 s0,[pc,#0x14c]
  000b0ef8: vldr.32 s2,[r0,#0x50]
  000b0efc: vmul.f32 s0,s16,s0
  000b0f00: vmul.f32 s0,s0,s2
  000b0f04: vstr.32 s0,[r4,#0x7c]
  000b0f08: b 0x000b0f10
  000b0f0a: ldrb.w r0,[r4,#0x158]
  000b0f0e: cbz r0,0x000b0f1a
  000b0f10: vpop {d8,d9}
  000b0f14: pop.w r11
  000b0f18: pop {r4,r5,r6,r7,pc}
  000b0f1a: ldrb.w r0,[r4,#0x356]
  000b0f1e: cbz r0,0x000b0f28
  000b0f20: ldr.w r0,[r4,#0x1c4]
  000b0f24: cmp r0,#0x1
  000b0f26: bne 0x000b0f10
  000b0f28: ldrb.w r0,[r4,#0x235]
  000b0f2c: movs r1,#0x1
  000b0f2e: str.w r1,[r4,#0x100]
  000b0f32: cbz r0,0x000b0f86
  000b0f34: ldr r0,[0x000b1060]
  000b0f36: add r0,pc
  000b0f38: ldr r6,[r0,#0x0]
  000b0f3a: ldr r0,[r6,#0x0]
  000b0f3c: blx 0x00071a58
  000b0f40: blx 0x00072bf8
  000b0f44: ldr r1,[r6,#0x0]
  000b0f46: vmov s0,r0
  000b0f4a: vcvt.f32.s32 s18,s0
  000b0f4e: mov r0,r1
  000b0f50: blx 0x00071a58
  000b0f54: blx 0x00072c04
  000b0f58: vmov s0,r0
  000b0f5c: vldr.32 s4,[pc,#0xe8]
  000b0f60: vmov.f32 s2,0x3f800000
  000b0f64: vcvt.f32.s32 s0,s0
  000b0f68: vdiv.f32 s0,s18,s0
  000b0f6c: vsub.f32 s0,s2,s0
  000b0f70: vldr.32 s2,[r4,#0x154]
  000b0f74: vmul.f32 s0,s2,s0
  000b0f78: vmul.f32 s18,s0,s4
  000b0f7c: vldr.32 s0,[pc,#0xcc]
  000b0f80: vmla.f32 s18,s2,s0
  000b0f84: b 0x000b0f8a
  000b0f86: vldr.32 s18,[r4,#0x154]
  000b0f8a: vldr.32 s0,[pc,#0xc4]
  000b0f8e: movs r1,#0x3f
  000b0f90: vmul.f32 s0,s16,s0
  000b0f94: vmul.f32 s0,s0,s18
  000b0f98: vcvt.s32.f32 s0,s0
  000b0f9c: vmov r0,s0
  000b0fa0: blx 0x0007198c
  000b0fa4: vmov s0,r0
  000b0fa8: vneg.f32 s4,s16
  000b0fac: vcvt.f32.s32 s0,s0
  000b0fb0: vldr.32 s2,[r4,#0x278]
  000b0fb4: vstr.32 s18,[r4,#0x258]
  000b0fb8: vstr.32 s4,[r4,#0x270]
  000b0fbc: vcmpe.f32 s2,s0
  000b0fc0: vmrs apsr,fpscr
  000b0fc4: ble 0x000b0f10
  000b0fc6: ldr r0,[0x000b1064]
  000b0fc8: add r0,pc
  000b0fca: ldr r0,[r0,#0x0]
  000b0fcc: ldr r0,[r0,#0x0]
  000b0fce: cbz r0,0x000b0fd6
  000b0fd0: vldr.32 s4,[pc,#0x80]
  000b0fd4: b 0x000b1006
  000b0fd6: ldr r0,[0x000b1068]
  000b0fd8: vmov.f32 s4,0x3fa00000
  000b0fdc: add r0,pc
  000b0fde: ldr r0,[r0,#0x0]
  000b0fe0: vldr.32 s8,[r0,#0x18]
  000b0fe4: vldr.32 s6,[r0,#0x14]
  000b0fe8: vmul.f32 s4,s8,s4
  000b0fec: ldrb r0,[r0,#0x11]
  000b0fee: vmov.f32 s8,0x41a00000
  000b0ff2: cmp r0,#0x0
  000b0ff4: it eq
  000b0ff6: vmov.eq.f32 s6,s4
  000b0ffa: vldr.32 s4,[pc,#0x5c]
  000b0ffe: vsub.f32 s4,s4,s6
  000b1002: vmul.f32 s4,s4,s8
  000b1006: vmov s6,r5
  000b100a: vcvt.f32.s32 s6,s6
  000b100e: vmul.f32 s6,s18,s6
  000b1012: vdiv.f32 s4,s6,s4
  000b1016: vsub.f32 s2,s2,s4
  000b101a: vcmpe.f32 s2,s0
  000b101e: vmrs apsr,fpscr
  000b1022: it mi
  000b1024: vmov.mi.f32 s2,s0
  000b1028: vstr.32 s2,[r4,#0x278]
  000b102c: b 0x000b0f10
  001abb18: bx pc
  001abb38: bx pc
```
