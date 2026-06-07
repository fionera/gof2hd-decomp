# HackingGame::render2D
elf 0x14f1e4 body 2054
Sig: undefined __thiscall render2D(HackingGame * this)

## decompile
```c

/* HackingGame::render2D() */

void __thiscall HackingGame::render2D(HackingGame *this)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  PaintCanvas *pPVar10;
  uint *puVar11;
  code *pcVar12;
  int *piVar13;
  int iVar14;
  int *piVar15;
  int *piVar16;
  uint in_fpscr;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  int local_8c;
  int local_88;
  int local_6c [13];
  
  piVar9 = *(int **)(DAT_0015f554 + 0x15f1fc);
  puVar11 = *(uint **)(DAT_0015f558 + 0x15f1fe);
  local_6c[0xc] = *piVar9;
  AbyssEngine::PaintCanvas::SetColor(*puVar11);
  uVar4 = 0;
  do {
    if (5 < uVar4) {
      bVar1 = true;
      goto LAB_0015f22a;
    }
    iVar2 = uVar4 * 4;
    iVar7 = uVar4 * 4;
    uVar4 = uVar4 + 1;
  } while (*(int *)(this + iVar7 + 0x1c) == *(int *)(this + iVar2 + 4));
  bVar1 = false;
LAB_0015f22a:
  iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar11);
  uVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
  for (iVar7 = 0; iVar7 != 6; iVar7 = iVar7 + 1) {
    local_6c[iVar7 * 2] = 0;
    local_6c[iVar7 * 2 + 1] = 0;
  }
  if (this[0x128] == (HackingGame)0x0) {
    if (this[0x129] != (HackingGame)0x0) {
      fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(this + 300),(byte)(in_fpscr >> 0x16) & 3);
      fVar20 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
      fVar19 = 1.0;
      if (fVar17 / DAT_0015f55c < 1.0) {
        fVar19 = fVar17 / DAT_0015f55c;
      }
      iVar7 = 5;
      fVar18 = (float)VectorSignedToFloat(iVar2,(byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
      local_6c[5] = (int)(fVar19 * fVar20);
      fVar17 = fVar19 * fVar18;
      local_6c[2] = (int)(fVar19 * fVar18);
      local_6c[9] = (int)-(fVar19 * fVar20);
      goto LAB_0015f33e;
    }
  }
  else {
    fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(this + 300),(byte)(in_fpscr >> 0x16) & 3);
    fVar20 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
    fVar19 = 1.0;
    if (fVar17 / DAT_0015f55c < 1.0) {
      fVar19 = fVar17 / DAT_0015f55c;
    }
    iVar7 = 4;
    fVar18 = (float)VectorSignedToFloat(iVar2,(byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
    local_6c[3] = (int)(fVar19 * fVar20);
    fVar17 = fVar19 * fVar18;
    local_6c[0] = (int)(fVar19 * fVar18);
    local_6c[7] = (int)-(fVar19 * fVar20);
LAB_0015f33e:
    in_fpscr = in_fpscr & 0xfffffff;
    local_6c[iVar7 * 2] = (int)-fVar17;
  }
  if (bVar1) {
    if (0x5dc < *(int *)(this + 0x130)) goto LAB_0015f9f0;
  }
  else {
    iVar8 = *(int *)(this + 0x110);
    iVar5 = **(int **)(DAT_0015f560 + 0x15f368);
    uVar4 = *puVar11;
    iVar7 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
    AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
    AbyssEngine::PaintCanvas::DrawImage2D(uVar4,iVar8,iVar5 / 2 - iVar7 / 2);
  }
  iVar8 = *(int *)(this + 0x10c);
  piVar16 = *(int **)(DAT_0015f568 + 0x15f3c0);
  iVar7 = *piVar16;
  uVar4 = *puVar11;
  iVar5 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
  pcVar12 = *(code **)(DAT_0015f56c + 0x15f3da);
  piVar13 = *(int **)(DAT_0015f570 + 0x15f3e6);
  (*pcVar12)(*puVar11,*(undefined4 *)(this + 0x110));
  (*pcVar12)(*puVar11,*(undefined4 *)(this + 0x10c));
  piVar15 = *(int **)(DAT_0015f574 + 0x15f40e);
  AbyssEngine::PaintCanvas::DrawImage2D(uVar4,iVar8,iVar7 / 2 - iVar5);
  pPVar10 = (PaintCanvas *)*puVar11;
  iVar7 = *piVar16;
  uVar4 = *(uint *)(this + 0x10c);
  iVar14 = *piVar13;
  iVar5 = (*pcVar12)(pPVar10,*(undefined4 *)(this + 0x110));
  iVar8 = (*pcVar12)(*puVar11,*(undefined4 *)(this + 0x10c));
  AbyssEngine::PaintCanvas::DrawImage2D
            (pPVar10,uVar4,iVar7 / 2,((iVar14 / 2 - iVar5 / 2) - iVar8) + *(int *)(*piVar15 + 0x30c)
             ,'\x01');
  if (!bVar1) {
    iVar8 = *(int *)(this + 0x114);
    uVar4 = *puVar11;
    iVar7 = *piVar16;
    iVar5 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
    AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
    AbyssEngine::PaintCanvas::DrawImage2D(uVar4,iVar8,iVar7 / 2 - iVar5);
    uVar4 = *(uint *)(this + 0x114);
    pPVar10 = (PaintCanvas *)*puVar11;
    iVar5 = *piVar16;
    iVar8 = *piVar13;
    iVar7 = AbyssEngine::PaintCanvas::GetImage2DHeight((uint)pPVar10);
    AbyssEngine::PaintCanvas::DrawImage2D
              (pPVar10,uVar4,iVar5 / 2,iVar7 / 2 + iVar8 / 2 + *(int *)(*piVar15 + 0x314),'\x01');
  }
  uVar4 = 0;
  fVar19 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
  for (; uVar4 != 6; uVar4 = uVar4 + 1) {
    if ((bVar1) && (uVar6 = __aeabi_idiv(*(undefined4 *)(this + 0x130),200), (uVar6 & 1) == 0)) {
      iVar7 = *(int *)(this + uVar4 * 4 + 0x1c);
      iVar5 = __aeabi_uidiv(uVar4 & 0xff,3);
      uVar6 = *puVar11;
      fVar20 = (float)VectorSignedToFloat(*piVar16 / 2,(byte)(in_fpscr >> 0x16) & 3);
      fVar17 = (float)VectorSignedToFloat(iVar2 * (uVar4 + iVar5 * -3 & 0xff),
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar18 = (float)VectorSignedToFloat(local_6c[uVar4 * 2],(byte)(in_fpscr >> 0x16) & 3);
      iVar7 = *(int *)(this + iVar7 * 4 + *(int *)(this + 0x124) * 0x30 + 100);
    }
    else {
      iVar7 = *(int *)(this + uVar4 * 4 + 0x1c);
      iVar5 = __aeabi_uidiv(uVar4 & 0xff,3);
      uVar6 = *puVar11;
      fVar20 = (float)VectorSignedToFloat(*piVar16 / 2,(byte)(in_fpscr >> 0x16) & 3);
      fVar17 = (float)VectorSignedToFloat(iVar2 * (uVar4 + iVar5 * -3 & 0xff),
                                          (byte)(in_fpscr >> 0x16) & 3);
      fVar18 = (float)VectorSignedToFloat(local_6c[uVar4 * 2],(byte)(in_fpscr >> 0x16) & 3);
      iVar7 = *(int *)(this + iVar7 * 4 + *(int *)(this + 0x124) * 0x30 + 0x4c);
    }
    AbyssEngine::PaintCanvas::GetImage2DHeight(uVar6);
    AbyssEngine::PaintCanvas::DrawImage2D
              (uVar6,iVar7,(int)((fVar20 - fVar19 * 1.5) + fVar17 + fVar18));
  }
  uVar4 = *puVar11;
  if (this[0x128] == (HackingGame)0x0) {
    local_8c = *(int *)(this + 0x11c);
    iVar5 = *piVar16;
    iVar7 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
    iVar7 = iVar7 - (iVar7 >> 0x1f);
    iVar5 = iVar5 / 2 - iVar2 / 2;
    AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
    uVar6 = *puVar11;
  }
  else {
    local_8c = *(int *)(this + 0x118);
    iVar5 = *piVar16;
    iVar7 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
    iVar7 = iVar7 - (iVar7 >> 0x1f);
    iVar5 = iVar5 / 2 - iVar2 / 2;
    AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
    uVar6 = *puVar11;
  }
  AbyssEngine::PaintCanvas::GetImage2DHeight(uVar6);
  AbyssEngine::PaintCanvas::DrawImage2D(uVar4,local_8c,iVar5 - (iVar7 >> 1));
  uVar4 = *puVar11;
  if (this[0x129] == (HackingGame)0x0) {
    local_88 = *(int *)(this + 0x11c);
    iVar5 = *piVar16;
    iVar7 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
    iVar7 = iVar7 - (iVar7 >> 0x1f);
    iVar5 = iVar5 / 2 + iVar2 / 2;
    AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
    uVar6 = *puVar11;
  }
  else {
    local_88 = *(int *)(this + 0x118);
    iVar5 = *piVar16;
    iVar7 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
    iVar7 = iVar7 - (iVar7 >> 0x1f);
    iVar5 = iVar5 / 2 + iVar2 / 2;
    AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
    uVar6 = *puVar11;
  }
  AbyssEngine::PaintCanvas::GetImage2DHeight(uVar6);
  AbyssEngine::PaintCanvas::DrawImage2D(uVar4,local_88,iVar5 - (iVar7 >> 1));
  if (!bVar1) {
    for (uVar4 = 0; uVar6 = *puVar11, uVar4 != 6; uVar4 = uVar4 + 1) {
      iVar7 = __aeabi_uidiv(uVar4 & 0xff,3);
      fVar17 = (float)VectorSignedToFloat(*piVar16 / 2,(byte)(in_fpscr >> 0x16) & 3);
      fVar20 = (float)VectorSignedToFloat(iVar2 * (uVar4 + iVar7 * -3 & 0xff),
                                          (byte)(in_fpscr >> 0x16) & 3);
      iVar7 = *(int *)(this + *(int *)(this + uVar4 * 4 + 4) * 4 + *(int *)(this + 0x124) * 0x30 +
                              0x4c);
      AbyssEngine::PaintCanvas::GetImage2DHeight(uVar6);
      AbyssEngine::PaintCanvas::DrawImage2D(uVar6,iVar7,(int)((fVar17 - fVar19 * 1.5) + fVar20));
    }
    iVar8 = *(int *)(this + 0x120);
    iVar7 = *piVar16;
    iVar5 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar6);
    (*pcVar12)(*puVar11,*(undefined4 *)(this + 0x110));
    (*pcVar12)(*puVar11,*(undefined4 *)(this + 0x120));
    AbyssEngine::PaintCanvas::DrawImage2D(uVar6,iVar8,(iVar7 / 2 - iVar2 / 2) - iVar5 / 2);
    iVar8 = *(int *)(this + 0x120);
    uVar4 = *puVar11;
    iVar7 = *piVar16;
    iVar5 = AbyssEngine::PaintCanvas::GetImage2DWidth(uVar4);
    (*pcVar12)(*puVar11,*(undefined4 *)(this + 0x110));
    (*pcVar12)(*puVar11,*(undefined4 *)(this + 0x120));
    AbyssEngine::PaintCanvas::DrawImage2D(uVar4,iVar8,(iVar7 / 2 + iVar2 / 2) - iVar5 / 2);
  }
LAB_0015f9f0:
  if (*piVar9 == local_6c[0xc]) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0015f1e4: push {r4,r5,r6,r7,lr}
  0015f1e6: add r7,sp,#0xc
  0015f1e8: push {r7,r8,r9,r10,r11}
  0015f1ec: vpush {d8,d9}
  0015f1f0: sub sp,#0x68
  0015f1f2: mov r6,r0
  0015f1f4: ldr r0,[0x0015f554]
  0015f1f6: ldr r1,[0x0015f558]
  0015f1f8: add r0,pc
  0015f1fa: add r1,pc
  0015f1fc: ldr r4,[r0,#0x0]
  0015f1fe: ldr r5,[r1,#0x0]
  0015f200: ldr r1,[r4,#0x0]
  0015f202: ldr r0,[r5,#0x0]
  0015f204: str r1,[sp,#0x64]
  0015f206: mov.w r1,#0xffffffff
  0015f20a: blx 0x0006fac0
  0015f20e: movs r0,#0x0
  0015f210: str r4,[sp,#0x8]
  0015f212: cmp r0,#0x5
  0015f214: bhi 0x0015f228
  0015f216: add.w r1,r6,r0, lsl #0x2
  0015f21a: adds r0,#0x1
  0015f21c: ldr r2,[r1,#0x4]
  0015f21e: ldr r1,[r1,#0x1c]
  0015f220: cmp r1,r2
  0015f222: beq 0x0015f212
  0015f224: movs r0,#0x0
  0015f226: b 0x0015f22a
  0015f228: movs r0,#0x1
  0015f22a: str r0,[sp,#0x10]
  0015f22c: add.w r4,r6,#0x4c
  0015f230: ldr.w r0,[r6,#0x124]
  0015f234: add.w r0,r0,r0, lsl #0x1
  0015f238: lsls r0,r0,#0x4
  0015f23a: ldr r1,[r4,r0]
  0015f23c: ldr r0,[r5,#0x0]
  0015f23e: blx 0x00072d84
  0015f242: ldr.w r1,[r6,#0x124]
  0015f246: str r0,[sp,#0x2c]
  0015f248: ldr r0,[r5,#0x0]
  0015f24a: add.w r1,r1,r1, lsl #0x1
  0015f24e: lsls r1,r1,#0x4
  0015f250: ldr r1,[r4,r1]
  0015f252: blx 0x00072d90
  0015f256: add r3,sp,#0x34
  0015f258: str r0,[sp,#0x28]
  0015f25a: movs r0,#0x0
  0015f25c: movs r1,#0x0
  0015f25e: b 0x0015f26c
  0015f260: add.w r2,r3,r1, lsl #0x3
  0015f264: str.w r0,[r3,r1,lsl #0x3]
  0015f268: adds r1,#0x1
  0015f26a: str r0,[r2,#0x4]
  0015f26c: cmp r1,#0x6
  0015f26e: bne 0x0015f260
  0015f270: ldrb.w r0,[r6,#0x128]
  0015f274: cbz r0,0x0015f2d8
  0015f276: vldr.32 s0,[r6,#0x12c]
  0015f27a: vldr.32 s2,[pc,#0x2e0]
  0015f27e: vcvt.f32.s32 s0,s0
  0015f282: ldr r0,[sp,#0x28]
  0015f284: vmov s4,r0
  0015f288: vcvt.f32.s32 s4,s4
  0015f28c: vdiv.f32 s0,s0,s2
  0015f290: vmov.f32 s2,0x3f800000
  0015f294: vcmpe.f32 s0,s2
  0015f298: vmrs apsr,fpscr
  0015f29c: it mi
  0015f29e: vmov.mi.f32 s2,s0
  0015f2a2: ldr r0,[sp,#0x2c]
  0015f2a4: vmul.f32 s6,s2,s4
  0015f2a8: vnmul.f32 s4,s2,s4
  0015f2ac: vmov s0,r0
  0015f2b0: movs r0,#0x4
  0015f2b2: vcvt.f32.s32 s0,s0
  0015f2b6: vcvt.s32.f32 s6,s6
  0015f2ba: vmul.f32 s8,s2,s0
  0015f2be: vnmul.f32 s0,s2,s0
  0015f2c2: vcvt.s32.f32 s8,s8
  0015f2c6: vcvt.s32.f32 s4,s4
  0015f2ca: vstr.32 s6,[sp,#0x40]
  0015f2ce: vstr.32 s8,[sp,#0x34]
  0015f2d2: vstr.32 s4,[sp,#0x50]
  0015f2d6: b 0x0015f33e
  0015f2d8: ldrb.w r0,[r6,#0x129]
  0015f2dc: cbz r0,0x0015f34a
  0015f2de: vldr.32 s0,[r6,#0x12c]
  0015f2e2: vldr.32 s2,[pc,#0x278]
  0015f2e6: vcvt.f32.s32 s0,s0
  0015f2ea: ldr r0,[sp,#0x28]
  0015f2ec: vmov s4,r0
  0015f2f0: vcvt.f32.s32 s4,s4
  0015f2f4: vdiv.f32 s0,s0,s2
  0015f2f8: vmov.f32 s2,0x3f800000
  0015f2fc: vcmpe.f32 s0,s2
  0015f300: vmrs apsr,fpscr
  0015f304: it mi
  0015f306: vmov.mi.f32 s2,s0
  0015f30a: ldr r0,[sp,#0x2c]
  0015f30c: vmul.f32 s6,s2,s4
  0015f310: vnmul.f32 s4,s2,s4
  0015f314: vmov s0,r0
  0015f318: movs r0,#0x5
  0015f31a: vcvt.f32.s32 s0,s0
  0015f31e: vcvt.s32.f32 s6,s6
  0015f322: vmul.f32 s8,s2,s0
  0015f326: vnmul.f32 s0,s2,s0
  0015f32a: vcvt.s32.f32 s8,s8
  0015f32e: vcvt.s32.f32 s4,s4
  0015f332: vstr.32 s6,[sp,#0x48]
  0015f336: vstr.32 s8,[sp,#0x3c]
  0015f33a: vstr.32 s4,[sp,#0x58]
  0015f33e: vcvt.s32.f32 s0,s0
  0015f342: vmov r1,s0
  0015f346: str.w r1,[r3,r0,lsl #0x3]
  0015f34a: ldr r0,[sp,#0x10]
  0015f34c: str r5,[sp,#0x30]
  0015f34e: cbz r0,0x0015f35e
  0015f350: ldr.w r0,[r6,#0x130]
  0015f354: movw r1,#0x5dc
  0015f358: cmp r0,r1
  0015f35a: ble 0x0015f3b6
  0015f35c: b 0x0015f9f0
  0015f35e: ldr r0,[0x0015f560]
  0015f360: ldr.w r10,[r6,#0x110]
  0015f364: add r0,pc
  0015f366: ldr r0,[r0,#0x0]
  0015f368: mov r1,r10
  0015f36a: ldr.w r8,[r0,#0x0]
  0015f36e: ldr r0,[sp,#0x30]
  0015f370: ldr r5,[r0,#0x0]
  0015f372: mov r0,r5
  0015f374: blx 0x00072d84
  0015f378: ldr r1,[0x0015f564]
  0015f37a: add.w r4,r0,r0, lsr #0x1f
  0015f37e: add r1,pc
  0015f380: ldr r1,[r1,#0x0]
  0015f382: ldr.w r9,[r1,#0x0]
  0015f386: ldr r1,[sp,#0x30]
  0015f388: ldr r2,[r1,#0x0]
  0015f38a: ldr.w r1,[r6,#0x110]
  0015f38e: mov r0,r2
  0015f390: blx 0x00072d90
  0015f394: add.w r1,r8,r8, lsr #0x1f
  0015f398: add.w r0,r0,r0, lsr #0x1f
  0015f39c: asrs r1,r1,#0x1
  0015f39e: sub.w r2,r1,r4, asr #0x1
  0015f3a2: add.w r1,r9,r9, lsr #0x1f
  0015f3a6: asrs r1,r1,#0x1
  0015f3a8: sub.w r3,r1,r0, asr #0x1
  0015f3ac: mov r0,r5
  0015f3ae: mov r1,r10
  0015f3b0: ldr r5,[sp,#0x30]
  0015f3b2: blx 0x00072dc0
  0015f3b6: ldr r0,[0x0015f568]
  0015f3b8: ldr.w r1,[r6,#0x10c]
  0015f3bc: add r0,pc
  0015f3be: str r1,[sp,#0x20]
  0015f3c0: ldr.w r11,[r0,#0x0]
  0015f3c4: ldr.w r0,[r11,#0x0]
  0015f3c8: str r0,[sp,#0x24]
  0015f3ca: ldr r0,[r5,#0x0]
  0015f3cc: str r0,[sp,#0x1c]
  0015f3ce: blx 0x00072d84
  0015f3d2: str r0,[sp,#0x18]
  0015f3d4: ldr r0,[0x0015f56c]
  0015f3d6: add r0,pc
  0015f3d8: ldr.w r8,[r0,#0x0]
  0015f3dc: ldr r0,[0x0015f570]
  0015f3de: ldr.w r1,[r6,#0x110]
  0015f3e2: add r0,pc
  0015f3e4: ldr.w r9,[r0,#0x0]
  0015f3e8: ldr r0,[r5,#0x0]
  0015f3ea: ldr.w r4,[r9,#0x0]
  0015f3ee: blx r8
  0015f3f0: add.w r1,r4,r4, lsr #0x1f
  0015f3f4: add.w r0,r0,r0, lsr #0x1f
  0015f3f8: asrs r1,r1,#0x1
  0015f3fa: sub.w r4,r1,r0, asr #0x1
  0015f3fe: ldr r0,[r5,#0x0]
  0015f400: ldr.w r1,[r6,#0x10c]
  0015f404: blx r8
  0015f406: ldr r1,[0x0015f574]
  0015f408: subs r0,r4,r0
  0015f40a: add r1,pc
  0015f40c: ldr.w r10,[r1,#0x0]
  0015f410: ldr.w r1,[r10,#0x0]
  0015f414: ldr.w r1,[r1,#0x30c]
  0015f418: adds r3,r0,r1
  0015f41a: ldr r0,[sp,#0x24]
  0015f41c: ldr r1,[sp,#0x18]
  0015f41e: add.w r0,r0,r0, lsr #0x1f
  0015f422: rsb r2,r1,r0, asr #0x1
  0015f426: ldrd r0,r1,[sp,#0x1c]
  0015f42a: blx 0x00072dc0
  0015f42e: ldr r4,[r5,#0x0]
  0015f430: ldr.w r0,[r11,#0x0]
  0015f434: ldr.w r1,[r6,#0x110]
  0015f438: str r0,[sp,#0x14]
  0015f43a: ldr.w r0,[r6,#0x10c]
  0015f43e: str r0,[sp,#0x18]
  0015f440: mov r0,r4
  0015f442: str.w r9,[sp,#0x20]
  0015f446: str.w r11,[sp,#0x24]
  0015f44a: ldr.w r9,[r9,#0x0]
  0015f44e: blx r8
  0015f450: mov r11,r0
  0015f452: ldr r0,[r5,#0x0]
  0015f454: ldr.w r1,[r6,#0x10c]
  0015f458: str.w r8,[sp,#0x4]
  0015f45c: blx r8
  0015f45e: ldr.w r1,[r10,#0x0]
  0015f462: movs r2,#0x1
  0015f464: add.w r3,r9,r9, lsr #0x1f
  0015f468: str.w r10,[sp,#0x1c]
  0015f46c: ldr.w r1,[r1,#0x30c]
  0015f470: asrs r3,r3,#0x1
  0015f472: str r2,[sp,#0x0]
  0015f474: add.w r2,r11,r11, lsr #0x1f
  0015f478: sub.w r2,r3,r2, asr #0x1
  0015f47c: subs r0,r2,r0
  0015f47e: adds r3,r0,r1
  0015f480: ldr r0,[sp,#0x14]
  0015f482: ldr r1,[sp,#0x18]
  0015f484: add.w r0,r0,r0, lsr #0x1f
  0015f488: asrs r2,r0,#0x1
  0015f48a: mov r0,r4
  0015f48c: blx 0x00074e3c
  0015f490: ldr r0,[sp,#0x10]
  0015f492: cmp r0,#0x0
  0015f494: bne 0x0015f534
  0015f496: ldr r4,[sp,#0x30]
  0015f498: ldr.w r8,[sp,#0x24]
  0015f49c: ldr.w r11,[r6,#0x114]
  0015f4a0: ldr r5,[r4,#0x0]
  0015f4a2: ldr.w r0,[r8,#0x0]
  0015f4a6: str r0,[sp,#0x18]
  0015f4a8: mov r1,r11
  0015f4aa: mov r0,r5
  0015f4ac: blx 0x00072d84
  0015f4b0: str r0,[sp,#0x14]
  0015f4b2: ldr.w r9,[sp,#0x20]
  0015f4b6: ldr r0,[r4,#0x0]
  0015f4b8: ldr.w r1,[r6,#0x110]
  0015f4bc: ldr.w r10,[r9,#0x0]
  0015f4c0: blx 0x00072d90
  0015f4c4: ldr r1,[sp,#0x18]
  0015f4c6: add.w r0,r0,r0, lsr #0x1f
  0015f4ca: ldr r2,[sp,#0x14]
  0015f4cc: asrs r0,r0,#0x1
  0015f4ce: add.w r1,r1,r1, lsr #0x1f
  0015f4d2: rsb r2,r2,r1, asr #0x1
  0015f4d6: add.w r1,r10,r10, lsr #0x1f
  0015f4da: ldr.w r10,[sp,#0x1c]
  0015f4de: add.w r0,r0,r1, asr #0x1
  0015f4e2: ldr.w r1,[r10,#0x0]
  0015f4e6: ldr.w r1,[r1,#0x314]
  0015f4ea: adds r3,r0,r1
  0015f4ec: mov r0,r5
  0015f4ee: mov r1,r11
  0015f4f0: blx 0x00072dc0
  0015f4f4: ldrd r1,r0,[r6,#0x110]
  0015f4f8: ldr r5,[r4,#0x0]
  0015f4fa: str r0,[sp,#0x18]
  0015f4fc: ldr.w r8,[r8,#0x0]
  0015f500: mov r0,r5
  0015f502: ldr.w r11,[r9,#0x0]
  0015f506: blx 0x00072d90
  0015f50a: ldr.w r1,[r10,#0x0]
  0015f50e: add.w r0,r0,r0, lsr #0x1f
  0015f512: movs r2,#0x1
  0015f514: asrs r0,r0,#0x1
  0015f516: ldr.w r1,[r1,#0x314]
  0015f51a: str r2,[sp,#0x0]
  0015f51c: add.w r2,r11,r11, lsr #0x1f
  0015f520: add.w r0,r0,r2, asr #0x1
  0015f524: adds r3,r0,r1
  0015f526: add.w r0,r8,r8, lsr #0x1f
  0015f52a: ldr r1,[sp,#0x18]
  0015f52c: asrs r2,r0,#0x1
  0015f52e: mov r0,r5
  0015f530: blx 0x00074e3c
  0015f534: ldr r0,[sp,#0x2c]
  0015f536: vmov.f32 s0,0x3fc00000
  0015f53a: mov.w r8,#0x0
  0015f53e: vmov s2,r0
  0015f542: add.w r0,r6,#0x1c
  0015f546: vcvt.f32.s32 s2,s2
  0015f54a: str r6,[sp,#0x18]
  0015f54c: str r0,[sp,#0xc]
  0015f54e: vmul.f32 s16,s2,s0
  0015f552: b 0x0015f6c0
  0015f578: ldr r0,[sp,#0x10]
  0015f57a: cmp r0,#0x0
  0015f57c: beq 0x0015f608
  0015f57e: ldr r0,[sp,#0x18]
  0015f580: movs r1,#0xc8
  0015f582: ldr.w r0,[r0,#0x130]
  0015f586: blx 0x0007198c
  0015f58a: lsls r0,r0,#0x1f
  0015f58c: bne 0x0015f608
  0015f58e: ldr r0,[sp,#0xc]
  0015f590: movs r1,#0x3
  0015f592: ldr.w r0,[r0,r8,lsl #0x2]
  0015f596: str r0,[sp,#0x14]
  0015f598: uxtb.w r0,r8
  0015f59c: blx 0x0006ec2c
  0015f5a0: ldr r6,[sp,#0x18]
  0015f5a2: add r3,sp,#0x34
  0015f5a4: ldr r5,[sp,#0x28]
  0015f5a6: add.w r4,r3,r8, lsl #0x3
  0015f5aa: add.w r3,r0,r0, lsl #0x1
  0015f5ae: subs r0,#0x2
  0015f5b0: ldr.w r2,[r6,#0x124]
  0015f5b4: mul r9,r5,r0
  0015f5b8: ldr.w r1,[r6,#0x110]
  0015f5bc: ldr.w r5,[r11,#0x0]
  0015f5c0: add.w r0,r2,r2, lsl #0x1
  0015f5c4: sub.w r2,r8,r3
  0015f5c8: ldr r3,[sp,#0x2c]
  0015f5ca: uxtb r2,r2
  0015f5cc: add.w r0,r6,r0, lsl #0x4
  0015f5d0: muls r2,r3
  0015f5d2: ldr r3,[sp,#0x20]
  0015f5d4: ldr.w r10,[r3,#0x0]
  0015f5d8: vmov s0,r2
  0015f5dc: ldr r2,[sp,#0x24]
  0015f5de: ldr r2,[r2,#0x0]
  0015f5e0: add.w r2,r2,r2, lsr #0x1f
  0015f5e4: asrs r2,r2,#0x1
  0015f5e6: vmov s2,r2
  0015f5ea: vcvt.f32.s32 s2,s2
  0015f5ee: vcvt.f32.s32 s0,s0
  0015f5f2: vldr.32 s4,[r4]
  0015f5f6: vcvt.f32.s32 s4,s4
  0015f5fa: ldr r2,[sp,#0x14]
  0015f5fc: add.w r0,r0,r2, lsl #0x2
  0015f600: vsub.f32 s2,s2,s16
  0015f604: ldr r6,[r0,#0x64]
  0015f606: b 0x0015f680
  0015f608: ldr r0,[sp,#0xc]
  0015f60a: movs r1,#0x3
  0015f60c: ldr.w r0,[r0,r8,lsl #0x2]
  0015f610: str r0,[sp,#0x14]
  0015f612: uxtb.w r0,r8
  0015f616: blx 0x0006ec2c
  0015f61a: ldr r6,[sp,#0x18]
  0015f61c: add r3,sp,#0x34
  0015f61e: ldr r5,[sp,#0x28]
  0015f620: add.w r4,r3,r8, lsl #0x3
  0015f624: add.w r3,r0,r0, lsl #0x1
  0015f628: subs r0,#0x2
  0015f62a: ldr.w r2,[r6,#0x124]
  0015f62e: mul r9,r5,r0
  0015f632: ldr.w r1,[r6,#0x110]
  0015f636: ldr.w r5,[r11,#0x0]
  0015f63a: add.w r0,r2,r2, lsl #0x1
  0015f63e: sub.w r2,r8,r3
  0015f642: ldr r3,[sp,#0x2c]
  0015f644: uxtb r2,r2
  0015f646: add.w r0,r6,r0, lsl #0x4
  0015f64a: muls r2,r3
  0015f64c: ldr r3,[sp,#0x20]
  0015f64e: ldr.w r10,[r3,#0x0]
  0015f652: vmov s0,r2
  0015f656: ldr r2,[sp,#0x24]
  0015f658: ldr r2,[r2,#0x0]
  0015f65a: add.w r2,r2,r2, lsr #0x1f
  0015f65e: asrs r2,r2,#0x1
  0015f660: vmov s2,r2
  0015f664: vcvt.f32.s32 s2,s2
  0015f668: vcvt.f32.s32 s0,s0
  0015f66c: vldr.32 s4,[r4]
  0015f670: vcvt.f32.s32 s4,s4
  0015f674: ldr r2,[sp,#0x14]
  0015f676: add.w r0,r0,r2, lsl #0x2
  0015f67a: vsub.f32 s2,s2,s16
  0015f67e: ldr r6,[r0,#0x4c]
  0015f680: vadd.f32 s0,s2,s0
  0015f684: mov r0,r5
  0015f686: vadd.f32 s18,s0,s4
  0015f68a: blx 0x00072d90
  0015f68e: ldr r1,[sp,#0x1c]
  0015f690: add.w r2,r10,r10, lsr #0x1f
  0015f694: ldr r3,[r4,#0x4]
  0015f696: add.w r0,r0,r0, lsr #0x1f
  0015f69a: add.w r2,r9,r2, asr #0x1
  0015f69e: ldr r1,[r1,#0x0]
  0015f6a0: sub.w r0,r2,r0, asr #0x1
  0015f6a4: ldr.w r1,[r1,#0x310]
  0015f6a8: vcvt.s32.f32 s0,s18
  0015f6ac: add r0,r1
  0015f6ae: mov r1,r6
  0015f6b0: add r3,r0
  0015f6b2: mov r0,r5
  0015f6b4: vmov r2,s0
  0015f6b8: blx 0x00072dc0
  0015f6bc: add.w r8,r8,#0x1
  0015f6c0: ldr.w r11,[sp,#0x30]
  0015f6c4: cmp.w r8,#0x6
  0015f6c8: bne.w 0x0015f578
  0015f6cc: ldr r6,[sp,#0x18]
  0015f6ce: mov r10,r11
  0015f6d0: ldr.w r11,[r11,#0x0]
  0015f6d4: ldrb.w r0,[r6,#0x128]
  0015f6d8: cbz r0,0x0015f71c
  0015f6da: ldr r0,[sp,#0x24]
  0015f6dc: ldr.w r1,[r6,#0x118]
  0015f6e0: str r1,[sp,#0x14]
  0015f6e2: ldr r4,[r0,#0x0]
  0015f6e4: mov r0,r11
  0015f6e6: blx 0x00072d84
  0015f6ea: ldr r1,[sp,#0x20]
  0015f6ec: add.w r2,r4,r4, lsr #0x1f
  0015f6f0: add.w r5,r0,r0, lsr #0x1f
  0015f6f4: ldr.w r0,[r10,#0x0]
  0015f6f8: asrs r2,r2,#0x1
  0015f6fa: ldr.w r8,[r1,#0x0]
  0015f6fe: ldr r1,[sp,#0x2c]
  0015f700: add.w r1,r1,r1, lsr #0x1f
  0015f704: sub.w r4,r2,r1, asr #0x1
  0015f708: ldr.w r1,[r6,#0x110]
  0015f70c: blx 0x00072d90
  0015f710: mov r9,r0
  0015f712: ldr.w r0,[r10,#0x0]
  0015f716: ldr.w r1,[r6,#0x118]
  0015f71a: b 0x0015f75c
  0015f71c: ldr r0,[sp,#0x24]
  0015f71e: ldr.w r1,[r6,#0x11c]
  0015f722: str r1,[sp,#0x14]
  0015f724: ldr r4,[r0,#0x0]
  0015f726: mov r0,r11
  0015f728: blx 0x00072d84
  0015f72c: ldr r1,[sp,#0x20]
  0015f72e: add.w r2,r4,r4, lsr #0x1f
  0015f732: add.w r5,r0,r0, lsr #0x1f
  0015f736: ldr.w r0,[r10,#0x0]
  0015f73a: asrs r2,r2,#0x1
  0015f73c: ldr.w r8,[r1,#0x0]
  0015f740: ldr r1,[sp,#0x2c]
  0015f742: add.w r1,r1,r1, lsr #0x1f
  0015f746: sub.w r4,r2,r1, asr #0x1
  0015f74a: ldr.w r1,[r6,#0x110]
  0015f74e: blx 0x00072d90
  0015f752: mov r9,r0
  0015f754: ldr.w r0,[r10,#0x0]
  0015f758: ldr.w r1,[r6,#0x11c]
  0015f75c: sub.w r5,r4,r5, asr #0x1
  0015f760: blx 0x00072d90
  0015f764: ldr r2,[sp,#0x28]
  0015f766: add.w r1,r8,r8, lsr #0x1f
  0015f76a: add.w r3,r9,r9, lsr #0x1f
  0015f76e: add.w r0,r0,r0, lsr #0x1f
  0015f772: rsb r1,r2,r1, asr #0x1
  0015f776: ldr r2,[sp,#0x1c]
  0015f778: sub.w r1,r1,r3, asr #0x1
  0015f77c: sub.w r0,r1,r0, asr #0x1
  0015f780: ldr r1,[sp,#0x14]
  0015f782: ldr r2,[r2,#0x0]
  0015f784: ldr.w r2,[r2,#0x310]
  0015f788: adds r3,r0,r2
  0015f78a: mov r0,r11
  0015f78c: mov r2,r5
  0015f78e: blx 0x00072dc0
  0015f792: ldrb.w r0,[r6,#0x129]
  0015f796: mov r11,r6
  0015f798: ldr.w r6,[r10,#0x0]
  0015f79c: cbz r0,0x0015f7e0
  0015f79e: ldr r0,[sp,#0x24]
  0015f7a0: ldr.w r1,[r11,#0x118]
  0015f7a4: str r1,[sp,#0x18]
  0015f7a6: ldr r4,[r0,#0x0]
  0015f7a8: mov r0,r6
  0015f7aa: blx 0x00072d84
  0015f7ae: ldr r1,[sp,#0x20]
  0015f7b0: add.w r2,r4,r4, lsr #0x1f
  0015f7b4: add.w r5,r0,r0, lsr #0x1f
  0015f7b8: ldr.w r0,[r10,#0x0]
  0015f7bc: asrs r2,r2,#0x1
  0015f7be: ldr.w r8,[r1,#0x0]
  0015f7c2: ldr r1,[sp,#0x2c]
  0015f7c4: add.w r1,r1,r1, lsr #0x1f
  0015f7c8: add.w r4,r2,r1, asr #0x1
  0015f7cc: ldr.w r1,[r11,#0x110]
  0015f7d0: blx 0x00072d90
  0015f7d4: mov r9,r0
  0015f7d6: ldr.w r0,[r10,#0x0]
  0015f7da: ldr.w r1,[r11,#0x118]
  0015f7de: b 0x0015f820
  0015f7e0: ldr r0,[sp,#0x24]
  0015f7e2: ldr.w r1,[r11,#0x11c]
  0015f7e6: str r1,[sp,#0x18]
  0015f7e8: ldr r4,[r0,#0x0]
  0015f7ea: mov r0,r6
  0015f7ec: blx 0x00072d84
  0015f7f0: ldr r1,[sp,#0x20]
  0015f7f2: add.w r2,r4,r4, lsr #0x1f
  0015f7f6: add.w r5,r0,r0, lsr #0x1f
  0015f7fa: ldr.w r0,[r10,#0x0]
  0015f7fe: asrs r2,r2,#0x1
  0015f800: ldr.w r8,[r1,#0x0]
  0015f804: ldr r1,[sp,#0x2c]
  0015f806: add.w r1,r1,r1, lsr #0x1f
  0015f80a: add.w r4,r2,r1, asr #0x1
  0015f80e: ldr.w r1,[r11,#0x110]
  0015f812: blx 0x00072d90
  0015f816: mov r9,r0
  0015f818: ldr.w r0,[r10,#0x0]
  0015f81c: ldr.w r1,[r11,#0x11c]
  0015f820: sub.w r5,r4,r5, asr #0x1
  0015f824: blx 0x00072d90
  0015f828: ldr r2,[sp,#0x28]
  0015f82a: add.w r1,r8,r8, lsr #0x1f
  0015f82e: add.w r3,r9,r9, lsr #0x1f
  0015f832: add.w r0,r0,r0, lsr #0x1f
  0015f836: rsb r1,r2,r1, asr #0x1
  0015f83a: ldr r2,[sp,#0x1c]
  0015f83c: sub.w r1,r1,r3, asr #0x1
  0015f840: sub.w r0,r1,r0, asr #0x1
  0015f844: ldr r1,[sp,#0x18]
  0015f846: ldr r2,[r2,#0x0]
  0015f848: ldr.w r2,[r2,#0x310]
  0015f84c: adds r3,r0,r2
  0015f84e: mov r0,r6
  0015f850: mov r2,r5
  0015f852: blx 0x00072dc0
  0015f856: ldr r0,[sp,#0x10]
  0015f858: cmp r0,#0x0
  0015f85a: ldr r0,[sp,#0x30]
  0015f85c: bne.w 0x0015f9f0
  0015f860: add.w r8,r11,#0x4
  0015f864: movs r4,#0x0
  0015f866: b 0x0015f8f8
  0015f868: uxtb r0,r4
  0015f86a: movs r1,#0x3
  0015f86c: blx 0x0006ec2c
  0015f870: ldr r1,[sp,#0x24]
  0015f872: add.w r2,r0,r0, lsl #0x1
  0015f876: ldr r1,[r1,#0x0]
  0015f878: add.w r1,r1,r1, lsr #0x1f
  0015f87c: asrs r1,r1,#0x1
  0015f87e: vmov s0,r1
  0015f882: subs r1,r4,r2
  0015f884: vcvt.f32.s32 s0,s0
  0015f888: ldr r2,[sp,#0x2c]
  0015f88a: uxtb r1,r1
  0015f88c: ldr.w r3,[r11,#0x124]
  0015f890: muls r1,r2
  0015f892: ldr.w r2,[r8,r4,lsl #0x2]
  0015f896: vsub.f32 s0,s0,s16
  0015f89a: vmov s2,r1
  0015f89e: ldr.w r1,[r11,#0x110]
  0015f8a2: vcvt.f32.s32 s2,s2
  0015f8a6: ldr r6,[sp,#0x28]
  0015f8a8: mul r5,r6,r0
  0015f8ac: add.w r0,r3,r3, lsl #0x1
  0015f8b0: add.w r0,r11,r0, lsl #0x4
  0015f8b4: add.w r0,r0,r2, lsl #0x2
  0015f8b8: ldr r2,[sp,#0x20]
  0015f8ba: vadd.f32 s18,s0,s2
  0015f8be: ldr.w r9,[r0,#0x4c]
  0015f8c2: mov r0,r10
  0015f8c4: ldr r6,[r2,#0x0]
  0015f8c6: blx 0x00072d90
  0015f8ca: ldr r1,[sp,#0x1c]
  0015f8cc: add.w r2,r6,r6, lsr #0x1f
  0015f8d0: add.w r0,r0,r0, lsr #0x1f
  0015f8d4: add.w r2,r5,r2, asr #0x1
  0015f8d8: ldr r1,[r1,#0x0]
  0015f8da: add.w r0,r2,r0, asr #0x1
  0015f8de: ldr.w r1,[r1,#0x318]
  0015f8e2: vcvt.s32.f32 s0,s18
  0015f8e6: adds r3,r0,r1
  0015f8e8: mov r0,r10
  0015f8ea: mov r1,r9
  0015f8ec: vmov r2,s0
  0015f8f0: blx 0x00072dc0
  0015f8f4: adds r4,#0x1
  0015f8f6: ldr r0,[sp,#0x30]
  0015f8f8: ldr.w r10,[r0,#0x0]
  0015f8fc: cmp r4,#0x6
  0015f8fe: bne 0x0015f868
  0015f900: ldr r0,[sp,#0x24]
  0015f902: ldr.w r9,[r11,#0x120]
  0015f906: ldr r0,[r0,#0x0]
  0015f908: str r0,[sp,#0x18]
  0015f90a: mov r0,r10
  0015f90c: mov r1,r9
  0015f90e: blx 0x00072d84
  0015f912: str r0,[sp,#0x14]
  0015f914: ldr r0,[sp,#0x20]
  0015f916: ldr.w r8,[sp,#0x30]
  0015f91a: ldr.w r1,[r11,#0x110]
  0015f91e: ldr r4,[r0,#0x0]
  0015f920: ldr.w r0,[r8,#0x0]
  0015f924: ldr r5,[sp,#0x4]
  0015f926: blx r5
  0015f928: add.w r6,r0,r0, lsr #0x1f
  0015f92c: ldr.w r0,[r8,#0x0]
  0015f930: ldr.w r1,[r11,#0x120]
  0015f934: blx r5
  0015f936: add.w r1,r4,r4, lsr #0x1f
  0015f93a: ldr r2,[sp,#0x28]
  0015f93c: add.w r0,r0,r0, lsr #0x1f
  0015f940: add.w r1,r2,r1, asr #0x1
  0015f944: add.w r1,r1,r6, asr #0x1
  0015f948: sub.w r0,r1,r0, asr #0x1
  0015f94c: ldr r1,[sp,#0x1c]
  0015f94e: ldr r1,[r1,#0x0]
  0015f950: ldr.w r1,[r1,#0x318]
  0015f954: adds r3,r0,r1
  0015f956: ldr r0,[sp,#0x2c]
  0015f958: add.w r1,r0,r0, lsr #0x1f
  0015f95c: ldr r0,[sp,#0x18]
  0015f95e: str r1,[sp,#0x2c]
  0015f960: add.w r0,r0,r0, lsr #0x1f
  0015f964: asrs r0,r0,#0x1
  0015f966: sub.w r0,r0,r1, asr #0x1
  0015f96a: ldr r1,[sp,#0x14]
  0015f96c: add.w r1,r1,r1, lsr #0x1f
  0015f970: sub.w r2,r0,r1, asr #0x1
  0015f974: mov r0,r10
  0015f976: mov r1,r9
  0015f978: blx 0x00072dc0
  0015f97c: ldr r0,[sp,#0x24]
  0015f97e: ldr.w r10,[r11,#0x120]
  0015f982: ldr.w r6,[r8,#0x0]
  0015f986: ldr r0,[r0,#0x0]
  0015f988: str r0,[sp,#0x24]
  0015f98a: mov r1,r10
  0015f98c: mov r0,r6
  0015f98e: blx 0x00072d84
  0015f992: str r0,[sp,#0x18]
  0015f994: ldr r0,[sp,#0x20]
  0015f996: ldr.w r1,[r11,#0x110]
  0015f99a: ldr r4,[r0,#0x0]
  0015f99c: ldr.w r0,[r8,#0x0]
  0015f9a0: blx r5
  0015f9a2: add.w r9,r0,r0, lsr #0x1f
  0015f9a6: ldr.w r0,[r8,#0x0]
  0015f9aa: ldr.w r1,[r11,#0x120]
  0015f9ae: blx r5
  0015f9b0: add.w r1,r4,r4, lsr #0x1f
  0015f9b4: ldr r2,[sp,#0x28]
  0015f9b6: add.w r0,r0,r0, lsr #0x1f
  0015f9ba: add.w r1,r2,r1, asr #0x1
  0015f9be: ldr r2,[sp,#0x1c]
  0015f9c0: add.w r1,r1,r9, asr #0x1
  0015f9c4: sub.w r0,r1,r0, asr #0x1
  0015f9c8: ldr r1,[sp,#0x2c]
  0015f9ca: ldr r2,[r2,#0x0]
  0015f9cc: ldr.w r2,[r2,#0x318]
  0015f9d0: adds r3,r0,r2
  0015f9d2: ldr r0,[sp,#0x24]
  0015f9d4: add.w r0,r0,r0, lsr #0x1f
  0015f9d8: asrs r0,r0,#0x1
  0015f9da: add.w r0,r0,r1, asr #0x1
  0015f9de: ldr r1,[sp,#0x18]
  0015f9e0: add.w r1,r1,r1, lsr #0x1f
  0015f9e4: sub.w r2,r0,r1, asr #0x1
  0015f9e8: mov r0,r6
  0015f9ea: mov r1,r10
  0015f9ec: blx 0x00072dc0
  0015f9f0: ldr r0,[sp,#0x64]
  0015f9f2: ldr r1,[sp,#0x8]
  0015f9f4: ldr r1,[r1,#0x0]
  0015f9f6: subs r0,r1,r0
  0015f9f8: itttt eq
  0015f9fa: add.eq sp,#0x68
  0015f9fc: vpop.eq {d8,d9}
  0015fa00: add.eq sp,#0x4
  0015fa02: pop.eq.w {r8,r9,r10,r11}
  0015fa06: it eq
  0015fa08: pop.eq {r4,r5,r6,r7,pc}
  0015fa0a: blx 0x0006e824
```
