# Layout::drawBGBorder
elf 0xd395c body 662
Sig: undefined __thiscall drawBGBorder(Layout * this, uint param_1, uint param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8)

## decompile
```c

/* Layout::drawBGBorder(unsigned int, unsigned int, int, int, int, int, int, int) */

void __thiscall
Layout::drawBGBorder
          (Layout *this,uint param_1,uint param_2,int param_3,int param_4,int param_5,int param_6,
          int param_7,int param_8)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint *puVar8;
  code *pcVar9;
  int iVar10;
  int iVar11;
  float extraout_s0;
  float fVar12;
  float extraout_s0_00;
  
  puVar8 = *(uint **)(DAT_000e3c00 + 0xe3972);
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar8);
  iVar2 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar8);
  iVar3 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar8);
  iVar4 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar8);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar8,param_1,param_7 + param_3);
  iVar11 = ((param_5 + param_3) - iVar1) - param_7;
  pcVar9 = *(code **)(DAT_000e3c04 + 0xe39d2);
  (*pcVar9)(*puVar8,param_1,iVar11,param_7 + param_4,1);
  iVar6 = ((param_4 + param_6) - iVar2) - param_7;
  (*pcVar9)(*puVar8,param_1,param_7 + param_3,iVar6,2);
  (*pcVar9)(*puVar8,param_1,iVar11,iVar6,3);
  iVar6 = param_5 + iVar1 * -2 + param_7 * -2;
  iVar11 = __aeabi_idiv(iVar6,iVar3);
  iVar5 = param_8 + param_3;
  iVar7 = iVar1 + param_8 + param_3;
  fVar12 = extraout_s0;
  for (iVar10 = 0; iVar10 < iVar11; iVar10 = iVar10 + 1) {
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar8,param_2,iVar7);
    fVar12 = (float)AbyssEngine::PaintCanvas::DrawImage2D
                              ((PaintCanvas *)*puVar8,param_2,iVar7,
                               (param_6 - param_7) + param_8 + param_4,'\x02');
    iVar7 = iVar7 + iVar3;
  }
  iVar7 = iVar6 - iVar11 * iVar3;
  if (0 < iVar7) {
    fVar12 = (float)AbyssEngine::PaintCanvas::DrawRegion2D
                              ((PaintCanvas *)*puVar8,param_2,0,0,iVar7,iVar4,fVar12,0,0,0,
                               iVar11 * iVar3 + iVar5 + iVar1);
    AbyssEngine::PaintCanvas::DrawRegion2D
              ((PaintCanvas *)*puVar8,param_2,0,0,iVar7,iVar4,fVar12,DAT_000e3bf4,0,0,
               iVar6 + iVar5 + iVar1);
  }
  iVar1 = param_6 + iVar2 * -2 + param_7 * -2;
  iVar2 = __aeabi_idiv(iVar1,iVar3);
  iVar6 = param_5 + param_3 + param_8;
  fVar12 = extraout_s0_00;
  for (iVar11 = 0; iVar11 < iVar2; iVar11 = iVar11 + 1) {
    fVar12 = (float)AbyssEngine::PaintCanvas::DrawRegion2D
                              ((PaintCanvas *)*puVar8,param_2,0,0,iVar3,iVar4,fVar12,DAT_000e3bf8,0,
                               0,iVar5);
    fVar12 = (float)AbyssEngine::PaintCanvas::DrawRegion2D
                              ((PaintCanvas *)*puVar8,param_2,0,0,iVar3,iVar4,fVar12,DAT_000e3bfc,0,
                               0,iVar6);
  }
  iVar1 = iVar1 - iVar2 * iVar3;
  if (0 < iVar1) {
    fVar12 = (float)AbyssEngine::PaintCanvas::DrawRegion2D
                              ((PaintCanvas *)*puVar8,param_2,0,0,iVar1,iVar4,fVar12,DAT_000e3bf8,0,
                               0,iVar5);
    AbyssEngine::PaintCanvas::DrawRegion2D
              ((PaintCanvas *)*puVar8,param_2,0,0,iVar1,iVar4,fVar12,DAT_000e3bfc,0,0,
               iVar6 + param_7 * -2);
  }
  return;
}

```

## target disasm
```
  000e395c: push {r4,r5,r6,r7,lr}
  000e395e: add r7,sp,#0xc
  000e3960: push {r8,r9,r10,r11}
  000e3964: sub sp,#0x54
  000e3966: ldr r0,[0x000e3c00]
  000e3968: mov r11,r3
  000e396a: mov r4,r2
  000e396c: mov r5,r1
  000e396e: add r0,pc
  000e3970: ldr r6,[r0,#0x0]
  000e3972: ldr r0,[r6,#0x0]
  000e3974: blx 0x00072d84
  000e3978: mov r8,r0
  000e397a: str r0,[sp,#0x30]
  000e397c: ldr r0,[r6,#0x0]
  000e397e: mov r1,r5
  000e3980: blx 0x00072d90
  000e3984: str r0,[sp,#0x2c]
  000e3986: mov r1,r4
  000e3988: ldr r0,[r6,#0x0]
  000e398a: blx 0x00072d84
  000e398e: str r0,[sp,#0x50]
  000e3990: mov r1,r4
  000e3992: ldr r0,[r6,#0x0]
  000e3994: str r4,[sp,#0x4c]
  000e3996: blx 0x00072d90
  000e399a: ldr.w r9,[r7,#0x14]
  000e399e: mov r1,r5
  000e39a0: ldr r3,[r7,#0x8]
  000e39a2: str r0,[sp,#0x44]
  000e39a4: add.w r2,r9,r11
  000e39a8: add.w r4,r9,r3
  000e39ac: ldr r0,[r6,#0x0]
  000e39ae: str r2,[sp,#0x48]
  000e39b0: mov r3,r4
  000e39b2: blx 0x00072dc0
  000e39b6: movs r1,#0x1
  000e39b8: ldr r0,[r6,#0x0]
  000e39ba: str r1,[sp,#0x0]
  000e39bc: mov r3,r4
  000e39be: ldr r1,[r7,#0xc]
  000e39c0: add r1,r11
  000e39c2: str r1,[sp,#0x34]
  000e39c4: sub.w r1,r1,r8
  000e39c8: sub.w r10,r1,r9
  000e39cc: ldr r1,[0x000e3c04]
  000e39ce: add r1,pc
  000e39d0: mov r2,r10
  000e39d2: ldr.w r8,[r1,#0x0]
  000e39d6: mov r1,r5
  000e39d8: blx r8
  000e39da: movs r1,#0x2
  000e39dc: ldr r2,[r7,#0x10]
  000e39de: str r1,[sp,#0x0]
  000e39e0: ldr r1,[r7,#0x8]
  000e39e2: ldr r0,[r6,#0x0]
  000e39e4: add r1,r2
  000e39e6: ldr r2,[sp,#0x2c]
  000e39e8: str r1,[sp,#0x24]
  000e39ea: subs r1,r1,r2
  000e39ec: ldr r2,[sp,#0x48]
  000e39ee: sub.w r4,r1,r9
  000e39f2: mov r1,r5
  000e39f4: mov r3,r4
  000e39f6: blx r8
  000e39f8: ldr r0,[r6,#0x0]
  000e39fa: movs r1,#0x3
  000e39fc: str r1,[sp,#0x0]
  000e39fe: mov r1,r5
  000e3a00: mov r2,r10
  000e3a02: mov r3,r4
  000e3a04: str r6,[sp,#0x48]
  000e3a06: blx r8
  000e3a08: ldr r4,[sp,#0x30]
  000e3a0a: mov r6,r9
  000e3a0c: ldr r0,[r7,#0xc]
  000e3a0e: sub.w r0,r0,r4, lsl #0x1
  000e3a12: sub.w r0,r0,r9, lsl #0x1
  000e3a16: ldr.w r9,[sp,#0x50]
  000e3a1a: str r0,[sp,#0x40]
  000e3a1c: mov r1,r9
  000e3a1e: blx 0x0007198c
  000e3a22: mov r5,r0
  000e3a24: ldr r1,[r7,#0x18]
  000e3a26: ldr r0,[r7,#0x8]
  000e3a28: mov.w r8,#0x0
  000e3a2c: adds r2,r1,r0
  000e3a2e: ldr r0,[r7,#0x10]
  000e3a30: str r2,[sp,#0x3c]
  000e3a32: subs r0,r0,r6
  000e3a34: add.w r10,r0,r2
  000e3a38: add.w r0,r1,r11
  000e3a3c: str r0,[sp,#0x38]
  000e3a3e: add r0,r4
  000e3a40: str r0,[sp,#0x20]
  000e3a42: adds r0,r4,r1
  000e3a44: mov r2,r4
  000e3a46: add.w r4,r0,r11
  000e3a4a: lsls r0,r6,#0x1
  000e3a4c: str r0,[sp,#0x28]
  000e3a4e: mul r0,r5,r9
  000e3a52: mov.w r9,#0x2
  000e3a56: str r0,[sp,#0x30]
  000e3a58: ldrd r6,r11,[sp,#0x48]
  000e3a5c: b 0x000e3a82
  000e3a5e: ldr r0,[r6,#0x0]
  000e3a60: mov r1,r11
  000e3a62: ldr r3,[sp,#0x3c]
  000e3a64: mov r2,r4
  000e3a66: blx 0x00072dc0
  000e3a6a: ldr r0,[r6,#0x0]
  000e3a6c: mov r1,r11
  000e3a6e: mov r2,r4
  000e3a70: mov r3,r10
  000e3a72: str.w r9,[sp,#0x0]
  000e3a76: blx 0x00074e3c
  000e3a7a: ldr r0,[sp,#0x50]
  000e3a7c: add.w r8,r8,#0x1
  000e3a80: add r4,r0
  000e3a82: cmp r8,r5
  000e3a84: blt 0x000e3a5e
  000e3a86: ldr.w r10,[r7,#0x18]
  000e3a8a: ldr r0,[sp,#0x40]
  000e3a8c: ldr r1,[sp,#0x30]
  000e3a8e: ldr.w r11,[sp,#0x3c]
  000e3a92: mov r6,r10
  000e3a94: subs r4,r0,r1
  000e3a96: cmp r4,#0x0
  000e3a98: ble 0x000e3aec
  000e3a9a: ldr r6,[sp,#0x48]
  000e3a9c: mov.w r9,#0x0
  000e3aa0: ldr r5,[sp,#0x20]
  000e3aa2: movs r2,#0x0
  000e3aa4: ldr.w r8,[sp,#0x44]
  000e3aa8: movs r3,#0x0
  000e3aaa: ldr r0,[r6,#0x0]
  000e3aac: add r1,r5
  000e3aae: str r4,[sp,#0x0]
  000e3ab0: strd r8,r9,[sp,#0x4]
  000e3ab4: strd r9,r9,[sp,#0xc]
  000e3ab8: strd r1,r11,[sp,#0x14]
  000e3abc: ldr.w r10,[sp,#0x4c]
  000e3ac0: mov r1,r10
  000e3ac2: blx 0x00072d9c
  000e3ac6: ldr r0,[r6,#0x0]
  000e3ac8: movs r3,#0x0
  000e3aca: ldr r6,[r7,#0x18]
  000e3acc: ldr r1,[sp,#0x24]
  000e3ace: ldr r2,[0x000e3bf4]
  000e3ad0: subs r1,r1,r6
  000e3ad2: str r1,[sp,#0x18]
  000e3ad4: ldr r1,[sp,#0x40]
  000e3ad6: add r1,r5
  000e3ad8: strd r9,r1,[sp,#0x10]
  000e3adc: mov r1,r10
  000e3ade: strd r2,r9,[sp,#0x8]
  000e3ae2: movs r2,#0x0
  000e3ae4: strd r4,r8,[sp,#0x0]
  000e3ae8: blx 0x00072d9c
  000e3aec: ldr r4,[sp,#0x2c]
  000e3aee: ldr r0,[r7,#0x10]
  000e3af0: ldr r1,[sp,#0x28]
  000e3af2: ldr r5,[sp,#0x50]
  000e3af4: sub.w r0,r0,r4, lsl #0x1
  000e3af8: subs r0,r0,r1
  000e3afa: str r0,[sp,#0x24]
  000e3afc: mov r1,r5
  000e3afe: blx 0x0007198c
  000e3b02: mul r2,r0,r5
  000e3b06: str r0,[sp,#0x40]
  000e3b08: ldr r0,[sp,#0x34]
  000e3b0a: ldr r1,[r7,#0x8]
  000e3b0c: mov.w r10,#0x0
  000e3b10: add r0,r6
  000e3b12: str r0,[sp,#0x34]
  000e3b14: add.w r0,r11,r4
  000e3b18: str r0,[sp,#0x3c]
  000e3b1a: str r2,[sp,#0x30]
  000e3b1c: adds r0,r4,r6
  000e3b1e: ldr.w r11,[sp,#0x50]
  000e3b22: add.w r8,r0,r1
  000e3b26: ldrd r5,r4,[sp,#0x44]
  000e3b2a: mov.w r9,#0x0
  000e3b2e: ldr r6,[sp,#0x4c]
  000e3b30: b 0x000e3b7e
  000e3b32: ldr r3,[0x000e3bf8]
  000e3b34: add.w r1,r11,r8
  000e3b38: ldr r0,[r4,#0x0]
  000e3b3a: strd r11,r5,[sp,#0x0]
  000e3b3e: strd r3,r9,[sp,#0x8]
  000e3b42: movs r3,#0x0
  000e3b44: ldr r2,[sp,#0x38]
  000e3b46: str.w r9,[sp,#0x10]
  000e3b4a: strd r2,r1,[sp,#0x14]
  000e3b4e: mov r1,r6
  000e3b50: movs r2,#0x0
  000e3b52: blx 0x00072d9c
  000e3b56: ldr r1,[0x000e3bfc]
  000e3b58: movs r2,#0x0
  000e3b5a: ldr r0,[r4,#0x0]
  000e3b5c: movs r3,#0x0
  000e3b5e: strd r11,r5,[sp,#0x0]
  000e3b62: strd r1,r9,[sp,#0x8]
  000e3b66: ldr r1,[sp,#0x34]
  000e3b68: str.w r9,[sp,#0x10]
  000e3b6c: strd r1,r8,[sp,#0x14]
  000e3b70: mov r1,r6
  000e3b72: blx 0x00072d9c
  000e3b76: ldr r2,[sp,#0x30]
  000e3b78: add.w r10,r10,#0x1
  000e3b7c: add r8,r2
  000e3b7e: ldr r0,[sp,#0x40]
  000e3b80: cmp r10,r0
  000e3b82: blt 0x000e3b32
  000e3b84: ldr r1,[sp,#0x24]
  000e3b86: subs r4,r1,r2
  000e3b88: cmp r4,#0x0
  000e3b8a: ble 0x000e3bea
  000e3b8c: ldr.w r9,[sp,#0x48]
  000e3b90: mov.w r8,#0x0
  000e3b94: ldr.w r11,[sp,#0x3c]
  000e3b98: mov r10,r2
  000e3b9a: ldr r6,[sp,#0x44]
  000e3b9c: ldr r3,[0x000e3bf8]
  000e3b9e: add r1,r11
  000e3ba0: ldr.w r0,[r9,#0x0]
  000e3ba4: str r4,[sp,#0x0]
  000e3ba6: str r6,[sp,#0x4]
  000e3ba8: strd r3,r8,[sp,#0x8]
  000e3bac: movs r3,#0x0
  000e3bae: ldr r2,[sp,#0x38]
  000e3bb0: str.w r8,[sp,#0x10]
  000e3bb4: strd r2,r1,[sp,#0x14]
  000e3bb8: movs r2,#0x0
  000e3bba: ldr r5,[sp,#0x4c]
  000e3bbc: mov r1,r5
  000e3bbe: blx 0x00072d9c
  000e3bc2: ldr r2,[sp,#0x28]
  000e3bc4: add.w r1,r11,r10
  000e3bc8: ldr r3,[sp,#0x34]
  000e3bca: ldr.w r0,[r9,#0x0]
  000e3bce: subs r2,r3,r2
  000e3bd0: ldr r3,[0x000e3bfc]
  000e3bd2: strd r4,r6,[sp,#0x0]
  000e3bd6: strd r3,r8,[sp,#0x8]
  000e3bda: movs r3,#0x0
  000e3bdc: strd r8,r2,[sp,#0x10]
  000e3be0: movs r2,#0x0
  000e3be2: str r1,[sp,#0x18]
  000e3be4: mov r1,r5
  000e3be6: blx 0x00072d9c
  000e3bea: add sp,#0x54
  000e3bec: pop.w {r8,r9,r10,r11}
  000e3bf0: pop {r4,r5,r6,r7,pc}
```
