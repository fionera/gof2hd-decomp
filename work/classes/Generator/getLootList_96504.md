# Generator::getLootList
elf 0x96504 body 466
Sig: undefined __stdcall getLootList(int param_1, int param_2)

## decompile
```c

/* Generator::getLootList(int, int) */

undefined8 Generator::getLootList(int param_1,int param_2)

{
  bool bVar1;
  Array *pAVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int in_r2;
  undefined4 unaff_r5;
  int *piVar8;
  int iVar9;
  uint uVar10;
  int *piVar11;
  int iVar12;
  
  if (param_2 < 0) {
    piVar8 = *(int **)(DAT_000a66e8 + 0xa6540);
    iVar12 = *piVar8;
    piVar11 = *(int **)(DAT_000a66ec + 0xa654a);
    iVar3 = AbyssEngine::AERandom::nextInt(*piVar11);
    iVar4 = AbyssEngine::AERandom::nextInt(*piVar11);
    if (iVar4 == 0) {
      pAVar2 = (Array *)0x0;
    }
    else {
      pAVar2 = operator_new(0xc);
      Array<int>::Array();
      uVar5 = iVar3 << 1;
      if (iVar3 == 0) {
        uVar5 = 2;
      }
      ArraySetLength<int>(uVar5,pAVar2);
      iVar4 = *piVar8;
      iVar3 = 0;
      for (uVar5 = 0; uVar5 < *(uint *)pAVar2; uVar5 = uVar5 + 2) {
        bVar1 = false;
        iVar9 = 0;
        for (uVar10 = 0; (!bVar1 && (uVar10 < 100)); uVar10 = uVar10 + 1) {
          iVar9 = AbyssEngine::AERandom::nextInt(*piVar11);
          iVar3 = Item::getType(*(Item **)(*(int *)(iVar4 + 4) + iVar9 * 4));
          iVar6 = Item::getIngredients();
          if ((iVar6 == 0) &&
             (iVar6 = AbyssEngine::AERandom::nextInt(*piVar11),
             iVar6 < *(int *)(DAT_000a66f0 + 0xa65d6 + iVar3 * 4))) {
            iVar6 = AbyssEngine::AERandom::nextInt(*piVar11);
            iVar7 = Item::getOccurence(*(Item **)(*(int *)(iVar12 + 4) + iVar9 * 4));
            if ((iVar7 <= iVar6) ||
               (iVar6 = Item::getSinglePrice(*(Item **)(*(int *)(iVar4 + 4) + iVar9 * 4)), iVar6 < 1
               )) goto LAB_000a662c;
            bVar1 = false;
            if ((1 < iVar9 - 0xd9U) && (iVar9 != 0xa4 && iVar9 != 0xaf)) {
              if ((iVar3 != 4) &&
                 (iVar6 = Item::getTecLevel(*(Item **)(*(int *)(iVar4 + 4) + iVar9 * 4)), 7 < iVar6)
                 ) goto LAB_000a662c;
              bVar1 = true;
            }
          }
          else {
LAB_000a662c:
            bVar1 = false;
          }
        }
        if (bVar1) {
          *(int *)(*(int *)(pAVar2 + 4) + uVar5 * 4) = iVar9;
          iVar9 = *piVar11;
          if (iVar3 == 4) goto LAB_000a6672;
          iVar9 = AbyssEngine::AERandom::nextInt(iVar9);
        }
        else {
          iVar3 = AbyssEngine::AERandom::nextInt(*piVar11);
          *(int *)(*(int *)(pAVar2 + 4) + uVar5 * 4) = iVar3 + 0x9a;
          iVar9 = *piVar11;
LAB_000a6672:
          iVar9 = AbyssEngine::AERandom::nextInt(iVar9);
          iVar3 = 4;
        }
        *(int *)(*(int *)(pAVar2 + 4) + (uVar5 | 1) * 4) = iVar9 + 1;
      }
      Status::getShip();
      iVar3 = Ship::hasJumpDrive();
      if (iVar3 != 0) {
        iVar3 = Status::getShip();
        iVar3 = Ship::hasCargo(iVar3,0x7a);
        if ((iVar3 == 0) && (iVar3 = AbyssEngine::AERandom::nextInt(*piVar11), iVar3 < 10)) {
          **(undefined4 **)(pAVar2 + 4) = 0x7a;
        }
      }
    }
  }
  else {
    pAVar2 = operator_new(0xc);
    Array<int>::Array();
    ArrayAdd<int>(param_2,pAVar2);
    ArrayAdd<int>(in_r2,pAVar2);
  }
  return CONCAT44(unaff_r5,pAVar2);
}

```

## target disasm
```
  000a6504: push {r4,r5,r6,r7,lr}
  000a6506: add r7,sp,#0xc
  000a6508: push {r5,r6,r7,r8,r9,r10,r11}
  000a650c: mov r5,r2
  000a650e: mov r6,r1
  000a6510: cmp r1,#0x0
  000a6512: blt 0x000a6538
  000a6514: movs r0,#0xc
  000a6516: blx 0x0006eb24
  000a651a: mov r4,r0
  000a651c: blx 0x000701f8
  000a6520: mov r0,r6
  000a6522: mov r1,r4
  000a6524: blx 0x0007021c
  000a6528: mov r0,r5
  000a652a: mov r1,r4
  000a652c: blx 0x0007021c
  000a6530: mov r0,r4
  000a6532: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000a6536: pop {r4,r5,r6,r7,pc}
  000a6538: ldr r0,[0x000a66e8]
  000a653a: movs r1,#0x3
  000a653c: add r0,pc
  000a653e: ldr r6,[r0,#0x0]
  000a6540: ldr.w r10,[r6,#0x0]
  000a6544: ldr r0,[0x000a66ec]
  000a6546: add r0,pc
  000a6548: ldr.w r9,[r0,#0x0]
  000a654c: ldr.w r0,[r9,#0x0]
  000a6550: blx 0x00071848
  000a6554: mov r5,r0
  000a6556: ldr.w r0,[r9,#0x0]
  000a655a: movs r1,#0x3
  000a655c: blx 0x00071848
  000a6560: cmp r0,#0x0
  000a6562: beq.w 0x000a66d2
  000a6566: movs r0,#0xc
  000a6568: blx 0x0006eb24
  000a656c: mov r4,r0
  000a656e: blx 0x000701f8
  000a6572: lsls r0,r5,#0x1
  000a6574: cmp r5,#0x0
  000a6576: it eq
  000a6578: mov.eq r0,#0x2
  000a657a: mov r1,r4
  000a657c: blx 0x00071a4c
  000a6580: ldr r5,[r6,#0x0]
  000a6582: mov.w r11,#0x0
  000a6586: movs r6,#0x0
  000a6588: str r4,[sp,#0x4]
  000a658a: b 0x000a668a
  000a658c: str r6,[sp,#0x8]
  000a658e: movs r0,#0x0
  000a6590: movs r6,#0x0
  000a6592: mov.w r8,#0x0
  000a6596: lsls r0,r0,#0x1f
  000a6598: bne 0x000a6638
  000a659a: cmp.w r8,#0x64
  000a659e: bcs 0x000a6638
  000a65a0: ldr.w r1,[r10,#0x0]
  000a65a4: ldr.w r0,[r9,#0x0]
  000a65a8: blx 0x00071848
  000a65ac: mov r6,r0
  000a65ae: ldr r0,[r5,#0x4]
  000a65b0: ldr.w r0,[r0,r6,lsl #0x2]
  000a65b4: blx 0x000718fc
  000a65b8: mov r11,r0
  000a65ba: ldr r0,[r5,#0x4]
  000a65bc: ldr.w r0,[r0,r6,lsl #0x2]
  000a65c0: blx 0x00071914
  000a65c4: cbnz r0,0x000a662c
  000a65c6: ldr.w r0,[r9,#0x0]
  000a65ca: movs r1,#0x64
  000a65cc: blx 0x00071848
  000a65d0: ldr r1,[0x000a66f0]
  000a65d2: add r1,pc
  000a65d4: ldr.w r1,[r1,r11,lsl #0x2]
  000a65d8: cmp r0,r1
  000a65da: bge 0x000a662c
  000a65dc: ldr.w r0,[r9,#0x0]
  000a65e0: movs r1,#0x64
  000a65e2: blx 0x00071848
  000a65e6: mov r4,r0
  000a65e8: ldr.w r0,[r10,#0x4]
  000a65ec: ldr.w r0,[r0,r6,lsl #0x2]
  000a65f0: blx 0x000718f0
  000a65f4: cmp r4,r0
  000a65f6: bge 0x000a662c
  000a65f8: ldr r0,[r5,#0x4]
  000a65fa: ldr.w r0,[r0,r6,lsl #0x2]
  000a65fe: blx 0x00071944
  000a6602: cmp r0,#0x1
  000a6604: blt 0x000a662c
  000a6606: sub.w r1,r6,#0xd9
  000a660a: movs r0,#0x0
  000a660c: cmp r1,#0x2
  000a660e: bcc 0x000a662e
  000a6610: cmp r6,#0xa4
  000a6612: it ne
  000a6614: cmp.ne r6,#0xaf
  000a6616: beq 0x000a662e
  000a6618: cmp.w r11,#0x4
  000a661c: beq 0x000a6634
  000a661e: ldr r0,[r5,#0x4]
  000a6620: ldr.w r0,[r0,r6,lsl #0x2]
  000a6624: blx 0x00071920
  000a6628: cmp r0,#0x7
  000a662a: ble 0x000a6634
  000a662c: movs r0,#0x0
  000a662e: add.w r8,r8,#0x1
  000a6632: b 0x000a6596
  000a6634: movs r0,#0x1
  000a6636: b 0x000a662e
  000a6638: cbz r0,0x000a6658
  000a663a: ldr r4,[sp,#0x4]
  000a663c: cmp.w r11,#0x4
  000a6640: ldr r1,[sp,#0x8]
  000a6642: ldr r0,[r4,#0x4]
  000a6644: str.w r6,[r0,r1,lsl #0x2]
  000a6648: mov r6,r1
  000a664a: ldr.w r0,[r9,#0x0]
  000a664e: beq 0x000a6672
  000a6650: movs r1,#0x3
  000a6652: blx 0x00071848
  000a6656: b 0x000a667c
  000a6658: ldr.w r0,[r9,#0x0]
  000a665c: movs r1,#0xa
  000a665e: blx 0x00071848
  000a6662: ldr r4,[sp,#0x4]
  000a6664: adds r0,#0x9a
  000a6666: ldr r6,[sp,#0x8]
  000a6668: ldr r1,[r4,#0x4]
  000a666a: str.w r0,[r1,r6,lsl #0x2]
  000a666e: ldr.w r0,[r9,#0x0]
  000a6672: movs r1,#0x9
  000a6674: blx 0x00071848
  000a6678: mov.w r11,#0x4
  000a667c: ldr r1,[r4,#0x4]
  000a667e: orr r2,r6,#0x1
  000a6682: adds r0,#0x1
  000a6684: adds r6,#0x2
  000a6686: str.w r0,[r1,r2,lsl #0x2]
  000a668a: ldr r0,[r4,#0x0]
  000a668c: cmp r6,r0
  000a668e: bcc.w 0x000a658c
  000a6692: ldr r0,[0x000a66f4]
  000a6694: add r0,pc
  000a6696: ldr r5,[r0,#0x0]
  000a6698: ldr r0,[r5,#0x0]
  000a669a: blx 0x00071a58
  000a669e: blx 0x00071a64
  000a66a2: cmp r0,#0x0
  000a66a4: beq.w 0x000a6530
  000a66a8: ldr r0,[r5,#0x0]
  000a66aa: blx 0x00071a58
  000a66ae: movs r1,#0x7a
  000a66b0: movs r2,#0x1
  000a66b2: blx 0x00071a70
  000a66b6: cmp r0,#0x0
  000a66b8: bne.w 0x000a6530
  000a66bc: ldr.w r0,[r9,#0x0]
  000a66c0: movs r1,#0x64
  000a66c2: blx 0x00071848
  000a66c6: cmp r0,#0x9
  000a66c8: ittt le
  000a66ca: ldr.le r0,[r4,#0x4]
  000a66cc: mov.le r1,#0x7a
  000a66ce: str.le r1,[r0,#0x0]
  000a66d0: b 0x000a6530
  000a66d2: movs r4,#0x0
  000a66d4: b 0x000a6530
```
