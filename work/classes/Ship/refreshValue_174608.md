# Ship::refreshValue
elf 0x174608 body 840
Sig: undefined __thiscall refreshValue(Ship * this)

## decompile
```c

/* Ship::refreshValue() */

void __thiscall Ship::refreshValue(Ship *this)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  float fVar4;
  undefined4 *puVar5;
  int iVar6;
  Standing *pSVar7;
  uint *puVar8;
  Item *pIVar9;
  undefined4 uVar10;
  uint uVar11;
  int iVar12;
  uint uVar13;
  code *pcVar14;
  int *piVar15;
  uint in_fpscr;
  uint uVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  
  uVar10 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar5 = (undefined4 *)(DAT_0018498c + 0x184626);
  *(undefined4 *)(this + 0x4c) = 0xffffffff;
  *(undefined2 *)(this + 0x50) = 0;
  this[0x5c] = (Ship)0x0;
  *(undefined4 *)(this + 0x54) = 0x3f800000;
  *(undefined4 *)(this + 0x58) = 0x3f800000;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = uVar10;
  *(undefined4 *)(this + 0x24) = uVar2;
  *(undefined4 *)(this + 0x28) = uVar3;
  piVar15 = (int *)*puVar5;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = uVar10;
  *(undefined4 *)(this + 0x44) = uVar2;
  *(undefined4 *)(this + 0x48) = uVar3;
  *(undefined4 *)(this + 0x60) = 0xffffffff;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = uVar10;
  *(undefined4 *)(this + 0x38) = uVar2;
  *(undefined4 *)(this + 0x3c) = uVar3;
  if ((*piVar15 != 0) && (iVar6 = Status::getStanding(), iVar6 != 0)) {
    pSVar7 = (Standing *)Status::getStanding();
    Standing::setPlayerSignatureRace(pSVar7,-1);
  }
  *(undefined4 *)(this + 8) = *(undefined4 *)(this + 0x14);
  fVar18 = DAT_0018499c;
  fVar17 = DAT_00184998;
  fVar4 = DAT_00184990;
  uVar13 = 0;
  pcVar14 = *(code **)(DAT_001849a0 + 0x184688);
  while( true ) {
    uVar16 = DAT_00184994;
    puVar8 = *(uint **)(this + 0x6c);
    uVar11 = *puVar8;
    if (uVar11 <= uVar13) break;
    pIVar9 = *(Item **)(puVar8[1] + uVar13 * 4);
    if (pIVar9 != (Item *)0x0) {
      uVar10 = Item::getSort(pIVar9);
      switch(uVar10) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 8:
      case 0x19:
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        fVar20 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        fVar19 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
        *(float *)(this + 0x48) = *(float *)(this + 0x48) + (fVar20 / fVar19) * fVar17;
        break;
      case 9:
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(undefined4 *)(this + 0x1c) = uVar10;
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(undefined4 *)(this + 0x24) = uVar10;
        break;
      case 10:
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(undefined4 *)(this + 0x20) = uVar10;
        break;
      case 0xc:
        iVar6 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(int *)(this + 0x28) = iVar6 + *(int *)(this + 0x28);
        break;
      case 0xe:
        uVar10 = (*pcVar14)(*(undefined4 *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4),0x19);
        *(undefined4 *)(this + 0x34) = uVar10;
        uVar10 = (*pcVar14)(*(undefined4 *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4),0x1b);
        *(undefined4 *)(this + 0x3c) = uVar10;
        uVar10 = (*pcVar14)(*(undefined4 *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4),0x1a);
        *(undefined4 *)(this + 0x38) = uVar10;
        break;
      case 0xf:
        iVar6 = Item::getIndex(*(Item **)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(uint *)(this + 0x4c) = (uint)(iVar6 != 0x4b);
        break;
      case 0x10:
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(undefined4 *)(this + 0x40) = uVar10;
        break;
      case 0x11:
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(undefined4 *)(this + 0x30) = uVar10;
        break;
      case 0x12:
        this[0x50] = (Ship)0x1;
        break;
      case 0x14:
        iVar6 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(int *)(this + 0x44) = iVar6 + *(int *)(this + 0x44);
        break;
      case 0x15:
        this[0x51] = (Ship)0x1;
        break;
      case 0x1b:
        this[0x5c] = (Ship)0x1;
        break;
      case 0x1c:
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        fVar19 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
        fVar19 = 1.0 - fVar19 / fVar4;
        uVar16 = in_fpscr & 0xfffffff | (uint)(fVar19 == fVar18) << 0x1e;
        if ((byte)(uVar16 >> 0x1e) != 0) {
          fVar19 = 1.0;
        }
        *(float *)(this + 0x54) = fVar19;
        uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        fVar19 = (float)VectorSignedToFloat(uVar10,(byte)(uVar16 >> 0x16) & 3);
        fVar19 = fVar19 / fVar4 + 1.0;
        in_fpscr = uVar16 & 0xfffffff | (uint)(fVar19 == fVar18) << 0x1e;
        *(float *)(this + 0x58) = fVar19;
        if ((byte)(in_fpscr >> 0x1e) != 0) {
          *(undefined4 *)(this + 0x58) = 0x3f800000;
        }
        break;
      case 0x1d:
        iVar6 = Item::getIndex(*(Item **)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
        *(int *)(this + 0x60) = iVar6 + -0xbd;
        if ((*piVar15 != 0) && (iVar6 = Status::getStanding(), iVar6 != 0)) {
          pSVar7 = (Standing *)Status::getStanding();
          Standing::setPlayerSignatureRace(pSVar7,*(int *)(this + 0x60));
        }
      }
      iVar6 = Item::getTotalPrice(*(Item **)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
      *(int *)(this + 8) = iVar6 + *(int *)(this + 8);
    }
    uVar13 = uVar13 + 1;
  }
  *(undefined4 *)(this + 0x48) = 0;
  for (uVar13 = 0; uVar13 < uVar11; uVar13 = uVar13 + 1) {
    pIVar9 = *(Item **)(puVar8[1] + uVar13 * 4);
    if (((pIVar9 != (Item *)0x0) && (uVar11 = Item::getSort(pIVar9), uVar11 < 0x1a)) &&
       ((1 << (uVar11 & 0xff) & uVar16) != 0)) {
      uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
      fVar19 = *(float *)(this + 0x58);
      fVar20 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
      uVar10 = Item::getAttribute(*(int *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar13 * 4));
      fVar18 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 0x48) =
           *(float *)(this + 0x48) +
           ((fVar19 * fVar20) / (*(float *)(this + 0x54) * fVar18)) * fVar17;
    }
    puVar8 = *(uint **)(this + 0x6c);
    uVar11 = *puVar8;
  }
  piVar15 = *(int **)(this + 0x78);
  if (piVar15 == (int *)0x0) {
    iVar6 = 0;
  }
  else {
    iVar6 = 0;
    iVar12 = 0;
    while (*piVar15 != iVar12) {
      iVar1 = iVar12 * 4;
      iVar12 = iVar12 + 1;
      if (*(int *)(piVar15[1] + iVar1) == 1) {
        iVar6 = iVar6 + 0x1e;
      }
    }
  }
  uVar10 = *(undefined4 *)(this + 0x28);
  fVar17 = (float)VectorSignedToFloat(uVar10,(byte)(in_fpscr >> 0x16) & 3);
  fVar18 = (float)VectorSignedToFloat(*(int *)(this + 0xc) + iVar6,(byte)(in_fpscr >> 0x16) & 3);
  *(int *)(this + 0x28) = iVar6 + (int)((fVar17 * fVar18) / fVar4);
  *(undefined4 *)(this + 0x2c) = uVar10;
  iVar6 = getCargoValue(this);
  *(int *)(this + 8) = iVar6 + *(int *)(this + 8);
  return;
}

```
## target disasm
```
  00184608: push {r4,r5,r6,r7,lr}
  0018460a: add r7,sp,#0xc
  0018460c: push {r8,r9,r10}
  00184610: vpush {d8,d9,d10,d11,d12}
  00184614: mov r4,r0
  00184616: ldr r0,[0x0018498c]
  00184618: mov.w r1,#0xffffffff
  0018461c: vmov.i32 q8,#0x0
  00184620: movs r2,#0x0
  00184622: add r0,pc
  00184624: str r1,[r4,#0x4c]
  00184626: mov.w r8,#0x3f800000
  0018462a: strh.w r2,[r4,#0x50]
  0018462e: strb.w r2,[r4,#0x5c]
  00184632: add.w r2,r4,#0x1c
  00184636: strd r8,r8,[r4,#0x54]
  0018463a: vst1.32 {d16,d17},[r2]
  0018463e: add.w r2,r4,#0x3c
  00184642: ldr.w r9,[r0,#0x0]
  00184646: add.w r0,r4,#0x30
  0018464a: vst1.32 {d16,d17},[r2]
  0018464e: str r1,[r4,#0x60]
  00184650: vst1.32 {d16,d17},[r0]
  00184654: ldr.w r0,[r9,#0x0]
  00184658: cbz r0,0x00184670
  0018465a: blx 0x00071b84
  0018465e: cbz r0,0x00184670
  00184660: ldr.w r0,[r9,#0x0]
  00184664: blx 0x00071b84
  00184668: mov.w r1,#0xffffffff
  0018466c: blx 0x00072f58
  00184670: ldr r0,[r4,#0x14]
  00184672: vmov.f32 s20,0x3f800000
  00184676: str r0,[r4,#0x8]
  00184678: movs r5,#0x0
  0018467a: ldr r0,[0x001849a0]
  0018467c: mov.w r10,#0x1
  00184680: vldr.32 s18,[pc,#0x314]
  00184684: add r0,pc
  00184686: vldr.32 s16,[pc,#0x308]
  0018468a: vldr.32 s22,[pc,#0x310]
  0018468e: ldr r6,[r0,#0x0]
  00184690: ldr r0,[r4,#0x6c]
  00184692: ldr r1,[r0,#0x0]
  00184694: cmp r5,r1
  00184696: bcs.w 0x001848ac
  0018469a: ldr r0,[r0,#0x4]
  0018469c: ldr.w r0,[r0,r5,lsl #0x2]
  001846a0: cmp r0,#0x0
  001846a2: beq 0x0018473a
  001846a4: blx 0x00071908
  001846a8: cmp r0,#0x1d
  001846aa: bhi 0x00184728
  001846ac: tbh [pc,r0]
  001846ec: ldr r0,[r4,#0x6c]
  001846ee: movs r1,#0x9
  001846f0: ldr r0,[r0,#0x4]
  001846f2: ldr.w r0,[r0,r5,lsl #0x2]
  001846f6: blx 0x000718e4
  001846fa: ldr r1,[r4,#0x6c]
  001846fc: vmov s0,r0
  00184700: ldr r1,[r1,#0x4]
  00184702: ldr.w r0,[r1,r5,lsl #0x2]
  00184706: movs r1,#0xb
  00184708: vcvt.f32.s32 s24,s0
  0018470c: blx 0x000718e4
  00184710: vmov s0,r0
  00184714: vcvt.f32.s32 s0,s0
  00184718: vldr.32 s2,[r4,#0x48]
  0018471c: vdiv.f32 s0,s24,s0
  00184720: vmla.f32 s2,s0,s18
  00184724: vstr.32 s2,[r4,#0x48]
  00184728: ldr r0,[r4,#0x6c]
  0018472a: ldr r0,[r0,#0x4]
  0018472c: ldr.w r0,[r0,r5,lsl #0x2]
  00184730: blx 0x00077e6c
  00184734: ldr r1,[r4,#0x8]
  00184736: add r0,r1
  00184738: str r0,[r4,#0x8]
  0018473a: adds r5,#0x1
  0018473c: b 0x00184690
  0018473e: ldr r0,[r4,#0x6c]
  00184740: movs r1,#0x12
  00184742: ldr r0,[r0,#0x4]
  00184744: ldr.w r0,[r0,r5,lsl #0x2]
  00184748: blx 0x000718e4
  0018474c: ldr r1,[r4,#0x6c]
  0018474e: str r0,[r4,#0x1c]
  00184750: ldr r0,[r1,#0x4]
  00184752: movs r1,#0x13
  00184754: ldr.w r0,[r0,r5,lsl #0x2]
  00184758: blx 0x000718e4
  0018475c: str r0,[r4,#0x24]
  0018475e: b 0x00184728
  00184760: ldr r0,[r4,#0x6c]
  00184762: movs r1,#0x14
  00184764: ldr r0,[r0,#0x4]
  00184766: ldr.w r0,[r0,r5,lsl #0x2]
  0018476a: blx 0x000718e4
  0018476e: str r0,[r4,#0x20]
  00184770: b 0x00184728
  00184772: ldr r0,[r4,#0x6c]
  00184774: movs r1,#0x16
  00184776: ldr r0,[r0,#0x4]
  00184778: ldr.w r0,[r0,r5,lsl #0x2]
  0018477c: blx 0x000718e4
  00184780: ldr r1,[r4,#0x28]
  00184782: add r0,r1
  00184784: str r0,[r4,#0x28]
  00184786: b 0x00184728
  00184788: ldr r0,[r4,#0x6c]
  0018478a: movs r1,#0x19
  0018478c: ldr r0,[r0,#0x4]
  0018478e: ldr.w r0,[r0,r5,lsl #0x2]
  00184792: blx r6
  00184794: ldr r1,[r4,#0x6c]
  00184796: str r0,[r4,#0x34]
  00184798: ldr r0,[r1,#0x4]
  0018479a: movs r1,#0x1b
  0018479c: ldr.w r0,[r0,r5,lsl #0x2]
  001847a0: blx r6
  001847a2: ldr r1,[r4,#0x6c]
  001847a4: str r0,[r4,#0x3c]
  001847a6: ldr r0,[r1,#0x4]
  001847a8: movs r1,#0x1a
  001847aa: ldr.w r0,[r0,r5,lsl #0x2]
  001847ae: blx r6
  001847b0: str r0,[r4,#0x38]
  001847b2: b 0x00184728
  001847b4: ldr r0,[r4,#0x6c]
  001847b6: ldr r0,[r0,#0x4]
  001847b8: ldr.w r0,[r0,r5,lsl #0x2]
  001847bc: blx 0x000718d8
  001847c0: subs r0,#0x4b
  001847c2: it ne
  001847c4: mov.ne r0,#0x1
  001847c6: str r0,[r4,#0x4c]
  001847c8: b 0x00184728
  001847ca: ldr r0,[r4,#0x6c]
  001847cc: movs r1,#0x1c
  001847ce: ldr r0,[r0,#0x4]
  001847d0: ldr.w r0,[r0,r5,lsl #0x2]
  001847d4: blx 0x000718e4
  001847d8: str r0,[r4,#0x40]
  001847da: b 0x00184728
  001847dc: ldr r0,[r4,#0x6c]
  001847de: movs r1,#0x1f
  001847e0: ldr r0,[r0,#0x4]
  001847e2: ldr.w r0,[r0,r5,lsl #0x2]
  001847e6: blx 0x000718e4
  001847ea: str r0,[r4,#0x30]
  001847ec: b 0x00184728
  001847ee: strb.w r10,[r4,#0x50]
  001847f2: b 0x00184728
  001847f4: ldr r0,[r4,#0x6c]
  001847f6: movs r1,#0x22
  001847f8: ldr r0,[r0,#0x4]
  001847fa: ldr.w r0,[r0,r5,lsl #0x2]
  001847fe: blx 0x000718e4
  00184802: ldr r1,[r4,#0x44]
  00184804: add r0,r1
  00184806: str r0,[r4,#0x44]
  00184808: b 0x00184728
  0018480a: strb.w r10,[r4,#0x51]
  0018480e: b 0x00184728
  00184810: strb.w r10,[r4,#0x5c]
  00184814: b 0x00184728
  00184816: ldr r0,[r4,#0x6c]
  00184818: movs r1,#0x27
  0018481a: ldr r0,[r0,#0x4]
  0018481c: ldr.w r0,[r0,r5,lsl #0x2]
  00184820: blx 0x000718e4
  00184824: vmov s0,r0
  00184828: movs r1,#0x28
  0018482a: vcvt.f32.s32 s0,s0
  0018482e: vdiv.f32 s0,s0,s16
  00184832: vsub.f32 s0,s20,s0
  00184836: vcmpe.f32 s0,s22
  0018483a: vmrs apsr,fpscr
  0018483e: it eq
  00184840: vmov.eq.f32 s0,s20
  00184844: ldr r0,[r4,#0x6c]
  00184846: vstr.32 s0,[r4,#0x54]
  0018484a: ldr r0,[r0,#0x4]
  0018484c: ldr.w r0,[r0,r5,lsl #0x2]
  00184850: blx 0x000718e4
  00184854: vmov s0,r0
  00184858: vcvt.f32.s32 s0,s0
  0018485c: vdiv.f32 s0,s0,s16
  00184860: vadd.f32 s0,s0,s20
  00184864: vcmpe.f32 s0,s22
  00184868: vstr.32 s0,[r4,#0x58]
  0018486c: vmrs apsr,fpscr
  00184870: it eq
  00184872: str.w.eq r8,[r4,#0x58]
  00184876: b 0x00184728
  00184878: ldr r0,[r4,#0x6c]
  0018487a: ldr r0,[r0,#0x4]
  0018487c: ldr.w r0,[r0,r5,lsl #0x2]
  00184880: blx 0x000718d8
  00184884: subs r0,#0xbd
  00184886: str r0,[r4,#0x60]
  00184888: ldr.w r0,[r9,#0x0]
  0018488c: cmp r0,#0x0
  0018488e: beq.w 0x00184728
  00184892: blx 0x00071b84
  00184896: cmp r0,#0x0
  00184898: beq.w 0x00184728
  0018489c: ldr.w r0,[r9,#0x0]
  001848a0: blx 0x00071b84
  001848a4: ldr r1,[r4,#0x60]
  001848a6: blx 0x00072f58
  001848aa: b 0x00184728
  001848ac: ldr r6,[0x00184994]
  001848ae: movs r5,#0x0
  001848b0: mov.w r8,#0x1
  001848b4: str r5,[r4,#0x48]
  001848b6: b 0x00184924
  001848b8: ldr r0,[r0,#0x4]
  001848ba: ldr.w r0,[r0,r5,lsl #0x2]
  001848be: cbz r0,0x0018491e
  001848c0: blx 0x00071908
  001848c4: cmp r0,#0x19
  001848c6: bhi 0x0018491e
  001848c8: lsl.w r0,r8,r0
  001848cc: tst r0,r6
  001848ce: mov r1,r6
  001848d0: beq 0x0018491e
  001848d2: ldr r0,[r4,#0x6c]
  001848d4: movs r1,#0x9
  001848d6: ldr r0,[r0,#0x4]
  001848d8: ldr.w r0,[r0,r5,lsl #0x2]
  001848dc: blx 0x000718e4
  001848e0: ldr r1,[r4,#0x6c]
  001848e2: vmov s0,r0
  001848e6: vldr.32 s20,[r4,#0x58]
  001848ea: ldr r1,[r1,#0x4]
  001848ec: ldr.w r0,[r1,r5,lsl #0x2]
  001848f0: movs r1,#0xb
  001848f2: vcvt.f32.s32 s22,s0
  001848f6: blx 0x000718e4
  001848fa: vmov s0,r0
  001848fe: vmul.f32 s6,s20,s22
  00184902: vcvt.f32.s32 s0,s0
  00184906: vldr.32 s4,[r4,#0x54]
  0018490a: vldr.32 s2,[r4,#0x48]
  0018490e: vmul.f32 s0,s4,s0
  00184912: vdiv.f32 s0,s6,s0
  00184916: vmla.f32 s2,s0,s18
  0018491a: vstr.32 s2,[r4,#0x48]
  0018491e: ldr r0,[r4,#0x6c]
  00184920: adds r5,#0x1
  00184922: ldr r1,[r0,#0x0]
  00184924: cmp r5,r1
  00184926: bcc 0x001848b8
  00184928: ldr r1,[r4,#0x78]
  0018492a: cbz r1,0x00184948
  0018492c: ldr r2,[r1,#0x0]
  0018492e: movs r0,#0x0
  00184930: movs r3,#0x0
  00184932: b 0x00184942
  00184934: ldr r6,[r1,#0x4]
  00184936: ldr.w r6,[r6,r3,lsl #0x2]
  0018493a: adds r3,#0x1
  0018493c: cmp r6,#0x1
  0018493e: it eq
  00184940: add.eq r0,#0x1e
  00184942: cmp r2,r3
  00184944: bne 0x00184934
  00184946: b 0x0018494a
  00184948: movs r0,#0x0
  0018494a: ldr r1,[r4,#0xc]
  0018494c: ldr r2,[r4,#0x28]
  0018494e: add r1,r0
  00184950: vmov s0,r2
  00184954: vmov s2,r1
  00184958: vcvt.f32.s32 s0,s0
  0018495c: vcvt.f32.s32 s2,s2
  00184960: vmul.f32 s0,s0,s2
  00184964: vdiv.f32 s0,s0,s16
  00184968: vcvt.s32.f32 s0,s0
  0018496c: vmov r1,s0
  00184970: add r0,r1
  00184972: strd r0,r2,[r4,#0x28]
  00184976: mov r0,r4
  00184978: blx 0x00077e78
  0018497c: ldr r1,[r4,#0x8]
  0018497e: add r0,r1
  00184980: str r0,[r4,#0x8]
  00184982: vpop {d8,d9,d10,d11,d12}
  00184986: pop.w {r8,r9,r10}
  0018498a: pop {r4,r5,r6,r7,pc}
```
