# Globals::setCoordsSteer
elf 0xe8404 body 754
Sig: undefined __thiscall setCoordsSteer(Globals * this, int param_1, int param_2, int param_3, int param_4, ushort * param_5, ushort * param_6, ushort * param_7, ushort * param_8, ushort * param_9, ushort * param_10, ushort * param_11, ushort * param_12, ushort * param_13, ushort * param_14)

## decompile
```c

/* Globals::setCoordsSteer(int, int, int, int, unsigned short&, unsigned short&, unsigned short&,
   unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned
   short&, unsigned short&) */

void __thiscall
Globals::setCoordsSteer
          (Globals *this,int param_1,int param_2,int param_3,int param_4,ushort *param_5,
          ushort *param_6,ushort *param_7,ushort *param_8,ushort *param_9,ushort *param_10,
          ushort *param_11,ushort *param_12,ushort *param_13,ushort *param_14)

{
  uint uVar1;
  uint uVar2;
  char cVar3;
  byte bVar4;
  float *pfVar5;
  uint uVar6;
  float *pfVar7;
  char cVar8;
  int iVar9;
  ushort uVar10;
  int iVar11;
  bool bVar12;
  uint in_fpscr;
  float fVar13;
  float fVar14;
  float fVar15;
  int iVar16;
  float fVar17;
  
  iVar11 = **(int **)(DAT_000f8734 + 0xf841a);
  cVar3 = **(char **)(DAT_000f8738 + 0xf841e);
  iVar9 = ((-0x19 - param_2) - param_3) + iVar11;
  fVar15 = DAT_000f86f8;
  if (cVar3 == '\0') {
    pfVar7 = (float *)&DAT_000f86fc;
    if (**(char **)(DAT_000f873c + 0xf8440) == '\0') {
      pfVar7 = (float *)&DAT_000f8700;
    }
    fVar15 = *pfVar7;
  }
  if (iVar9 < param_1) {
    param_1 = iVar9;
  }
  fVar14 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  uVar1 = in_fpscr & 0xfffffff;
  uVar2 = uVar1 | (uint)(fVar15 < fVar14) << 0x1f | (uint)(fVar15 == fVar14) << 0x1e;
  bVar4 = (byte)(uVar2 >> 0x18);
  if ((bool)(bVar4 >> 6 & 1) || (bool)(bVar4 >> 7) != (NAN(fVar15) || NAN(fVar14))) {
    iVar9 = (int)fVar14;
    uVar10 = (ushort)iVar9;
    *(int *)(*(int *)(DAT_000f8744 + 0xf8498) + 0x54) = (int)fVar14;
    if (cVar3 == '\0') {
      cVar8 = **(char **)(DAT_000f8748 + 0xf8506);
      goto LAB_000f8508;
    }
  }
  else {
    if (cVar3 == '\0') {
      cVar8 = **(char **)(DAT_000f874c + 0xf84e4);
      iVar9 = 300;
      if (cVar8 == '\0') {
        iVar9 = 0x96;
      }
      *(int *)(*(int *)(DAT_000f8750 + 0xf84e6) + 0x54) = iVar9;
LAB_000f8508:
      uVar10 = 0x28;
      if (cVar8 == '\0') {
        uVar10 = 0x14;
      }
      *param_5 = uVar10;
      *param_6 = (ushort)iVar9;
      *param_13 = uVar10;
      pfVar7 = (float *)&DAT_000f870c;
      fVar15 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar2 >> 0x16) & 3);
      if (cVar8 == '\0') {
        pfVar7 = (float *)&DAT_000f8710;
      }
      pfVar5 = (float *)&DAT_000f8714;
      if (cVar8 == '\0') {
        pfVar5 = (float *)&DAT_000f8718;
      }
      fVar14 = *pfVar5;
      *param_14 = (ushort)(0.0 < fVar15 - *pfVar7) * (short)(int)(fVar15 - *pfVar7);
      *param_7 = 0x14;
      goto LAB_000f8556;
    }
    uVar10 = 0xd2;
    *(undefined4 *)(*(int *)(DAT_000f8740 + 0xf8480) + 0x54) = 0xd2;
  }
  fVar15 = DAT_000f8704;
  *param_5 = 0x1c;
  *param_6 = uVar10;
  *param_13 = 0x1c;
  fVar14 = DAT_000f8708;
  fVar13 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar2 >> 0x16) & 3);
  *param_14 = (ushort)(0.0 < fVar13 + fVar15) * (short)(int)(fVar13 + fVar15);
  *param_7 = 0x14;
LAB_000f8556:
  uVar6 = param_2 - (param_2 >> 0x1f);
  fVar15 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar2 >> 0x16) & 3);
  *param_8 = (ushort)(0.0 < fVar14 + fVar15) * (short)(int)(fVar14 + fVar15);
  *param_9 = *param_7 + (short)(uVar6 >> 1);
  *param_10 = *param_8 + (short)(uVar6 >> 1);
  fVar15 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar2 >> 0x16) & 3);
  if (cVar3 == '\0') {
    pfVar7 = (float *)&DAT_000f8724;
    fVar14 = 4.0;
    bVar12 = **(char **)(DAT_000f8754 + 0xf85b8) == '\0';
    if (bVar12) {
      pfVar7 = (float *)&DAT_000f8728;
    }
    fVar15 = *pfVar7 + fVar15;
    if (bVar12) {
      fVar14 = 2.0;
    }
  }
  else {
    fVar15 = fVar15 + DAT_000f871c;
    fVar14 = DAT_000f8720;
  }
  iVar9 = (uint)(0.0 < fVar15) * (int)fVar15;
  *param_12 = (ushort)iVar9;
  fVar15 = (float)VectorSignedToFloat(iVar11 - param_4,(byte)(uVar2 >> 0x16) & 3);
  fVar13 = (float)VectorUnsignedToFloat(iVar9,(byte)(uVar2 >> 0x16) & 3);
  if (fVar13 <= fVar15 - fVar14) {
    if (cVar3 == '\0') {
      uVar10 = 0x28;
      if (**(char **)(DAT_000f8760 + 0xf86bc) == '\0') {
        uVar10 = 0x14;
      }
    }
    else {
      uVar10 = 0x1c;
    }
    *param_11 = uVar10;
  }
  else {
    if (cVar3 == '\0') {
      fVar14 = 4.0;
      iVar9 = 0x15a;
      cVar8 = **(char **)(DAT_000f8758 + 0xf863e);
      if (cVar8 == '\0') {
        fVar14 = 2.0;
      }
      iVar11 = 0x10a;
      fVar13 = fVar13 - (fVar15 - fVar14);
      iVar16 = (uint)(0.0 < fVar13) * (int)fVar13;
      *param_12 = (ushort)iVar16;
      if (cVar8 == '\0') {
        iVar9 = 0xad;
        iVar11 = 0x85;
      }
    }
    else {
      iVar9 = 0xf3;
      iVar11 = 0xbb;
      fVar13 = fVar13 - (fVar15 + DAT_000f872c);
      iVar16 = (uint)(0.0 < fVar13) * (int)fVar13;
      *param_12 = (ushort)iVar16;
    }
    fVar13 = (float)VectorUnsignedToFloat(iVar16,(byte)(uVar1 >> 0x16) & 3);
    fVar17 = (float)VectorSignedToFloat(iVar9 - iVar11,(byte)(uVar1 >> 0x16) & 3);
    fVar14 = 1.0;
    if (fVar13 / DAT_000f8730 < 1.0) {
      fVar14 = fVar13 / DAT_000f8730;
    }
    fVar13 = (float)VectorSignedToFloat(iVar11,(byte)(uVar1 >> 0x16) & 3);
    fVar13 = fVar13 + fVar14 * fVar17;
    *param_11 = (ushort)(0.0 < fVar13) * (short)(int)fVar13;
    fVar14 = DAT_000f8720;
    if ((cVar3 == '\0') && (fVar14 = 4.0, **(char **)(DAT_000f875c + 0xf86d8) == '\0')) {
      fVar14 = 2.0;
    }
    *param_12 = (ushort)(0.0 < fVar15 - fVar14) * (short)(int)(fVar15 - fVar14);
  }
  return;
}

```

## target disasm
```
  000f8404: push {r4,r5,r6,r7,lr}
  000f8406: add r7,sp,#0xc
  000f8408: push {r8,r9,r10,r11}
  000f840c: ldr r0,[0x000f8734]
  000f840e: mvn r5,#0x18
  000f8412: ldr r4,[0x000f8738]
  000f8414: subs r5,r5,r2
  000f8416: add r0,pc
  000f8418: subs r3,r5,r3
  000f841a: add r4,pc
  000f841c: ldr r0,[r0,#0x0]
  000f841e: ldr r6,[r4,#0x0]
  000f8420: ldr.w r12,[r0,#0x0]
  000f8424: ldrb.w r10,[r6,#0x0]
  000f8428: add.w r4,r3,r12
  000f842c: cmp.w r10,#0x0
  000f8430: beq 0x000f8438
  000f8432: vldr.32 s0,[pc,#0x2c4]
  000f8436: b 0x000f844c
  000f8438: ldr r3,[0x000f873c]
  000f843a: adr r6,[0xf86fc]
  000f843c: add r3,pc
  000f843e: ldr r3,[r3,#0x0]
  000f8440: ldrb r3,[r3,#0x0]
  000f8442: cmp r3,#0x0
  000f8444: it eq
  000f8446: add.eq r6,#0x4
  000f8448: vldr.32 s0,[r6]
  000f844c: ldrd r5,r6,[r7,#0x2c]
  000f8450: cmp r4,r1
  000f8452: ldrd lr,r3,[r7,#0x1c]
  000f8456: ldrd r8,r0,[r7,#0x14]
  000f845a: ldrd r11,r9,[r7,#0xc]
  000f845e: it lt
  000f8460: mov.lt r1,r4
  000f8462: vmov s2,r1
  000f8466: vcvt.f32.s32 s2,s2
  000f846a: vcmpe.f32 s0,s2
  000f846e: vmrs apsr,fpscr
  000f8472: ble 0x000f8486
  000f8474: cmp.w r10,#0x0
  000f8478: beq 0x000f84d8
  000f847a: ldr r1,[0x000f8740]
  000f847c: add r1,pc
  000f847e: ldr r4,[r1,#0x0]
  000f8480: movs r1,#0xd2
  000f8482: str r1,[r4,#0x54]
  000f8484: b 0x000f84a2
  000f8486: ldr r1,[0x000f8744]
  000f8488: vcvt.s32.f32 s0,s2
  000f848c: vcvt.s32.f32 s2,s2
  000f8490: cmp.w r10,#0x0
  000f8494: add r1,pc
  000f8496: ldr r1,[r1,#0x0]
  000f8498: vstr.32 s0,[r1,#0x54]
  000f849c: vmov r1,s2
  000f84a0: beq 0x000f84fe
  000f84a2: movs r4,#0x1c
  000f84a4: vldr.32 s2,[pc,#0x25c]
  000f84a8: strh.w r4,[r11,#0x0]
  000f84ac: strh.w r1,[r9,#0x0]
  000f84b0: strh r4,[r5,#0x0]
  000f84b2: ldrh.w r1,[r9,#0x0]
  000f84b6: vmov s0,r1
  000f84ba: movs r1,#0x14
  000f84bc: vcvt.f32.u32 s0,s0
  000f84c0: vadd.f32 s0,s0,s2
  000f84c4: vcvt.u32.f32 s2,s0
  000f84c8: vldr.32 s0,[pc,#0x23c]
  000f84cc: vmov r5,s2
  000f84d0: strh r5,[r6,#0x0]
  000f84d2: strh.w r1,[r8,#0x0]
  000f84d6: b 0x000f8556
  000f84d8: ldr r1,[0x000f874c]
  000f84da: mov r0,r6
  000f84dc: ldr r4,[0x000f8750]
  000f84de: mov r6,lr
  000f84e0: add r1,pc
  000f84e2: add r4,pc
  000f84e4: ldr r1,[r1,#0x0]
  000f84e6: ldr.w lr,[r4,#0x0]
  000f84ea: ldrb r4,[r1,#0x0]
  000f84ec: mov.w r1,#0x12c
  000f84f0: cmp r4,#0x0
  000f84f2: it eq
  000f84f4: mov.eq r1,#0x96
  000f84f6: str.w r1,[lr,#0x54]
  000f84fa: mov lr,r6
  000f84fc: b 0x000f8508
  000f84fe: ldr r4,[0x000f8748]
  000f8500: mov r0,r6
  000f8502: add r4,pc
  000f8504: ldr r4,[r4,#0x0]
  000f8506: ldrb r4,[r4,#0x0]
  000f8508: movs r6,#0x28
  000f850a: cmp r4,#0x0
  000f850c: it eq
  000f850e: mov.eq r6,#0x14
  000f8510: cmp r4,#0x0
  000f8512: strh.w r6,[r11,#0x0]
  000f8516: strh.w r1,[r9,#0x0]
  000f851a: strh r6,[r5,#0x0]
  000f851c: mov.w r6,#0x14
  000f8520: ldrh.w r1,[r9,#0x0]
  000f8524: vmov s0,r1
  000f8528: adr r1,[0xf870c]
  000f852a: vcvt.f32.u32 s0,s0
  000f852e: it eq
  000f8530: add.eq r1,#0x4
  000f8532: vldr.32 s2,[r1]
  000f8536: adr r1,[0xf8714]
  000f8538: cmp r4,#0x0
  000f853a: it eq
  000f853c: add.eq r1,#0x4
  000f853e: vsub.f32 s0,s0,s2
  000f8542: vcvt.u32.f32 s2,s0
  000f8546: vldr.32 s0,[r1]
  000f854a: vmov r1,s2
  000f854e: strh r1,[r0,#0x0]
  000f8550: ldr r0,[r7,#0x18]
  000f8552: strh.w r6,[r8,#0x0]
  000f8556: ldrh.w r6,[r9,#0x0]
  000f855a: add.w r2,r2,r2, lsr #0x1f
  000f855e: ldr r1,[r7,#0x28]
  000f8560: cmp.w r10,#0x0
  000f8564: ldr r5,[r7,#0x8]
  000f8566: vmov s2,r6
  000f856a: vcvt.f32.u32 s2,s2
  000f856e: vadd.f32 s0,s0,s2
  000f8572: vcvt.u32.f32 s0,s0
  000f8576: vmov r6,s0
  000f857a: strh r6,[r0,#0x0]
  000f857c: ldrh.w r6,[r8,#0x0]
  000f8580: add.w r6,r6,r2, lsr #0x1
  000f8584: strh.w r6,[lr,#0x0]
  000f8588: ldrh r6,[r0,#0x0]
  000f858a: add.w r2,r6,r2, lsr #0x1
  000f858e: strh r2,[r3,#0x0]
  000f8590: ldrh.w r2,[r9,#0x0]
  000f8594: vmov s0,r2
  000f8598: vcvt.f32.u32 s0,s0
  000f859c: beq 0x000f85ac
  000f859e: vldr.32 s2,[pc,#0x17c]
  000f85a2: vadd.f32 s0,s0,s2
  000f85a6: vldr.32 s2,[pc,#0x178]
  000f85aa: b 0x000f85d2
  000f85ac: ldr r2,[0x000f8754]
  000f85ae: adr r3,[0xf8724]
  000f85b0: vmov.f32 s2,0x40800000
  000f85b4: add r2,pc
  000f85b6: ldr r2,[r2,#0x0]
  000f85b8: ldrb r2,[r2,#0x0]
  000f85ba: cmp r2,#0x0
  000f85bc: it eq
  000f85be: add.eq r3,#0x4
  000f85c0: vldr.32 s4,[r3]
  000f85c4: vadd.f32 s0,s4,s0
  000f85c8: vmov.f32 s4,0x40000000
  000f85cc: it eq
  000f85ce: vmov.eq.f32 s2,s4
  000f85d2: vcvt.u32.f32 s0,s0
  000f85d6: vmov r3,s0
  000f85da: sub.w r6,r12,r5
  000f85de: ldr r2,[r7,#0x24]
  000f85e0: vmov s0,r6
  000f85e4: strh r3,[r1,#0x0]
  000f85e6: vcvt.f32.s32 s0,s0
  000f85ea: vmov s4,r3
  000f85ee: vcvt.f32.u32 s4,s4
  000f85f2: vsub.f32 s2,s0,s2
  000f85f6: vcmpe.f32 s2,s4
  000f85fa: vmrs apsr,fpscr
  000f85fe: bpl 0x000f8622
  000f8600: cmp.w r10,#0x0
  000f8604: beq 0x000f862c
  000f8606: vldr.32 s2,[pc,#0x124]
  000f860a: movs r6,#0xf3
  000f860c: movs r3,#0xbb
  000f860e: vadd.f32 s2,s0,s2
  000f8612: vsub.f32 s2,s4,s2
  000f8616: vcvt.u32.f32 s2,s2
  000f861a: vmov r5,s2
  000f861e: strh r5,[r1,#0x0]
  000f8620: b 0x000f8668
  000f8622: cmp.w r10,#0x0
  000f8626: beq 0x000f86b6
  000f8628: movs r0,#0x1c
  000f862a: b 0x000f86c6
  000f862c: ldr r3,[0x000f8758]
  000f862e: vmov.f32 s6,0x40000000
  000f8632: vmov.f32 s2,0x40800000
  000f8636: mov.w r6,#0x15a
  000f863a: add r3,pc
  000f863c: ldr r3,[r3,#0x0]
  000f863e: ldrb r3,[r3,#0x0]
  000f8640: cmp r3,#0x0
  000f8642: it eq
  000f8644: vmov.eq.f32 s2,s6
  000f8648: vsub.f32 s2,s0,s2
  000f864c: cmp r3,#0x0
  000f864e: mov.w r3,#0x10a
  000f8652: vsub.f32 s2,s4,s2
  000f8656: vcvt.u32.f32 s2,s2
  000f865a: vmov r5,s2
  000f865e: strh r5,[r1,#0x0]
  000f8660: it eq
  000f8662: mov.eq r6,#0xad
  000f8664: it eq
  000f8666: mov.eq r3,#0x85
  000f8668: vmov s2,r5
  000f866c: vldr.32 s4,[pc,#0xc0]
  000f8670: subs r6,r6,r3
  000f8672: vcvt.f32.u32 s2,s2
  000f8676: vmov s6,r6
  000f867a: vcvt.f32.s32 s6,s6
  000f867e: vdiv.f32 s2,s2,s4
  000f8682: vmov.f32 s4,0x3f800000
  000f8686: vcmpe.f32 s2,s4
  000f868a: vmrs apsr,fpscr
  000f868e: it mi
  000f8690: vmov.mi.f32 s4,s2
  000f8694: vmov s2,r3
  000f8698: cmp.w r10,#0x0
  000f869c: vcvt.f32.s32 s2,s2
  000f86a0: vmla.f32 s2,s4,s6
  000f86a4: vcvt.u32.f32 s2,s2
  000f86a8: vmov r3,s2
  000f86ac: strh r3,[r2,#0x0]
  000f86ae: beq 0x000f86ca
  000f86b0: vldr.32 s2,[pc,#0x6c]
  000f86b4: b 0x000f86e2
  000f86b6: ldr r0,[0x000f8760]
  000f86b8: add r0,pc
  000f86ba: ldr r0,[r0,#0x0]
  000f86bc: ldrb r1,[r0,#0x0]
  000f86be: movs r0,#0x28
  000f86c0: cmp r1,#0x0
  000f86c2: it eq
  000f86c4: mov.eq r0,#0x14
  000f86c6: strh r0,[r2,#0x0]
  000f86c8: b 0x000f86f0
  000f86ca: ldr r0,[0x000f875c]
  000f86cc: vmov.f32 s4,0x40000000
  000f86d0: vmov.f32 s2,0x40800000
  000f86d4: add r0,pc
  000f86d6: ldr r0,[r0,#0x0]
  000f86d8: ldrb r0,[r0,#0x0]
  000f86da: cmp r0,#0x0
  000f86dc: it eq
  000f86de: vmov.eq.f32 s2,s4
  000f86e2: vsub.f32 s0,s0,s2
  000f86e6: vcvt.u32.f32 s0,s0
  000f86ea: vmov r0,s0
  000f86ee: strh r0,[r1,#0x0]
  000f86f0: pop.w {r8,r9,r10,r11}
  000f86f4: pop {r4,r5,r6,r7,pc}
```
