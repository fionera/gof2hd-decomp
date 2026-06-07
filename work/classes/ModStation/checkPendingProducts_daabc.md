# ModStation::checkPendingProducts
elf 0xdaabc body 534
Sig: undefined __stdcall checkPendingProducts(void)

## decompile
```c

/* ModStation::checkPendingProducts() */

void ModStation::checkPendingProducts(void)

{
  int in_r0;
  int iVar1;
  int *piVar2;
  String *pSVar3;
  uint *puVar4;
  int *piVar5;
  char *pcVar6;
  Station *this;
  int iVar7;
  Item *this_00;
  Item *pIVar8;
  PendingProduct *this_01;
  void *pvVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  code *pcVar13;
  int iVar14;
  int *piVar15;
  undefined4 *puVar16;
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar15 = *(int **)(DAT_000ead34 + 0xeaad0);
  puVar16 = *(undefined4 **)(DAT_000ead38 + 0xeaad2);
  local_28 = *piVar15;
  iVar1 = Status::getCurrentCampaignMission();
  piVar2 = *(int **)(DAT_000ead3c + 0xeaaf0);
  pSVar3 = (String *)GameText::getText(*piVar2);
  AbyssEngine::String::String(aSStack_40,(char *)(DAT_000ead40 + 0xeab04),false);
  AbyssEngine::operator+(aSStack_34,pSVar3);
  AbyssEngine::String::~String(aSStack_40);
  puVar4 = (uint *)Status::getPendingProducts((Status *)*puVar16);
  if (puVar4 != (uint *)0x0) {
    iVar11 = 0;
    piVar5 = *(int **)(DAT_000ead44 + 0xeab42);
    pcVar6 = (char *)(DAT_000ead48 + 0xeab4a);
    for (uVar10 = 0; uVar10 < *puVar4; uVar10 = uVar10 + 1) {
      iVar14 = *(int *)(puVar4[1] + iVar11);
      if (iVar14 != 0) {
        iVar12 = *(int *)(iVar14 + 0xc);
        this = (Station *)Status::getStation();
        iVar7 = Station::getIndex(this);
        if (iVar12 == iVar7) {
          if (iVar1 == 0x92 && *(int *)(iVar14 + 0x14) == 0xd2) {
            uVar10 = puVar4[1];
            if (*(PendingProduct **)(uVar10 + iVar11) != (PendingProduct *)0x0) {
              pvVar9 = (void *)PendingProduct::~PendingProduct
                                         (*(PendingProduct **)(uVar10 + iVar11));
              operator_delete(pvVar9);
              uVar10 = puVar4[1];
            }
            *(undefined4 *)(uVar10 + iVar11) = 0;
            goto LAB_000eac94;
          }
          this_00 = (Item *)Item::makeItem(*(int *)(*(int *)(*piVar5 + 4) +
                                                   *(int *)(iVar14 + 0x14) * 4));
          pIVar8 = (Item *)Status::getShip();
          Ship::addCargo(pIVar8);
          AbyssEngine::String::String(aSStack_70,pcVar6,false);
          AbyssEngine::operator+(aSStack_64,aSStack_34);
          iVar14 = Item::getAmount();
          AbyssEngine::String::String(aSStack_7c,iVar14);
          AbyssEngine::operator+(aSStack_58,aSStack_64);
          AbyssEngine::String::String(aSStack_88,(char *)(DAT_000ead4c + 0xeabde),false);
          AbyssEngine::operator+(aSStack_4c,aSStack_58);
          iVar14 = *piVar2;
          Item::getIndex(this_00);
          GameText::getText(iVar14);
          AbyssEngine::operator+(aSStack_40,aSStack_4c);
          AbyssEngine::String::operator=(aSStack_34,aSStack_40);
          pcVar13 = *(code **)(DAT_000ead50 + 0xeac20);
          (*pcVar13)(aSStack_40);
          (*pcVar13)(aSStack_4c);
          (*pcVar13)(aSStack_88);
          (*pcVar13)(aSStack_58);
          (*pcVar13)(aSStack_7c);
          (*pcVar13)(aSStack_64);
          (*pcVar13)(aSStack_70);
          this_01 = *(PendingProduct **)(puVar4[1] + iVar11);
          if (this_01 == (PendingProduct *)0x0) {
            puVar16 = (undefined4 *)(puVar4[1] + uVar10 * 4);
          }
          else {
            pvVar9 = (void *)PendingProduct::~PendingProduct(this_01);
            operator_delete(pvVar9);
            puVar16 = (undefined4 *)(puVar4[1] + iVar11);
          }
          *puVar16 = 0;
          *(undefined1 *)(in_r0 + 99) = 1;
        }
      }
      iVar11 = iVar11 + 4;
    }
  }
  if (*(char *)(in_r0 + 99) != '\0') {
    ChoiceWindow::set(*(String **)(in_r0 + 0x70));
  }
LAB_000eac94:
  AbyssEngine::String::~String(aSStack_34);
  if (*piVar15 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000eaabc: push {r4,r5,r6,r7,lr}
  000eaabe: add r7,sp,#0xc
  000eaac0: push {r8,r9,r10,r11}
  000eaac4: sub sp,#0x8c
  000eaac6: mov r4,r0
  000eaac8: ldr r0,[0x000ead34]
  000eaaca: ldr r1,[0x000ead38]
  000eaacc: add r0,pc
  000eaace: add r1,pc
  000eaad0: ldr.w r9,[r0,#0x0]
  000eaad4: ldr.w r11,[r1,#0x0]
  000eaad8: ldr.w r1,[r9,#0x0]
  000eaadc: ldr.w r0,[r11,#0x0]
  000eaae0: str r1,[sp,#0x88]
  000eaae2: blx 0x00071770
  000eaae6: mov r10,r0
  000eaae8: ldr r0,[0x000ead3c]
  000eaaea: movs r1,#0xd5
  000eaaec: add r0,pc
  000eaaee: ldr r0,[r0,#0x0]
  000eaaf0: str r0,[sp,#0x1c]
  000eaaf2: ldr r0,[r0,#0x0]
  000eaaf4: blx 0x00072f70
  000eaaf8: ldr r1,[0x000ead40]
  000eaafa: add r5,sp,#0x70
  000eaafc: mov r6,r0
  000eaafe: movs r2,#0x0
  000eab00: add r1,pc
  000eab02: mov r0,r5
  000eab04: blx 0x0006ee18
  000eab08: add r0,sp,#0x7c
  000eab0a: mov r1,r6
  000eab0c: mov r2,r5
  000eab0e: blx 0x0006ef98
  000eab12: add r0,sp,#0x70
  000eab14: blx 0x0006ee30
  000eab18: ldr.w r0,[r11,#0x0]
  000eab1c: blx 0x00074668
  000eab20: mov r2,r0
  000eab22: cmp r0,#0x0
  000eab24: beq.w 0x000eac84
  000eab28: strd r4,r9,[sp,#0x0]
  000eab2c: add.w r0,r4,#0x63
  000eab30: str r0,[sp,#0x10]
  000eab32: add r1,sp,#0x28
  000eab34: ldr r0,[0x000ead44]
  000eab36: add.w r9,sp,#0x70
  000eab3a: movs r4,#0x0
  000eab3c: movs r3,#0x0
  000eab3e: add r0,pc
  000eab40: ldr r0,[r0,#0x0]
  000eab42: str r0,[sp,#0x18]
  000eab44: ldr r0,[0x000ead48]
  000eab46: add r0,pc
  000eab48: str r0,[sp,#0x14]
  000eab4a: strd r11,r10,[sp,#0x8]
  000eab4e: str r2,[sp,#0x20]
  000eab50: b 0x000eac74
  000eab52: ldr r0,[r2,#0x4]
  000eab54: ldr r6,[r0,r4]
  000eab56: cmp r6,#0x0
  000eab58: beq.w 0x000eac70
  000eab5c: ldr r5,[r6,#0xc]
  000eab5e: mov r8,r1
  000eab60: ldr.w r0,[r11,#0x0]
  000eab64: str r3,[sp,#0x24]
  000eab66: blx 0x00071c14
  000eab6a: blx 0x00071824
  000eab6e: ldr r2,[sp,#0x20]
  000eab70: cmp r5,r0
  000eab72: ldr r3,[sp,#0x24]
  000eab74: add r5,sp,#0x40
  000eab76: mov r1,r8
  000eab78: bne 0x000eac70
  000eab7a: ldr r0,[r6,#0x14]
  000eab7c: cmp.w r10,#0x92
  000eab80: it eq
  000eab82: cmp.eq r0,#0xd2
  000eab84: beq.w 0x000eacb0
  000eab88: ldr r1,[sp,#0x18]
  000eab8a: ldr r1,[r1,#0x0]
  000eab8c: ldr r2,[r1,#0x4]
  000eab8e: ldr r1,[r6,#0x10]
  000eab90: ldr.w r0,[r2,r0,lsl #0x2]
  000eab94: blx 0x000718c0
  000eab98: mov r10,r0
  000eab9a: ldr.w r0,[r11,#0x0]
  000eab9e: blx 0x00071a58
  000eaba2: mov r1,r10
  000eaba4: blx 0x00072cdc
  000eaba8: ldr r1,[sp,#0x14]
  000eabaa: mov r0,r5
  000eabac: movs r2,#0x0
  000eabae: blx 0x0006ee18
  000eabb2: add r0,sp,#0x4c
  000eabb4: add r1,sp,#0x7c
  000eabb6: mov r2,r5
  000eabb8: blx 0x0006ef98
  000eabbc: mov r0,r10
  000eabbe: blx 0x0007183c
  000eabc2: mov r1,r0
  000eabc4: add r6,sp,#0x34
  000eabc6: mov r0,r6
  000eabc8: blx 0x0006f658
  000eabcc: add r0,sp,#0x58
  000eabce: add r1,sp,#0x4c
  000eabd0: mov r2,r6
  000eabd2: blx 0x0006ef98
  000eabd6: ldr r1,[0x000ead4c]
  000eabd8: mov r5,r9
  000eabda: add r1,pc
  000eabdc: mov r0,r8
  000eabde: movs r2,#0x0
  000eabe0: mov r9,r8
  000eabe2: blx 0x0006ee18
  000eabe6: add r0,sp,#0x64
  000eabe8: add r1,sp,#0x58
  000eabea: mov r2,r9
  000eabec: blx 0x0006ef98
  000eabf0: ldr r0,[sp,#0x1c]
  000eabf2: ldr.w r11,[r0,#0x0]
  000eabf6: mov r0,r10
  000eabf8: blx 0x000718d8
  000eabfc: mov r9,r5
  000eabfe: addw r1,r0,#0x4fa
  000eac02: mov r0,r11
  000eac04: blx 0x00072f70
  000eac08: mov r2,r0
  000eac0a: add r1,sp,#0x64
  000eac0c: mov r0,r9
  000eac0e: blx 0x0006ef98
  000eac12: add r0,sp,#0x7c
  000eac14: mov r1,r9
  000eac16: blx 0x0006f2b0
  000eac1a: ldr r0,[0x000ead50]
  000eac1c: add r0,pc
  000eac1e: ldr r5,[r0,#0x0]
  000eac20: mov r0,r9
  000eac22: blx r5
  000eac24: add r0,sp,#0x64
  000eac26: blx r5
  000eac28: mov r0,r8
  000eac2a: blx r5
  000eac2c: add r0,sp,#0x58
  000eac2e: blx r5
  000eac30: mov r0,r6
  000eac32: blx r5
  000eac34: add r0,sp,#0x4c
  000eac36: blx r5
  000eac38: add r0,sp,#0x40
  000eac3a: blx r5
  000eac3c: ldr r2,[sp,#0x20]
  000eac3e: ldr r1,[r2,#0x4]
  000eac40: ldr r0,[r1,r4]
  000eac42: cbz r0,0x000eac5a
  000eac44: blx 0x00073b28
  000eac48: blx 0x0006eb48
  000eac4c: ldr r2,[sp,#0x20]
  000eac4e: ldr r0,[r2,#0x4]
  000eac50: ldrd r11,r10,[sp,#0x8]
  000eac54: ldr r3,[sp,#0x24]
  000eac56: add r0,r4
  000eac58: b 0x000eac64
  000eac5a: ldr r3,[sp,#0x24]
  000eac5c: ldrd r11,r10,[sp,#0x8]
  000eac60: add.w r0,r1,r3, lsl #0x2
  000eac64: movs r1,#0x0
  000eac66: str r1,[r0,#0x0]
  000eac68: movs r1,#0x1
  000eac6a: ldr r0,[sp,#0x10]
  000eac6c: strb r1,[r0,#0x0]
  000eac6e: mov r1,r8
  000eac70: adds r4,#0x4
  000eac72: adds r3,#0x1
  000eac74: ldr r0,[r2,#0x0]
  000eac76: cmp r3,r0
  000eac78: bcc.w 0x000eab52
  000eac7c: ldrd r4,r9,[sp,#0x0]
  000eac80: ldr r2,[sp,#0x10]
  000eac82: b 0x000eac88
  000eac84: add.w r2,r4,#0x63
  000eac88: ldrb r0,[r2,#0x0]
  000eac8a: cbz r0,0x000eac94
  000eac8c: ldr r0,[r4,#0x70]
  000eac8e: add r1,sp,#0x7c
  000eac90: blx 0x00074794
  000eac94: add r0,sp,#0x7c
  000eac96: blx 0x0006ee30
  000eac9a: ldr r0,[sp,#0x88]
  000eac9c: ldr.w r1,[r9,#0x0]
  000eaca0: subs r0,r1,r0
  000eaca2: ittt eq
  000eaca4: add.eq sp,#0x8c
  000eaca6: pop.eq.w {r8,r9,r10,r11}
  000eacaa: pop.eq {r4,r5,r6,r7,pc}
  000eacac: blx 0x0006e824
  000eacb0: ldr r1,[r2,#0x4]
  000eacb2: ldr r0,[r1,r4]
  000eacb4: cbz r0,0x000eacc6
  000eacb6: mov r5,r2
  000eacb8: blx 0x00073b28
  000eacbc: blx 0x0006eb48
  000eacc0: ldr r0,[r5,#0x4]
  000eacc2: add r0,r4
  000eacc4: b 0x000eacc8
  000eacc6: adds r0,r1,r4
  000eacc8: ldr.w r9,[sp,#0x4]
  000eaccc: movs r1,#0x0
  000eacce: str r1,[r0,#0x0]
  000eacd0: b 0x000eac94
```
