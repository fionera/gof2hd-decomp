# MiningGame::render2D
elf 0x11f12c body 1352
Sig: undefined __thiscall render2D(MiningGame * this)

## decompile
```c

/* MiningGame::render2D() */

void __thiscall MiningGame::render2D(MiningGame *this)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  float fVar4;
  float fVar5;
  undefined4 *puVar6;
  uint *puVar7;
  undefined4 uVar8;
  MiningGame *pMVar9;
  undefined4 uVar10;
  int iVar11;
  undefined4 uVar12;
  uint uVar13;
  String *pSVar14;
  code *pcVar15;
  uint in_fpscr;
  int iVar16;
  float fVar17;
  float extraout_s0;
  undefined8 uVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  int local_3c;
  
  piVar1 = *(int **)(DAT_0012f4f0 + 0x12f144);
  puVar7 = *(uint **)(DAT_0012f4f4 + 0x12f146);
  local_3c = *piVar1;
  AbyssEngine::PaintCanvas::SetColor(*puVar7);
  iVar2 = *(int *)(DAT_0012f4f8 + 0x12f182);
  piVar3 = *(int **)(DAT_0012f4fc + 0x12f184);
  pcVar15 = *(code **)(DAT_0012f500 + 0x12f194);
  for (uVar13 = *(uint *)(this + 0x78); (int)uVar13 < *(int *)(this + 0x7c); uVar13 = uVar13 + 1) {
    fVar17 = (float)VectorSignedToFloat(*(undefined4 *)
                                         (iVar2 + *(int *)(this + 0x7c) * -0x1c + uVar13 * 4 + 0xc4)
                                        ,(byte)(in_fpscr >> 0x16) & 3);
    iVar11 = *piVar3;
    iVar16 = (int)(*(float *)(iVar11 + 0xe8) * fVar17);
    if ((uVar13 & 1) == 0) {
      pMVar9 = this + 0xac;
      if ((*(int *)(iVar11 + 0xec) < iVar16) &&
         (pMVar9 = this + 0xb4, iVar16 < *(int *)(iVar11 + 0xf4))) {
        pMVar9 = this + 0xb0;
      }
    }
    else {
      pMVar9 = this + 0xb8;
      if ((*(int *)(iVar11 + 0xec) < iVar16) &&
         (pMVar9 = this + 0xc0, iVar16 < *(int *)(iVar11 + 0xf4))) {
        pMVar9 = this + 0xbc;
      }
    }
    uVar8 = *(undefined4 *)pMVar9;
    iVar16 = iVar16 / 2;
    (*pcVar15)(*puVar7,uVar8,*(undefined4 *)(this + 0x58),*(undefined4 *)(this + 0x5c),iVar16,iVar16
               ,0x11,0x22,0);
    (*pcVar15)(*puVar7,uVar8,*(undefined4 *)(this + 0x58),*(undefined4 *)(this + 0x5c),iVar16,iVar16
               ,0x11,0x21,1);
    (*pcVar15)(*puVar7,uVar8,*(undefined4 *)(this + 0x58),*(undefined4 *)(this + 0x5c),iVar16,iVar16
               ,0x11,0x12,2);
    (*pcVar15)(*puVar7,uVar8,*(undefined4 *)(this + 0x58),*(undefined4 *)(this + 0x5c),iVar16,iVar16
               ,0x11,0x11,3);
  }
  if (this[0x80] != (MiningGame)0x0) {
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar7,*(int *)(this + 0x60),*(int *)(this + 0x58),
               (uchar)*(undefined4 *)(this + 0x5c),'\x11');
  }
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar7,*(int *)(this + 0x98),(int)*(float *)(this + 0x10),
             (uchar)(int)*(float *)(this + 0x14),'\x11');
  uVar18 = Sprite::setRefPixelPosition
                     (*(Sprite **)(this + 0x94),(int)*(float *)(this + 0x10),
                      (int)*(float *)(this + 0x14));
  Sprite::draw((float)uVar18,(float)((ulonglong)uVar18 >> 0x20));
  fVar17 = (float)AbyssEngine::PaintCanvas::DrawImage2D
                            (*puVar7,*(int *)(this + 0xa4),
                             *(int *)(this + 0x50) - *(int *)(*piVar3 + 0xfc));
  iVar2 = *(int *)(this + 0x20);
  if (0x341 < iVar2) {
    uVar13 = *puVar7;
    fVar4 = (float)Layout::getPulseValue((Layout *)*piVar3,fVar17);
    fVar17 = DAT_0012f6bc;
    fVar5 = (float)Layout::getPulseValue((Layout *)*piVar3,extraout_s0);
    fVar4 = fVar4 * fVar17;
    fVar5 = fVar5 * fVar17;
    AbyssEngine::PaintCanvas::SetColor
              ((uchar)uVar13,0xff,(0.0 < fVar4) * (char)(int)fVar4,(0.0 < fVar5) * (char)(int)fVar5)
    ;
    iVar2 = *(int *)(this + 0x20);
  }
  fVar17 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x48),(byte)(in_fpscr >> 0x16) & 3);
  iVar2 = (int)(((DAT_0012f6c0 - fVar17) / DAT_0012f6c0) * fVar4);
  AbyssEngine::PaintCanvas::DrawRegion2D
            ((PaintCanvas *)*puVar7,*(uint *)(this + 0xa8),0,0,iVar2,*(int *)(this + 0x4c),
             (float)iVar2,0,0,0,*(int *)(this + 0x50));
  AbyssEngine::PaintCanvas::SetColor(*puVar7);
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar7,*(int *)(this + 0xc4),*(int *)(this + 0x58),
             (char)*(undefined4 *)(this + 0x54) + 0xfd,'\x11');
  MarqueeImage::draw(*(MarqueeImage **)(this + 0x8c));
  MarqueeImage::draw(*(MarqueeImage **)(this + 0x90));
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x40),(byte)(in_fpscr >> 0x16) & 3);
  fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x34),(byte)(in_fpscr >> 0x16) & 3);
  MarqueeImage::draw(*(MarqueeImage **)(this + 0x88),(int)(*(float *)(this + 0x10) + fVar4),
                     (int)(*(float *)(this + 0x14) - fVar17));
  fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x40),(byte)(in_fpscr >> 0x16) & 3);
  VectorSignedToFloat(*(undefined4 *)(*piVar3 + 0x100),(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar3 + 0xfc),(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar7,*(int *)(this + 0x9c),(int)((*(float *)(this + 0x10) + fVar17) - fVar4));
  AbyssEngine::String::String(aSStack_54,(int)*(float *)(this + 0x24));
  AbyssEngine::String::String(aSStack_60,(char *)(DAT_0012f6c8 + 0x12f486),false);
  AbyssEngine::operator+(aSStack_48,aSStack_54);
  AbyssEngine::String::~String(aSStack_60);
  AbyssEngine::String::~String(aSStack_54);
  Status::getShip();
  iVar2 = Ship::getFreeSpace();
  if (iVar2 < (int)*(float *)(this + 0x24)) {
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar7,0xff,'*','\0');
  }
  else {
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar7,0xff,0xff,0xff);
  }
  puVar6 = *(undefined4 **)(DAT_0012f6d0 + 0x12f516);
  uVar12 = *(undefined4 *)(this + 0x40);
  uVar10 = *(undefined4 *)(this + 0x44);
  pSVar14 = (String *)*puVar6;
  uVar8 = *(undefined4 *)(*piVar3 + 0xfc);
  fVar21 = *(float *)(this + 0x10);
  uVar13 = *puVar7;
  iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(uVar13,pSVar14);
  fVar17 = (float)VectorSignedToFloat(uVar12,(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
  fVar19 = (float)VectorSignedToFloat(uVar8,(byte)(in_fpscr >> 0x16) & 3);
  fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(*piVar3 + 0x104),(byte)(in_fpscr >> 0x16) & 3);
  fVar20 = (float)VectorSignedToFloat(iVar2 >> 1,(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::PaintCanvas::DrawString
            (uVar13,pSVar14,(int)aSStack_48,(int)(((fVar21 + fVar17 + fVar4) - fVar19) - fVar20),
             SUB41((int)(*(float *)(this + 0x14) + fVar5),0));
  AbyssEngine::PaintCanvas::SetColor(*puVar7);
  iVar2 = Status::getCurrentCampaignMission();
  if (iVar2 < 5) {
    VectorSignedToFloat(*(undefined4 *)(this + 0xcc),(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::PaintCanvas::SetColor((uchar)*puVar7,0xff,0xff,0xff);
    pSVar14 = (String *)GameText::getText(**(int **)(DAT_0012f6d4 + 0x12f5f8));
    AbyssEngine::String::String(aSStack_54,pSVar14,false);
    uVar13 = *puVar7;
    iVar11 = **(int **)(DAT_0012f6d8 + 0x12f61a);
    pSVar14 = (String *)*puVar6;
    iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(uVar13,pSVar14);
    AbyssEngine::PaintCanvas::DrawString
              (uVar13,pSVar14,(int)aSStack_54,iVar11 / 2 - iVar2 / 2,
               (bool)((char)*(undefined4 *)(*piVar3 + 0x70) + (char)*(undefined4 *)(this + 0x54)));
    AbyssEngine::PaintCanvas::SetColor(*puVar7);
    AbyssEngine::String::~String(aSStack_54);
  }
  AbyssEngine::String::~String(aSStack_48);
  if (*piVar1 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012f12c: push {r4,r5,r6,r7,lr}
  0012f12e: add r7,sp,#0xc
  0012f130: push {r7,r8,r9,r10,r11}
  0012f134: vpush {d8,d9}
  0012f138: sub sp,#0x68
  0012f13a: mov r4,r0
  0012f13c: ldr r0,[0x0012f4f0]
  0012f13e: ldr r1,[0x0012f4f4]
  0012f140: add r0,pc
  0012f142: add r1,pc
  0012f144: ldr r0,[r0,#0x0]
  0012f146: ldr r2,[r1,#0x0]
  0012f148: str r0,[sp,#0x20]
  0012f14a: ldr r1,[r0,#0x0]
  0012f14c: ldr r0,[r2,#0x0]
  0012f14e: str r2,[sp,#0x3c]
  0012f150: str r1,[sp,#0x64]
  0012f152: mov.w r1,#0xffffffff
  0012f156: blx 0x0006fac0
  0012f15a: add.w r0,r4,#0xc0
  0012f15e: str r0,[sp,#0x28]
  0012f160: add.w r0,r4,#0xb4
  0012f164: str r0,[sp,#0x24]
  0012f166: add.w r0,r4,#0xb8
  0012f16a: str r0,[sp,#0x30]
  0012f16c: add.w r0,r4,#0xac
  0012f170: str r0,[sp,#0x2c]
  0012f172: ldr.w r9,[r4,#0x78]
  0012f176: mov.w r8,#0x11
  0012f17a: ldr r0,[0x0012f4f8]
  0012f17c: ldr r1,[0x0012f4fc]
  0012f17e: add r0,pc
  0012f180: add r1,pc
  0012f182: ldr r0,[r0,#0x0]
  0012f184: str r0,[sp,#0x34]
  0012f186: ldr r0,[r1,#0x0]
  0012f188: str r0,[sp,#0x38]
  0012f18a: ldr r0,[0x0012f500]
  0012f18c: ldr.w r10,[sp,#0x3c]
  0012f190: add r0,pc
  0012f192: ldr.w r11,[r0,#0x0]
  0012f196: b 0x0012f26e
  0012f198: sub.w r0,r0,r0, lsl #0x3
  0012f19c: ldr r1,[sp,#0x34]
  0012f19e: tst r9,#0x1
  0012f1a2: add.w r0,r1,r0, lsl #0x2
  0012f1a6: add.w r0,r0,r9, lsl #0x2
  0012f1aa: vldr.32 s0,[r0,#0xc4]
  0012f1ae: vcvt.f32.s32 s0,s0
  0012f1b2: ldr r0,[sp,#0x38]
  0012f1b4: ldr r0,[r0,#0x0]
  0012f1b6: vldr.32 s2,[r0,#0xe8]
  0012f1ba: ldr.w r2,[r0,#0xec]
  0012f1be: vmul.f32 s0,s2,s0
  0012f1c2: vcvt.s32.f32 s0,s0
  0012f1c6: vmov r1,s0
  0012f1ca: bne 0x0012f1e2
  0012f1cc: ldr r6,[sp,#0x2c]
  0012f1ce: cmp r2,r1
  0012f1d0: bge 0x0012f1f6
  0012f1d2: ldr.w r0,[r0,#0xf4]
  0012f1d6: ldr r6,[sp,#0x24]
  0012f1d8: cmp r0,r1
  0012f1da: it gt
  0012f1dc: add.gt.w r6,r4,#0xb0
  0012f1e0: b 0x0012f1f6
  0012f1e2: ldr r6,[sp,#0x30]
  0012f1e4: cmp r2,r1
  0012f1e6: bge 0x0012f1f6
  0012f1e8: ldr.w r0,[r0,#0xf4]
  0012f1ec: ldr r6,[sp,#0x28]
  0012f1ee: cmp r0,r1
  0012f1f0: it gt
  0012f1f2: add.gt.w r6,r4,#0xbc
  0012f1f6: add.w r1,r1,r1, lsr #0x1f
  0012f1fa: ldrd r2,r3,[r4,#0x58]
  0012f1fe: ldr r5,[r6,#0x0]
  0012f200: asrs r6,r1,#0x1
  0012f202: ldr.w r0,[r10,#0x0]
  0012f206: movs r1,#0x22
  0012f208: strd r6,r6,[sp,#0x0]
  0012f20c: str r1,[sp,#0xc]
  0012f20e: movs r1,#0x0
  0012f210: str r1,[sp,#0x10]
  0012f212: mov r1,r5
  0012f214: str.w r8,[sp,#0x8]
  0012f218: blx r11
  0012f21a: ldrd r2,r3,[r4,#0x58]
  0012f21e: movs r1,#0x21
  0012f220: ldr.w r0,[r10,#0x0]
  0012f224: strd r6,r6,[sp,#0x0]
  0012f228: str r1,[sp,#0xc]
  0012f22a: movs r1,#0x1
  0012f22c: str r1,[sp,#0x10]
  0012f22e: mov r1,r5
  0012f230: str.w r8,[sp,#0x8]
  0012f234: blx r11
  0012f236: ldrd r2,r3,[r4,#0x58]
  0012f23a: movs r1,#0x12
  0012f23c: ldr.w r0,[r10,#0x0]
  0012f240: strd r6,r6,[sp,#0x0]
  0012f244: str r1,[sp,#0xc]
  0012f246: movs r1,#0x2
  0012f248: str r1,[sp,#0x10]
  0012f24a: mov r1,r5
  0012f24c: str.w r8,[sp,#0x8]
  0012f250: blx r11
  0012f252: ldrd r2,r3,[r4,#0x58]
  0012f256: movs r1,#0x3
  0012f258: ldr.w r0,[r10,#0x0]
  0012f25c: strd r6,r6,[sp,#0x0]
  0012f260: strd r8,r8,[sp,#0x8]
  0012f264: str r1,[sp,#0x10]
  0012f266: mov r1,r5
  0012f268: blx r11
  0012f26a: add.w r9,r9,#0x1
  0012f26e: ldr r0,[r4,#0x7c]
  0012f270: cmp r9,r0
  0012f272: blt 0x0012f198
  0012f274: ldrb.w r0,[r4,#0x80]
  0012f278: ldr.w r10,[sp,#0x3c]
  0012f27c: cbz r0,0x0012f294
  0012f27e: ldrd r2,r3,[r4,#0x58]
  0012f282: movs r6,#0x44
  0012f284: ldr r1,[r4,#0x60]
  0012f286: movs r5,#0x11
  0012f288: ldr.w r0,[r10,#0x0]
  0012f28c: strd r5,r6,[sp,#0x0]
  0012f290: blx 0x00071d70
  0012f294: vldr.32 s0,[r4,#0x10]
  0012f298: movs r2,#0x44
  0012f29a: vldr.32 s2,[r4,#0x14]
  0012f29e: mov.w r8,#0x11
  0012f2a2: ldr.w r1,[r4,#0x98]
  0012f2a6: vcvt.s32.f32 s0,s0
  0012f2aa: ldr.w r0,[r10,#0x0]
  0012f2ae: vcvt.s32.f32 s2,s2
  0012f2b2: strd r8,r2,[sp,#0x0]
  0012f2b6: vmov r2,s0
  0012f2ba: vmov r3,s2
  0012f2be: blx 0x00071d70
  0012f2c2: vldr.32 s0,[r4,#0x10]
  0012f2c6: vldr.32 s2,[r4,#0x14]
  0012f2ca: ldr.w r0,[r4,#0x94]
  0012f2ce: vcvt.s32.f32 s0,s0
  0012f2d2: vcvt.s32.f32 s2,s2
  0012f2d6: vmov r1,s0
  0012f2da: vmov r2,s2
  0012f2de: blx 0x000762ac
  0012f2e2: ldr.w r0,[r4,#0x94]
  0012f2e6: mov.w r1,#0x3f800000
  0012f2ea: mov.w r2,#0x3f800000
  0012f2ee: blx 0x0007606c
  0012f2f2: ldr.w r11,[sp,#0x38]
  0012f2f6: ldr.w r0,[r11,#0x0]
  0012f2fa: ldrd r2,r3,[r4,#0x50]
  0012f2fe: ldr.w r1,[r4,#0xa4]
  0012f302: ldr.w r6,[r0,#0xfc]
  0012f306: ldr.w r0,[r10,#0x0]
  0012f30a: subs r2,r2,r6
  0012f30c: subs r3,r3,r6
  0012f30e: blx 0x00072dc0
  0012f312: ldr r0,[r4,#0x20]
  0012f314: movw r1,#0x342
  0012f318: cmp r0,r1
  0012f31a: blt 0x0012f36a
  0012f31c: ldr r5,[0x0012f6b8]
  0012f31e: ldr.w r0,[r11,#0x0]
  0012f322: ldr.w r9,[r10,#0x0]
  0012f326: mov r1,r5
  0012f328: blx 0x00074674
  0012f32c: ldr.w r1,[r11,#0x0]
  0012f330: vmov s18,r0
  0012f334: vldr.32 s16,[pc,#0x384]
  0012f338: mov r0,r1
  0012f33a: mov r1,r5
  0012f33c: blx 0x00074674
  0012f340: vmov s0,r0
  0012f344: movs r0,#0xff
  0012f346: vmul.f32 s2,s18,s16
  0012f34a: movs r1,#0xff
  0012f34c: vmul.f32 s0,s0,s16
  0012f350: vcvt.u32.f32 s2,s2
  0012f354: vcvt.u32.f32 s0,s0
  0012f358: str r0,[sp,#0x0]
  0012f35a: mov r0,r9
  0012f35c: vmov r2,s2
  0012f360: vmov r3,s0
  0012f364: blx 0x0006ff28
  0012f368: ldr r0,[r4,#0x20]
  0012f36a: vmov s0,r0
  0012f36e: vldr.32 s2,[pc,#0x350]
  0012f372: add.w r6,r4,#0x4c
  0012f376: movs r5,#0x0
  0012f378: vcvt.f32.s32 s0,s0
  0012f37c: vsub.f32 s0,s2,s0
  0012f380: vdiv.f32 s0,s0,s2
  0012f384: vldr.32 s2,[r4,#0x48]
  0012f388: vcvt.f32.s32 s2,s2
  0012f38c: ldr.w r1,[r4,#0xa8]
  0012f390: ldmia r6,{r2,r3,r6}
  0012f392: ldr.w r0,[r10,#0x0]
  0012f396: vmul.f32 s0,s0,s2
  0012f39a: vcvt.s32.f32 s0,s0
  0012f39e: strd r2,r5,[sp,#0x4]
  0012f3a2: strd r5,r5,[sp,#0xc]
  0012f3a6: movs r2,#0x0
  0012f3a8: strd r3,r6,[sp,#0x14]
  0012f3ac: movs r3,#0x0
  0012f3ae: vstr.32 s0,[sp]
  0012f3b2: blx 0x00072d9c
  0012f3b6: ldr.w r0,[r10,#0x0]
  0012f3ba: mov.w r1,#0xffffffff
  0012f3be: blx 0x0006fac0
  0012f3c2: ldrd r3,r2,[r4,#0x54]
  0012f3c6: movs r6,#0x24
  0012f3c8: ldr.w r1,[r4,#0xc4]
  0012f3cc: ldr.w r0,[r10,#0x0]
  0012f3d0: subs r3,#0x3
  0012f3d2: strd r8,r6,[sp,#0x0]
  0012f3d6: blx 0x00071d70
  0012f3da: ldr.w r0,[r4,#0x8c]
  0012f3de: blx 0x000762b8
  0012f3e2: ldr.w r0,[r4,#0x90]
  0012f3e6: blx 0x000762b8
  0012f3ea: vldr.32 s4,[r4,#0x34]
  0012f3ee: vldr.32 s6,[r4,#0x40]
  0012f3f2: vldr.32 s0,[r4,#0x10]
  0012f3f6: vldr.32 s2,[r4,#0x14]
  0012f3fa: vcvt.f32.s32 s6,s6
  0012f3fe: vcvt.f32.s32 s4,s4
  0012f402: vadd.f32 s0,s0,s6
  0012f406: vsub.f32 s2,s2,s4
  0012f40a: vcvt.s32.f32 s0,s0
  0012f40e: ldr.w r0,[r4,#0x88]
  0012f412: vcvt.s32.f32 s2,s2
  0012f416: vmov r1,s0
  0012f41a: vmov r2,s2
  0012f41e: blx 0x000762c4
  0012f422: vldr.32 s4,[r4,#0x40]
  0012f426: vldr.32 s0,[r4,#0x10]
  0012f42a: vldr.32 s2,[r4,#0x14]
  0012f42e: ldr.w r0,[r11,#0x0]
  0012f432: vcvt.f32.s32 s4,s4
  0012f436: vldr.32 s6,[r0,#0xfc]
  0012f43a: vldr.32 s8,[r0,#0x100]
  0012f43e: vcvt.f32.s32 s8,s8
  0012f442: vcvt.f32.s32 s6,s6
  0012f446: ldr.w r1,[r4,#0x9c]
  0012f44a: vadd.f32 s0,s0,s4
  0012f44e: vsub.f32 s2,s2,s8
  0012f452: vsub.f32 s0,s0,s6
  0012f456: vcvt.s32.f32 s2,s2
  0012f45a: ldr.w r0,[r10,#0x0]
  0012f45e: vcvt.s32.f32 s0,s0
  0012f462: vmov r3,s2
  0012f466: vmov r2,s0
  0012f46a: blx 0x00072dc0
  0012f46e: vldr.32 s0,[r4,#0x24]
  0012f472: add r0,sp,#0x4c
  0012f474: vcvt.s32.f32 s0,s0
  0012f478: vmov r1,s0
  0012f47c: blx 0x0006f658
  0012f480: ldr r1,[0x0012f6c8]
  0012f482: add r1,pc
  0012f484: add r0,sp,#0x40
  0012f486: movs r2,#0x0
  0012f488: blx 0x0006ee18
  0012f48c: add r0,sp,#0x58
  0012f48e: add r1,sp,#0x4c
  0012f490: add r2,sp,#0x40
  0012f492: blx 0x0006ef98
  0012f496: add r0,sp,#0x40
  0012f498: blx 0x0006ee30
  0012f49c: add r0,sp,#0x4c
  0012f49e: blx 0x0006ee30
  0012f4a2: ldr r0,[0x0012f6cc]
  0012f4a4: add r0,pc
  0012f4a6: ldr.w r11,[r0,#0x0]
  0012f4aa: ldr.w r0,[r11,#0x0]
  0012f4ae: blx 0x00071a58
  0012f4b2: blx 0x000722ec
  0012f4b6: mov r1,r0
  0012f4b8: vldr.32 s16,[pc,#0x200]
  0012f4bc: vldr.32 s2,[r4,#0x64]
  0012f4c0: vldr.32 s0,[r4,#0x24]
  0012f4c4: vmul.f32 s2,s2,s16
  0012f4c8: vcvt.s32.f32 s0,s0
  0012f4cc: ldr r0,[sp,#0x3c]
  0012f4ce: ldr r0,[r0,#0x0]
  0012f4d0: vcvt.u32.f32 s2,s2
  0012f4d4: vmov r3,s0
  0012f4d8: vmov r2,s2
  0012f4dc: cmp r1,r3
  0012f4de: bge 0x0012f504
  0012f4e0: str r2,[sp,#0x0]
  0012f4e2: movs r1,#0xff
  0012f4e4: movs r2,#0x2a
  0012f4e6: movs r3,#0x0
  0012f4e8: blx 0x0006ff28
  0012f4ec: b 0x0012f510
  0012f504: str r2,[sp,#0x0]
  0012f506: movs r1,#0xff
  0012f508: movs r2,#0xff
  0012f50a: movs r3,#0xff
  0012f50c: blx 0x0006ff28
  0012f510: ldr r0,[0x0012f6d0]
  0012f512: add r0,pc
  0012f514: ldr r1,[r0,#0x0]
  0012f516: ldr r0,[sp,#0x38]
  0012f518: ldr r0,[r0,#0x0]
  0012f51a: ldrd r8,r6,[r4,#0x40]
  0012f51e: str r1,[sp,#0x34]
  0012f520: ldr.w r9,[r1,#0x0]
  0012f524: ldr r1,[sp,#0x3c]
  0012f526: ldr.w r5,[r0,#0xfc]
  0012f52a: vldr.32 s18,[r4,#0x10]
  0012f52e: ldr.w r10,[r1,#0x0]
  0012f532: add r2,sp,#0x58
  0012f534: mov r0,r10
  0012f536: mov r1,r9
  0012f538: blx 0x0006faa8
  0012f53c: vmov s0,r8
  0012f540: asrs r0,r0,#0x1
  0012f542: vmov s2,r6
  0012f546: vcvt.f32.s32 s0,s0
  0012f54a: vcvt.f32.s32 s2,s2
  0012f54e: ldr r1,[sp,#0x38]
  0012f550: vmov s4,r5
  0012f554: vmov s8,r0
  0012f558: ldr r1,[r1,#0x0]
  0012f55a: vadd.f32 s0,s18,s0
  0012f55e: vldr.32 s6,[r1,#0x104]
  0012f562: vcvt.f32.s32 s4,s4
  0012f566: vadd.f32 s0,s0,s2
  0012f56a: vcvt.f32.s32 s2,s6
  0012f56e: vldr.32 s6,[r4,#0x14]
  0012f572: vcvt.f32.s32 s8,s8
  0012f576: vsub.f32 s0,s0,s4
  0012f57a: vadd.f32 s2,s6,s2
  0012f57e: vsub.f32 s0,s0,s8
  0012f582: movs r0,#0x0
  0012f584: vcvt.s32.f32 s2,s2
  0012f588: str r0,[sp,#0x4]
  0012f58a: add r2,sp,#0x58
  0012f58c: mov r0,r10
  0012f58e: mov r1,r9
  0012f590: vcvt.s32.f32 s0,s0
  0012f594: vstr.32 s2,[sp]
  0012f598: vmov r3,s0
  0012f59c: blx 0x0006fe14
  0012f5a0: ldr r0,[sp,#0x3c]
  0012f5a2: ldr r0,[r0,#0x0]
  0012f5a4: mov.w r1,#0xffffffff
  0012f5a8: blx 0x0006fac0
  0012f5ac: ldr.w r0,[r11,#0x0]
  0012f5b0: blx 0x00071770
  0012f5b4: cmp r0,#0x4
  0012f5b6: bgt 0x0012f666
  0012f5b8: vldr.32 s0,[r4,#0xcc]
  0012f5bc: vldr.32 s2,[pc,#0x104]
  0012f5c0: vcvt.f32.s32 s0,s0
  0012f5c4: vdiv.f32 s0,s0,s2
  0012f5c8: vmul.f32 s0,s0,s16
  0012f5cc: vcvt.s32.f32 s0,s0
  0012f5d0: ldr.w r8,[sp,#0x3c]
  0012f5d4: ldr.w r0,[r8,#0x0]
  0012f5d8: vmov r1,s0
  0012f5dc: cmp r1,#0xff
  0012f5de: it gt
  0012f5e0: rsb.gt.w r1,r1,#0xff
  0012f5e4: uxtb r1,r1
  0012f5e6: str r1,[sp,#0x0]
  0012f5e8: movs r1,#0xff
  0012f5ea: movs r2,#0xff
  0012f5ec: movs r3,#0xff
  0012f5ee: blx 0x0006ff28
  0012f5f2: ldr r0,[0x0012f6d4]
  0012f5f4: add r0,pc
  0012f5f6: ldr r0,[r0,#0x0]
  0012f5f8: ldr r0,[r0,#0x0]
  0012f5fa: mov.w r1,#0x268
  0012f5fe: blx 0x00072f70
  0012f602: mov r1,r0
  0012f604: ldr.w r10,[sp,#0x38]
  0012f608: add r0,sp,#0x4c
  0012f60a: movs r2,#0x0
  0012f60c: blx 0x0006f028
  0012f610: ldr r0,[0x0012f6d8]
  0012f612: ldr.w r5,[r8,#0x0]
  0012f616: add r0,pc
  0012f618: ldr r0,[r0,#0x0]
  0012f61a: ldr r6,[r0,#0x0]
  0012f61c: ldr r0,[sp,#0x34]
  0012f61e: ldr.w r9,[r0,#0x0]
  0012f622: add r2,sp,#0x4c
  0012f624: mov r0,r5
  0012f626: mov r1,r9
  0012f628: blx 0x0006faa8
  0012f62c: ldr.w r1,[r10,#0x0]
  0012f630: ldr r2,[r4,#0x54]
  0012f632: ldr r1,[r1,#0x70]
  0012f634: add r1,r2
  0012f636: movs r3,#0x0
  0012f638: strd r1,r3,[sp,#0x0]
  0012f63c: add.w r1,r6,r6, lsr #0x1f
  0012f640: add.w r0,r0,r0, lsr #0x1f
  0012f644: add r2,sp,#0x4c
  0012f646: asrs r1,r1,#0x1
  0012f648: sub.w r3,r1,r0, asr #0x1
  0012f64c: mov r0,r5
  0012f64e: mov r1,r9
  0012f650: blx 0x0006fe14
  0012f654: ldr.w r0,[r8,#0x0]
  0012f658: mov.w r1,#0xffffffff
  0012f65c: blx 0x0006fac0
  0012f660: add r0,sp,#0x4c
  0012f662: blx 0x0006ee30
  0012f666: add r0,sp,#0x58
  0012f668: blx 0x0006ee30
  0012f66c: ldr r0,[sp,#0x64]
  0012f66e: ldr r1,[sp,#0x20]
  0012f670: ldr r1,[r1,#0x0]
  0012f672: subs r0,r1,r0
  0012f674: itttt eq
  0012f676: add.eq sp,#0x68
  0012f678: vpop.eq {d8,d9}
  0012f67c: add.eq sp,#0x4
  0012f67e: pop.eq.w {r8,r9,r10,r11}
  0012f682: it eq
  0012f684: pop.eq {r4,r5,r6,r7,pc}
  0012f686: blx 0x0006e824
```
