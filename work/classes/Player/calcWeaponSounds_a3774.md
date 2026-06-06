# Player::calcWeaponSounds
elf 0xa3774 body 394
Sig: undefined __thiscall calcWeaponSounds(Player * this, int param_1)

## decompile
```c

/* Player::calcWeaponSounds(int) */

void __thiscall Player::calcWeaponSounds(Player *this,int param_1)

{
  bool bVar1;
  bool bVar2;
  uint uVar3;
  void *pvVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  uint *puVar10;
  uint uVar11;
  undefined4 *puVar12;
  int iVar13;
  int *piVar14;
  
  puVar10 = *(uint **)this;
  if (puVar10 != (uint *)0x0) {
    if (*(uint **)puVar10[1] != (uint *)0x0) {
      uVar11 = **(uint **)puVar10[1];
      uVar3 = (uint)((ulonglong)uVar11 * 4);
      if ((int)((ulonglong)uVar11 * 4 >> 0x20) != 0) {
        uVar3 = 0xffffffff;
      }
      pvVar4 = operator_new__(uVar3);
      for (iVar5 = 0; iVar5 < (int)uVar11; iVar5 = iVar5 + 1) {
        *(undefined4 *)((int)pvVar4 + iVar5 * 4) =
             *(undefined4 *)(*(int *)(*(int *)(*(int *)puVar10[1] + 4) + iVar5 * 4) + 0x58);
      }
      bVar1 = true;
      iVar5 = 1;
      piVar14 = *(int **)(DAT_000b38fc + 0xb37da);
      do {
        for (; iVar5 < (int)uVar11; iVar5 = iVar5 + 1) {
          iVar13 = iVar5 + -1;
          iVar6 = Item::getSinglePrice
                            (*(Item **)(*(int *)(*piVar14 + 4) +
                                       *(int *)((int)pvVar4 + iVar13 * 4) * 4));
          iVar7 = Item::getSinglePrice
                            (*(Item **)(*(int *)(*piVar14 + 4) +
                                       *(int *)((int)pvVar4 + iVar5 * 4) * 4));
          if (iVar6 < iVar7) {
            bVar1 = false;
            uVar8 = *(undefined4 *)((int)pvVar4 + iVar13 * 4);
            *(undefined4 *)((int)pvVar4 + iVar13 * 4) = *(undefined4 *)((int)pvVar4 + iVar5 * 4);
            *(undefined4 *)((int)pvVar4 + iVar5 * 4) = uVar8;
          }
        }
        bVar2 = !bVar1;
        bVar1 = true;
        iVar5 = 1;
      } while (bVar2);
      for (uVar3 = 0; (int)uVar3 < (int)uVar11; uVar3 = uVar3 + 1) {
        for (uVar9 = 0; uVar11 != uVar9; uVar9 = uVar9 + 1) {
          if ((uVar3 != uVar9) &&
             (*(int *)((int)pvVar4 + uVar3 * 4) == *(int *)((int)pvVar4 + uVar9 * 4))) {
            *(undefined4 *)((int)pvVar4 + uVar9 * 4) = 0xffffffff;
          }
        }
      }
      iVar5 = 0;
      piVar14 = *(int **)(DAT_000b3900 + 0xb3870);
      do {
        if ((int)uVar11 <= iVar5) break;
        if (-1 < *(int *)((int)pvVar4 + iVar5 * 4)) {
          *(undefined1 *)(*(int *)(*(int *)(**(int **)(*(int *)this + 4) + 4) + iVar5 * 4) + 0x89) =
               1;
          Globals::addSoundResourceToList(*piVar14);
          param_1 = param_1 + -1;
        }
        iVar5 = iVar5 + 1;
      } while (param_1 != 0);
      operator_delete__(pvVar4);
      puVar10 = *(uint **)this;
    }
    if ((((2 < *puVar10) && (piVar14 = *(int **)(puVar10[1] + 8), piVar14 != (int *)0x0)) &&
        (*piVar14 != 0)) && (iVar5 = *(int *)piVar14[1], iVar5 != 0)) {
      puVar12 = *(undefined4 **)(DAT_000b3908 + 0xb38d8);
      uVar8 = *(undefined4 *)(DAT_000b390c + 0xb38dc + *(int *)(iVar5 + 0x58) * 4);
      *(undefined1 *)(iVar5 + 0x89) = 1;
      (*(code *)(DAT_001abd14 + 0x1abd18))(*puVar12,uVar8);
      return;
    }
  }
  return;
}

```
## target disasm
```
  000b3774: push {r4,r5,r6,r7,lr}
  000b3776: add r7,sp,#0xc
  000b3778: push {r5,r6,r7,r8,r9,r10,r11}
  000b377c: ldr r4,[r0,#0x0]
  000b377e: mov r9,r1
  000b3780: mov r8,r0
  000b3782: cmp r4,#0x0
  000b3784: beq.w 0x000b38f6
  000b3788: ldr r0,[r4,#0x4]
  000b378a: ldr r0,[r0,#0x0]
  000b378c: cmp r0,#0x0
  000b378e: beq.w 0x000b38ba
  000b3792: ldr r5,[r0,#0x0]
  000b3794: movs r0,#0x4
  000b3796: umull r0,r1,r5,r0
  000b379a: cmp r1,#0x0
  000b379c: it ne
  000b379e: mov.ne r1,#0x1
  000b37a0: cmp r1,#0x0
  000b37a2: it ne
  000b37a4: mov.ne.w r0,#0xffffffff
  000b37a8: blx 0x0006ec08
  000b37ac: mov r6,r0
  000b37ae: movs r0,#0x0
  000b37b0: b 0x000b37c4
  000b37b2: ldr r1,[r4,#0x4]
  000b37b4: ldr r1,[r1,#0x0]
  000b37b6: ldr r1,[r1,#0x4]
  000b37b8: ldr.w r1,[r1,r0,lsl #0x2]
  000b37bc: ldr r1,[r1,#0x58]
  000b37be: str.w r1,[r6,r0,lsl #0x2]
  000b37c2: adds r0,#0x1
  000b37c4: cmp r0,r5
  000b37c6: blt 0x000b37b2
  000b37c8: strd r9,r8,[sp,#0x4]
  000b37cc: mov.w r11,#0x1
  000b37d0: ldr r0,[0x000b38fc]
  000b37d2: mov.w r8,#0x1
  000b37d6: add r0,pc
  000b37d8: ldr.w r10,[r0,#0x0]
  000b37dc: cmp r8,r5
  000b37de: bge 0x000b3828
  000b37e0: ldr.w r0,[r10,#0x0]
  000b37e4: sub.w r9,r8,#0x1
  000b37e8: ldr.w r1,[r6,r9,lsl #0x2]
  000b37ec: ldr r0,[r0,#0x4]
  000b37ee: ldr.w r0,[r0,r1,lsl #0x2]
  000b37f2: blx 0x00071944
  000b37f6: mov r4,r0
  000b37f8: ldr.w r0,[r10,#0x0]
  000b37fc: ldr.w r1,[r6,r8,lsl #0x2]
  000b3800: ldr r0,[r0,#0x4]
  000b3802: ldr.w r0,[r0,r1,lsl #0x2]
  000b3806: blx 0x00071944
  000b380a: cmp r4,r0
  000b380c: bge 0x000b3822
  000b380e: ldr.w r0,[r6,r8,lsl #0x2]
  000b3812: mov.w r11,#0x0
  000b3816: ldr.w r1,[r6,r9,lsl #0x2]
  000b381a: str.w r0,[r6,r9,lsl #0x2]
  000b381e: str.w r1,[r6,r8,lsl #0x2]
  000b3822: add.w r8,r8,#0x1
  000b3826: b 0x000b37dc
  000b3828: lsls.w r0,r11,#0x1f
  000b382c: mov.w r11,#0x1
  000b3830: mov.w r8,#0x1
  000b3834: beq 0x000b37dc
  000b3836: ldrd r11,r9,[sp,#0x4]
  000b383a: movs r0,#0x0
  000b383c: mov.w r1,#0xffffffff
  000b3840: b 0x000b3862
  000b3842: movs r2,#0x0
  000b3844: b 0x000b385c
  000b3846: cmp r0,r2
  000b3848: beq 0x000b385a
  000b384a: ldr.w r3,[r6,r2,lsl #0x2]
  000b384e: ldr.w r4,[r6,r0,lsl #0x2]
  000b3852: cmp r4,r3
  000b3854: it eq
  000b3856: str.eq.w r1,[r6,r2,lsl #0x2]
  000b385a: adds r2,#0x1
  000b385c: cmp r5,r2
  000b385e: bne 0x000b3846
  000b3860: adds r0,#0x1
  000b3862: cmp r0,r5
  000b3864: blt 0x000b3842
  000b3866: ldr r0,[0x000b3900]
  000b3868: movs r4,#0x0
  000b386a: ldr r1,[0x000b3904]
  000b386c: add r0,pc
  000b386e: add r1,pc
  000b3870: ldr.w r8,[r0,#0x0]
  000b3874: mov r10,r1
  000b3876: cmp r4,r5
  000b3878: bge 0x000b38b0
  000b387a: ldr.w r0,[r6,r4,lsl #0x2]
  000b387e: cmp r0,#0x0
  000b3880: blt 0x000b38a8
  000b3882: ldr.w r0,[r9,#0x0]
  000b3886: movs r2,#0x1
  000b3888: ldr r0,[r0,#0x4]
  000b388a: ldr r0,[r0,#0x0]
  000b388c: ldr r0,[r0,#0x4]
  000b388e: ldr.w r0,[r0,r4,lsl #0x2]
  000b3892: ldr r1,[r0,#0x58]
  000b3894: strb.w r2,[r0,#0x89]
  000b3898: ldr.w r0,[r8,#0x0]
  000b389c: ldr.w r1,[r10,r1,lsl #0x2]
  000b38a0: blx 0x00072298
  000b38a4: sub.w r11,r11,#0x1
  000b38a8: adds r4,#0x1
  000b38aa: cmp.w r11,#0x0
  000b38ae: bne 0x000b3876
  000b38b0: mov r0,r6
  000b38b2: blx 0x0006ebfc
  000b38b6: ldr.w r4,[r9,#0x0]
  000b38ba: ldr r0,[r4,#0x0]
  000b38bc: cmp r0,#0x3
  000b38be: bcc 0x000b38f6
  000b38c0: ldr r0,[r4,#0x4]
  000b38c2: ldr r0,[r0,#0x8]
  000b38c4: cbz r0,0x000b38f6
  000b38c6: ldr r1,[r0,#0x0]
  000b38c8: cbz r1,0x000b38f6
  000b38ca: ldr r0,[r0,#0x4]
  000b38cc: ldr r0,[r0,#0x0]
  000b38ce: cbz r0,0x000b38f6
  000b38d0: ldr r1,[0x000b3908]
  000b38d2: ldr r2,[0x000b390c]
  000b38d4: add r1,pc
  000b38d6: ldr r3,[r0,#0x58]
  000b38d8: add r2,pc
  000b38da: ldr r6,[r1,#0x0]
  000b38dc: ldr.w r1,[r2,r3,lsl #0x2]
  000b38e0: movs r2,#0x1
  000b38e2: strb.w r2,[r0,#0x89]
  000b38e6: ldr r0,[r6,#0x0]
  000b38e8: add sp,#0xc
  000b38ea: pop.w {r8,r9,r10,r11}
  000b38ee: pop.w {r4,r5,r6,r7,lr}
  000b38f2: b.w 0x001abd08
  000b38f6: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000b38fa: pop {r4,r5,r6,r7,pc}
  001abd08: bx pc
```
