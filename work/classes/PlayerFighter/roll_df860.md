# PlayerFighter::roll
elf 0xdf860 body 434
Sig: undefined __stdcall roll(int param_1)

## decompile
```c

/* PlayerFighter::roll(int) */

void PlayerFighter::roll(int param_1)

{
  byte bVar1;
  undefined1 uVar2;
  int iVar3;
  float *pfVar4;
  int in_r1;
  int *piVar5;
  uint in_fpscr;
  uint uVar6;
  uint uVar7;
  float fVar8;
  float fVar9;
  float extraout_s1;
  float fVar10;
  float fVar11;
  AEMath aAStack_58 [60];
  int local_1c;
  
  piVar5 = *(int **)(DAT_000efa4c + 0xef874);
  local_1c = *piVar5;
  if (*(char *)(param_1 + 0x100) == '\0') goto LAB_000ef9fc;
  iVar3 = AEGeometry::getMatrix();
  fVar8 = *(float *)(iVar3 + 0x10);
  fVar10 = *(float *)(iVar3 + 0x14);
  uVar7 = in_fpscr & 0xfffffff | (uint)(fVar10 < 0.0) << 0x1f | (uint)(fVar10 == 0.0) << 0x1e;
  fVar9 = -fVar8;
  if (0.0 < fVar8) {
    fVar9 = fVar8;
  }
  if (0x3b < in_r1) {
    in_r1 = 0x3c;
  }
  bVar1 = (byte)(uVar7 >> 0x18);
  if ((!(bool)(bVar1 >> 6 & 1) && (bool)(bVar1 >> 7) == NAN(fVar10)) &&
     (uVar7 = in_fpscr & 0xfffffff, fVar9 < DAT_000efa14)) {
    AbyssEngine::AEMath::MatrixIdentity(aAStack_58,(Matrix *)(param_1 + 600));
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)(param_1 + 600),(Matrix *)aAStack_58);
    *(undefined1 *)(param_1 + 0x100) = 0;
    *(undefined2 *)(param_1 + 0x254) = 0;
    goto LAB_000ef9fc;
  }
  if (*(char *)(param_1 + 0x254) == '\0') {
    uVar6 = uVar7 & 0xfffffff;
    uVar7 = uVar6 | (uint)(fVar8 < 0.0) << 0x1f;
    if ((fVar8 == 0.0 || SUB41(uVar7 >> 0x1f,0) != NAN(fVar8)) ||
       (fVar11 = DAT_000efa48, *(char *)(param_1 + 0x255) != '\x01')) {
      uVar7 = uVar6;
      if (0.0 <= fVar8) {
        if ((NAN(fVar8)) || (fVar11 = DAT_000efa24, 0.0 <= fVar10)) {
LAB_000ef96e:
          uVar7 = uVar6 | (uint)(fVar8 < 0.0) << 0x1f;
          fVar11 = fVar9;
          if ((fVar8 != 0.0 && SUB41(uVar7 >> 0x1f,0) == NAN(fVar8)) &&
             (uVar7 = uVar6 | (uint)(fVar10 < 0.0) << 0x1f,
             fVar10 != 0.0 && SUB41(uVar7 >> 0x1f,0) == NAN(fVar10))) {
            pfVar4 = (float *)&UNK_000efa3c;
            fVar10 = DAT_000efa38;
            if (DAT_000efa38 < fVar8) {
              pfVar4 = (float *)&DAT_000efa40;
            }
LAB_000ef9a8:
            uVar7 = uVar6;
            fVar11 = *pfVar4;
          }
        }
      }
      else {
        fVar11 = DAT_000efa44;
        if (*(char *)(param_1 + 0x255) == '\x02') goto LAB_000ef944;
        fVar11 = DAT_000efa28;
        if (0.0 <= fVar10) {
          if (NAN(fVar10)) goto LAB_000ef96e;
          pfVar4 = (float *)&DAT_000efa30;
          uVar6 = uVar6 | (uint)(fVar8 < DAT_000efa2c) << 0x1f;
          fVar10 = DAT_000efa2c;
          if (fVar8 != DAT_000efa2c && SUB41(uVar6 >> 0x1f,0) == (NAN(fVar8) || NAN(DAT_000efa2c)))
          {
            pfVar4 = (float *)&DAT_000efa34;
          }
          goto LAB_000ef9a8;
        }
      }
    }
    else {
LAB_000ef944:
      *(undefined1 *)(param_1 + 0x254) = 1;
    }
  }
  else {
    uVar7 = uVar7 & 0xfffffff;
    pfVar4 = (float *)&DAT_000efa1c;
    if (!NAN(fVar10)) {
      pfVar4 = (float *)&DAT_000efa20;
    }
    fVar10 = *pfVar4;
    fVar11 = DAT_000efa18;
    if (fVar8 < 0.0) {
      fVar11 = fVar10;
    }
  }
  if ((fVar8 != 0.0) && (!NAN(fVar8))) {
    uVar2 = 2;
    if (fVar8 < 0.0) {
      uVar2 = 1;
    }
    *(undefined1 *)(param_1 + 0x255) = uVar2;
  }
  fVar9 = (float)VectorSignedToFloat(in_r1,(byte)((uVar7 & 0xfffffff) >> 0x16) & 3);
  *(undefined1 *)(param_1 + 0x100) = 1;
  AbyssEngine::AEMath::MatrixSetRotation((Matrix *)aAStack_58,fVar11 * fVar9,extraout_s1,fVar10);
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(param_1 + 600),(Matrix *)aAStack_58);
LAB_000ef9fc:
  if (*piVar5 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000ef860: push {r4,r5,r6,r7,lr}
  000ef862: add r7,sp,#0xc
  000ef864: push.w r8
  000ef868: sub sp,#0x48
  000ef86a: mov r4,r0
  000ef86c: ldr r0,[0x000efa4c]
  000ef86e: mov r5,r1
  000ef870: add r0,pc
  000ef872: ldr.w r8,[r0,#0x0]
  000ef876: ldr.w r0,[r8,#0x0]
  000ef87a: str r0,[sp,#0x44]
  000ef87c: ldrb.w r0,[r4,#0x100]
  000ef880: cmp r0,#0x0
  000ef882: beq.w 0x000ef9fc
  000ef886: ldr r0,[r4,#0x8]
  000ef888: blx 0x000721cc
  000ef88c: vldr.32 s0,[r0,#0x10]
  000ef890: vldr.32 s2,[r0,#0x14]
  000ef894: vcmpe.f32 s0,#0
  000ef898: vmrs apsr,fpscr
  000ef89c: vneg.f32 s4,s0
  000ef8a0: vcmpe.f32 s2,#0
  000ef8a4: it gt
  000ef8a6: vmov.gt.f32 s4,s0
  000ef8aa: cmp r5,#0x3c
  000ef8ac: it ge
  000ef8ae: mov.ge r5,#0x3c
  000ef8b0: vmrs apsr,fpscr
  000ef8b4: ble 0x000ef8e6
  000ef8b6: vldr.32 s6,[pc,#0x15c]
  000ef8ba: vcmpe.f32 s4,s6
  000ef8be: vmrs apsr,fpscr
  000ef8c2: bpl 0x000ef8e6
  000ef8c4: add.w r5,r4,#0x258
  000ef8c8: add r6,sp,#0x8
  000ef8ca: mov r0,r6
  000ef8cc: mov r1,r5
  000ef8ce: blx 0x0006fb2c
  000ef8d2: mov r0,r5
  000ef8d4: mov r1,r6
  000ef8d6: blx 0x0006f148
  000ef8da: movs r0,#0x0
  000ef8dc: strb.w r0,[r4,#0x100]
  000ef8e0: strh.w r0,[r4,#0x254]
  000ef8e4: b 0x000ef9fc
  000ef8e6: ldrb.w r0,[r4,#0x254]
  000ef8ea: cbz r0,0x000ef912
  000ef8ec: vcmpe.f32 s2,s2
  000ef8f0: adr r0,[0xefa1c]
  000ef8f2: vmrs apsr,fpscr
  000ef8f6: vcmpe.f32 s0,#0
  000ef8fa: vldr.32 s4,[pc,#0x11c]
  000ef8fe: it vc
  000ef900: add.vc r0,#0x4
  000ef902: vmrs apsr,fpscr
  000ef906: vldr.32 s2,[r0]
  000ef90a: it mi
  000ef90c: vmov.mi.f32 s4,s2
  000ef910: b 0x000ef9ac
  000ef912: vcmpe.f32 s0,#0
  000ef916: vmrs apsr,fpscr
  000ef91a: ble 0x000ef92c
  000ef91c: ldrb.w r0,[r4,#0x255]
  000ef920: cmp r0,#0x1
  000ef922: bne 0x000ef92c
  000ef924: movs r0,#0x1
  000ef926: vldr.32 s4,[pc,#0x120]
  000ef92a: b 0x000ef944
  000ef92c: vcmpe.f32 s0,#0
  000ef930: vmrs apsr,fpscr
  000ef934: bpl 0x000ef94a
  000ef936: ldrb.w r0,[r4,#0x255]
  000ef93a: cmp r0,#0x2
  000ef93c: bne 0x000ef95c
  000ef93e: movs r0,#0x1
  000ef940: vldr.32 s4,[pc,#0x100]
  000ef944: strb.w r0,[r4,#0x254]
  000ef948: b 0x000ef9ac
  000ef94a: blt 0x000ef96e
  000ef94c: vcmpe.f32 s2,#0
  000ef950: vmrs apsr,fpscr
  000ef954: bpl 0x000ef96e
  000ef956: vldr.32 s4,[pc,#0xcc]
  000ef95a: b 0x000ef9ac
  000ef95c: vcmpe.f32 s2,#0
  000ef960: vmrs apsr,fpscr
  000ef964: bpl 0x000ef96c
  000ef966: vldr.32 s4,[pc,#0xc0]
  000ef96a: b 0x000ef9ac
  000ef96c: bge 0x000ef996
  000ef96e: vcmpe.f32 s0,#0
  000ef972: vmrs apsr,fpscr
  000ef976: ble 0x000ef9ac
  000ef978: vcmpe.f32 s2,#0
  000ef97c: vmrs apsr,fpscr
  000ef980: ble 0x000ef9ac
  000ef982: vldr.32 s2,[pc,#0xb4]
  000ef986: adr r0,[0xefa3c]
  000ef988: vcmpe.f32 s0,s2
  000ef98c: vmrs apsr,fpscr
  000ef990: it hi
  000ef992: add.hi r0,#0x4
  000ef994: b 0x000ef9a8
  000ef996: vldr.32 s2,[pc,#0x94]
  000ef99a: adr r0,[0xefa30]
  000ef99c: vcmpe.f32 s0,s2
  000ef9a0: vmrs apsr,fpscr
  000ef9a4: it gt
  000ef9a6: add.gt r0,#0x4
  000ef9a8: vldr.32 s4,[r0]
  000ef9ac: vcmpe.f32 s0,#0
  000ef9b0: vmrs apsr,fpscr
  000ef9b4: beq 0x000ef9cc
  000ef9b6: bvs 0x000ef9cc
  000ef9b8: b 0x000ef9ba
  000ef9ba: vcmpe.f32 s0,#0
  000ef9be: movs r0,#0x2
  000ef9c0: vmrs apsr,fpscr
  000ef9c4: it mi
  000ef9c6: mov.mi r0,#0x1
  000ef9c8: strb.w r0,[r4,#0x255]
  000ef9cc: vmov s0,r5
  000ef9d0: movs r0,#0x1
  000ef9d2: add r5,sp,#0x8
  000ef9d4: movs r2,#0x0
  000ef9d6: vcvt.f32.s32 s0,s0
  000ef9da: strb.w r0,[r4,#0x100]
  000ef9de: add.w r4,r4,#0x258
  000ef9e2: mov r0,r5
  000ef9e4: movs r3,#0x0
  000ef9e6: mov r1,r4
  000ef9e8: vmul.f32 s0,s4,s0
  000ef9ec: vstr.32 s0,[sp]
  000ef9f0: blx 0x00072094
  000ef9f4: mov r0,r4
  000ef9f6: mov r1,r5
  000ef9f8: blx 0x0006f148
  000ef9fc: ldr r0,[sp,#0x44]
  000ef9fe: ldr.w r1,[r8,#0x0]
  000efa02: subs r0,r1,r0
  000efa04: ittt eq
  000efa06: add.eq sp,#0x48
  000efa08: pop.eq.w r8
  000efa0c: pop.eq {r4,r5,r6,r7,pc}
  000efa0e: blx 0x0006e824
```
