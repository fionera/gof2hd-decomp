# MiningGame::MiningGame
elf 0x11e7a8 body 900
Sig: undefined __thiscall MiningGame(MiningGame * this, int param_1, int param_2, Hud * param_3)

## decompile
```c

/* MiningGame::MiningGame(int, int, Hud*) */

void __thiscall MiningGame::MiningGame(MiningGame *this,int param_1,int param_2,Hud *param_3)

{
  float fVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  Sprite *this_00;
  MarqueeImage *pMVar5;
  ushort uVar6;
  undefined4 *puVar7;
  int *piVar8;
  code *pcVar9;
  int iVar10;
  uint *puVar11;
  int *piVar12;
  int *piVar13;
  uint in_fpscr;
  undefined4 uVar14;
  float fVar15;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  undefined4 uVar16;
  undefined4 in_s14;
  uint local_38;
  int local_34;
  
  iVar4 = DAT_0012eb5c;
  piVar2 = *(int **)(DAT_0012eb50 + 0x12e7c0);
  piVar12 = *(int **)(DAT_0012eb54 + 0x12e7c6);
  _local_38 = CONCAT44(*piVar2,in_s14);
  puVar7 = (undefined4 *)(DAT_0012eb58 + 0x12e7d8);
  *(int *)(this + 0x18) = param_1;
  *(int *)(this + 0x1c) = param_2;
  *(Hud **)(this + 0xd0) = param_3;
  piVar13 = (int *)*puVar7;
  iVar3 = *piVar12 >> 1;
  piVar8 = *(int **)(iVar4 + 0x12e7de);
  *(int *)(this + 0x58) = iVar3;
  iVar10 = *piVar13;
  uVar14 = VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  iVar4 = *(int *)(iVar10 + 0xd0) + (*piVar8 >> 1);
  *(int *)(this + 0x5c) = iVar4;
  uVar16 = VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 200) = *(undefined4 *)(iVar10 + 0xd4);
  this[0x80] = (MiningGame)(param_1 == 7);
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0x9c4;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  *(int *)(this + 0x7c) = param_1;
  *(undefined2 *)(this + 0x81) = 0;
  this[0x83] = (MiningGame)0x0;
  *(undefined4 *)(this + 0x10) = uVar14;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x14) = uVar16;
  iVar4 = Status::getShip();
  iVar4 = Ship::getFirstEquipmentOfSort(iVar4);
  if (iVar4 != 0) {
    uVar14 = Item::getAttribute(iVar4);
    fVar1 = DAT_0012eb44;
    fVar15 = (float)VectorSignedToFloat(uVar14,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 0x2c) = DAT_0012eb48 + (fVar15 / DAT_0012eb44) * 1.5;
    uVar14 = Item::getAttribute(iVar4);
    fVar15 = (float)VectorSignedToFloat(uVar14,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 0x28) = fVar15 / fVar1;
  }
  _local_38 = CONCAT44(local_34,0xffffffff);
  puVar11 = *(uint **)(DAT_0012eb64 + 0x12e8bc);
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar11,0x4e6,&local_38);
  iVar4 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
  this_00 = operator_new(0x40);
  Sprite::Sprite(this_00,local_38,iVar4,iVar4);
  *(Sprite **)(this + 0x94) = this_00;
  Sprite::defineReferencePixel(this_00,iVar4 >> 1,iVar4 >> 1);
  iVar4 = DAT_0012eb68;
  *(undefined4 *)(this + 0x68) = 0;
  pcVar9 = *(code **)(iVar4 + 0x12e904);
  (*pcVar9)(*puVar11,0x4e2,this + 0xac);
  (*pcVar9)(*puVar11,0x4dd,this + 0xb0);
  (*pcVar9)(*puVar11,0x4de,this + 0xb4);
  (*pcVar9)(*puVar11,0x4e1,this + 0xb8);
  (*pcVar9)(*puVar11,0x4df,this + 0xbc);
  (*pcVar9)(*puVar11,0x4e0,this + 0xc0);
  (*pcVar9)(*puVar11,0x4e5,this + 0x9c);
  (*pcVar9)(*puVar11,0x4e4,this + 0xa0);
  (*pcVar9)(*puVar11,0x4e7,this + 0x98);
  (*pcVar9)(*puVar11,0x4e3,this + 0xa4);
  (*pcVar9)(*puVar11,0x4e8,this + 0xa8);
  (*pcVar9)(*puVar11,0x4ed,this + 0xc4);
  if (this[0x80] != (MiningGame)0x0) {
    uVar6 = 0x523;
    if (param_2 == 0xa4) {
      uVar6 = 0x522;
    }
    AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar11,uVar6,(uint *)(this + 0x60));
  }
  uVar14 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar11);
  *(undefined4 *)(this + 0x48) = uVar14;
  iVar4 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
  *(int *)(this + 0x4c) = iVar4;
  iVar10 = *piVar12 / 2 - *(int *)(this + 0x48) / 2;
  *(int *)(this + 0x50) = iVar10;
  iVar3 = *(int *)(*piVar13 + 0xd8);
  *(int *)(this + 0x54) = iVar3;
  pMVar5 = operator_new(0x24);
  MarqueeImage::MarqueeImage(pMVar5,0x4eb,*(int *)(this + 200),iVar10,iVar4 + iVar3 + 5,extraout_s0)
  ;
  *(MarqueeImage **)(this + 0x8c) = pMVar5;
  pMVar5 = operator_new(0x24);
  MarqueeImage::MarqueeImage
            (pMVar5,0x4ec,*(int *)(this + 200),
             (*(int *)(this + 0x50) - *(int *)(this + 200)) + *(int *)(this + 0x48),
             *(int *)(this + 0x4c) + *(int *)(this + 0x54) + 5,extraout_s0_00);
  puVar7 = (undefined4 *)(DAT_0012eb6c + 0x12ea64);
  *(MarqueeImage **)(this + 0x90) = pMVar5;
  pcVar9 = (code *)*puVar7;
  iVar4 = (*pcVar9)(*puVar11,*(undefined4 *)(this + 0x98));
  *(int *)(this + 0x40) = iVar4 / 2 + 5;
  iVar4 = (*pcVar9)(*puVar11,*(undefined4 *)(this + 0x9c));
  *(int *)(this + 0x44) = iVar4 >> 1;
  uVar14 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
  *(undefined4 *)(this + 0x34) = uVar14;
  pMVar5 = operator_new(0x24);
  iVar4 = (*pcVar9)(*puVar11,*(undefined4 *)(this + 0x9c));
  MarqueeImage::MarqueeImage(pMVar5,0x4e4,iVar4 + -8,0,0,extraout_s0_01);
  piVar8 = (int *)(DAT_0012eb70 + 0x12ead0);
  *(MarqueeImage **)(this + 0x88) = pMVar5;
  MarqueeImage::setSpeed
            (pMVar5,*(float *)(*piVar13 + 0xe0) * *(float *)(*piVar8 + *(int *)(this + 0x78) * 4));
  *(undefined4 *)(this + 0xcc) = 0;
  *(undefined4 *)(this + 100) = 0x3f800000;
  iVar4 = Status::getCurrentCampaignMission();
  this[0x84] = (MiningGame)(4 < iVar4);
  iVar4 = *piVar2 - local_34;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  0012e7a8: push {r4,r5,r6,r7,lr}
  0012e7aa: add r7,sp,#0xc
  0012e7ac: push {r7,r8,r9,r10,r11}
  0012e7b0: vpush {d5,d6,d7,d8}
  0012e7b4: mov r4,r0
  0012e7b6: ldr r0,[0x0012eb50]
  0012e7b8: mov r8,r2
  0012e7ba: ldr r2,[0x0012eb54]
  0012e7bc: add r0,pc
  0012e7be: vmov.i32 q8,#0x0
  0012e7c2: add r2,pc
  0012e7c4: ldr r0,[r0,#0x0]
  0012e7c6: str r0,[sp,#0xc]
  0012e7c8: ldr.w r10,[r2,#0x0]
  0012e7cc: ldr r0,[r0,#0x0]
  0012e7ce: ldr r2,[0x0012eb58]
  0012e7d0: ldr r5,[0x0012eb5c]
  0012e7d2: str r0,[sp,#0x14]
  0012e7d4: add r2,pc
  0012e7d6: strd r1,r8,[r4,#0x18]
  0012e7da: add r5,pc
  0012e7dc: str.w r3,[r4,#0xd0]
  0012e7e0: ldr.w r0,[r10,#0x0]
  0012e7e4: ldr.w r11,[r2,#0x0]
  0012e7e8: ldr r3,[0x0012eb60]
  0012e7ea: asrs r0,r0,#0x1
  0012e7ec: ldr r2,[r5,#0x0]
  0012e7ee: str r0,[r4,#0x58]
  0012e7f0: add r3,pc
  0012e7f2: ldr.w r6,[r11,#0x0]
  0012e7f6: vmov s0,r0
  0012e7fa: ldr r2,[r2,#0x0]
  0012e7fc: movs r5,#0x0
  0012e7fe: ldr.w r12,[r3,#0x0]
  0012e802: ldr.w r3,[r6,#0xd0]
  0012e806: vcvt.f32.s32 s0,s0
  0012e80a: add.w r0,r3,r2, asr #0x1
  0012e80e: str r0,[r4,#0x5c]
  0012e810: ldr.w r2,[r6,#0xd4]
  0012e814: vmov s2,r0
  0012e818: subs r0,r1,#0x7
  0012e81a: clz r0,r0
  0012e81e: vcvt.f32.s32 s2,s2
  0012e822: str.w r2,[r4,#0xc8]
  0012e826: lsrs r0,r0,#0x5
  0012e828: strb.w r0,[r4,#0x80]
  0012e82c: movw r0,#0x9c4
  0012e830: strd r5,r5,[r4,#0x20]
  0012e834: str r5,[r4,#0x28]
  0012e836: strd r5,r0,[r4,#0x6c]
  0012e83a: movs r0,#0x14
  0012e83c: strd r5,r5,[r4,#0x74]
  0012e840: str r1,[r4,#0x7c]
  0012e842: mov r1,r4
  0012e844: strh.w r5,[r4,#0x81]
  0012e848: strb.w r5,[r4,#0x83]
  0012e84c: vstr.32 s0,[r4,#0x10]
  0012e850: vst1.32 {d16,d17},[r1],r0
  0012e854: vstr.32 s2,[r1]
  0012e858: ldr.w r0,[r12,#0x0]
  0012e85c: str.w r12,[sp,#0x8]
  0012e860: blx 0x00071a58
  0012e864: movs r1,#0x13
  0012e866: blx 0x0007228c
  0012e86a: mov r6,r0
  0012e86c: cbz r0,0x0012e8ae
  0012e86e: mov r0,r6
  0012e870: movs r1,#0x20
  0012e872: blx 0x000718e4
  0012e876: vmov s0,r0
  0012e87a: vldr.32 s16,[pc,#0x2c8]
  0012e87e: vmov.f32 s2,0x3fc00000
  0012e882: vldr.32 s4,[pc,#0x2c4]
  0012e886: vcvt.f32.s32 s0,s0
  0012e88a: mov r0,r6
  0012e88c: movs r1,#0x21
  0012e88e: vdiv.f32 s0,s0,s16
  0012e892: vmla.f32 s4,s0,s2
  0012e896: vstr.32 s4,[r4,#0x2c]
  0012e89a: blx 0x000718e4
  0012e89e: vmov s0,r0
  0012e8a2: vcvt.f32.s32 s0,s0
  0012e8a6: vdiv.f32 s0,s0,s16
  0012e8aa: vstr.32 s0,[r4,#0x28]
  0012e8ae: ldr r0,[0x0012eb64]
  0012e8b0: mov.w r1,#0xffffffff
  0012e8b4: add r2,sp,#0x10
  0012e8b6: str r1,[sp,#0x10]
  0012e8b8: add r0,pc
  0012e8ba: movw r1,#0x4e6
  0012e8be: ldr.w r9,[r0,#0x0]
  0012e8c2: ldr.w r0,[r9,#0x0]
  0012e8c6: blx 0x00071cf8
  0012e8ca: ldr.w r0,[r9,#0x0]
  0012e8ce: ldr r1,[sp,#0x10]
  0012e8d0: blx 0x00072d90
  0012e8d4: mov r6,r0
  0012e8d6: movs r0,#0x40
  0012e8d8: blx 0x0006eb24
  0012e8dc: ldr r1,[sp,#0x10]
  0012e8de: mov r5,r0
  0012e8e0: mov r2,r6
  0012e8e2: mov r3,r6
  0012e8e4: blx 0x00076258
  0012e8e8: asrs r1,r6,#0x1
  0012e8ea: mov r0,r5
  0012e8ec: mov r2,r1
  0012e8ee: str.w r5,[r4,#0x94]
  0012e8f2: blx 0x00076264
  0012e8f6: ldr r0,[0x0012eb68]
  0012e8f8: movs r1,#0x0
  0012e8fa: str r1,[r4,#0x68]
  0012e8fc: add.w r2,r4,#0xac
  0012e900: add r0,pc
  0012e902: movw r1,#0x4e2
  0012e906: ldr r5,[r0,#0x0]
  0012e908: ldr.w r0,[r9,#0x0]
  0012e90c: blx r5
  0012e90e: ldr.w r0,[r9,#0x0]
  0012e912: add.w r2,r4,#0xb0
  0012e916: movw r1,#0x4dd
  0012e91a: blx r5
  0012e91c: ldr.w r0,[r9,#0x0]
  0012e920: add.w r2,r4,#0xb4
  0012e924: movw r1,#0x4de
  0012e928: blx r5
  0012e92a: ldr.w r0,[r9,#0x0]
  0012e92e: add.w r2,r4,#0xb8
  0012e932: movw r1,#0x4e1
  0012e936: blx r5
  0012e938: ldr.w r0,[r9,#0x0]
  0012e93c: add.w r2,r4,#0xbc
  0012e940: movw r1,#0x4df
  0012e944: blx r5
  0012e946: ldr.w r0,[r9,#0x0]
  0012e94a: add.w r2,r4,#0xc0
  0012e94e: mov.w r1,#0x4e0
  0012e952: blx r5
  0012e954: ldr.w r0,[r9,#0x0]
  0012e958: add.w r2,r4,#0x9c
  0012e95c: movw r1,#0x4e5
  0012e960: blx r5
  0012e962: ldr.w r0,[r9,#0x0]
  0012e966: add.w r2,r4,#0xa0
  0012e96a: movw r1,#0x4e4
  0012e96e: blx r5
  0012e970: ldr.w r0,[r9,#0x0]
  0012e974: add.w r2,r4,#0x98
  0012e978: movw r1,#0x4e7
  0012e97c: blx r5
  0012e97e: ldr.w r0,[r9,#0x0]
  0012e982: add.w r2,r4,#0xa4
  0012e986: movw r1,#0x4e3
  0012e98a: blx r5
  0012e98c: ldr.w r0,[r9,#0x0]
  0012e990: add.w r2,r4,#0xa8
  0012e994: mov.w r1,#0x4e8
  0012e998: blx r5
  0012e99a: ldr.w r0,[r9,#0x0]
  0012e99e: add.w r2,r4,#0xc4
  0012e9a2: movw r1,#0x4ed
  0012e9a6: blx r5
  0012e9a8: ldrb.w r0,[r4,#0x80]
  0012e9ac: cbz r0,0x0012e9c8
  0012e9ae: ldr.w r0,[r9,#0x0]
  0012e9b2: movw r1,#0x523
  0012e9b6: add.w r2,r4,#0x60
  0012e9ba: cmp.w r8,#0xa4
  0012e9be: it eq
  0012e9c0: movw.eq r1,#0x522
  0012e9c4: blx 0x00071cf8
  0012e9c8: ldr.w r1,[r4,#0xa8]
  0012e9cc: ldr.w r0,[r9,#0x0]
  0012e9d0: blx 0x00072d84
  0012e9d4: ldr.w r1,[r4,#0xa8]
  0012e9d8: str r0,[r4,#0x48]
  0012e9da: ldr.w r0,[r9,#0x0]
  0012e9de: blx 0x00072d90
  0012e9e2: mov r6,r0
  0012e9e4: ldr r0,[r4,#0x48]
  0012e9e6: str r6,[r4,#0x4c]
  0012e9e8: ldr.w r1,[r10,#0x0]
  0012e9ec: add.w r0,r0,r0, lsr #0x1f
  0012e9f0: add.w r1,r1,r1, lsr #0x1f
  0012e9f4: asrs r1,r1,#0x1
  0012e9f6: sub.w r10,r1,r0, asr #0x1
  0012e9fa: str.w r10,[r4,#0x50]
  0012e9fe: ldr.w r0,[r11,#0x0]
  0012ea02: ldr.w r5,[r0,#0xd8]
  0012ea06: movs r0,#0x24
  0012ea08: str r5,[r4,#0x54]
  0012ea0a: blx 0x0006eb24
  0012ea0e: mov r8,r0
  0012ea10: ldr r0,[0x0012eb4c]
  0012ea12: ldr.w r2,[r4,#0xc8]
  0012ea16: adds r1,r6,r5
  0012ea18: mov r3,r10
  0012ea1a: adds r1,#0x5
  0012ea1c: strd r1,r0,[sp,#0x0]
  0012ea20: mov r0,r8
  0012ea22: movw r1,#0x4eb
  0012ea26: blx 0x00076270
  0012ea2a: movs r0,#0x24
  0012ea2c: str.w r8,[r4,#0x8c]
  0012ea30: blx 0x0006eb24
  0012ea34: add.w r6,r4,#0x48
  0012ea38: mov r5,r0
  0012ea3a: ldmia r6,{r0,r1,r3,r6}
  0012ea3c: ldr.w r2,[r4,#0xc8]
  0012ea40: add r1,r6
  0012ea42: mov.w r12,#0x42000000
  0012ea46: adds r1,#0x5
  0012ea48: strd r1,r12,[sp,#0x0]
  0012ea4c: subs r1,r3,r2
  0012ea4e: adds r3,r1,r0
  0012ea50: mov r0,r5
  0012ea52: movw r1,#0x4ec
  0012ea56: blx 0x00076270
  0012ea5a: ldr r2,[0x0012eb6c]
  0012ea5c: ldr.w r1,[r4,#0x98]
  0012ea60: add r2,pc
  0012ea62: str.w r5,[r4,#0x90]
  0012ea66: ldr.w r0,[r9,#0x0]
  0012ea6a: ldr r6,[r2,#0x0]
  0012ea6c: blx r6
  0012ea6e: add.w r0,r0,r0, lsr #0x1f
  0012ea72: movs r2,#0x5
  0012ea74: ldr.w r1,[r4,#0x9c]
  0012ea78: add.w r0,r2,r0, asr #0x1
  0012ea7c: str r0,[r4,#0x40]
  0012ea7e: ldr.w r0,[r9,#0x0]
  0012ea82: blx r6
  0012ea84: asrs r0,r0,#0x1
  0012ea86: ldr.w r1,[r4,#0xa0]
  0012ea8a: str r0,[r4,#0x44]
  0012ea8c: ldr.w r0,[r9,#0x0]
  0012ea90: blx 0x00072d90
  0012ea94: str r0,[r4,#0x34]
  0012ea96: ldr.w r0,[r11,#0x0]
  0012ea9a: vldr.32 s16,[r0,#0xdc]
  0012ea9e: movs r0,#0x24
  0012eaa0: blx 0x0006eb24
  0012eaa4: mov r5,r0
  0012eaa6: ldr.w r0,[r9,#0x0]
  0012eaaa: ldr.w r1,[r4,#0x9c]
  0012eaae: blx r6
  0012eab0: sub.w r2,r0,#0x8
  0012eab4: movs r6,#0x0
  0012eab6: mov r0,r5
  0012eab8: movw r1,#0x4e4
  0012eabc: movs r3,#0x0
  0012eabe: vstr.32 s16,[sp,#0x4]
  0012eac2: str r6,[sp,#0x0]
  0012eac4: blx 0x00076270
  0012eac8: ldr r0,[0x0012eb70]
  0012eaca: ldr r1,[r4,#0x78]
  0012eacc: add r0,pc
  0012eace: str.w r5,[r4,#0x88]
  0012ead2: ldr.w r2,[r11,#0x0]
  0012ead6: ldr r0,[r0,#0x0]
  0012ead8: add.w r0,r0,r1, lsl #0x2
  0012eadc: vldr.32 s2,[r2,#0xe0]
  0012eae0: vldr.32 s0,[r0]
  0012eae4: mov r0,r5
  0012eae6: vmul.f32 s0,s2,s0
  0012eaea: vmov r1,s0
  0012eaee: blx 0x0007627c
  0012eaf2: mov.w r0,#0x3f800000
  0012eaf6: str.w r6,[r4,#0xcc]
  0012eafa: str r0,[r4,#0x64]
  0012eafc: ldr r0,[sp,#0x8]
  0012eafe: ldr r0,[r0,#0x0]
  0012eb00: blx 0x00071770
  0012eb04: cmp r0,#0x4
  0012eb06: it gt
  0012eb08: mov.gt r6,#0x1
  0012eb0a: strb.w r6,[r4,#0x84]
  0012eb0e: ldr r0,[sp,#0x14]
  0012eb10: ldr r1,[sp,#0xc]
  0012eb12: ldr r1,[r1,#0x0]
  0012eb14: subs r0,r1,r0
  0012eb16: itttt eq
  0012eb18: mov.eq r0,r4
  0012eb1a: vpop.eq {d5,d6,d7,d8}
  0012eb1e: add.eq sp,#0x4
  0012eb20: pop.eq.w {r8,r9,r10,r11}
  0012eb24: it eq
  0012eb26: pop.eq {r4,r5,r6,r7,pc}
  0012eb28: blx 0x0006e824
```
