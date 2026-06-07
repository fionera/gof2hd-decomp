# Layout::drawBGPattern
elf 0xd3574 body 378
Sig: undefined __thiscall drawBGPattern(Layout * this, uint param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* Layout::drawBGPattern(unsigned int, int, int, int, int) */

void __thiscall
Layout::drawBGPattern(Layout *this,uint param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint *puVar9;
  float extraout_s0;
  float fVar10;
  
  puVar9 = *(uint **)(DAT_000e36f0 + 0xe358a);
  AbyssEngine::PaintCanvas::SetColor(*puVar9);
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar9);
  iVar2 = __aeabi_idiv(param_4,iVar1);
  iVar6 = iVar2 * iVar1;
  iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar9);
  iVar4 = __aeabi_idiv(param_5,iVar3);
  fVar10 = extraout_s0;
  for (iVar7 = 0; iVar7 < iVar4; iVar7 = iVar7 + 1) {
    iVar5 = param_2;
    for (iVar8 = 0; iVar8 < iVar2; iVar8 = iVar8 + 1) {
      fVar10 = (float)AbyssEngine::PaintCanvas::DrawImage2D(*puVar9,param_1,iVar5);
      iVar5 = iVar5 + iVar1;
    }
  }
  iVar7 = param_5 - iVar4 * iVar3;
  iVar5 = param_4 - iVar6;
  if (0 < iVar5) {
    for (iVar8 = 0; iVar8 < iVar4; iVar8 = iVar8 + 1) {
      fVar10 = (float)AbyssEngine::PaintCanvas::DrawRegion2D
                                ((PaintCanvas *)*puVar9,param_1,0,0,iVar5,iVar3,fVar10,0,0,0,
                                 param_2 + iVar6);
    }
  }
  if (0 < iVar7) {
    iVar3 = param_2;
    for (iVar4 = 0; iVar4 < iVar2; iVar4 = iVar4 + 1) {
      fVar10 = (float)AbyssEngine::PaintCanvas::DrawRegion2D
                                ((PaintCanvas *)*puVar9,param_1,0,0,iVar1,iVar7,fVar10,0,0,0,iVar3);
      iVar3 = iVar3 + iVar1;
    }
  }
  if ((0 < iVar5) || (0 < iVar7)) {
    AbyssEngine::PaintCanvas::DrawRegion2D
              ((PaintCanvas *)*puVar9,param_1,0,0,iVar5,iVar7,fVar10,0,0,0,param_2 + iVar6);
  }
  return;
}

```

## target disasm
```
  000e3574: push {r4,r5,r6,r7,lr}
  000e3576: add r7,sp,#0xc
  000e3578: push {r8,r9,r10,r11}
  000e357c: sub sp,#0x44
  000e357e: strd r3,r2,[sp,#0x30]
  000e3582: mov r4,r1
  000e3584: ldr r1,[0x000e36f0]
  000e3586: add r1,pc
  000e3588: ldr.w r11,[r1,#0x0]
  000e358c: ldr.w r1,[r0,#0x3b0]
  000e3590: ldr.w r0,[r11,#0x0]
  000e3594: blx 0x0006fac0
  000e3598: ldr.w r0,[r11,#0x0]
  000e359c: mov r1,r4
  000e359e: blx 0x00072d84
  000e35a2: mov r5,r0
  000e35a4: ldr r0,[r7,#0x8]
  000e35a6: mov r1,r5
  000e35a8: blx 0x0007198c
  000e35ac: mov r6,r0
  000e35ae: ldr.w r0,[r11,#0x0]
  000e35b2: mul r1,r6,r5
  000e35b6: str r1,[sp,#0x24]
  000e35b8: mov r1,r4
  000e35ba: blx 0x00072d90
  000e35be: mov r8,r0
  000e35c0: ldr r0,[r7,#0xc]
  000e35c2: mov r1,r8
  000e35c4: blx 0x0007198c
  000e35c8: strd r8,r0,[sp,#0x38]
  000e35cc: mul r0,r0,r8
  000e35d0: movs r2,#0x0
  000e35d2: str r0,[sp,#0x20]
  000e35d4: b 0x000e3606
  000e35d6: ldr r1,[sp,#0x38]
  000e35d8: mov.w r10,#0x0
  000e35dc: ldr r0,[sp,#0x30]
  000e35de: str r2,[sp,#0x40]
  000e35e0: mla r8,r2,r1,r0
  000e35e4: ldr.w r9,[sp,#0x34]
  000e35e8: b 0x000e35fe
  000e35ea: ldr.w r0,[r11,#0x0]
  000e35ee: mov r1,r4
  000e35f0: mov r2,r9
  000e35f2: mov r3,r8
  000e35f4: blx 0x00072dc0
  000e35f8: add r9,r5
  000e35fa: add.w r10,r10,#0x1
  000e35fe: cmp r10,r6
  000e3600: blt 0x000e35ea
  000e3602: ldr r2,[sp,#0x40]
  000e3604: adds r2,#0x1
  000e3606: ldr r0,[sp,#0x3c]
  000e3608: cmp r2,r0
  000e360a: blt 0x000e35d6
  000e360c: ldr r0,[sp,#0x20]
  000e360e: ldr r1,[r7,#0xc]
  000e3610: subs r0,r1,r0
  000e3612: str r0,[sp,#0x40]
  000e3614: ldr r0,[sp,#0x24]
  000e3616: ldr r1,[r7,#0x8]
  000e3618: subs r0,r1,r0
  000e361a: str r0,[sp,#0x2c]
  000e361c: cmp r0,#0x1
  000e361e: blt 0x000e3664
  000e3620: ldr.w r9,[sp,#0x30]
  000e3624: mov.w r10,#0x0
  000e3628: ldr.w r8,[sp,#0x38]
  000e362c: ldr r0,[sp,#0x34]
  000e362e: ldr r1,[sp,#0x24]
  000e3630: add r0,r1
  000e3632: str r0,[sp,#0x28]
  000e3634: b 0x000e365e
  000e3636: ldr r1,[sp,#0x2c]
  000e3638: movs r2,#0x0
  000e363a: ldr.w r0,[r11,#0x0]
  000e363e: movs r3,#0x0
  000e3640: strd r1,r8,[sp,#0x0]
  000e3644: movs r1,#0x0
  000e3646: strd r1,r1,[sp,#0x8]
  000e364a: str r1,[sp,#0x10]
  000e364c: ldr r1,[sp,#0x28]
  000e364e: strd r1,r9,[sp,#0x14]
  000e3652: mov r1,r4
  000e3654: blx 0x00072d9c
  000e3658: add r9,r8
  000e365a: add.w r10,r10,#0x1
  000e365e: ldr r0,[sp,#0x3c]
  000e3660: cmp r10,r0
  000e3662: blt 0x000e3636
  000e3664: ldr r0,[sp,#0x40]
  000e3666: cmp r0,#0x1
  000e3668: blt 0x000e36ac
  000e366a: ldr.w r9,[sp,#0x34]
  000e366e: mov.w r8,#0x0
  000e3672: ldr r0,[sp,#0x30]
  000e3674: mov.w r10,#0x0
  000e3678: ldr r1,[sp,#0x20]
  000e367a: add r0,r1
  000e367c: str r0,[sp,#0x3c]
  000e367e: b 0x000e36a8
  000e3680: ldr r1,[sp,#0x40]
  000e3682: movs r2,#0x0
  000e3684: ldr.w r0,[r11,#0x0]
  000e3688: movs r3,#0x0
  000e368a: str r5,[sp,#0x0]
  000e368c: strd r1,r8,[sp,#0x4]
  000e3690: strd r8,r8,[sp,#0xc]
  000e3694: ldr r1,[sp,#0x3c]
  000e3696: str r1,[sp,#0x18]
  000e3698: mov r1,r4
  000e369a: str.w r9,[sp,#0x14]
  000e369e: blx 0x00072d9c
  000e36a2: add r9,r5
  000e36a4: add.w r10,r10,#0x1
  000e36a8: cmp r10,r6
  000e36aa: blt 0x000e3680
  000e36ac: ldr r0,[sp,#0x2c]
  000e36ae: cmp r0,#0x0
  000e36b0: bgt 0x000e36b8
  000e36b2: ldr r0,[sp,#0x40]
  000e36b4: cmp r0,#0x1
  000e36b6: blt 0x000e36e6
  000e36b8: ldr r1,[sp,#0x30]
  000e36ba: ldr r2,[sp,#0x20]
  000e36bc: ldr r3,[sp,#0x24]
  000e36be: add r1,r2
  000e36c0: ldr r2,[sp,#0x34]
  000e36c2: ldr r6,[sp,#0x2c]
  000e36c4: ldr.w r0,[r11,#0x0]
  000e36c8: add r2,r3
  000e36ca: str r6,[sp,#0x0]
  000e36cc: movs r3,#0x0
  000e36ce: ldr r6,[sp,#0x40]
  000e36d0: strd r6,r3,[sp,#0x4]
  000e36d4: strd r3,r3,[sp,#0xc]
  000e36d8: movs r3,#0x0
  000e36da: strd r2,r1,[sp,#0x14]
  000e36de: mov r1,r4
  000e36e0: movs r2,#0x0
  000e36e2: blx 0x00072d9c
  000e36e6: add sp,#0x44
  000e36e8: pop.w {r8,r9,r10,r11}
  000e36ec: pop {r4,r5,r6,r7,pc}
```
