# Engine::ResetLightParam
elf 0x84348 body 406
Sig: undefined __thiscall ResetLightParam(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ResetLightParam() */

void __thiscall AbyssEngine::Engine::ResetLightParam(Engine *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 *puVar12;
  char *pcVar13;
  int iVar14;
  int *piVar15;
  code *pcVar16;
  
  uVar11 = DAT_00094514;
  uVar10 = DAT_00094510;
  uVar9 = DAT_00094508;
  uVar8 = DAT_00094500;
  uVar7 = DAT_000944f8;
  uVar6 = DAT_000944f0;
  uVar5 = DAT_000944e8;
  uVar4 = DAT_000944e0;
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar12 = (undefined4 *)(DAT_00094518 + 0x94374);
  piVar15 = *(int **)(DAT_0009451c + 0x94362);
  iVar14 = *piVar15;
  *(undefined4 *)(this + 0x488) = 0x3f800000;
  *(undefined4 *)(this + 0x32c) = 1;
  *(undefined4 *)(this + 0x298) = uVar10;
  *(undefined4 *)(this + 0x29c) = uVar10;
  *(undefined4 *)(this + 0x2a0) = uVar10;
  *(undefined4 *)(this + 0x2a4) = 0x3f800000;
  *(undefined4 *)(this + 0x2a8) = uVar11;
  *(undefined4 *)(this + 0x2ac) = uVar11;
  *(undefined4 *)(this + 0x2b0) = uVar11;
  *(undefined4 *)(this + 0x2b4) = 0x3f800000;
  *(undefined4 *)(this + 0x2b8) = 0;
  *(undefined4 *)(this + 700) = 0;
  *(undefined4 *)(this + 0x2c0) = 0;
  *(undefined4 *)(this + 0x2c4) = 0x3f800000;
  *(undefined4 *)(this + 0x2c8) = 0;
  *(undefined4 *)(this + 0x268) = 0;
  *(undefined4 *)(this + 0x26c) = 0;
  *(undefined4 *)(this + 0x270) = 0;
  *(undefined4 *)(this + 0x274) = 0x3f800000;
  *(undefined8 *)(this + 0x288) = uVar4;
  *(undefined8 *)(this + 0x290) = uVar5;
  *(undefined8 *)(this + 0x254) = uVar6;
  *(undefined8 *)(this + 0x25c) = uVar7;
  *(undefined8 *)(this + 0x234) = uVar6;
  *(undefined8 *)(this + 0x23c) = uVar7;
  *(undefined8 *)(this + 0x278) = uVar4;
  *(undefined8 *)(this + 0x280) = uVar5;
  *(undefined8 *)(this + 0x304) = uVar8;
  *(undefined8 *)(this + 0x30c) = uVar9;
  *(undefined4 *)(this + 0x314) = 0;
  *(undefined4 *)(this + 0x318) = 0;
  *(undefined4 *)(this + 0x31c) = 0;
  *(undefined4 *)(this + 0x228) = 0x3f800000;
  *(undefined4 *)(this + 0x22c) = 0x3f800000;
  *(undefined4 *)(this + 0x230) = 0x3f800000;
  *(undefined4 *)(this + 0x264) = 0x3f800000;
  *(undefined4 *)(this + 0x244) = 0x3f800000;
  *(undefined4 *)(this + 0x248) = 0x3f800000;
  *(undefined4 *)(this + 0x24c) = 0x3f800000;
  *(undefined4 *)(this + 0x250) = 0x3f800000;
  *(undefined4 *)(this + 0x2fc) = uVar10;
  *(undefined4 *)(this + 0x300) = uVar10;
  pcVar13 = (char *)*puVar12;
  *(undefined4 *)(this + 0x2dc) = 0;
  *(undefined4 *)(this + 0x2e0) = uVar1;
  *(undefined4 *)(this + 0x2e4) = uVar2;
  *(undefined4 *)(this + 0x2e8) = uVar3;
  *(undefined4 *)(this + 0x2ec) = 0;
  *(undefined4 *)(this + 0x2f0) = uVar1;
  *(undefined4 *)(this + 0x2f4) = uVar2;
  *(undefined4 *)(this + 0x2f8) = uVar3;
  *(undefined4 *)(this + 0x2cc) = 0;
  *(undefined4 *)(this + 0x2d0) = uVar1;
  *(undefined4 *)(this + 0x2d4) = uVar2;
  *(undefined4 *)(this + 0x2d8) = uVar3;
  if (*pcVar13 == '\0') {
    pcVar16 = *(code **)(DAT_00094520 + 0x94440);
    (*pcVar16)(0x4000,0x1200,this + 0x268);
    (*pcVar16)(0x4000,0x1201,this + 0x228);
    (*pcVar16)(0x4000,0x1202,this + 0x248);
    pcVar16 = *(code **)(DAT_00094524 + 0x94470);
    (*pcVar16)(0x408,0x1200,this + 0x2a8);
    (*pcVar16)(0x408,0x1201,this + 0x298);
    (*pcVar16)(0x408,0x1202,this + 0x2b8);
    glMaterialf(0x408,0x1601,*(undefined4 *)(this + 0x2c8));
  }
  AEMath::Vector::operator=((Vector *)(this + 0x468),(Vector *)&stack0xffffffcc);
  *(undefined4 *)(this + 0x378) = 0;
  AEMath::Vector::operator=((Vector *)(this + 0x474),(Vector *)&stack0xffffffcc);
  *(undefined4 *)(this + 0x37c) = 0;
  if (*piVar15 != iVar14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00094348: push {r4,r5,r6,r7,lr}
  0009434a: add r7,sp,#0xc
  0009434c: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00094350: ldr r1,[0x00094518]
  00094352: mov r4,r0
  00094354: ldr r3,[0x0009451c]
  00094356: adr r0,[0x944e0]
  00094358: adr r2,[0x944f0]
  0009435a: vld1.64 {d18,d19},[r0]
  0009435e: add r3,pc
  00094360: mov.w r8,#0x3f800000
  00094364: vld1.64 {d20,d21},[r2]
  00094368: mov.w r9,#0x0
  0009436c: vmov.i32 q8,#0x0
  00094370: add r1,pc
  00094372: ldr r6,[r3,#0x0]
  00094374: adr r3,[0x94500]
  00094376: ldr r0,[0x00094510]
  00094378: ldr r2,[0x00094514]
  0009437a: vld1.64 {d22,d23},[r3]
  0009437e: ldr r3,[r6,#0x0]
  00094380: str r3,[sp,#0x18]
  00094382: movs r3,#0x1
  00094384: str.w r8,[r4,#0x488]
  00094388: str.w r3,[r4,#0x32c]
  0009438c: strd r0,r0,[r4,#0x298]
  00094390: strd r0,r8,[r4,#0x2a0]
  00094394: strd r2,r2,[r4,#0x2a8]
  00094398: strd r2,r8,[r4,#0x2b0]
  0009439c: add.w r2,r4,#0x288
  000943a0: strd r9,r9,[r4,#0x2b8]
  000943a4: strd r9,r8,[r4,#0x2c0]
  000943a8: str.w r9,[r4,#0x2c8]
  000943ac: strd r9,r9,[r4,#0x268]
  000943b0: str.w r9,[r4,#0x270]
  000943b4: str.w r8,[r4,#0x274]
  000943b8: vst1.64 {d18,d19},[r2]
  000943bc: add.w r2,r4,#0x254
  000943c0: vst1.32 {d20,d21},[r2]
  000943c4: add.w r2,r4,#0x234
  000943c8: vst1.32 {d20,d21},[r2]
  000943cc: add.w r2,r4,#0x278
  000943d0: vst1.64 {d18,d19},[r2]
  000943d4: add.w r2,r4,#0x304
  000943d8: vst1.32 {d22,d23},[r2]
  000943dc: add.w r2,r4,#0x2ec
  000943e0: strd r9,r9,[r4,#0x314]
  000943e4: str.w r9,[r4,#0x31c]
  000943e8: strd r8,r8,[r4,#0x228]
  000943ec: str.w r8,[r4,#0x230]
  000943f0: str.w r8,[r4,#0x264]
  000943f4: strd r8,r8,[r4,#0x244]
  000943f8: strd r8,r8,[r4,#0x24c]
  000943fc: strd r0,r0,[r4,#0x2fc]
  00094400: add.w r0,r4,#0x2dc
  00094404: ldr r1,[r1,#0x0]
  00094406: vst1.32 {d16,d17},[r0]
  0009440a: add.w r0,r4,#0x2cc
  0009440e: vst1.32 {d16,d17},[r2]
  00094412: vst1.32 {d16,d17},[r0]
  00094416: ldrb r0,[r1,#0x0]
  00094418: cmp r0,#0x0
  0009441a: bne 0x0009449e
  0009441c: add.w r0,r4,#0x2b8
  00094420: str r0,[sp,#0x8]
  00094422: add.w r0,r4,#0x298
  00094426: str r0,[sp,#0x4]
  00094428: add.w r0,r4,#0x2a8
  0009442c: str r0,[sp,#0x0]
  0009442e: ldr r0,[0x00094520]
  00094430: add.w r2,r4,#0x268
  00094434: mov.w r1,#0x1200
  00094438: add.w r5,r4,#0x228
  0009443c: add r0,pc
  0009443e: add.w r11,r4,#0x248
  00094442: ldr.w r10,[r0,#0x0]
  00094446: mov.w r0,#0x4000
  0009444a: blx r10
  0009444c: mov.w r0,#0x4000
  00094450: movw r1,#0x1201
  00094454: mov r2,r5
  00094456: blx r10
  00094458: mov.w r0,#0x4000
  0009445c: movw r1,#0x1202
  00094460: mov r2,r11
  00094462: blx r10
  00094464: ldr r0,[0x00094524]
  00094466: mov.w r1,#0x1200
  0009446a: ldr r2,[sp,#0x0]
  0009446c: add r0,pc
  0009446e: ldr r5,[r0,#0x0]
  00094470: mov.w r0,#0x408
  00094474: blx r5
  00094476: ldr r2,[sp,#0x4]
  00094478: mov.w r0,#0x408
  0009447c: movw r1,#0x1201
  00094480: blx r5
  00094482: ldr r2,[sp,#0x8]
  00094484: mov.w r0,#0x408
  00094488: movw r1,#0x1202
  0009448c: blx r5
  0009448e: ldr.w r2,[r4,#0x2c8]
  00094492: mov.w r0,#0x408
  00094496: movw r1,#0x1601
  0009449a: blx 0x00070534
  0009449e: add.w r0,r4,#0x468
  000944a2: add r1,sp,#0xc
  000944a4: strd r9,r8,[sp,#0xc]
  000944a8: str.w r9,[sp,#0x14]
  000944ac: blx 0x0006eb3c
  000944b0: addw r0,r4,#0x474
  000944b4: add r1,sp,#0xc
  000944b6: str.w r9,[r4,#0x378]
  000944ba: strd r9,r8,[sp,#0xc]
  000944be: str.w r9,[sp,#0x14]
  000944c2: blx 0x0006eb3c
  000944c6: str.w r9,[r4,#0x37c]
  000944ca: ldr r0,[sp,#0x18]
  000944cc: ldr r1,[r6,#0x0]
  000944ce: subs r0,r1,r0
  000944d0: ittt eq
  000944d2: add.eq sp,#0x1c
  000944d4: pop.eq.w {r8,r9,r10,r11}
  000944d8: pop.eq {r4,r5,r6,r7,pc}
  000944da: blx 0x0006e824
```
