# PlayerEgo::left
elf 0xa094c body 520
Sig: undefined __stdcall left(int param_1, float param_2)

## decompile
```c

/* PlayerEgo::left(int, float) */

float PlayerEgo::left(int param_1,float param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 in_r1;
  float in_r2;
  float *pfVar3;
  code *UNRECOVERED_JUMPTABLE;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  undefined4 extraout_s5;
  undefined8 uVar6;
  undefined4 extraout_s7;
  
  if (*(int *)(param_1 + 0x1e4) != 0) {
    fVar4 = (float)(*(code *)(DAT_001abb14 + 0x1abb18))(*(int *)(param_1 + 0x1e4),-in_r2);
    return fVar4;
  }
  if (*(char *)(param_1 + 0x1a0) == '\0') {
    if (*(int *)(param_1 + 0x194) == 0) {
      if ((*(char *)(param_1 + 0x158) == '\0') &&
         ((*(char *)(param_1 + 0x356) == '\0' || (*(int *)(param_1 + 0x1c4) == 1)))) {
        *(undefined4 *)(param_1 + 0x104) = 1;
        if (*(char *)(param_1 + 0x235) == '\0') {
          fVar4 = *(float *)(param_1 + 0x154);
        }
        else {
          Status::getShip();
          uVar1 = Ship::getCurrentLoad();
          fVar5 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
          Status::getShip();
          uVar1 = Ship::getMaxLoad();
          fVar4 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
          fVar4 = *(float *)(param_1 + 0x154) * (1.0 - fVar5 / fVar4) * DAT_000b0b68 +
                  *(float *)(param_1 + 0x154) * DAT_000b0b6c;
        }
        uVar1 = __aeabi_idiv((int)(in_r2 * DAT_000b0b70 * fVar4),0x3f);
        param_2 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(param_1 + 0x25c) = fVar4;
        *(float *)(param_1 + 0x268) = -in_r2;
        if (*(float *)(param_1 + 0x27c) < param_2) {
          if (**(int **)(DAT_000b0b84 + 0xb0ae4) == 0) {
            iVar2 = *(int *)(DAT_000b0b88 + 0xb0b14);
            pfVar3 = (float *)(iVar2 + 0x14);
            if (*(char *)(iVar2 + 0x11) == '\0') {
              pfVar3 = (float *)(iVar2 + 0x18);
            }
            fVar5 = (float)VectorSignedToFloat(in_r1,(byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
            fVar4 = (fVar4 * fVar5) / ((DAT_000b0b7c - *pfVar3) * 20.0);
          }
          else {
            uVar6 = FloatVectorMin(CONCAT44(extraout_s5,(fVar4 * 25.0) / DAT_000b0b74),
                                   CONCAT44(extraout_s7,DAT_000b0b78),2,0x20);
            fVar4 = (float)uVar6;
          }
          fVar4 = *(float *)(param_1 + 0x27c) + fVar4;
          if (param_2 < fVar4) {
            fVar4 = param_2;
          }
          *(float *)(param_1 + 0x27c) = fVar4;
        }
      }
    }
    else {
      fVar4 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
      param_2 = fVar4 * in_r2;
      fVar4 = param_2 * DAT_000b0b64;
      *(float *)(param_1 + 0x80) =
           in_r2 * DAT_000b0b60 * *(float *)(*(int *)(param_1 + 0x194) + 0x50);
      *(float *)(param_1 + 0x198) = *(float *)(param_1 + 0x198) + fVar4;
    }
    return param_2;
  }
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x1f8),(byte)(in_fpscr >> 0x16) & 3);
  fVar5 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
  UNRECOVERED_JUMPTABLE = *(code **)(DAT_000b0b8c + 0xb09a4);
  fVar4 = ((fVar5 * in_r2) / (DAT_000b0b54 / fVar4)) * DAT_000b0b58 * DAT_000b0b5c;
  (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(param_1 + 0x178),0,fVar4,0);
  (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(param_1 + 0xdc),0,fVar4,0);
                    /* WARNING: Could not recover jumptable at 0x000b09e8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  fVar4 = (float)(*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(param_1 + 0x28),0,fVar4,0);
  return fVar4;
}

```

## target disasm
```
  000b094c: push {r4,r5,r6,r7,lr}
  000b094e: add r7,sp,#0xc
  000b0950: push.w r11
  000b0954: vpush {d8,d9}
  000b0958: mov r4,r0
  000b095a: ldr.w r0,[r0,#0x1e4]
  000b095e: vmov s16,r2
  000b0962: mov r5,r1
  000b0964: cbz r0,0x000b097a
  000b0966: eor r1,r2,#0x80000000
  000b096a: vpop {d8,d9}
  000b096e: pop.w r11
  000b0972: pop.w {r4,r5,r6,r7,lr}
  000b0976: b.w 0x001abb08
  000b097a: ldrb.w r0,[r4,#0x1a0]
  000b097e: cbz r0,0x000b09ea
  000b0980: vldr.32 s0,[r4,#0x1f8]
  000b0984: movs r3,#0x0
  000b0986: vldr.32 s2,[pc,#0x1cc]
  000b098a: vcvt.f32.s32 s0,s0
  000b098e: vdiv.f32 s0,s2,s0
  000b0992: vmov s2,r5
  000b0996: vcvt.f32.s32 s2,s2
  000b099a: ldr r1,[0x000b0b8c]
  000b099c: ldr.w r0,[r4,#0x178]
  000b09a0: add r1,pc
  000b09a2: ldr r6,[r1,#0x0]
  000b09a4: movs r1,#0x0
  000b09a6: vmul.f32 s2,s2,s16
  000b09aa: vdiv.f32 s0,s2,s0
  000b09ae: vldr.32 s2,[pc,#0x1a8]
  000b09b2: vmul.f32 s0,s0,s2
  000b09b6: vldr.32 s2,[pc,#0x1a4]
  000b09ba: vmul.f32 s0,s0,s2
  000b09be: vmov r5,s0
  000b09c2: mov r2,r5
  000b09c4: blx r6
  000b09c6: ldr.w r0,[r4,#0xdc]
  000b09ca: movs r1,#0x0
  000b09cc: mov r2,r5
  000b09ce: movs r3,#0x0
  000b09d0: blx r6
  000b09d2: ldr r0,[r4,#0x28]
  000b09d4: movs r1,#0x0
  000b09d6: mov r2,r5
  000b09d8: movs r3,#0x0
  000b09da: mov r12,r6
  000b09dc: vpop {d8,d9}
  000b09e0: pop.w r11
  000b09e4: pop.w {r4,r5,r6,r7,lr}
  000b09e8: bx r12
  000b09ea: ldr.w r0,[r4,#0x194]
  000b09ee: cbz r0,0x000b0a22
  000b09f0: vmov s0,r5
  000b09f4: vldr.32 s2,[pc,#0x168]
  000b09f8: vldr.32 s6,[pc,#0x168]
  000b09fc: vcvt.f32.s32 s0,s0
  000b0a00: vldr.32 s4,[r0,#0x50]
  000b0a04: vmul.f32 s2,s16,s2
  000b0a08: vldr.32 s8,[r4,#0x198]
  000b0a0c: vmul.f32 s0,s0,s16
  000b0a10: vmul.f32 s2,s2,s4
  000b0a14: vmla.f32 s8,s0,s6
  000b0a18: vstr.32 s2,[r4,#0x80]
  000b0a1c: vstr.32 s8,[r4,#0x198]
  000b0a20: b 0x000b0a28
  000b0a22: ldrb.w r0,[r4,#0x158]
  000b0a26: cbz r0,0x000b0a32
  000b0a28: vpop {d8,d9}
  000b0a2c: pop.w r11
  000b0a30: pop {r4,r5,r6,r7,pc}
  000b0a32: ldrb.w r0,[r4,#0x356]
  000b0a36: cbz r0,0x000b0a40
  000b0a38: ldr.w r0,[r4,#0x1c4]
  000b0a3c: cmp r0,#0x1
  000b0a3e: bne 0x000b0a28
  000b0a40: ldrb.w r0,[r4,#0x235]
  000b0a44: movs r1,#0x1
  000b0a46: str.w r1,[r4,#0x104]
  000b0a4a: cbz r0,0x000b0a9e
  000b0a4c: ldr r0,[0x000b0b80]
  000b0a4e: add r0,pc
  000b0a50: ldr r6,[r0,#0x0]
  000b0a52: ldr r0,[r6,#0x0]
  000b0a54: blx 0x00071a58
  000b0a58: blx 0x00072bf8
  000b0a5c: ldr r1,[r6,#0x0]
  000b0a5e: vmov s0,r0
  000b0a62: vcvt.f32.s32 s18,s0
  000b0a66: mov r0,r1
  000b0a68: blx 0x00071a58
  000b0a6c: blx 0x00072c04
  000b0a70: vmov s0,r0
  000b0a74: vldr.32 s4,[pc,#0xf0]
  000b0a78: vmov.f32 s2,0x3f800000
  000b0a7c: vcvt.f32.s32 s0,s0
  000b0a80: vdiv.f32 s0,s18,s0
  000b0a84: vsub.f32 s0,s2,s0
  000b0a88: vldr.32 s2,[r4,#0x154]
  000b0a8c: vmul.f32 s0,s2,s0
  000b0a90: vmul.f32 s18,s0,s4
  000b0a94: vldr.32 s0,[pc,#0xd4]
  000b0a98: vmla.f32 s18,s2,s0
  000b0a9c: b 0x000b0aa2
  000b0a9e: vldr.32 s18,[r4,#0x154]
  000b0aa2: vldr.32 s0,[pc,#0xcc]
  000b0aa6: movs r1,#0x3f
  000b0aa8: vmul.f32 s0,s16,s0
  000b0aac: vmul.f32 s0,s0,s18
  000b0ab0: vcvt.s32.f32 s0,s0
  000b0ab4: vmov r0,s0
  000b0ab8: blx 0x0007198c
  000b0abc: vmov s0,r0
  000b0ac0: vneg.f32 s4,s16
  000b0ac4: vcvt.f32.s32 s0,s0
  000b0ac8: vldr.32 s2,[r4,#0x27c]
  000b0acc: vstr.32 s18,[r4,#0x25c]
  000b0ad0: vstr.32 s4,[r4,#0x268]
  000b0ad4: vcmpe.f32 s2,s0
  000b0ad8: vmrs apsr,fpscr
  000b0adc: bpl 0x000b0a28
  000b0ade: ldr r0,[0x000b0b84]
  000b0ae0: add r0,pc
  000b0ae2: ldr r0,[r0,#0x0]
  000b0ae4: ldr r0,[r0,#0x0]
  000b0ae6: cbz r0,0x000b0b02
  000b0ae8: vmov.f32 s4,0x41c80000
  000b0aec: vldr.32 s6,[pc,#0x84]
  000b0af0: vmul.f32 s4,s18,s4
  000b0af4: vdiv.f32 s4,s4,s6
  000b0af8: vldr.32 s6,[pc,#0x7c]
  000b0afc: vmin.f32 d2,d2,d3
  000b0b00: b 0x000b0b3a
  000b0b02: ldr r0,[0x000b0b88]
  000b0b04: vmov s6,r5
  000b0b08: vldr.32 s4,[pc,#0x70]
  000b0b0c: vmov.f32 s10,0x41a00000
  000b0b10: add r0,pc
  000b0b12: ldr r0,[r0,#0x0]
  000b0b14: add.w r2,r0,#0x14
  000b0b18: ldrb r1,[r0,#0x11]
  000b0b1a: cmp r1,#0x0
  000b0b1c: it eq
  000b0b1e: add.eq.w r2,r0,#0x18
  000b0b22: vldr.32 s8,[r2]
  000b0b26: vcvt.f32.s32 s6,s6
  000b0b2a: vsub.f32 s4,s4,s8
  000b0b2e: vmul.f32 s6,s18,s6
  000b0b32: vmul.f32 s4,s4,s10
  000b0b36: vdiv.f32 s4,s6,s4
  000b0b3a: vadd.f32 s2,s2,s4
  000b0b3e: vcmpe.f32 s2,s0
  000b0b42: vmrs apsr,fpscr
  000b0b46: it gt
  000b0b48: vmov.gt.f32 s2,s0
  000b0b4c: vstr.32 s2,[r4,#0x27c]
  000b0b50: b 0x000b0a28
  001abb08: bx pc
```
