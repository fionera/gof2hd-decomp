# HangarWindow::autoEquipSecondaryWeapons
elf 0x14bc50 body 504
Sig: undefined __stdcall autoEquipSecondaryWeapons(int param_1)

## decompile
```c

/* HangarWindow::autoEquipSecondaryWeapons(int) */

void HangarWindow::autoEquipSecondaryWeapons(int param_1)

{
  ListItem *this;
  int iVar1;
  undefined4 *puVar2;
  Ship *pSVar3;
  uint *puVar4;
  int iVar5;
  Item *pIVar6;
  String *pSVar7;
  undefined4 uVar8;
  int in_r1;
  code *pcVar9;
  Item *pIVar10;
  HangarList *this_00;
  int *piVar11;
  uint uVar12;
  undefined4 uVar13;
  int *piVar14;
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  undefined1 auStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar14 = *(int **)(DAT_0015be80 + 0x15bc62);
  local_28 = *piVar14;
  this = (ListItem *)HangarList::getCurrentItemAt(*(HangarList **)(param_1 + 0x14),in_r1);
  if ((((this != (ListItem *)0x0) && (*(Item **)(this + 0x10) != (Item *)0x0)) &&
      (iVar1 = Item::getType(*(Item **)(this + 0x10)), iVar1 == 1)) &&
     (((iVar1 = ::ListItem::isItem(this), iVar1 != 0 &&
       (iVar1 = Item::getType(*(Item **)(this + 0x10)), iVar1 == 1)) &&
      (iVar1 = Item::getAmount(), 0 < iVar1)))) {
    puVar2 = *(undefined4 **)(DAT_0015be84 + 0x15bcb6);
    pSVar3 = (Ship *)Status::getShip();
    puVar4 = (uint *)Ship::getEquipment(pSVar3,1);
    if (puVar4 != (uint *)0x0) {
      for (uVar12 = 0; uVar12 < *puVar4; uVar12 = uVar12 + 1) {
        pIVar10 = *(Item **)(puVar4[1] + uVar12 * 4);
        if (pIVar10 != (Item *)0x0) {
          iVar1 = Item::getIndex(pIVar10);
          iVar5 = Item::getIndex(*(Item **)(this + 0x10));
          if (iVar1 == iVar5) {
            iVar1 = *(int *)(this + 0x10);
            Item::getAmount();
            Item::getAmount();
            pIVar10 = (Item *)Item::makeItem(iVar1);
            puVar4 = *(uint **)(param_1 + 0x10);
            if (puVar4 != (uint *)0x0) {
              for (uVar12 = 0; uVar12 < *puVar4; uVar12 = uVar12 + 1) {
                iVar1 = Item::getIndex(*(Item **)(puVar4[1] + uVar12 * 4));
                iVar5 = Item::getIndex(pIVar10);
                if (iVar1 == iVar5) {
                  Item::setAmount(*(Item **)(*(int *)(*(int *)(param_1 + 0x10) + 4) + uVar12 * 4),0)
                  ;
                }
                puVar4 = *(uint **)(param_1 + 0x10);
              }
            }
            pcVar9 = *(code **)(DAT_0015be88 + 0x15bd5c);
            pIVar6 = (Item *)(*pcVar9)(*puVar2);
            Ship::setEquipment(pIVar6,(int)pIVar10);
            iVar1 = (*pcVar9)(*puVar2);
            iVar5 = Item::getIndex(pIVar10);
            Item::getAmount();
            Ship::removeCargo(iVar1,iVar5);
            this_00 = *(HangarList **)(param_1 + 0x14);
            pSVar3 = (Ship *)(*pcVar9)(*puVar2);
            HangarList::initShipTab(this_00,pSVar3);
            piVar11 = *(int **)(DAT_0015be8c + 0x15bda4);
            pSVar7 = (String *)GameText::getText(*piVar11);
            AbyssEngine::String::String(aSStack_34,pSVar7,false);
            uVar13 = *puVar2;
            AbyssEngine::String::String(aSStack_4c,aSStack_34,false);
            iVar1 = *piVar11;
            Item::getIndex(pIVar10);
            pSVar7 = (String *)GameText::getText(iVar1);
            AbyssEngine::String::String(aSStack_58,pSVar7,false);
            uVar8 = AbyssEngine::String::String(aSStack_64,(char *)(DAT_0015be90 + 0x15bde6),false);
            Status::replaceHash(auStack_40,uVar13,aSStack_4c,aSStack_58,uVar8);
            AbyssEngine::String::operator=(aSStack_34,auStack_40);
            pcVar9 = *(code **)(DAT_0015be94 + 0x15be08);
            (*pcVar9)(auStack_40);
            (*pcVar9)(aSStack_64);
            (*pcVar9)(aSStack_58);
            (*pcVar9)(aSStack_4c);
            ChoiceWindow::set(*(String **)(param_1 + 0x20));
            *(undefined1 *)(param_1 + 0xad) = 1;
            *(undefined1 *)(param_1 + 0x3c) = 1;
            AbyssEngine::String::~String(aSStack_34);
            break;
          }
        }
      }
    }
  }
  if (*piVar14 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0015bc50: push {r4,r5,r6,r7,lr}
  0015bc52: add r7,sp,#0xc
  0015bc54: push {r8,r9,r10,r11}
  0015bc58: sub sp,#0x4c
  0015bc5a: mov r8,r0
  0015bc5c: ldr r0,[0x0015be80]
  0015bc5e: add r0,pc
  0015bc60: ldr.w r11,[r0,#0x0]
  0015bc64: ldr.w r0,[r11,#0x0]
  0015bc68: str r0,[sp,#0x48]
  0015bc6a: ldr.w r0,[r8,#0x14]
  0015bc6e: blx 0x0007624c
  0015bc72: mov r5,r0
  0015bc74: cmp r0,#0x0
  0015bc76: beq.w 0x0015be32
  0015bc7a: ldr r0,[r5,#0x10]
  0015bc7c: cmp r0,#0x0
  0015bc7e: beq.w 0x0015be32
  0015bc82: blx 0x000718fc
  0015bc86: cmp r0,#0x1
  0015bc88: bne.w 0x0015be32
  0015bc8c: mov r0,r5
  0015bc8e: blx 0x000768e8
  0015bc92: cmp r0,#0x0
  0015bc94: beq.w 0x0015be32
  0015bc98: ldr r0,[r5,#0x10]
  0015bc9a: blx 0x000718fc
  0015bc9e: cmp r0,#0x1
  0015bca0: bne.w 0x0015be32
  0015bca4: ldr r0,[r5,#0x10]
  0015bca6: blx 0x0007183c
  0015bcaa: cmp r0,#0x1
  0015bcac: blt.w 0x0015be32
  0015bcb0: ldr r0,[0x0015be84]
  0015bcb2: add r0,pc
  0015bcb4: ldr r0,[r0,#0x0]
  0015bcb6: str r0,[sp,#0x8]
  0015bcb8: ldr r0,[r0,#0x0]
  0015bcba: blx 0x00071a58
  0015bcbe: movs r1,#0x1
  0015bcc0: blx 0x00072118
  0015bcc4: mov r10,r0
  0015bcc6: cmp r0,#0x0
  0015bcc8: beq.w 0x0015be32
  0015bccc: mov.w r9,#0x0
  0015bcd0: b 0x0015bcf2
  0015bcd2: ldr.w r0,[r10,#0x4]
  0015bcd6: ldr.w r6,[r0,r9,lsl #0x2]
  0015bcda: cbz r6,0x0015bcee
  0015bcdc: mov r0,r6
  0015bcde: blx 0x000718d8
  0015bce2: mov r4,r0
  0015bce4: ldr r0,[r5,#0x10]
  0015bce6: blx 0x000718d8
  0015bcea: cmp r4,r0
  0015bcec: beq 0x0015bcfc
  0015bcee: add.w r9,r9,#0x1
  0015bcf2: ldr.w r0,[r10,#0x0]
  0015bcf6: cmp r9,r0
  0015bcf8: bcc 0x0015bcd2
  0015bcfa: b 0x0015be32
  0015bcfc: ldr r4,[r5,#0x10]
  0015bcfe: mov r0,r4
  0015bd00: blx 0x0007183c
  0015bd04: mov r5,r0
  0015bd06: mov r0,r6
  0015bd08: blx 0x0007183c
  0015bd0c: adds r1,r0,r5
  0015bd0e: mov r0,r4
  0015bd10: blx 0x000718c0
  0015bd14: mov r5,r0
  0015bd16: ldr.w r0,[r8,#0x10]
  0015bd1a: cbz r0,0x0015bd52
  0015bd1c: movs r6,#0x0
  0015bd1e: b 0x0015bd4c
  0015bd20: ldr r0,[r0,#0x4]
  0015bd22: ldr.w r0,[r0,r6,lsl #0x2]
  0015bd26: blx 0x000718d8
  0015bd2a: mov r4,r0
  0015bd2c: mov r0,r5
  0015bd2e: blx 0x000718d8
  0015bd32: cmp r4,r0
  0015bd34: bne 0x0015bd46
  0015bd36: ldr.w r0,[r8,#0x10]
  0015bd3a: movs r1,#0x0
  0015bd3c: ldr r0,[r0,#0x4]
  0015bd3e: ldr.w r0,[r0,r6,lsl #0x2]
  0015bd42: blx 0x00076fc0
  0015bd46: ldr.w r0,[r8,#0x10]
  0015bd4a: adds r6,#0x1
  0015bd4c: ldr r1,[r0,#0x0]
  0015bd4e: cmp r6,r1
  0015bd50: bcc 0x0015bd20
  0015bd52: ldr r1,[0x0015be88]
  0015bd54: ldr.w r10,[sp,#0x8]
  0015bd58: add r1,pc
  0015bd5a: ldr.w r0,[r10,#0x0]
  0015bd5e: ldr r4,[r1,#0x0]
  0015bd60: blx r4
  0015bd62: mov r1,r5
  0015bd64: mov r2,r9
  0015bd66: blx 0x0007387c
  0015bd6a: ldr.w r0,[r10,#0x0]
  0015bd6e: blx r4
  0015bd70: mov r9,r0
  0015bd72: mov r0,r5
  0015bd74: blx 0x000718d8
  0015bd78: mov r6,r0
  0015bd7a: mov r0,r5
  0015bd7c: blx 0x0007183c
  0015bd80: mov r2,r0
  0015bd82: mov r0,r9
  0015bd84: mov r1,r6
  0015bd86: blx 0x00072454
  0015bd8a: ldr.w r0,[r10,#0x0]
  0015bd8e: ldr.w r6,[r8,#0x14]
  0015bd92: blx r4
  0015bd94: mov r1,r0
  0015bd96: mov r0,r6
  0015bd98: blx 0x000761e0
  0015bd9c: ldr r0,[0x0015be8c]
  0015bd9e: movs r1,#0xd0
  0015bda0: add r0,pc
  0015bda2: ldr r6,[r0,#0x0]
  0015bda4: ldr r0,[r6,#0x0]
  0015bda6: blx 0x00072f70
  0015bdaa: add r4,sp,#0x3c
  0015bdac: mov r1,r0
  0015bdae: movs r2,#0x0
  0015bdb0: mov r0,r4
  0015bdb2: blx 0x0006f028
  0015bdb6: ldr.w r9,[r10,#0x0]
  0015bdba: add r0,sp,#0x24
  0015bdbc: mov r1,r4
  0015bdbe: movs r2,#0x0
  0015bdc0: blx 0x0006f028
  0015bdc4: ldr r4,[r6,#0x0]
  0015bdc6: mov r0,r5
  0015bdc8: blx 0x000718d8
  0015bdcc: addw r1,r0,#0x4fa
  0015bdd0: mov r0,r4
  0015bdd2: blx 0x00072f70
  0015bdd6: mov r1,r0
  0015bdd8: add r0,sp,#0x18
  0015bdda: movs r2,#0x0
  0015bddc: blx 0x0006f028
  0015bde0: ldr r1,[0x0015be90]
  0015bde2: add r1,pc
  0015bde4: add r0,sp,#0xc
  0015bde6: movs r2,#0x0
  0015bde8: blx 0x0006ee18
  0015bdec: str r0,[sp,#0x0]
  0015bdee: add r0,sp,#0x30
  0015bdf0: add r2,sp,#0x24
  0015bdf2: add r3,sp,#0x18
  0015bdf4: mov r1,r9
  0015bdf6: blx 0x000739b4
  0015bdfa: add r0,sp,#0x3c
  0015bdfc: add r1,sp,#0x30
  0015bdfe: blx 0x0006f2b0
  0015be02: ldr r0,[0x0015be94]
  0015be04: add r0,pc
  0015be06: ldr r4,[r0,#0x0]
  0015be08: add r0,sp,#0x30
  0015be0a: blx r4
  0015be0c: add r0,sp,#0xc
  0015be0e: blx r4
  0015be10: add r0,sp,#0x18
  0015be12: blx r4
  0015be14: add r0,sp,#0x24
  0015be16: blx r4
  0015be18: ldr.w r0,[r8,#0x20]
  0015be1c: add r1,sp,#0x3c
  0015be1e: blx 0x00074794
  0015be22: movs r0,#0x1
  0015be24: strb.w r0,[r8,#0xad]
  0015be28: strb.w r0,[r8,#0x3c]
  0015be2c: add r0,sp,#0x3c
  0015be2e: blx 0x0006ee30
  0015be32: ldr r0,[sp,#0x48]
  0015be34: ldr.w r1,[r11,#0x0]
  0015be38: subs r0,r1,r0
  0015be3a: ittt eq
  0015be3c: add.eq sp,#0x4c
  0015be3e: pop.eq.w {r8,r9,r10,r11}
  0015be42: pop.eq {r4,r5,r6,r7,pc}
  0015be44: blx 0x0006e824
```
