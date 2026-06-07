# KIPlayer::captureCrate
elf 0xa5e60 body 670
Sig: undefined __thiscall captureCrate(KIPlayer * this, Hud * param_1)

## decompile
```c

/* KIPlayer::captureCrate(Hud*) */

void __thiscall KIPlayer::captureCrate(KIPlayer *this,Hud *param_1)

{
  bool bVar1;
  undefined1 uVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  Item *this_00;
  Standing *this_01;
  Ship *pSVar6;
  Item *pIVar7;
  bool bVar8;
  int iVar9;
  Status *this_02;
  uint uVar10;
  int *piVar11;
  
  if ((*(int *)(this + 0x88) - 3U < 2) && (this[0x4c] = (KIPlayer)0x0, this[0x101] != (KIPlayer)0x0)
     ) {
    setActive(SUB41(this,0));
  }
  puVar3 = *(uint **)(this + 0x50);
  *(undefined4 *)(this + 0x78) = 0;
  if (puVar3 != (uint *)0x0) {
    for (uVar10 = 0; uVar10 < *puVar3; uVar10 = uVar10 + 2) {
      iVar9 = *(int *)(puVar3[1] + uVar10 * 4 + 4);
      if (0 < iVar9) {
        if (1 < *(int *)(this + 0x88) - 3U) {
          iVar9 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000b6100 + 0xb5ec2));
        }
        piVar11 = *(int **)(DAT_000b6104 + 0xb5ed0);
        Status::getShip();
        iVar4 = Ship::getFreeSpace();
        iVar5 = iVar9;
        if (iVar4 <= iVar9) {
          Status::getShip();
          iVar5 = Ship::getFreeSpace();
        }
        if (iVar5 < 1) {
          iVar9 = 1;
        }
        else {
          Status::getShip();
          iVar5 = Ship::getFreeSpace();
          if (iVar5 <= iVar9) {
            Status::getShip();
            iVar9 = Ship::getFreeSpace();
          }
        }
        this_00 = (Item *)Item::makeItem(*(int *)(*(int *)(**(int **)(DAT_000b6108 + 0xb5f1e) + 4) +
                                                 *(int *)(*(int *)(*(int *)(this + 0x50) + 4) +
                                                         uVar10 * 4) * 4));
        iVar5 = *(int *)(*(int *)(this + 0x50) + 4) + uVar10 * 4;
        *(int *)(iVar5 + 4) = *(int *)(iVar5 + 4) - iVar9;
        if (this_00 == (Item *)0x0) {
          return;
        }
        if (*(char *)(*(int *)(this + 4) + 0x5d) != '\0') {
          Level::stealFriendCargo(*(Level **)(this + 0x54));
        }
        if (this[0x3c] == (KIPlayer)0x0) {
          this_01 = (Standing *)Status::getStanding();
          Standing::applyStealCargo(this_01,*(int *)(this + 0x28));
        }
        if (this[0xd0] == (KIPlayer)0x0) {
          bVar1 = false;
        }
        else {
          iVar9 = Item::getIndex(this_00);
          if (iVar9 == 0x74) {
            bVar1 = true;
          }
          else {
            iVar9 = Item::getIndex(this_00);
            bVar1 = iVar9 == 0x75;
          }
        }
        pSVar6 = (Ship *)Status::getShip();
        iVar9 = Item::getAmount();
        iVar9 = Ship::spaceAvailable(pSVar6,iVar9);
        if (iVar9 == 0) {
          if (bVar1) {
            this[0x68] = (KIPlayer)0x1;
          }
          iVar9 = Item::getIndex(this_00);
          uVar2 = Item::getAmount();
          bVar1 = false;
          bVar8 = true;
          goto LAB_000b60f2;
        }
        this_02 = (Status *)*piVar11;
        iVar9 = Item::getAmount();
        Status::crateCaptured(this_02,iVar9);
        if (bVar1) {
          Item::setUnsaleable(SUB41(this_00,0));
        }
        iVar9 = Item::getType(this_00);
        if (iVar9 != 1) goto LAB_000b6038;
        pSVar6 = (Ship *)Status::getShip();
        puVar3 = (uint *)Ship::getEquipment(pSVar6);
        if (puVar3 == (uint *)0x0) goto LAB_000b6038;
        bVar8 = false;
        for (uVar10 = 0; uVar10 < *puVar3; uVar10 = uVar10 + 1) {
          pIVar7 = *(Item **)(puVar3[1] + uVar10 * 4);
          if (pIVar7 != (Item *)0x0) {
            iVar9 = Item::getIndex(pIVar7);
            iVar5 = Item::getIndex(this_00);
            if (iVar9 == iVar5) {
              pIVar7 = *(Item **)(puVar3[1] + uVar10 * 4);
              iVar9 = Item::getAmount();
              Item::changeAmount(pIVar7,iVar9);
              bVar8 = true;
            }
          }
        }
        if (!bVar8) {
LAB_000b6038:
          pIVar7 = (Item *)Status::getShip();
          Ship::addCargo(pIVar7);
        }
        iVar9 = Item::getAmount();
        *(int *)(*(int *)(this + 0x54) + 0x1c) = iVar9 + *(int *)(*(int *)(this + 0x54) + 0x1c);
        if (bVar1) {
          this[0x69] = (KIPlayer)0x1;
        }
        else if (*(int *)(this + 0x28) == 9) {
          iVar9 = Item::getAmount();
          *(int *)(*piVar11 + 0xcc) = iVar9 + *(int *)(*piVar11 + 0xcc);
        }
        else {
          iVar9 = Item::getIndex(this_00);
          if ((0x83 < iVar9) && (iVar9 = Item::getIndex(this_00), iVar9 < 0x9a)) {
            iVar5 = *(int *)(*piVar11 + 0xac);
            iVar9 = Item::getIndex(this_00);
            *(undefined1 *)(iVar9 + *(int *)(iVar5 + 4) + -0x84) = 1;
          }
        }
        iVar9 = Item::getIndex(this_00);
        uVar2 = Item::getAmount();
        bVar8 = false;
LAB_000b60f2:
        Hud::catchCargo((int)param_1,iVar9,(bool)uVar2,bVar8,bVar1,false,false);
        return;
      }
    }
  }
  return;
}

```

## target disasm
```
  000b5e60: push {r4,r5,r6,r7,lr}
  000b5e62: add r7,sp,#0xc
  000b5e64: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000b5e68: mov r5,r0
  000b5e6a: ldr.w r0,[r0,#0x88]
  000b5e6e: mov r8,r1
  000b5e70: subs r0,#0x3
  000b5e72: cmp r0,#0x1
  000b5e74: bhi 0x000b5e8a
  000b5e76: ldrb.w r0,[r5,#0x101]
  000b5e7a: movs r1,#0x0
  000b5e7c: strb.w r1,[r5,#0x4c]
  000b5e80: cbz r0,0x000b5e8a
  000b5e82: mov r0,r5
  000b5e84: movs r1,#0x0
  000b5e86: blx 0x000732f4
  000b5e8a: ldr r0,[r5,#0x50]
  000b5e8c: movs r1,#0x0
  000b5e8e: str r1,[r5,#0x78]
  000b5e90: cmp r0,#0x0
  000b5e92: beq.w 0x000b60f6
  000b5e96: ldr r1,[r0,#0x0]
  000b5e98: movs r6,#0x0
  000b5e9a: b 0x000b5eaa
  000b5e9c: ldr r2,[r0,#0x4]
  000b5e9e: add.w r2,r2,r6, lsl #0x2
  000b5ea2: ldr r4,[r2,#0x4]
  000b5ea4: cmp r4,#0x1
  000b5ea6: bge 0x000b5eb0
  000b5ea8: adds r6,#0x2
  000b5eaa: cmp r6,r1
  000b5eac: bcc 0x000b5e9c
  000b5eae: b 0x000b60f6
  000b5eb0: ldr.w r0,[r5,#0x88]
  000b5eb4: subs r0,#0x3
  000b5eb6: cmp r0,#0x2
  000b5eb8: bcc 0x000b5eca
  000b5eba: ldr r0,[0x000b6100]
  000b5ebc: mov r1,r4
  000b5ebe: add r0,pc
  000b5ec0: ldr r0,[r0,#0x0]
  000b5ec2: ldr r0,[r0,#0x0]
  000b5ec4: blx 0x00071848
  000b5ec8: mov r4,r0
  000b5eca: ldr r0,[0x000b6104]
  000b5ecc: add r0,pc
  000b5ece: ldr.w r10,[r0,#0x0]
  000b5ed2: ldr.w r0,[r10,#0x0]
  000b5ed6: blx 0x00071a58
  000b5eda: blx 0x000722ec
  000b5ede: cmp r0,r4
  000b5ee0: mov r0,r4
  000b5ee2: bgt 0x000b5ef0
  000b5ee4: ldr.w r0,[r10,#0x0]
  000b5ee8: blx 0x00071a58
  000b5eec: blx 0x000722ec
  000b5ef0: cmp r0,#0x1
  000b5ef2: blt 0x000b5f14
  000b5ef4: ldr.w r0,[r10,#0x0]
  000b5ef8: blx 0x00071a58
  000b5efc: blx 0x000722ec
  000b5f00: cmp r0,r4
  000b5f02: bgt 0x000b5f16
  000b5f04: ldr.w r0,[r10,#0x0]
  000b5f08: blx 0x00071a58
  000b5f0c: blx 0x000722ec
  000b5f10: mov r4,r0
  000b5f12: b 0x000b5f16
  000b5f14: movs r4,#0x1
  000b5f16: ldr r0,[0x000b6108]
  000b5f18: ldr r1,[r5,#0x50]
  000b5f1a: add r0,pc
  000b5f1c: ldr r0,[r0,#0x0]
  000b5f1e: ldr r1,[r1,#0x4]
  000b5f20: ldr r0,[r0,#0x0]
  000b5f22: ldr.w r1,[r1,r6,lsl #0x2]
  000b5f26: ldr r0,[r0,#0x4]
  000b5f28: ldr.w r0,[r0,r1,lsl #0x2]
  000b5f2c: mov r1,r4
  000b5f2e: blx 0x000718c0
  000b5f32: mov r9,r0
  000b5f34: ldr r0,[r5,#0x50]
  000b5f36: cmp.w r9,#0x0
  000b5f3a: ldr r0,[r0,#0x4]
  000b5f3c: add.w r0,r0,r6, lsl #0x2
  000b5f40: ldr r1,[r0,#0x4]
  000b5f42: sub.w r1,r1,r4
  000b5f46: str r1,[r0,#0x4]
  000b5f48: beq.w 0x000b60f6
  000b5f4c: ldr r0,[r5,#0x4]
  000b5f4e: ldrb.w r0,[r0,#0x5d]
  000b5f52: cbz r0,0x000b5f5a
  000b5f54: ldr r0,[r5,#0x54]
  000b5f56: blx 0x00073300
  000b5f5a: ldrb.w r0,[r5,#0x3c]
  000b5f5e: cbnz r0,0x000b5f6e
  000b5f60: ldr.w r0,[r10,#0x0]
  000b5f64: blx 0x00071b84
  000b5f68: ldr r1,[r5,#0x28]
  000b5f6a: blx 0x0007330c
  000b5f6e: ldrb.w r0,[r5,#0xd0]
  000b5f72: cbz r0,0x000b5f82
  000b5f74: mov r0,r9
  000b5f76: blx 0x000718d8
  000b5f7a: cmp r0,#0x74
  000b5f7c: bne 0x000b5f86
  000b5f7e: movs r6,#0x1
  000b5f80: b 0x000b5f94
  000b5f82: movs r6,#0x0
  000b5f84: b 0x000b5f94
  000b5f86: mov r0,r9
  000b5f88: blx 0x000718d8
  000b5f8c: subs r0,#0x75
  000b5f8e: clz r0,r0
  000b5f92: lsrs r6,r0,#0x5
  000b5f94: ldr.w r0,[r10,#0x0]
  000b5f98: blx 0x00071a58
  000b5f9c: mov r4,r0
  000b5f9e: mov r0,r9
  000b5fa0: blx 0x0007183c
  000b5fa4: mov r1,r0
  000b5fa6: mov r0,r4
  000b5fa8: blx 0x00073318
  000b5fac: cmp r0,#0x0
  000b5fae: beq 0x000b6060
  000b5fb0: mov r0,r9
  000b5fb2: ldr.w r4,[r10,#0x0]
  000b5fb6: blx 0x0007183c
  000b5fba: mov r1,r0
  000b5fbc: mov r0,r4
  000b5fbe: blx 0x00073324
  000b5fc2: cbz r6,0x000b5fcc
  000b5fc4: mov r0,r9
  000b5fc6: movs r1,#0x1
  000b5fc8: blx 0x00073330
  000b5fcc: mov r0,r9
  000b5fce: blx 0x000718fc
  000b5fd2: cmp r0,#0x1
  000b5fd4: bne 0x000b6038
  000b5fd6: ldr.w r0,[r10,#0x0]
  000b5fda: blx 0x00071a58
  000b5fde: blx 0x0007333c
  000b5fe2: mov r4,r0
  000b5fe4: cbz r0,0x000b6038
  000b5fe6: strd r6,r8,[sp,#0x14]
  000b5fea: mov.w r8,#0x0
  000b5fee: mov.w r11,#0x0
  000b5ff2: b 0x000b6028
  000b5ff4: ldr r0,[r4,#0x4]
  000b5ff6: ldr.w r0,[r0,r11,lsl #0x2]
  000b5ffa: cbz r0,0x000b6024
  000b5ffc: blx 0x000718d8
  000b6000: mov r6,r0
  000b6002: mov r0,r9
  000b6004: blx 0x000718d8
  000b6008: cmp r6,r0
  000b600a: bne 0x000b6024
  000b600c: ldr r0,[r4,#0x4]
  000b600e: ldr.w r6,[r0,r11,lsl #0x2]
  000b6012: mov r0,r9
  000b6014: blx 0x0007183c
  000b6018: mov r1,r0
  000b601a: mov r0,r6
  000b601c: blx 0x00071854
  000b6020: mov.w r8,#0x1
  000b6024: add.w r11,r11,#0x1
  000b6028: ldr r0,[r4,#0x0]
  000b602a: cmp r11,r0
  000b602c: bcc 0x000b5ff4
  000b602e: lsls.w r0,r8,#0x1f
  000b6032: ldrd r6,r8,[sp,#0x14]
  000b6036: bne 0x000b6046
  000b6038: ldr.w r0,[r10,#0x0]
  000b603c: blx 0x00071a58
  000b6040: mov r1,r9
  000b6042: blx 0x00072cdc
  000b6046: mov r0,r9
  000b6048: blx 0x0007183c
  000b604c: ldr r1,[r5,#0x54]
  000b604e: cmp r6,#0x0
  000b6050: ldr r2,[r1,#0x1c]
  000b6052: add r0,r2
  000b6054: str r0,[r1,#0x1c]
  000b6056: beq 0x000b608a
  000b6058: movs r0,#0x1
  000b605a: strb.w r0,[r5,#0x69]
  000b605e: b 0x000b60d2
  000b6060: cbz r6,0x000b6068
  000b6062: movs r0,#0x1
  000b6064: strb.w r0,[r5,#0x68]
  000b6068: mov r0,r9
  000b606a: blx 0x000718d8
  000b606e: mov r4,r0
  000b6070: mov r0,r9
  000b6072: blx 0x0007183c
  000b6076: mov r2,r0
  000b6078: movs r0,#0x0
  000b607a: strd r0,r0,[sp,#0x0]
  000b607e: mov r1,r4
  000b6080: strd r0,r0,[sp,#0x8]
  000b6084: mov r0,r8
  000b6086: movs r3,#0x1
  000b6088: b 0x000b60f2
  000b608a: ldr r0,[r5,#0x28]
  000b608c: cmp r0,#0x9
  000b608e: bne 0x000b60a6
  000b6090: mov r0,r9
  000b6092: blx 0x0007183c
  000b6096: ldr.w r1,[r10,#0x0]
  000b609a: ldr.w r2,[r1,#0xcc]
  000b609e: add r0,r2
  000b60a0: str.w r0,[r1,#0xcc]
  000b60a4: b 0x000b60d2
  000b60a6: mov r0,r9
  000b60a8: blx 0x000718d8
  000b60ac: cmp r0,#0x84
  000b60ae: blt 0x000b60d2
  000b60b0: mov r0,r9
  000b60b2: blx 0x000718d8
  000b60b6: cmp r0,#0x99
  000b60b8: bgt 0x000b60d2
  000b60ba: ldr.w r0,[r10,#0x0]
  000b60be: ldr.w r4,[r0,#0xac]
  000b60c2: mov r0,r9
  000b60c4: blx 0x000718d8
  000b60c8: ldr r1,[r4,#0x4]
  000b60ca: add r0,r1
  000b60cc: movs r1,#0x1
  000b60ce: strb.w r1,[r0,#-0x84]
  000b60d2: mov r0,r9
  000b60d4: blx 0x000718d8
  000b60d8: mov r4,r0
  000b60da: mov r0,r9
  000b60dc: blx 0x0007183c
  000b60e0: mov r2,r0
  000b60e2: movs r0,#0x0
  000b60e4: strd r6,r0,[sp,#0x0]
  000b60e8: mov r1,r4
  000b60ea: strd r0,r0,[sp,#0x8]
  000b60ee: mov r0,r8
  000b60f0: movs r3,#0x0
  000b60f2: blx 0x00072ce8
  000b60f6: add sp,#0x1c
  000b60f8: pop.w {r8,r9,r10,r11}
  000b60fc: pop {r4,r5,r6,r7,pc}
```
