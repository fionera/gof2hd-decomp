# AbyssEngine::ImageCreateFontFromFile
elf 0x6f13c body 896
Sig: undefined __stdcall ImageCreateFontFromFile(Engine * param_1, char * param_2, ushort param_3, ImageFont * * param_4)

## decompile
```c

/* AbyssEngine::ImageCreateFontFromFile(AbyssEngine::Engine*, char const*, unsigned short,
   AbyssEngine::ImageFont**) */

void AbyssEngine::ImageCreateFontFromFile
               (Engine *param_1,char *param_2,ushort param_3,ImageFont **param_4)

{
  char *pcVar1;
  char *pcVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  ImageFont *pIVar6;
  void *pvVar7;
  uint uVar8;
  undefined4 *puVar9;
  float *pfVar10;
  ushort uVar11;
  ushort uVar12;
  int *piVar13;
  uint in_fpscr;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  ulonglong in_d2;
  float fVar18;
  float fVar19;
  ulonglong in_d3;
  undefined8 in_d4;
  ulonglong in_d5;
  undefined8 in_d6;
  uint in_s14;
  double dVar20;
  double dVar21;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 local_40;
  uint local_38;
  int local_34;
  
  piVar13 = *(int **)(DAT_0007f4cc + 0x7f154);
  local_34 = *piVar13;
  uVar3 = 0xfffffffc;
  local_60 = in_d2;
  local_58 = in_d3;
  local_50 = in_d4;
  uStack_48 = in_d5;
  local_40 = in_d6;
  local_38 = in_s14;
  if ((param_1 != (Engine *)0x0) && (param_2 != (char *)0x0)) {
    local_38 = 0;
    local_40 = 0;
    uStack_48 = in_d5 & 0xffffffffffffff;
    iVar4 = AEFile::OpenRead((AEFile *)param_2,(char *)&local_38,&local_38);
    if (iVar4 != 0) {
      local_50 = CONCAT44(DAT_0007f4bc,0x2a2a2a2a);
      iVar4 = AEFile::Read(8,&local_50,local_38);
      if (iVar4 == 0) {
LAB_0007f496:
        ImageFontRelease(param_1,param_4);
      }
      else {
        uVar5 = 0;
        do {
          if (7 < uVar5) {
            iVar4 = AEFile::Read(1,(void *)((int)&uStack_48 + 7),local_38);
            if ((((iVar4 == 0) ||
                 (iVar4 = AEFile::Read(2,(void *)((int)&local_40 + 2),local_38), iVar4 == 0)) ||
                (iVar4 = AEFile::Read(2,&local_40,local_38), iVar4 == 0)) ||
               (iVar4 = AEFile::Read(2,(void *)((int)&local_40 + 4),local_38), iVar4 == 0))
            goto LAB_0007f496;
            iVar4 = AEFile::Skip((uint)local_40._4_2_ << 3,local_38);
            if (iVar4 == 0) goto LAB_0007f496;
            uVar5 = (uint)uStack_48._7_1_;
            uVar8 = uVar5 - 3;
            if (uVar8 < 0x1f) {
              if ((1 << (uVar8 & 0xff) & DAT_0007f4c0) != 0) goto LAB_0007f246;
              if (uVar8 != 0) goto LAB_0007f232;
LAB_0007f260:
              uVar5 = (uint)local_40._2_2_ * (uint)(ushort)local_40 * 4;
LAB_0007f26e:
              iVar4 = AEFile::Skip(uVar5,local_38);
              if (iVar4 == 0) goto LAB_0007f496;
            }
            else {
LAB_0007f232:
              if ((uVar5 - 0x24 < 0x1f) && ((1 << (uVar5 - 0x24 & 0xff) & DAT_0007f4c4) != 0)) {
LAB_0007f246:
                iVar4 = AEFile::Read(4,(void *)((int)&local_58 + 4),local_38);
                if (iVar4 != 0) {
                  uVar5 = local_58._4_4_;
                  goto LAB_0007f26e;
                }
                goto LAB_0007f496;
              }
              if (uVar5 == 1) goto LAB_0007f260;
            }
            iVar4 = AEFile::Read(2,(void *)((int)&local_40 + 6),local_38);
            if ((iVar4 == 0) || (uVar11 = local_40._6_2_, local_40._6_2_ <= param_3))
            goto LAB_0007f496;
            pIVar6 = operator_new(0x14);
            uVar12 = 0;
            *(undefined2 *)pIVar6 = 0;
            *(undefined8 *)(pIVar6 + 4) = 0;
            *(undefined8 *)(pIVar6 + 0xc) = 0;
            *param_4 = pIVar6;
            goto LAB_0007f47e;
          }
          pcVar1 = (char *)((int)&local_50 + uVar5);
          pcVar2 = (char *)(DAT_0007f4d0 + 0x7f1b8 + uVar5);
          uVar5 = uVar5 + 1;
        } while (*pcVar2 == *pcVar1);
      }
    }
    uVar3 = 0xffffffff;
  }
LAB_0007f4a2:
  if (*piVar13 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
LAB_0007f47e:
  if (uVar11 <= uVar12) goto code_r0x0007f48c;
  if (uVar12 == param_3) {
    iVar4 = AEFile::Read(2,*param_4,local_38);
    if (iVar4 == 0) goto LAB_0007f496;
    pIVar6 = *param_4;
    pvVar7 = operator_new__((uint)*(ushort *)pIVar6 << 1);
    *(void **)(pIVar6 + 4) = pvVar7;
    iVar4 = AEFile::Read((uint)*(ushort *)*param_4 << 1,*(void **)(*param_4 + 4),local_38);
    if (iVar4 == 0) goto LAB_0007f496;
    pIVar6 = *param_4;
    pvVar7 = operator_new__((uint)*(ushort *)pIVar6 << 2);
    *(void **)(pIVar6 + 0xc) = pvVar7;
    uVar5 = 0;
    pIVar6 = *param_4;
    while (uVar5 < *(ushort *)pIVar6) {
      local_60 = CONCAT44(local_60._4_4_,*(int *)(pIVar6 + 0xc) + uVar5 * 4);
      iVar4 = MeshCreate(param_1,4,2,0x13);
      if (iVar4 != 1) goto LAB_0007f496;
      local_58 = local_58 & 0xffff000000000000;
      local_60 = local_60 & 0xffffffffffff;
      iVar4 = AEFile::Read(2,(void *)((int)&local_58 + 4),local_38);
      if ((((iVar4 == 0) ||
           (iVar4 = AEFile::Read(2,(void *)((int)&local_58 + 2),local_38), iVar4 == 0)) ||
          (iVar4 = AEFile::Read(2,&local_58,local_38), iVar4 == 0)) ||
         (iVar4 = AEFile::Read(2,(void *)((int)&local_60 + 6),local_38), iVar4 == 0))
      goto LAB_0007f496;
      pIVar6 = *param_4;
      iVar4 = *(int *)(*(int *)(pIVar6 + 0xc) + uVar5 * 4);
      uVar5 = uVar5 + 1;
      puVar9 = *(undefined4 **)(iVar4 + 4);
      *puVar9 = 0;
      puVar9[1] = 0;
      puVar9[2] = 0;
      fVar14 = (float)VectorUnsignedToFloat((uint)(ushort)local_58,(byte)(in_fpscr >> 0x16) & 3);
      puVar9[4] = 0;
      puVar9[5] = 0;
      puVar9[3] = fVar14;
      puVar9[6] = fVar14;
      fVar15 = (float)VectorUnsignedToFloat((uint)local_60._6_2_,(byte)(in_fpscr >> 0x16) & 3);
      puVar9[8] = 0;
      puVar9[9] = 0;
      puVar9[0xb] = 0;
      puVar9[7] = fVar15;
      puVar9[10] = fVar15;
      dVar20 = (double)VectorUnsignedToFloat((uint)(ushort)local_40,(byte)(in_fpscr >> 0x16) & 3);
      dVar21 = (double)VectorUnsignedToFloat((uint)local_40._2_2_,(byte)(in_fpscr >> 0x16) & 3);
      fVar16 = (float)VectorUnsignedToFloat((uint)local_58._4_2_,(byte)(in_fpscr >> 0x16) & 3);
      fVar18 = (float)VectorUnsignedToFloat((uint)local_58._2_2_,(byte)(in_fpscr >> 0x16) & 3);
      pfVar10 = *(float **)(iVar4 + 8);
      fVar17 = fVar16 * (float)(1.0 / dVar21);
      fVar19 = fVar18 * (float)(1.0 / dVar20);
      fVar15 = (fVar15 + fVar18) * (float)(1.0 / dVar20);
      fVar14 = (fVar14 + fVar16) * (float)(1.0 / dVar21);
      *pfVar10 = fVar17;
      pfVar10[1] = fVar19;
      pfVar10[2] = fVar14;
      pfVar10[3] = fVar19;
      pfVar10[4] = fVar14;
      pfVar10[5] = fVar15;
      pfVar10[6] = fVar17;
      pfVar10[7] = fVar15;
      puVar9 = *(undefined4 **)(iVar4 + 0x2c);
      *puVar9 = 0x20000;
      puVar9[1] = 1;
      puVar9[2] = DAT_0007f4c8;
    }
  }
  else {
    local_58._0_6_ = (uint6)(uint)local_58;
    iVar4 = AEFile::Read(2,(void *)((int)&local_58 + 4),local_38);
    if (iVar4 == 0) goto LAB_0007f496;
    iVar4 = AEFile::Skip((uint)local_58._4_2_ * 10,local_38);
    if (iVar4 == 0) goto LAB_0007f496;
  }
  uVar12 = uVar12 + 1;
  uVar11 = local_40._6_2_;
  goto LAB_0007f47e;
code_r0x0007f48c:
  AEFile::Close(local_38);
  uVar3 = 1;
  goto LAB_0007f4a2;
}

```

## target disasm
```
  0007f13c: push {r4,r5,r6,r7,lr}
  0007f13e: add r7,sp,#0xc
  0007f140: push {r7,r8,r9,r10,r11}
  0007f144: vpush {d2,d3,d4,d5,d6,d7,d8}
  0007f148: mov r5,r0
  0007f14a: ldr r0,[0x0007f4cc]
  0007f14c: mov r4,r3
  0007f14e: mov r11,r2
  0007f150: add r0,pc
  0007f152: cmp r5,#0x0
  0007f154: ldr.w r10,[r0,#0x0]
  0007f158: ldr.w r0,[r10,#0x0]
  0007f15c: str r0,[sp,#0x2c]
  0007f15e: mvn r0,#0x3
  0007f162: beq.w 0x0007f4a2
  0007f166: cmp r1,#0x0
  0007f168: beq.w 0x0007f4a2
  0007f16c: add r2,sp,#0x28
  0007f16e: movs r0,#0x0
  0007f170: str r0,[sp,#0x28]
  0007f172: strh r0,[r7,#-0x32]
  0007f176: strh.w r0,[sp,#0x24]
  0007f17a: strh r0,[r7,#-0x36]
  0007f17e: strh.w r0,[sp,#0x20]
  0007f182: strb.w r0,[r7,#-0x39]
  0007f186: mov r0,r1
  0007f188: mov r1,r2
  0007f18a: blx 0x0006ecd4
  0007f18e: cmp r0,#0x0
  0007f190: beq.w 0x0007f49e
  0007f194: ldr r0,[0x0007f4bc]
  0007f196: add r6,sp,#0x10
  0007f198: ldr r2,[sp,#0x28]
  0007f19a: str r0,[sp,#0x14]
  0007f19c: mov.w r0,#0x2a2a2a2a
  0007f1a0: str r0,[sp,#0x10]
  0007f1a2: movs r0,#0x8
  0007f1a4: mov r1,r6
  0007f1a6: blx 0x0006eb90
  0007f1aa: cmp r0,#0x0
  0007f1ac: beq.w 0x0007f496
  0007f1b0: ldr r1,[0x0007f4d0]
  0007f1b2: movs r0,#0x0
  0007f1b4: add r1,pc
  0007f1b6: cmp r0,#0x7
  0007f1b8: bhi 0x0007f1c6
  0007f1ba: ldrb r2,[r6,r0]
  0007f1bc: ldrb r3,[r1,r0]
  0007f1be: adds r0,#0x1
  0007f1c0: cmp r3,r2
  0007f1c2: beq 0x0007f1b6
  0007f1c4: b 0x0007f49e
  0007f1c6: ldr r2,[sp,#0x28]
  0007f1c8: sub.w r1,r7,#0x39
  0007f1cc: movs r0,#0x1
  0007f1ce: blx 0x0006eb90
  0007f1d2: cmp r0,#0x0
  0007f1d4: beq.w 0x0007f496
  0007f1d8: ldr r2,[sp,#0x28]
  0007f1da: sub.w r1,r7,#0x36
  0007f1de: movs r0,#0x2
  0007f1e0: blx 0x0006eb90
  0007f1e4: cmp r0,#0x0
  0007f1e6: beq.w 0x0007f496
  0007f1ea: ldr r2,[sp,#0x28]
  0007f1ec: add r1,sp,#0x20
  0007f1ee: movs r0,#0x2
  0007f1f0: blx 0x0006eb90
  0007f1f4: cmp r0,#0x0
  0007f1f6: beq.w 0x0007f496
  0007f1fa: ldr r2,[sp,#0x28]
  0007f1fc: add r1,sp,#0x24
  0007f1fe: movs r0,#0x2
  0007f200: blx 0x0006eb90
  0007f204: cmp r0,#0x0
  0007f206: beq.w 0x0007f496
  0007f20a: ldrh.w r0,[sp,#0x24]
  0007f20e: ldr r1,[sp,#0x28]
  0007f210: lsls r0,r0,#0x3
  0007f212: blx 0x0006f1f0
  0007f216: cmp r0,#0x0
  0007f218: beq.w 0x0007f496
  0007f21c: ldrb.w r0,[r7,#-0x39]
  0007f220: subs r1,r0,#0x3
  0007f222: cmp r1,#0x1e
  0007f224: bhi 0x0007f232
  0007f226: ldr r2,[0x0007f4c0]
  0007f228: movs r3,#0x1
  0007f22a: lsls r3,r1
  0007f22c: tst r3,r2
  0007f22e: bne 0x0007f246
  0007f230: cbz r1,0x0007f260
  0007f232: sub.w r1,r0,#0x24
  0007f236: cmp r1,#0x1e
  0007f238: bhi 0x0007f25c
  0007f23a: ldr r2,[0x0007f4c4]
  0007f23c: movs r3,#0x1
  0007f23e: lsl.w r1,r3,r1
  0007f242: tst r1,r2
  0007f244: beq 0x0007f25c
  0007f246: ldr r2,[sp,#0x28]
  0007f248: add r1,sp,#0xc
  0007f24a: movs r0,#0x4
  0007f24c: blx 0x0006eb90
  0007f250: cmp r0,#0x0
  0007f252: beq.w 0x0007f496
  0007f256: ldr r1,[sp,#0x28]
  0007f258: ldr r0,[sp,#0xc]
  0007f25a: b 0x0007f26e
  0007f25c: cmp r0,#0x1
  0007f25e: bne 0x0007f278
  0007f260: ldrh.w r0,[sp,#0x20]
  0007f264: ldrh.w r1,[r7,#-0x36]
  0007f268: muls r0,r1
  0007f26a: ldr r1,[sp,#0x28]
  0007f26c: lsls r0,r0,#0x2
  0007f26e: blx 0x0006f1f0
  0007f272: cmp r0,#0x0
  0007f274: beq.w 0x0007f496
  0007f278: ldr r2,[sp,#0x28]
  0007f27a: sub.w r1,r7,#0x32
  0007f27e: movs r0,#0x2
  0007f280: blx 0x0006eb90
  0007f284: cmp r0,#0x0
  0007f286: beq.w 0x0007f496
  0007f28a: ldrh.w r8,[r7,#-0x32]
  0007f28e: cmp r8,r11
  0007f290: bls.w 0x0007f496
  0007f294: movs r0,#0x14
  0007f296: blx 0x0006eb24
  0007f29a: vmov.f64 d8,0x3ff0000000000000
  0007f29e: movs r6,#0x0
  0007f2a0: mov r1,r0
  0007f2a2: mov.w r9,#0x0
  0007f2a6: strh r6,[r1],#0x4
  0007f2aa: vmov.i32 q8,#0x0
  0007f2ae: vst1.32 {d16,d17},[r1]
  0007f2b2: str r0,[r4,#0x0]
  0007f2b4: b 0x0007f47e
  0007f2b6: cmp r0,r11
  0007f2b8: bne.w 0x0007f454
  0007f2bc: ldr r2,[sp,#0x28]
  0007f2be: movs r0,#0x2
  0007f2c0: ldr r1,[r4,#0x0]
  0007f2c2: blx 0x0006eb90
  0007f2c6: cmp r0,#0x0
  0007f2c8: beq.w 0x0007f496
  0007f2cc: ldr.w r8,[r4,#0x0]
  0007f2d0: ldrh.w r0,[r8,#0x0]
  0007f2d4: lsls r0,r0,#0x1
  0007f2d6: blx 0x0006ec08
  0007f2da: str.w r0,[r8,#0x4]
  0007f2de: ldr r0,[r4,#0x0]
  0007f2e0: ldr r2,[sp,#0x28]
  0007f2e2: ldrh r3,[r0,#0x0]
  0007f2e4: ldr r1,[r0,#0x4]
  0007f2e6: lsls r0,r3,#0x1
  0007f2e8: blx 0x0006eb90
  0007f2ec: cmp r0,#0x0
  0007f2ee: beq.w 0x0007f496
  0007f2f2: ldr.w r8,[r4,#0x0]
  0007f2f6: ldrh.w r0,[r8,#0x0]
  0007f2fa: lsls r0,r0,#0x2
  0007f2fc: blx 0x0006ec08
  0007f300: str.w r0,[r8,#0xc]
  0007f304: mov.w r8,#0x0
  0007f308: ldr r0,[r4,#0x0]
  0007f30a: b 0x0007f44a
  0007f30c: ldr r0,[r0,#0xc]
  0007f30e: movs r1,#0x4
  0007f310: movs r2,#0x2
  0007f312: movs r3,#0x13
  0007f314: add.w r0,r0,r8, lsl #0x2
  0007f318: str r0,[sp,#0x0]
  0007f31a: mov r0,r5
  0007f31c: blx 0x0006f1fc
  0007f320: cmp r0,#0x1
  0007f322: bne.w 0x0007f496
  0007f326: ldr r2,[sp,#0x28]
  0007f328: add r1,sp,#0xc
  0007f32a: movs r0,#0x2
  0007f32c: strh.w r6,[sp,#0xc]
  0007f330: strh r6,[r7,#-0x4e]
  0007f334: strh.w r6,[sp,#0x8]
  0007f338: strh r6,[r7,#-0x52]
  0007f33c: blx 0x0006eb90
  0007f340: cmp r0,#0x0
  0007f342: beq.w 0x0007f496
  0007f346: ldr r2,[sp,#0x28]
  0007f348: sub.w r1,r7,#0x4e
  0007f34c: movs r0,#0x2
  0007f34e: blx 0x0006eb90
  0007f352: cmp r0,#0x0
  0007f354: beq.w 0x0007f496
  0007f358: ldr r2,[sp,#0x28]
  0007f35a: add r1,sp,#0x8
  0007f35c: movs r0,#0x2
  0007f35e: blx 0x0006eb90
  0007f362: cmp r0,#0x0
  0007f364: beq.w 0x0007f496
  0007f368: ldr r2,[sp,#0x28]
  0007f36a: sub.w r1,r7,#0x52
  0007f36e: movs r0,#0x2
  0007f370: blx 0x0006eb90
  0007f374: cmp r0,#0x0
  0007f376: beq.w 0x0007f496
  0007f37a: ldr r0,[r4,#0x0]
  0007f37c: ldr r1,[r0,#0xc]
  0007f37e: ldr.w r1,[r1,r8,lsl #0x2]
  0007f382: add.w r8,r8,#0x1
  0007f386: ldr r2,[r1,#0x4]
  0007f388: strd r6,r6,[r2,#0x0]
  0007f38c: str r6,[r2,#0x8]
  0007f38e: ldrh.w r3,[sp,#0x8]
  0007f392: vmov s0,r3
  0007f396: vcvt.f32.u32 s0,s0
  0007f39a: strd r6,r6,[r2,#0x10]
  0007f39e: vstr.32 s0,[r2,#0xc]
  0007f3a2: vstr.32 s0,[r2,#0x18]
  0007f3a6: ldrh.w r3,[r7,#-0x52]
  0007f3aa: vmov s2,r3
  0007f3ae: vcvt.f32.u32 s2,s2
  0007f3b2: strd r6,r6,[r2,#0x20]
  0007f3b6: str r6,[r2,#0x2c]
  0007f3b8: vstr.32 s2,[r2,#0x1c]
  0007f3bc: vstr.32 s2,[r2,#0x28]
  0007f3c0: ldrh.w r2,[sp,#0x20]
  0007f3c4: vmov s4,r2
  0007f3c8: vcvt.f64.u32 d16,s4
  0007f3cc: ldrh.w r2,[r7,#-0x36]
  0007f3d0: vmov s4,r2
  0007f3d4: vdiv.f64 d16,d8,d16
  0007f3d8: vcvt.f64.u32 d17,s4
  0007f3dc: ldrh.w r2,[sp,#0xc]
  0007f3e0: ldrh.w r3,[r7,#-0x4e]
  0007f3e4: vdiv.f64 d17,d8,d17
  0007f3e8: vmov s4,r2
  0007f3ec: vmov s6,r3
  0007f3f0: vcvt.f32.u32 s4,s4
  0007f3f4: vcvt.f32.u32 s6,s6
  0007f3f8: ldr r2,[r1,#0x8]
  0007f3fa: vcvt.f32.f64 s8,d16
  0007f3fe: vadd.f32 s2,s2,s6
  0007f402: vcvt.f32.f64 s10,d17
  0007f406: vadd.f32 s0,s0,s4
  0007f40a: vmul.f32 s4,s4,s10
  0007f40e: vmul.f32 s6,s6,s8
  0007f412: vmul.f32 s2,s2,s8
  0007f416: vmul.f32 s0,s0,s10
  0007f41a: vstr.32 s4,[r2]
  0007f41e: vstr.32 s6,[r2,#0x4]
  0007f422: vstr.32 s0,[r2,#0x8]
  0007f426: vstr.32 s6,[r2,#0xc]
  0007f42a: vstr.32 s0,[r2,#0x10]
  0007f42e: vstr.32 s2,[r2,#0x14]
  0007f432: vstr.32 s4,[r2,#0x18]
  0007f436: vstr.32 s2,[r2,#0x1c]
  0007f43a: mov.w r2,#0x20000
  0007f43e: ldr r1,[r1,#0x2c]
  0007f440: str r2,[r1,#0x0]
  0007f442: movs r2,#0x1
  0007f444: str r2,[r1,#0x4]
  0007f446: ldr r2,[0x0007f4c8]
  0007f448: str r2,[r1,#0x8]
  0007f44a: ldrh r1,[r0,#0x0]
  0007f44c: cmp r8,r1
  0007f44e: bcc.w 0x0007f30c
  0007f452: b 0x0007f476
  0007f454: ldr r2,[sp,#0x28]
  0007f456: add r1,sp,#0xc
  0007f458: movs r0,#0x2
  0007f45a: strh.w r6,[sp,#0xc]
  0007f45e: blx 0x0006eb90
  0007f462: cbz r0,0x0007f496
  0007f464: ldrh.w r0,[sp,#0xc]
  0007f468: ldr r1,[sp,#0x28]
  0007f46a: add.w r0,r0,r0, lsl #0x2
  0007f46e: lsls r0,r0,#0x1
  0007f470: blx 0x0006f1f0
  0007f474: cbz r0,0x0007f496
  0007f476: ldrh.w r8,[r7,#-0x32]
  0007f47a: add.w r9,r9,#0x1
  0007f47e: uxth.w r1,r8
  0007f482: uxth.w r0,r9
  0007f486: cmp r0,r1
  0007f488: bcc.w 0x0007f2b6
  0007f48c: ldr r0,[sp,#0x28]
  0007f48e: blx 0x0006ece0
  0007f492: movs r0,#0x1
  0007f494: b 0x0007f4a2
  0007f496: mov r0,r5
  0007f498: mov r1,r4
  0007f49a: blx 0x0006f208
  0007f49e: mov.w r0,#0xffffffff
  0007f4a2: ldr r1,[sp,#0x2c]
  0007f4a4: ldr.w r2,[r10,#0x0]
  0007f4a8: subs r1,r2,r1
  0007f4aa: itttt eq
  0007f4ac: vpop.eq {d2,d3,d4,d5,d6,d7,d8}
  0007f4b0: add.eq sp,#0x4
  0007f4b2: pop.eq.w {r8,r9,r10,r11}
  0007f4b6: pop.eq {r4,r5,r6,r7,pc}
  0007f4b8: blx 0x0006e824
```
