# Status::calcCargoPrices
elf 0xaca10 body 666
Sig: undefined __thiscall calcCargoPrices(Status * this)

## decompile
```c

/* Status::calcCargoPrices() */

void __thiscall Status::calcCargoPrices(Status *this)

{
  bool bVar1;
  float fVar2;
  float fVar3;
  undefined4 *puVar4;
  int iVar5;
  int *piVar6;
  uint *puVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  uint uVar13;
  undefined4 extraout_r1;
  Galaxy *pGVar14;
  int iVar15;
  Item *this_00;
  uint uVar16;
  code *pcVar17;
  code *pcVar18;
  code *pcVar19;
  uint in_fpscr;
  float fVar20;
  float fVar21;
  float fVar22;
  
  puVar4 = *(undefined4 **)(DAT_000bccb8 + 0xbca24);
  iVar5 = Galaxy::getSystems((Galaxy *)*puVar4);
  fVar3 = DAT_000bccb0;
  fVar2 = DAT_000bccac;
  piVar6 = *(int **)(DAT_000bccbc + 0xbca3e);
  for (uVar13 = 0; uVar13 != 3; uVar13 = uVar13 + 1) {
    if ((uVar13 & 0x7fffffff) == 1) {
      puVar7 = (uint *)Ship::getEquipment(*(Ship **)(this + 400));
    }
    else if ((uVar13 & 0x7fffffff) == 0) {
      puVar7 = (uint *)Ship::getCargo();
    }
    else {
      puVar7 = (uint *)Station::getItems();
    }
    if (puVar7 != (uint *)0x0) {
      iVar15 = *piVar6;
      Station::getIndex(*(Station **)(this + 0x19c));
      AbyssEngine::AERandom::setSeed(CONCAT44(extraout_r1,iVar15));
      iVar15 = inAlienOrbit();
      if (iVar15 == 0) {
        iVar15 = SolarSystem::getIndex();
        bVar1 = iVar15 == 0x19;
      }
      else {
        bVar1 = false;
      }
      for (uVar16 = 0; uVar16 < *puVar7; uVar16 = uVar16 + 1) {
        this_00 = *(Item **)(puVar7[1] + uVar16 * 4);
        if (this_00 != (Item *)0x0) {
          pcVar19 = *(code **)(DAT_000bccc4 + 0xbcae0);
          pGVar14 = (Galaxy *)*puVar4;
          iVar15 = (*pcVar19)(this_00);
          pcVar17 = *(code **)(DAT_000bccc8 + 0xbcaf2);
          iVar15 = (*pcVar17)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar15 * 4));
          iVar8 = (*pcVar19)(this_00);
          pcVar18 = *(code **)(DAT_000bcccc + 0xbcb10);
          iVar8 = (*pcVar18)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar8 * 4));
          iVar9 = Item::getMaxPriceSystem(this_00);
          iVar9 = (*pcVar17)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar9 * 4));
          iVar10 = Item::getMaxPriceSystem(this_00);
          iVar10 = (*pcVar18)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar10 * 4));
          uVar11 = Galaxy::distancePercent(pGVar14,iVar15,iVar8,iVar9,iVar10);
          pGVar14 = (Galaxy *)*puVar4;
          iVar15 = (*pcVar19)(*(undefined4 *)(puVar7[1] + uVar16 * 4));
          iVar15 = (*pcVar17)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar15 * 4));
          iVar8 = (*pcVar19)(*(undefined4 *)(puVar7[1] + uVar16 * 4));
          iVar8 = (*pcVar18)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar8 * 4));
          iVar9 = Station::getSystem(*(Station **)(this + 0x19c));
          iVar9 = (*pcVar17)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar9 * 4));
          iVar10 = Station::getSystem(*(Station **)(this + 0x19c));
          iVar10 = (*pcVar18)(*(undefined4 *)(*(int *)(iVar5 + 4) + iVar10 * 4));
          uVar12 = Galaxy::distancePercent(pGVar14,iVar15,iVar8,iVar9,iVar10);
          fVar20 = (float)VectorSignedToFloat(uVar11,(byte)(in_fpscr >> 0x16) & 3);
          fVar21 = (float)VectorSignedToFloat(uVar12,(byte)(in_fpscr >> 0x16) & 3);
          fVar22 = ((fVar2 / fVar20) * fVar21) / fVar2;
          iVar15 = Item::getMinPrice(this_00);
          iVar8 = Item::getMaxPrice(this_00);
          iVar9 = Item::getMinPrice(this_00);
          in_fpscr = in_fpscr & 0xfffffff;
          fVar21 = (float)VectorSignedToFloat(iVar8 - iVar9,(byte)(in_fpscr >> 0x16) & 3);
          fVar20 = 1.0;
          if (fVar22 < 1.0) {
            fVar20 = fVar22;
          }
          iVar8 = Item::getSinglePrice(this_00);
          if (0 < iVar8) {
            if (bVar1) {
              iVar9 = Item::getMaxPrice(this_00);
            }
            else {
              iVar15 = iVar15 + (int)(fVar20 * fVar21);
              fVar20 = (float)VectorSignedToFloat(iVar15,(byte)(in_fpscr >> 0x16) & 3);
              iVar8 = (int)(fVar20 * fVar3);
              if (iVar8 < 2) {
                iVar8 = 1;
              }
              iVar9 = AbyssEngine::AERandom::nextInt(*piVar6);
              iVar9 = (iVar15 - iVar8) + iVar9;
            }
            if (**(int **)(DAT_000bccd0 + 0xbcc56) != 0) {
              VectorSignedToFloat(**(int **)(DAT_000bccd0 + 0xbcc56),(byte)(in_fpscr >> 0x16) & 3);
              VectorSignedToFloat(iVar9,(byte)(in_fpscr >> 0x16) & 3);
            }
            Item::setPrice((int)this_00);
          }
        }
      }
      AbyssEngine::AERandom::reset();
    }
  }
  return;
}

```
## target disasm
```
  000bca10: push {r4,r5,r6,r7,lr}
  000bca12: add r7,sp,#0xc
  000bca14: push {r7,r8,r9,r10,r11}
  000bca18: vpush {d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12}
  000bca1c: str r0,[sp,#0x18]
  000bca1e: ldr r0,[0x000bccb8]
  000bca20: add r0,pc
  000bca22: ldr r0,[r0,#0x0]
  000bca24: str r0,[sp,#0x28]
  000bca26: ldr r0,[r0,#0x0]
  000bca28: blx 0x00071884
  000bca2c: str r0,[sp,#0x14]
  000bca2e: vmov.f32 s18,0x3f800000
  000bca32: ldr r0,[0x000bccbc]
  000bca34: movs r1,#0x0
  000bca36: vldr.32 s16,[pc,#0x274]
  000bca3a: add r0,pc
  000bca3c: vldr.32 s20,[pc,#0x274]
  000bca40: vldr.32 s22,[pc,#0x26c]
  000bca44: ldr r0,[r0,#0x0]
  000bca46: str r0,[sp,#0xc]
  000bca48: ldr r0,[0x000bccc0]
  000bca4a: add r0,pc
  000bca4c: ldr r0,[r0,#0x0]
  000bca4e: str r0,[sp,#0x4]
  000bca50: b 0x000bcc98
  000bca52: bic r0,r1,#0x80000000
  000bca56: str r1,[sp,#0x8]
  000bca58: cmp r0,#0x1
  000bca5a: beq 0x000bca6a
  000bca5c: cbnz r0,0x000bca76
  000bca5e: ldr r0,[sp,#0x18]
  000bca60: ldr.w r0,[r0,#0x190]
  000bca64: blx 0x00073750
  000bca68: b 0x000bca80
  000bca6a: ldr r0,[sp,#0x18]
  000bca6c: ldr.w r0,[r0,#0x190]
  000bca70: blx 0x0007333c
  000bca74: b 0x000bca80
  000bca76: ldr r0,[sp,#0x18]
  000bca78: ldr.w r0,[r0,#0x19c]
  000bca7c: blx 0x00071830
  000bca80: ldrd r2,r1,[sp,#0x4]
  000bca84: mov r8,r0
  000bca86: cmp r0,#0x0
  000bca88: beq.w 0x000bcc96
  000bca8c: ldr r0,[sp,#0x18]
  000bca8e: mov r5,r2
  000bca90: ldr r1,[sp,#0xc]
  000bca92: ldr.w r0,[r0,#0x19c]
  000bca96: ldr r4,[r1,#0x0]
  000bca98: blx 0x00071824
  000bca9c: mov r2,r0
  000bca9e: asrs r3,r0,#0x1f
  000bcaa0: mov r0,r4
  000bcaa2: blx 0x000739c0
  000bcaa6: ldr r0,[r5,#0x0]
  000bcaa8: blx 0x000723d0
  000bcaac: cbz r0,0x000bcab2
  000bcaae: movs r0,#0x0
  000bcab0: b 0x000bcac4
  000bcab2: ldr r0,[r5,#0x0]
  000bcab4: ldr.w r0,[r0,#0x1a4]
  000bcab8: blx 0x00071a7c
  000bcabc: subs r0,#0x19
  000bcabe: clz r0,r0
  000bcac2: lsrs r0,r0,#0x5
  000bcac4: movs r5,#0x0
  000bcac6: str r0,[sp,#0x10]
  000bcac8: b 0x000bcc82
  000bcaca: ldr.w r0,[r8,#0x4]
  000bcace: ldr.w r4,[r0,r5,lsl #0x2]
  000bcad2: cmp r4,#0x0
  000bcad4: beq.w 0x000bcc80
  000bcad8: ldr r0,[0x000bccc4]
  000bcada: ldr r1,[sp,#0x28]
  000bcadc: add r0,pc
  000bcade: ldr.w r11,[r0,#0x0]
  000bcae2: mov r0,r4
  000bcae4: ldr r1,[r1,#0x0]
  000bcae6: str r1,[sp,#0x24]
  000bcae8: blx r11
  000bcaea: ldr r1,[0x000bccc8]
  000bcaec: ldr r6,[sp,#0x14]
  000bcaee: add r1,pc
  000bcaf0: str r5,[sp,#0x2c]
  000bcaf2: ldr r2,[r6,#0x4]
  000bcaf4: ldr.w r9,[r1,#0x0]
  000bcaf8: ldr.w r0,[r2,r0,lsl #0x2]
  000bcafc: blx r9
  000bcafe: str r0,[sp,#0x20]
  000bcb00: mov r0,r4
  000bcb02: blx r11
  000bcb04: ldr r1,[r6,#0x4]
  000bcb06: ldr.w r0,[r1,r0,lsl #0x2]
  000bcb0a: ldr r1,[0x000bcccc]
  000bcb0c: add r1,pc
  000bcb0e: ldr.w r10,[r1,#0x0]
  000bcb12: blx r10
  000bcb14: str r0,[sp,#0x1c]
  000bcb16: mov r0,r4
  000bcb18: blx 0x000739cc
  000bcb1c: ldr r1,[r6,#0x4]
  000bcb1e: ldr.w r0,[r1,r0,lsl #0x2]
  000bcb22: blx r9
  000bcb24: mov r5,r8
  000bcb26: mov r8,r0
  000bcb28: mov r0,r4
  000bcb2a: blx 0x000739cc
  000bcb2e: ldr r1,[r6,#0x4]
  000bcb30: ldr.w r0,[r1,r0,lsl #0x2]
  000bcb34: blx r10
  000bcb36: str r0,[sp,#0x0]
  000bcb38: mov r3,r8
  000bcb3a: ldr r0,[sp,#0x24]
  000bcb3c: ldrd r2,r1,[sp,#0x1c]
  000bcb40: blx 0x000739d8
  000bcb44: str r0,[sp,#0x24]
  000bcb46: ldr r0,[r5,#0x4]
  000bcb48: ldr r1,[sp,#0x2c]
  000bcb4a: ldr.w r0,[r0,r1,lsl #0x2]
  000bcb4e: ldr r1,[sp,#0x28]
  000bcb50: ldr r1,[r1,#0x0]
  000bcb52: str r1,[sp,#0x20]
  000bcb54: blx r11
  000bcb56: ldr r1,[r6,#0x4]
  000bcb58: ldr.w r0,[r1,r0,lsl #0x2]
  000bcb5c: blx r9
  000bcb5e: str r0,[sp,#0x1c]
  000bcb60: ldr r0,[r5,#0x4]
  000bcb62: ldr r1,[sp,#0x2c]
  000bcb64: ldr.w r0,[r0,r1,lsl #0x2]
  000bcb68: blx r11
  000bcb6a: ldr r1,[r6,#0x4]
  000bcb6c: ldr.w r0,[r1,r0,lsl #0x2]
  000bcb70: blx r10
  000bcb72: ldr.w r8,[sp,#0x18]
  000bcb76: mov r11,r0
  000bcb78: ldr.w r0,[r8,#0x19c]
  000bcb7c: blx 0x0007189c
  000bcb80: ldr r1,[r6,#0x4]
  000bcb82: ldr.w r0,[r1,r0,lsl #0x2]
  000bcb86: blx r9
  000bcb88: mov r9,r0
  000bcb8a: ldr.w r0,[r8,#0x19c]
  000bcb8e: mov r8,r5
  000bcb90: blx 0x0007189c
  000bcb94: ldr r1,[r6,#0x4]
  000bcb96: ldr.w r0,[r1,r0,lsl #0x2]
  000bcb9a: blx r10
  000bcb9c: str r0,[sp,#0x0]
  000bcb9e: mov r2,r11
  000bcba0: ldrd r1,r0,[sp,#0x1c]
  000bcba4: mov r3,r9
  000bcba6: ldr r5,[sp,#0x2c]
  000bcba8: blx 0x000739d8
  000bcbac: ldr r1,[sp,#0x24]
  000bcbae: vmov s2,r0
  000bcbb2: mov r0,r4
  000bcbb4: vmov s0,r1
  000bcbb8: vcvt.f32.s32 s0,s0
  000bcbbc: vcvt.f32.s32 s2,s2
  000bcbc0: vdiv.f32 s0,s16,s0
  000bcbc4: vmul.f32 s0,s0,s2
  000bcbc8: vdiv.f32 s24,s0,s16
  000bcbcc: blx 0x000739e4
  000bcbd0: mov r9,r0
  000bcbd2: mov r0,r4
  000bcbd4: blx 0x00071cbc
  000bcbd8: mov r6,r0
  000bcbda: mov r0,r4
  000bcbdc: blx 0x000739e4
  000bcbe0: vcmpe.f32 s24,s18
  000bcbe4: subs r0,r6,r0
  000bcbe6: vmrs apsr,fpscr
  000bcbea: vmov.f32 s2,s18
  000bcbee: vmov s0,r0
  000bcbf2: mov r0,r4
  000bcbf4: vcvt.f32.s32 s0,s0
  000bcbf8: it mi
  000bcbfa: vmov.mi.f32 s2,s24
  000bcbfe: vmul.f32 s0,s2,s0
  000bcc02: vcvt.s32.f32 s24,s0
  000bcc06: blx 0x00071944
  000bcc0a: cmp r0,#0x1
  000bcc0c: blt 0x000bcc80
  000bcc0e: ldr r0,[sp,#0x10]
  000bcc10: cbz r0,0x000bcc1c
  000bcc12: mov r0,r4
  000bcc14: blx 0x00071cbc
  000bcc18: mov r1,r0
  000bcc1a: b 0x000bcc50
  000bcc1c: vmov r0,s24
  000bcc20: movs r1,#0x1
  000bcc22: add r9,r0
  000bcc24: vmov s0,r9
  000bcc28: vcvt.f32.s32 s0,s0
  000bcc2c: vmul.f32 s0,s0,s22
  000bcc30: vcvt.s32.f32 s0,s0
  000bcc34: ldr r0,[sp,#0xc]
  000bcc36: ldr r0,[r0,#0x0]
  000bcc38: vmov r6,s0
  000bcc3c: cmp r6,#0x1
  000bcc3e: it le
  000bcc40: mov.le r6,r1
  000bcc42: orr.w r1,r1,r6, lsl #0x1
  000bcc46: blx 0x00071848
  000bcc4a: sub.w r1,r9,r6
  000bcc4e: add r1,r0
  000bcc50: ldr r0,[0x000bccd0]
  000bcc52: add r0,pc
  000bcc54: ldr r0,[r0,#0x0]
  000bcc56: ldr r0,[r0,#0x0]
  000bcc58: cbz r0,0x000bcc7a
  000bcc5a: vmov s0,r0
  000bcc5e: vmov s2,r1
  000bcc62: vcvt.f32.s32 s0,s0
  000bcc66: vcvt.f32.s32 s2,s2
  000bcc6a: vmul.f32 s0,s2,s0
  000bcc6e: vmla.f32 s2,s0,s20
  000bcc72: vcvt.s32.f32 s0,s2
  000bcc76: vmov r1,s0
  000bcc7a: mov r0,r4
  000bcc7c: blx 0x00073864
  000bcc80: adds r5,#0x1
  000bcc82: ldr.w r0,[r8,#0x0]
  000bcc86: cmp r5,r0
  000bcc88: bcc.w 0x000bcaca
  000bcc8c: ldr r0,[sp,#0xc]
  000bcc8e: ldr r0,[r0,#0x0]
  000bcc90: blx 0x000718cc
  000bcc94: ldr r1,[sp,#0x8]
  000bcc96: adds r1,#0x1
  000bcc98: cmp r1,#0x3
  000bcc9a: bne.w 0x000bca52
  000bcc9e: vpop {d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12}
  000bcca2: add sp,#0x4
  000bcca4: pop.w {r8,r9,r10,r11}
  000bcca8: pop {r4,r5,r6,r7,pc}
```
