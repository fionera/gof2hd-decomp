# Globals::setCoordsFire
elf 0xe8764 body 1016
Sig: undefined __thiscall setCoordsFire(Globals * this, int param_1, int param_2, uint param_3, uint param_4, uint * param_5, ushort * param_6, ushort * param_7, ushort * param_8, ushort * param_9, ushort * param_10, ushort * param_11, ushort * param_12, ushort * param_13, ushort * param_14, ushort * param_15, ushort * param_16, ushort * param_17)

## decompile
```c

/* Globals::setCoordsFire(int, int, unsigned int, unsigned int, unsigned int&, unsigned short&,
   unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned
   short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&) */

void __thiscall
Globals::setCoordsFire
          (Globals *this,int param_1,int param_2,uint param_3,uint param_4,uint *param_5,
          ushort *param_6,ushort *param_7,ushort *param_8,ushort *param_9,ushort *param_10,
          ushort *param_11,ushort *param_12,ushort *param_13,ushort *param_14,ushort *param_15,
          ushort *param_16,ushort *param_17)

{
  uint uVar1;
  byte bVar2;
  char cVar3;
  float *pfVar4;
  float *pfVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  uint in_fpscr;
  uint uVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  ushort uVar14;
  float fVar15;
  float fVar16;
  
  fVar13 = DAT_000f8b90;
  cVar3 = **(char **)(DAT_000f8b0c + 0xf8774);
  fVar11 = DAT_000f8b14;
  fVar12 = DAT_000f8b10;
  if (cVar3 == '\0') {
    pfVar5 = (float *)&DAT_000f8b80;
    pfVar4 = (float *)&DAT_000f8b88;
    if (**(char **)(DAT_000f8b18 + 0xf878c) == '\0') {
      pfVar5 = (float *)&DAT_000f8b84;
      pfVar4 = (float *)&DAT_000f8b8c;
    }
    fVar11 = *pfVar4;
    fVar12 = *pfVar5;
  }
  iVar8 = **(int **)(DAT_000f8b1c + 0xf87ae) - param_2;
  fVar10 = (float)VectorSignedToFloat(iVar8,(byte)(in_fpscr >> 0x16) & 3);
  fVar15 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  uVar9 = in_fpscr & 0xfffffff;
  fVar16 = fVar15;
  if (fVar12 + fVar10 < fVar15) {
    fVar16 = DAT_000f8b7c;
    if (cVar3 == '\0') {
      pfVar5 = (float *)&DAT_000f8b80;
      if (**(char **)(DAT_000f8b70 + 0xf87e2) == '\0') {
        pfVar5 = (float *)&DAT_000f8b84;
      }
      fVar16 = *pfVar5;
    }
    fVar16 = fVar16 + fVar10;
  }
  uVar1 = uVar9 | (uint)(fVar11 < fVar16) << 0x1f | (uint)(fVar11 == fVar16) << 0x1e;
  bVar2 = (byte)(uVar1 >> 0x18);
  if ((bool)(bVar2 >> 6 & 1) || (bool)(bVar2 >> 7) != (NAN(fVar11) || NAN(fVar16))) {
    fVar13 = DAT_000f8b7c;
    if (cVar3 == '\0') {
      pfVar5 = (float *)&DAT_000f8b80;
      if (**(char **)(DAT_000f8bfc + 0xf8864) == '\0') {
        pfVar5 = (float *)&DAT_000f8b84;
      }
      fVar13 = *pfVar5;
    }
    if (fVar13 + fVar10 < fVar15) {
      fVar15 = DAT_000f8b7c;
      if (cVar3 == '\0') {
        pfVar5 = (float *)&DAT_000f8b80;
        if (**(char **)(DAT_000f8c00 + 0xf888e) == '\0') {
          pfVar5 = (float *)&DAT_000f8b84;
        }
        fVar15 = *pfVar5;
      }
      fVar15 = fVar15 + fVar10;
    }
    fVar11 = (float)VectorSignedToFloat(**(int **)(DAT_000f8c04 + 0xf88ac) - param_2,
                                        (byte)(uVar9 >> 0x16) & 3);
    iVar7 = (int)fVar15;
    *(int *)(*(int *)(DAT_000f8c08 + 0xf88ae) + 0x58) = (int)fVar15;
    fVar13 = DAT_000f8b90;
    if (cVar3 == '\0') {
      cVar6 = **(char **)(DAT_000f8c0c + 0xf88d8);
      goto LAB_000f88da;
    }
  }
  else {
    uVar9 = uVar1;
    if (cVar3 == '\0') {
      fVar11 = (float)VectorSignedToFloat(**(int **)(DAT_000f8b74 + 0xf880e) - param_2,
                                          (byte)(uVar1 >> 0x16) & 3);
      cVar6 = **(char **)(DAT_000f8bf4 + 0xf883e);
      iVar7 = 300;
      if (cVar6 == '\0') {
        iVar7 = 0x96;
      }
      *(int *)(*(int *)(DAT_000f8bf8 + 0xf8840) + 0x58) = iVar7;
LAB_000f88da:
      pfVar5 = (float *)&DAT_000f8b94;
      if (cVar6 == '\0') {
        pfVar5 = (float *)&DAT_000f8b98;
      }
      fVar13 = *pfVar5;
    }
    else {
      iVar7 = 0xd2;
      fVar11 = (float)VectorSignedToFloat(**(int **)(DAT_000f8b74 + 0xf880e) - param_2,
                                          (byte)(uVar1 >> 0x16) & 3);
      *(undefined4 *)(*(int *)(DAT_000f8b78 + 0xf8818) + 0x58) = 0xd2;
    }
  }
  *param_6 = (ushort)(0.0 < fVar11 + fVar13) * (short)(int)(fVar11 + fVar13);
  *param_7 = (ushort)iVar7;
  *param_8 = *param_6 + (short)((uint)param_2 >> 1);
  *param_9 = *param_7 + (short)((uint)param_2 >> 1);
  fVar13 = DAT_000f8ba0;
  fVar11 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar9 >> 0x16) & 3);
  if (cVar3 == '\0') {
    pfVar5 = (float *)&DAT_000f8bb4;
    cVar6 = **(char **)(DAT_000f8c10 + 0xf89ba);
    if (cVar6 == '\0') {
      pfVar5 = (float *)&DAT_000f8bb8;
    }
    fVar13 = 8.0;
    *param_10 = (ushort)(0.0 < *pfVar5 + fVar11) * (short)(int)(*pfVar5 + fVar11);
    if (cVar6 == '\0') {
      fVar13 = 4.0;
    }
    fVar11 = (float)VectorUnsignedToFloat((uint)*param_7,(byte)(uVar9 >> 0x16) & 3);
    fVar12 = 28.0;
    *param_11 = (ushort)(0.0 < fVar13 + fVar11) * (short)(int)(fVar13 + fVar11);
    if (cVar6 == '\0') {
      fVar12 = 15.0;
    }
    fVar13 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar9 >> 0x16) & 3);
    fVar11 = 27.0;
    *param_12 = (ushort)(0.0 < fVar12 + fVar13) * (short)(int)(fVar12 + fVar13);
    if (cVar6 == '\0') {
      fVar11 = 13.0;
    }
    fVar13 = (float)VectorUnsignedToFloat((uint)*param_7,(byte)(uVar9 >> 0x16) & 3);
    *param_13 = (ushort)(0.0 < fVar11 + fVar13) * (short)(int)(fVar11 + fVar13);
    pfVar5 = (float *)&DAT_000f8bbc;
    fVar13 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar9 >> 0x16) & 3);
    if (cVar6 == '\0') {
      pfVar5 = (float *)&DAT_000f8bc0;
    }
    pfVar4 = (float *)&DAT_000f8bc4;
    uVar14 = (ushort)(0.0 < *pfVar5 + fVar13) * (short)(int)(*pfVar5 + fVar13);
    if (cVar6 == '\0') {
      pfVar4 = (float *)&DAT_000f8bc8;
    }
    fVar13 = *pfVar4;
  }
  else {
    *param_10 = (ushort)(0.0 < fVar11 + DAT_000f8b9c) * (short)(int)(fVar11 + DAT_000f8b9c);
    fVar11 = DAT_000f8ba4;
    fVar12 = (float)VectorUnsignedToFloat((uint)*param_7,(byte)(uVar9 >> 0x16) & 3);
    *param_11 = (ushort)(0.0 < fVar12 + fVar13) * (short)(int)(fVar12 + fVar13);
    fVar13 = DAT_000f8ba8;
    fVar12 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar9 >> 0x16) & 3);
    *param_12 = (ushort)(0.0 < fVar12 + fVar11) * (short)(int)(fVar12 + fVar11);
    fVar11 = DAT_000f8bac;
    fVar12 = (float)VectorUnsignedToFloat((uint)*param_7,(byte)(uVar9 >> 0x16) & 3);
    *param_13 = (ushort)(0.0 < fVar12 + fVar13) * (short)(int)(fVar12 + fVar13);
    fVar13 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar9 >> 0x16) & 3);
    uVar14 = (ushort)(0.0 < fVar13 + fVar11) * (short)(int)(fVar13 + fVar11);
    fVar13 = DAT_000f8bb0;
  }
  *param_16 = uVar14;
  fVar11 = (float)VectorUnsignedToFloat((uint)*param_7,(byte)(uVar9 >> 0x16) & 3);
  *param_17 = (ushort)(0.0 < fVar11 - fVar13) * (short)(int)(fVar11 - fVar13);
  if (iVar8 < iVar7) {
    *param_5 = param_3;
    fVar13 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar9 >> 0x16) & 3);
    if (cVar3 != '\0') {
      uVar14 = (ushort)(0.0 < fVar13 + -2.0) * (short)(int)(fVar13 + -2.0);
      fVar13 = DAT_000f8be4;
      goto LAB_000f8b48;
    }
    pfVar4 = (float *)&DAT_000f8bec;
    cVar3 = **(char **)(DAT_000f8c14 + 0xf8b00);
    fVar11 = -2.0;
    if (cVar3 == '\0') {
      fVar11 = DAT_000f8be8;
    }
  }
  else {
    *param_5 = param_4;
    fVar13 = (float)VectorUnsignedToFloat((uint)*param_6,(byte)(uVar9 >> 0x16) & 3);
    if (cVar3 != '\0') {
      uVar14 = (ushort)(0.0 < fVar13 + DAT_000f8bcc) * (short)(int)(fVar13 + DAT_000f8bcc);
      fVar13 = DAT_000f8bd0;
      goto LAB_000f8b48;
    }
    pfVar5 = (float *)&DAT_000f8bd4;
    cVar3 = **(char **)(DAT_000f8c18 + 0xf8b28);
    if (cVar3 == '\0') {
      pfVar5 = (float *)&DAT_000f8bd8;
    }
    pfVar4 = (float *)&UNK_000f8bdc;
    fVar11 = *pfVar5;
  }
  uVar14 = (ushort)(0.0 < fVar11 + fVar13) * (short)(int)(fVar11 + fVar13);
  if (cVar3 == '\0') {
    pfVar4 = pfVar4 + 1;
  }
  fVar13 = *pfVar4;
LAB_000f8b48:
  *param_14 = uVar14;
  fVar11 = (float)VectorUnsignedToFloat((uint)*param_7,(byte)(uVar9 >> 0x16) & 3);
  *param_15 = (ushort)(0.0 < fVar13 + fVar11) * (short)(int)(fVar13 + fVar11);
  return;
}

```

## target disasm
```
  000f8764: push {r4,r5,r6,r7,lr}
  000f8766: add r7,sp,#0xc
  000f8768: push {r7,r8,r9,r10,r11}
  000f876c: ldr r0,[0x000f8b0c]
  000f876e: mov r5,r3
  000f8770: add r0,pc
  000f8772: ldr r0,[r0,#0x0]
  000f8774: ldrb r0,[r0,#0x0]
  000f8776: cbz r0,0x000f8782
  000f8778: vldr.32 s6,[pc,#0x394]
  000f877c: vldr.32 s4,[pc,#0x394]
  000f8780: b 0x000f87a4
  000f8782: ldr r3,[0x000f8b18]
  000f8784: adr.w r6,0xf8b80
  000f8788: add r3,pc
  000f878a: ldr r3,[r3,#0x0]
  000f878c: ldrb r3,[r3,#0x0]
  000f878e: cmp r3,#0x0
  000f8790: adr.w r3,0xf8b88
  000f8794: it eq
  000f8796: add.eq r6,#0x4
  000f8798: it eq
  000f879a: add.eq r3,#0x4
  000f879c: vldr.32 s6,[r6]
  000f87a0: vldr.32 s4,[r3]
  000f87a4: ldr r3,[0x000f8b1c]
  000f87a6: vmov s2,r1
  000f87aa: add r3,pc
  000f87ac: ldr r3,[r3,#0x0]
  000f87ae: ldr r3,[r3,#0x0]
  000f87b0: sub.w r12,r3,r2
  000f87b4: vmov s0,r12
  000f87b8: vcvt.f32.s32 s0,s0
  000f87bc: vcvt.f32.s32 s2,s2
  000f87c0: vadd.f32 s6,s6,s0
  000f87c4: vcmpe.f32 s6,s2
  000f87c8: vmov.f32 s6,s2
  000f87cc: vmrs apsr,fpscr
  000f87d0: bpl 0x000f87f2
  000f87d2: cbz r0,0x000f87da
  000f87d4: vldr.32 s6,[pc,#0x3a4]
  000f87d8: b 0x000f87ee
  000f87da: ldr r1,[0x000f8b70]
  000f87dc: adr r3,[0xf8b80]
  000f87de: add r1,pc
  000f87e0: ldr r1,[r1,#0x0]
  000f87e2: ldrb r1,[r1,#0x0]
  000f87e4: cmp r1,#0x0
  000f87e6: it eq
  000f87e8: add.eq r3,#0x4
  000f87ea: vldr.32 s6,[r3]
  000f87ee: vadd.f32 s6,s6,s0
  000f87f2: ldrd r10,r11,[r7,#0x18]
  000f87f6: vcmpe.f32 s4,s6
  000f87fa: ldrd r1,lr,[r7,#0x10]
  000f87fe: vmrs apsr,fpscr
  000f8802: str r5,[sp,#0x0]
  000f8804: ble 0x000f882c
  000f8806: ldr r3,[0x000f8b74]
  000f8808: cmp r0,#0x0
  000f880a: add r3,pc
  000f880c: ldr r3,[r3,#0x0]
  000f880e: beq 0x000f8834
  000f8810: ldr r4,[0x000f8b78]
  000f8812: ldr r3,[r3,#0x0]
  000f8814: add r4,pc
  000f8816: vldr.32 s2,[pc,#0x378]
  000f881a: subs r3,r3,r2
  000f881c: vmov s0,r3
  000f8820: ldr r3,[r4,#0x0]
  000f8822: movs r4,#0xd2
  000f8824: vcvt.f32.s32 s0,s0
  000f8828: str r4,[r3,#0x58]
  000f882a: b 0x000f88e6
  000f882c: cbz r0,0x000f885c
  000f882e: vldr.32 s4,[pc,#0x34c]
  000f8832: b 0x000f8870
  000f8834: ldr r4,[0x000f8bf4]
  000f8836: ldr r5,[0x000f8bf8]
  000f8838: ldr r3,[r3,#0x0]
  000f883a: add r4,pc
  000f883c: add r5,pc
  000f883e: subs r3,r3,r2
  000f8840: ldr r4,[r4,#0x0]
  000f8842: ldr r5,[r5,#0x0]
  000f8844: vmov s0,r3
  000f8848: vcvt.f32.s32 s0,s0
  000f884c: ldrb r3,[r4,#0x0]
  000f884e: mov.w r4,#0x12c
  000f8852: cmp r3,#0x0
  000f8854: it eq
  000f8856: mov.eq r4,#0x96
  000f8858: str r4,[r5,#0x58]
  000f885a: b 0x000f88da
  000f885c: ldr r3,[0x000f8bfc]
  000f885e: adr r4,[0xf8b80]
  000f8860: add r3,pc
  000f8862: ldr r3,[r3,#0x0]
  000f8864: ldrb r3,[r3,#0x0]
  000f8866: cmp r3,#0x0
  000f8868: it eq
  000f886a: add.eq r4,#0x4
  000f886c: vldr.32 s4,[r4]
  000f8870: vadd.f32 s4,s4,s0
  000f8874: vcmpe.f32 s4,s2
  000f8878: vmrs apsr,fpscr
  000f887c: bpl 0x000f889e
  000f887e: cbz r0,0x000f8886
  000f8880: vldr.32 s2,[pc,#0x2f8]
  000f8884: b 0x000f889a
  000f8886: ldr r3,[0x000f8c00]
  000f8888: adr r4,[0xf8b80]
  000f888a: add r3,pc
  000f888c: ldr r3,[r3,#0x0]
  000f888e: ldrb r3,[r3,#0x0]
  000f8890: cmp r3,#0x0
  000f8892: it eq
  000f8894: add.eq r4,#0x4
  000f8896: vldr.32 s2,[r4]
  000f889a: vadd.f32 s2,s2,s0
  000f889e: ldr r3,[0x000f8c04]
  000f88a0: cmp r0,#0x0
  000f88a2: ldr r4,[0x000f8c08]
  000f88a4: vcvt.s32.f32 s4,s2
  000f88a8: add r3,pc
  000f88aa: add r4,pc
  000f88ac: ldr r3,[r3,#0x0]
  000f88ae: ldr r3,[r3,#0x0]
  000f88b0: sub.w r3,r3,r2
  000f88b4: vmov s0,r3
  000f88b8: ldr r3,[r4,#0x0]
  000f88ba: vcvt.f32.s32 s0,s0
  000f88be: vcvt.s32.f32 s2,s2
  000f88c2: vstr.32 s4,[r3,#0x58]
  000f88c6: vmov r4,s2
  000f88ca: beq 0x000f88d2
  000f88cc: vldr.32 s2,[pc,#0x2c0]
  000f88d0: b 0x000f88e6
  000f88d2: ldr r3,[0x000f8c0c]
  000f88d4: add r3,pc
  000f88d6: ldr r3,[r3,#0x0]
  000f88d8: ldrb r3,[r3,#0x0]
  000f88da: adr r5,[0xf8b94]
  000f88dc: cmp r3,#0x0
  000f88de: it eq
  000f88e0: add.eq r5,#0x4
  000f88e2: vldr.32 s2,[r5]
  000f88e6: vadd.f32 s0,s0,s2
  000f88ea: ldrd r3,r8,[r7,#0x20]
  000f88ee: ldr r6,[r7,#0x28]
  000f88f0: cmp r0,#0x0
  000f88f2: vcvt.u32.f32 s0,s0
  000f88f6: vmov r9,s0
  000f88fa: strh.w r9,[r1,#0x0]
  000f88fe: strh.w r4,[lr,#0x0]
  000f8902: ldrh r5,[r1,#0x0]
  000f8904: add.w r5,r5,r2, lsr #0x1
  000f8908: strh.w r5,[r10,#0x0]
  000f890c: ldrh.w r5,[lr,#0x0]
  000f8910: add.w r2,r5,r2, lsr #0x1
  000f8914: strh.w r2,[r11,#0x0]
  000f8918: ldrh r2,[r1,#0x0]
  000f891a: vmov s0,r2
  000f891e: vcvt.f32.u32 s0,s0
  000f8922: beq 0x000f89ae
  000f8924: vldr.32 s2,[pc,#0x274]
  000f8928: vadd.f32 s0,s0,s2
  000f892c: vldr.32 s2,[pc,#0x270]
  000f8930: vcvt.u32.f32 s0,s0
  000f8934: vmov r2,s0
  000f8938: strh r2,[r3,#0x0]
  000f893a: ldrh.w r2,[lr,#0x0]
  000f893e: ldr r3,[r7,#0x2c]
  000f8940: vmov s0,r2
  000f8944: vcvt.f32.u32 s0,s0
  000f8948: vadd.f32 s0,s0,s2
  000f894c: vldr.32 s2,[pc,#0x254]
  000f8950: vcvt.u32.f32 s0,s0
  000f8954: vmov r2,s0
  000f8958: strh.w r2,[r8,#0x0]
  000f895c: ldrh r2,[r1,#0x0]
  000f895e: vmov s0,r2
  000f8962: vcvt.f32.u32 s0,s0
  000f8966: vadd.f32 s0,s0,s2
  000f896a: vldr.32 s2,[pc,#0x23c]
  000f896e: vcvt.u32.f32 s0,s0
  000f8972: vmov r2,s0
  000f8976: strh r2,[r6,#0x0]
  000f8978: ldrh.w r2,[lr,#0x0]
  000f897c: vmov s0,r2
  000f8980: vcvt.f32.u32 s0,s0
  000f8984: vadd.f32 s0,s0,s2
  000f8988: vldr.32 s2,[pc,#0x220]
  000f898c: vcvt.u32.f32 s0,s0
  000f8990: vmov r2,s0
  000f8994: strh r2,[r3,#0x0]
  000f8996: ldrh r2,[r1,#0x0]
  000f8998: vmov s0,r2
  000f899c: vcvt.f32.u32 s0,s0
  000f89a0: vadd.f32 s0,s0,s2
  000f89a4: vcvt.u32.f32 s2,s0
  000f89a8: vldr.32 s0,[pc,#0x204]
  000f89ac: b 0x000f8a7a
  000f89ae: ldr r2,[0x000f8c10]
  000f89b0: adr r5,[0xf8bb4]
  000f89b2: vmov.f32 s4,0x40800000
  000f89b6: add r2,pc
  000f89b8: ldr r2,[r2,#0x0]
  000f89ba: ldrb r2,[r2,#0x0]
  000f89bc: cmp r2,#0x0
  000f89be: it eq
  000f89c0: add.eq r5,#0x4
  000f89c2: vldr.32 s2,[r5]
  000f89c6: cmp r2,#0x0
  000f89c8: vadd.f32 s0,s2,s0
  000f89cc: vmov.f32 s2,0x41000000
  000f89d0: vcvt.u32.f32 s0,s0
  000f89d4: vmov r5,s0
  000f89d8: strh r5,[r3,#0x0]
  000f89da: ldrh.w r3,[lr,#0x0]
  000f89de: it eq
  000f89e0: vmov.eq.f32 s2,s4
  000f89e4: vmov.f32 s4,0x41700000
  000f89e8: cmp r2,#0x0
  000f89ea: ldr r5,[r7,#0x2c]
  000f89ec: vmov s0,r3
  000f89f0: vcvt.f32.u32 s0,s0
  000f89f4: vadd.f32 s0,s2,s0
  000f89f8: vmov.f32 s2,0x41e00000
  000f89fc: vcvt.u32.f32 s0,s0
  000f8a00: vmov r3,s0
  000f8a04: strh.w r3,[r8,#0x0]
  000f8a08: ldrh r3,[r1,#0x0]
  000f8a0a: it eq
  000f8a0c: vmov.eq.f32 s2,s4
  000f8a10: vmov.f32 s4,0x41500000
  000f8a14: cmp r2,#0x0
  000f8a16: vmov s0,r3
  000f8a1a: vcvt.f32.u32 s0,s0
  000f8a1e: vadd.f32 s0,s2,s0
  000f8a22: vmov.f32 s2,0x41d80000
  000f8a26: vcvt.u32.f32 s0,s0
  000f8a2a: vmov r3,s0
  000f8a2e: strh r3,[r6,#0x0]
  000f8a30: ldrh.w r3,[lr,#0x0]
  000f8a34: it eq
  000f8a36: vmov.eq.f32 s2,s4
  000f8a3a: cmp r2,#0x0
  000f8a3c: vmov s0,r3
  000f8a40: vcvt.f32.u32 s0,s0
  000f8a44: vadd.f32 s0,s2,s0
  000f8a48: vcvt.u32.f32 s0,s0
  000f8a4c: vmov r3,s0
  000f8a50: strh r3,[r5,#0x0]
  000f8a52: ldrh r3,[r1,#0x0]
  000f8a54: vmov s0,r3
  000f8a58: adr r3,[0xf8bbc]
  000f8a5a: vcvt.f32.u32 s0,s0
  000f8a5e: it eq
  000f8a60: add.eq r3,#0x4
  000f8a62: vldr.32 s2,[r3]
  000f8a66: adr r3,[0xf8bc4]
  000f8a68: cmp r2,#0x0
  000f8a6a: vadd.f32 s0,s2,s0
  000f8a6e: vcvt.u32.f32 s2,s0
  000f8a72: it eq
  000f8a74: add.eq r3,#0x4
  000f8a76: vldr.32 s0,[r3]
  000f8a7a: vmov r2,s2
  000f8a7e: ldr r3,[r7,#0x38]
  000f8a80: cmp r4,r12
  000f8a82: strh r2,[r3,#0x0]
  000f8a84: ldrd r5,r8,[r7,#0x30]
  000f8a88: ldrh.w r6,[lr,#0x0]
  000f8a8c: ldr r3,[r7,#0xc]
  000f8a8e: ldr r2,[r7,#0x3c]
  000f8a90: vmov s2,r6
  000f8a94: vcvt.f32.u32 s2,s2
  000f8a98: vsub.f32 s0,s2,s0
  000f8a9c: vcvt.u32.f32 s0,s0
  000f8aa0: vmov r6,s0
  000f8aa4: strh r6,[r2,#0x0]
  000f8aa6: ble 0x000f8acc
  000f8aa8: ldr r6,[sp,#0x0]
  000f8aaa: cmp r0,#0x0
  000f8aac: str r6,[r3,#0x0]
  000f8aae: ldrh r1,[r1,#0x0]
  000f8ab0: vmov s0,r1
  000f8ab4: vcvt.f32.u32 s0,s0
  000f8ab8: beq 0x000f8af0
  000f8aba: vmov.f32 s2,0xc0000000
  000f8abe: vadd.f32 s0,s0,s2
  000f8ac2: vcvt.u32.f32 s2,s0
  000f8ac6: vldr.32 s0,[pc,#0x11c]
  000f8aca: b 0x000f8b48
  000f8acc: ldr r6,[r7,#0x8]
  000f8ace: cmp r0,#0x0
  000f8ad0: str r6,[r3,#0x0]
  000f8ad2: ldrh r1,[r1,#0x0]
  000f8ad4: vmov s0,r1
  000f8ad8: vcvt.f32.u32 s0,s0
  000f8adc: beq 0x000f8b20
  000f8ade: vldr.32 s2,[pc,#0xec]
  000f8ae2: vadd.f32 s0,s0,s2
  000f8ae6: vcvt.u32.f32 s2,s0
  000f8aea: vldr.32 s0,[pc,#0xe4]
  000f8aee: b 0x000f8b48
  000f8af0: ldr r0,[0x000f8c14]
  000f8af2: vmov.f32 s2,0xc0000000
  000f8af6: vldr.32 s4,[pc,#0xf0]
  000f8afa: adr r1,[0xf8bec]
  000f8afc: add r0,pc
  000f8afe: ldr r0,[r0,#0x0]
  000f8b00: ldrb r0,[r0,#0x0]
  000f8b02: cmp r0,#0x0
  000f8b04: it eq
  000f8b06: vmov.eq.f32 s2,s4
  000f8b0a: b 0x000f8b36
  000f8b20: ldr r0,[0x000f8c18]
  000f8b22: adr r1,[0xf8bd4]
  000f8b24: add r0,pc
  000f8b26: ldr r0,[r0,#0x0]
  000f8b28: ldrb r0,[r0,#0x0]
  000f8b2a: cmp r0,#0x0
  000f8b2c: it eq
  000f8b2e: add.eq r1,#0x4
  000f8b30: vldr.32 s2,[r1]
  000f8b34: adr r1,[0xf8bdc]
  000f8b36: vadd.f32 s0,s2,s0
  000f8b3a: vcvt.u32.f32 s2,s0
  000f8b3e: cmp r0,#0x0
  000f8b40: it eq
  000f8b42: add.eq r1,#0x4
  000f8b44: vldr.32 s0,[r1]
  000f8b48: vmov r0,s2
  000f8b4c: strh r0,[r5,#0x0]
  000f8b4e: ldrh.w r0,[lr,#0x0]
  000f8b52: vmov s2,r0
  000f8b56: vcvt.f32.u32 s2,s2
  000f8b5a: vadd.f32 s0,s0,s2
  000f8b5e: vcvt.u32.f32 s0,s0
  000f8b62: vmov r0,s0
  000f8b66: strh.w r0,[r8,#0x0]
  000f8b6a: pop.w {r3,r8,r9,r10,r11}
  000f8b6e: pop {r4,r5,r6,r7,pc}
```
