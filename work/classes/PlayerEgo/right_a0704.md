# PlayerEgo::right
elf 0xa0704 body 524
Sig: undefined __stdcall right(int param_1, float param_2)

## decompile
```c

/* PlayerEgo::right(int, float) */

float PlayerEgo::right(int param_1,float param_2)

{
  uint uVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 in_r1;
  float in_r2;
  float *pfVar5;
  code *UNRECOVERED_JUMPTABLE;
  uint in_fpscr;
  float fVar6;
  float fVar7;
  undefined4 extraout_s5;
  undefined8 uVar8;
  undefined4 extraout_s7;
  float fVar9;
  
  if (*(int *)(param_1 + 0x1e4) != 0) {
    fVar6 = (float)(*(code *)(DAT_001abb04 + 0x1abb08))();
    return fVar6;
  }
  if (*(char *)(param_1 + 0x1a0) == '\0') {
    if (*(int *)(param_1 + 0x194) == 0) {
      if ((*(char *)(param_1 + 0x158) == '\0') &&
         ((*(char *)(param_1 + 0x356) == '\0' || (*(int *)(param_1 + 0x1c4) == 1)))) {
        *(undefined4 *)(param_1 + 0x104) = 0xffffffff;
        if (*(char *)(param_1 + 0x235) == '\0') {
          fVar7 = *(float *)(param_1 + 0x154);
          fVar6 = fVar7;
        }
        else {
          Status::getShip();
          uVar3 = Ship::getCurrentLoad();
          fVar9 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
          Status::getShip();
          uVar3 = Ship::getMaxLoad();
          fVar6 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
          fVar7 = *(float *)(param_1 + 0x154);
          fVar6 = fVar7 * (1.0 - fVar9 / fVar6) * DAT_000b0924 + fVar7 * DAT_000b0928;
        }
        uVar3 = __aeabi_idiv((int)(in_r2 * DAT_000b092c * fVar6),0x3f);
        param_2 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
        fVar9 = *(float *)(param_1 + 0x27c);
        *(float *)(param_1 + 0x268) = in_r2;
        *(float *)(param_1 + 0x25c) = -fVar7;
        uVar1 = in_fpscr & 0xfffffff | (uint)(fVar9 < param_2) << 0x1f |
                (uint)(fVar9 == param_2) << 0x1e;
        bVar2 = (byte)(uVar1 >> 0x18);
        if (!(bool)(bVar2 >> 6 & 1) && (bool)(bVar2 >> 7) == (NAN(fVar9) || NAN(param_2))) {
          if (**(int **)(DAT_000b0940 + 0xb08a0) == 0) {
            iVar4 = *(int *)(DAT_000b0944 + 0xb08d0);
            pfVar5 = (float *)(iVar4 + 0x14);
            if (*(char *)(iVar4 + 0x11) == '\0') {
              pfVar5 = (float *)(iVar4 + 0x18);
            }
            fVar7 = (float)VectorSignedToFloat(in_r1,(byte)(uVar1 >> 0x16) & 3);
            fVar6 = (fVar6 * fVar7) / ((DAT_000b0938 - *pfVar5) * 20.0);
          }
          else {
            uVar8 = FloatVectorMin(CONCAT44(extraout_s5,(fVar6 * 25.0) / DAT_000b0930),
                                   CONCAT44(extraout_s7,DAT_000b0934),2,0x20);
            fVar6 = (float)uVar8;
          }
          fVar7 = fVar9 - fVar6;
          if ((int)((uint)(fVar9 - fVar6 < param_2) << 0x1f) < 0) {
            fVar7 = param_2;
          }
          *(float *)(param_1 + 0x27c) = fVar7;
        }
      }
    }
    else {
      fVar6 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
      param_2 = fVar6 * in_r2;
      fVar6 = param_2 * DAT_000b0920;
      *(float *)(param_1 + 0x80) =
           in_r2 * DAT_000b091c * *(float *)(*(int *)(param_1 + 0x194) + 0x50);
      *(float *)(param_1 + 0x198) = *(float *)(param_1 + 0x198) + fVar6;
    }
    return param_2;
  }
  fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x1f8),(byte)(in_fpscr >> 0x16) & 3);
  fVar7 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
  UNRECOVERED_JUMPTABLE = *(code **)(DAT_000b0948 + 0xb075a);
  fVar6 = ((fVar7 * in_r2) / (DAT_000b0910 / fVar6)) * DAT_000b0914 * DAT_000b0918;
  (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(param_1 + 0x178),0,fVar6,0);
  (*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(param_1 + 0xdc),0,fVar6,0);
                    /* WARNING: Could not recover jumptable at 0x000b079e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  fVar6 = (float)(*UNRECOVERED_JUMPTABLE)(*(undefined4 *)(param_1 + 0x28),0,fVar6,0);
  return fVar6;
}

```

## target disasm
```
  000b0704: push {r4,r5,r6,r7,lr}
  000b0706: add r7,sp,#0xc
  000b0708: push.w r11
  000b070c: vpush {d8,d9,d10}
  000b0710: mov r4,r0
  000b0712: ldr.w r0,[r0,#0x1e4]
  000b0716: vmov s16,r2
  000b071a: mov r5,r1
  000b071c: cbz r0,0x000b0730
  000b071e: mov r1,r2
  000b0720: vpop {d8,d9,d10}
  000b0724: pop.w r11
  000b0728: pop.w {r4,r5,r6,r7,lr}
  000b072c: b.w 0x001abaf8
  000b0730: ldrb.w r0,[r4,#0x1a0]
  000b0734: cbz r0,0x000b07a0
  000b0736: vldr.32 s0,[r4,#0x1f8]
  000b073a: movs r3,#0x0
  000b073c: vldr.32 s2,[pc,#0x1d0]
  000b0740: vcvt.f32.s32 s0,s0
  000b0744: vdiv.f32 s0,s2,s0
  000b0748: vmov s2,r5
  000b074c: vcvt.f32.s32 s2,s2
  000b0750: ldr r1,[0x000b0948]
  000b0752: ldr.w r0,[r4,#0x178]
  000b0756: add r1,pc
  000b0758: ldr r6,[r1,#0x0]
  000b075a: movs r1,#0x0
  000b075c: vmul.f32 s2,s2,s16
  000b0760: vdiv.f32 s0,s2,s0
  000b0764: vldr.32 s2,[pc,#0x1ac]
  000b0768: vmul.f32 s0,s0,s2
  000b076c: vldr.32 s2,[pc,#0x1a8]
  000b0770: vmul.f32 s0,s0,s2
  000b0774: vmov r5,s0
  000b0778: mov r2,r5
  000b077a: blx r6
  000b077c: ldr.w r0,[r4,#0xdc]
  000b0780: movs r1,#0x0
  000b0782: mov r2,r5
  000b0784: movs r3,#0x0
  000b0786: blx r6
  000b0788: ldr r0,[r4,#0x28]
  000b078a: movs r1,#0x0
  000b078c: mov r2,r5
  000b078e: movs r3,#0x0
  000b0790: mov r12,r6
  000b0792: vpop {d8,d9,d10}
  000b0796: pop.w r11
  000b079a: pop.w {r4,r5,r6,r7,lr}
  000b079e: bx r12
  000b07a0: ldr.w r0,[r4,#0x194]
  000b07a4: cbz r0,0x000b07d8
  000b07a6: vmov s0,r5
  000b07aa: vldr.32 s2,[pc,#0x170]
  000b07ae: vldr.32 s6,[pc,#0x170]
  000b07b2: vcvt.f32.s32 s0,s0
  000b07b6: vldr.32 s4,[r0,#0x50]
  000b07ba: vmul.f32 s2,s16,s2
  000b07be: vldr.32 s8,[r4,#0x198]
  000b07c2: vmul.f32 s0,s0,s16
  000b07c6: vmul.f32 s2,s2,s4
  000b07ca: vmla.f32 s8,s0,s6
  000b07ce: vstr.32 s2,[r4,#0x80]
  000b07d2: vstr.32 s8,[r4,#0x198]
  000b07d6: b 0x000b07de
  000b07d8: ldrb.w r0,[r4,#0x158]
  000b07dc: cbz r0,0x000b07e8
  000b07de: vpop {d8,d9,d10}
  000b07e2: pop.w r11
  000b07e6: pop {r4,r5,r6,r7,pc}
  000b07e8: ldrb.w r0,[r4,#0x356]
  000b07ec: cbz r0,0x000b07f6
  000b07ee: ldr.w r0,[r4,#0x1c4]
  000b07f2: cmp r0,#0x1
  000b07f4: bne 0x000b07de
  000b07f6: ldrb.w r0,[r4,#0x235]
  000b07fa: mov.w r1,#0xffffffff
  000b07fe: str.w r1,[r4,#0x104]
  000b0802: cbz r0,0x000b0856
  000b0804: ldr r0,[0x000b093c]
  000b0806: add r0,pc
  000b0808: ldr r6,[r0,#0x0]
  000b080a: ldr r0,[r6,#0x0]
  000b080c: blx 0x00071a58
  000b0810: blx 0x00072bf8
  000b0814: ldr r1,[r6,#0x0]
  000b0816: vmov s0,r0
  000b081a: vcvt.f32.s32 s18,s0
  000b081e: mov r0,r1
  000b0820: blx 0x00071a58
  000b0824: blx 0x00072c04
  000b0828: vmov s0,r0
  000b082c: vmov.f32 s2,0x3f800000
  000b0830: vcvt.f32.s32 s0,s0
  000b0834: vldr.32 s20,[r4,#0x154]
  000b0838: vdiv.f32 s0,s18,s0
  000b083c: vsub.f32 s0,s2,s0
  000b0840: vldr.32 s2,[pc,#0xe0]
  000b0844: vmul.f32 s0,s20,s0
  000b0848: vmul.f32 s18,s0,s2
  000b084c: vldr.32 s0,[pc,#0xd8]
  000b0850: vmla.f32 s18,s20,s0
  000b0854: b 0x000b085e
  000b0856: vldr.32 s20,[r4,#0x154]
  000b085a: vmov.f32 s18,s20
  000b085e: vldr.32 s0,[pc,#0xcc]
  000b0862: movs r1,#0x3f
  000b0864: vmul.f32 s0,s16,s0
  000b0868: vmul.f32 s0,s0,s18
  000b086c: vcvt.s32.f32 s0,s0
  000b0870: vmov r0,s0
  000b0874: blx 0x0007198c
  000b0878: vmov s0,r0
  000b087c: vneg.f32 s4,s20
  000b0880: vcvt.f32.s32 s0,s0
  000b0884: vldr.32 s2,[r4,#0x27c]
  000b0888: vstr.32 s16,[r4,#0x268]
  000b088c: vstr.32 s4,[r4,#0x25c]
  000b0890: vcmpe.f32 s2,s0
  000b0894: vmrs apsr,fpscr
  000b0898: ble 0x000b07de
  000b089a: ldr r0,[0x000b0940]
  000b089c: add r0,pc
  000b089e: ldr r0,[r0,#0x0]
  000b08a0: ldr r0,[r0,#0x0]
  000b08a2: cbz r0,0x000b08be
  000b08a4: vmov.f32 s4,0x41c80000
  000b08a8: vldr.32 s6,[pc,#0x84]
  000b08ac: vmul.f32 s4,s18,s4
  000b08b0: vdiv.f32 s4,s4,s6
  000b08b4: vldr.32 s6,[pc,#0x7c]
  000b08b8: vmin.f32 d2,d2,d3
  000b08bc: b 0x000b08f6
  000b08be: ldr r0,[0x000b0944]
  000b08c0: vmov s6,r5
  000b08c4: vldr.32 s4,[pc,#0x70]
  000b08c8: vmov.f32 s10,0x41a00000
  000b08cc: add r0,pc
  000b08ce: ldr r0,[r0,#0x0]
  000b08d0: add.w r2,r0,#0x14
  000b08d4: ldrb r1,[r0,#0x11]
  000b08d6: cmp r1,#0x0
  000b08d8: it eq
  000b08da: add.eq.w r2,r0,#0x18
  000b08de: vldr.32 s8,[r2]
  000b08e2: vcvt.f32.s32 s6,s6
  000b08e6: vsub.f32 s4,s4,s8
  000b08ea: vmul.f32 s6,s18,s6
  000b08ee: vmul.f32 s4,s4,s10
  000b08f2: vdiv.f32 s4,s6,s4
  000b08f6: vsub.f32 s2,s2,s4
  000b08fa: vcmpe.f32 s2,s0
  000b08fe: vmrs apsr,fpscr
  000b0902: it mi
  000b0904: vmov.mi.f32 s2,s0
  000b0908: vstr.32 s2,[r4,#0x27c]
  000b090c: b 0x000b07de
  001abaf8: bx pc
```
