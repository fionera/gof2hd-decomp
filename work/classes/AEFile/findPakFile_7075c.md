# AEFile::findPakFile
elf 0x7075c body 322
Sig: undefined __stdcall findPakFile(String * param_1)

## decompile
```c

/* AEFile::findPakFile(AbyssEngine::String const&) */

void AEFile::findPakFile(String *param_1)

{
  undefined1 *puVar1;
  uint uVar2;
  ushort *puVar3;
  int iVar4;
  int iVar5;
  uint *puVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int *piVar10;
  int *piVar11;
  int *piVar12;
  bool bVar13;
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar12 = *(int **)(DAT_000808b4 + 0x8076e);
  piVar11 = *(int **)(DAT_000808b8 + 0x80770);
  local_28 = *piVar11;
  if (*(String **)*piVar12 != (String *)0x0) {
    uVar2 = crc32_ccitt((AEFile *)param_1,*(String **)*piVar12);
    iVar5 = *(int *)*piVar12;
    iVar4 = 0;
LAB_00080790:
    do {
      iVar7 = iVar5 - iVar4 >> 1;
      iVar9 = iVar4 + iVar7;
      do {
        if (iVar5 <= iVar4) goto LAB_00080886;
        puVar6 = *(uint **)(((int *)*piVar12)[1] + iVar9 * 4);
        uVar8 = *puVar6;
        if (uVar8 == uVar2) {
          puVar3 = AbyssEngine::String::operator_cast_to_unsigned_short_((String *)(puVar6 + 1));
          piVar10 = (int *)**(undefined4 **)(DAT_000808bc + 0x807de);
          if (*(int *)(*(int *)(*(int *)(*piVar12 + 4) + iVar9 * 4) + 0x18) == -1) {
            AbyssEngine::String::String(aSStack_40,puVar3,false);
            iVar5 = *(int *)(*(int *)(*piVar12 + 4) + iVar9 * 4);
            iVar5 = (**(code **)(*piVar10 + 8))
                              (piVar10,aSStack_40,*(undefined4 *)(iVar5 + 0xc),0,0,0,
                               *(undefined4 *)(iVar5 + 0x10));
            puVar1 = &stack0xffffffec;
          }
          else {
            AbyssEngine::String::String(aSStack_34,puVar3,false);
            iVar5 = *(int *)(*(int *)(*piVar12 + 4) + iVar9 * 4);
            iVar5 = (**(code **)(*piVar10 + 8))
                              (piVar10,aSStack_34,*(undefined4 *)(iVar5 + 0xc),1,
                               *(undefined4 *)(iVar5 + 0x14),*(undefined4 *)(iVar5 + 0x18),
                               *(undefined4 *)(iVar5 + 0x10));
            puVar1 = &stack0xfffffff8;
          }
          AbyssEngine::String::~String((String *)(puVar1 + -0x2c));
          piVar10 = operator_new(0x14);
          iVar4 = *(int *)(*(int *)(*piVar12 + 4) + iVar9 * 4);
          iVar7 = *(int *)(iVar4 + 0x14);
          iVar4 = *(int *)(iVar4 + 0x18);
          *piVar10 = *(int *)(DAT_000808c0 + 0x80870) + 8;
          piVar10[1] = iVar5;
          piVar10[2] = iVar7;
          piVar10[3] = iVar4;
          piVar10[4] = 0;
          goto LAB_00080886;
        }
        if (uVar2 <= uVar8) {
          bVar13 = iVar5 == iVar9;
          iVar5 = iVar9;
          if (bVar13) goto LAB_00080886;
          goto LAB_00080790;
        }
      } while (uVar2 <= uVar8);
      iVar4 = iVar9;
    } while (iVar7 != 0);
  }
LAB_00080886:
  iVar5 = *piVar11 - local_28;
  if (iVar5 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar5);
}

```
## target disasm
```
  0008075c: push {r4,r5,r6,r7,lr}
  0008075e: add r7,sp,#0xc
  00080760: push {r8,r9,r10,r11}
  00080764: sub sp,#0x2c
  00080766: ldr r1,[0x000808b4]
  00080768: ldr r2,[0x000808b8]
  0008076a: add r1,pc
  0008076c: add r2,pc
  0008076e: ldr.w r11,[r1,#0x0]
  00080772: ldr.w r10,[r2,#0x0]
  00080776: ldr.w r1,[r11,#0x0]
  0008077a: ldr.w r2,[r10,#0x0]
  0008077e: str r2,[sp,#0x28]
  00080780: ldr r1,[r1,#0x0]
  00080782: cbz r1,0x000807c6
  00080784: blx 0x0006f358
  00080788: ldr.w r2,[r11,#0x0]
  0008078c: movs r4,#0x0
  0008078e: ldr r3,[r2,#0x0]
  00080790: subs r1,r3,r4
  00080792: add.w r6,r4,r1, asr #0x1
  00080796: asr.w r12,r1, asr #0x1
  0008079a: cmp r3,r4
  0008079c: ble 0x000807c6
  0008079e: ldr r1,[r2,#0x4]
  000807a0: ldr.w r1,[r1,r6,lsl #0x2]
  000807a4: ldr r5,[r1,#0x0]
  000807a6: cmp r5,r0
  000807a8: beq 0x000807ca
  000807aa: bhi 0x000807ba
  000807ac: cmp r5,r0
  000807ae: bcs 0x0008079a
  000807b0: cmp.w r12,#0x0
  000807b4: mov r4,r6
  000807b6: bne 0x00080790
  000807b8: b 0x000807c6
  000807ba: cmp r3,r6
  000807bc: mov.w r1,#0x0
  000807c0: mov r3,r6
  000807c2: bne 0x00080790
  000807c4: b 0x00080886
  000807c6: movs r1,#0x0
  000807c8: b 0x00080886
  000807ca: adds r0,r1,#0x4
  000807cc: blx 0x0006f3ac
  000807d0: mov r1,r0
  000807d2: ldr.w r0,[r11,#0x0]
  000807d6: ldr r0,[r0,#0x4]
  000807d8: ldr r2,[0x000808bc]
  000807da: add r2,pc
  000807dc: ldr.w r0,[r0,r6,lsl #0x2]
  000807e0: ldr r2,[r2,#0x0]
  000807e2: ldr r0,[r0,#0x18]
  000807e4: ldr.w r9,[r2,#0x0]
  000807e8: adds r0,#0x1
  000807ea: beq 0x00080824
  000807ec: add.w r8,sp,#0x1c
  000807f0: movs r2,#0x0
  000807f2: mov r0,r8
  000807f4: blx 0x0006ef50
  000807f8: ldr.w r0,[r11,#0x0]
  000807fc: ldr.w r1,[r9,#0x0]
  00080800: ldr r0,[r0,#0x4]
  00080802: ldr r5,[r1,#0x8]
  00080804: ldr.w r0,[r0,r6,lsl #0x2]
  00080808: ldrd r2,r1,[r0,#0xc]
  0008080c: ldrd r3,r0,[r0,#0x14]
  00080810: strd r3,r0,[sp,#0x0]
  00080814: mov r0,r9
  00080816: str r1,[sp,#0x8]
  00080818: mov r1,r8
  0008081a: movs r3,#0x1
  0008081c: blx r5
  0008081e: mov r8,r0
  00080820: add r0,sp,#0x1c
  00080822: b 0x00080858
  00080824: add.w r8,sp,#0x10
  00080828: movs r2,#0x0
  0008082a: movs r4,#0x0
  0008082c: mov r0,r8
  0008082e: blx 0x0006ef50
  00080832: ldr.w r0,[r11,#0x0]
  00080836: ldr.w r1,[r9,#0x0]
  0008083a: ldr r0,[r0,#0x4]
  0008083c: ldr r5,[r1,#0x8]
  0008083e: ldr.w r0,[r0,r6,lsl #0x2]
  00080842: ldrd r2,r0,[r0,#0xc]
  00080846: strd r4,r4,[sp,#0x0]
  0008084a: mov r1,r8
  0008084c: str r0,[sp,#0x8]
  0008084e: mov r0,r9
  00080850: movs r3,#0x0
  00080852: blx r5
  00080854: mov r8,r0
  00080856: add r0,sp,#0x10
  00080858: blx 0x0006ee30
  0008085c: movs r0,#0x14
  0008085e: blx 0x0006eb24
  00080862: mov r1,r0
  00080864: ldr.w r0,[r11,#0x0]
  00080868: ldr r0,[r0,#0x4]
  0008086a: ldr r2,[0x000808c0]
  0008086c: add r2,pc
  0008086e: ldr.w r0,[r0,r6,lsl #0x2]
  00080872: movs r6,#0x0
  00080874: ldr r2,[r2,#0x0]
  00080876: ldrd r3,r0,[r0,#0x14]
  0008087a: adds r2,#0x8
  0008087c: strd r2,r8,[r1,#0x0]
  00080880: strd r3,r0,[r1,#0x8]
  00080884: str r6,[r1,#0x10]
  00080886: ldr r0,[sp,#0x28]
  00080888: ldr.w r2,[r10,#0x0]
  0008088c: subs r0,r2,r0
  0008088e: itttt eq
  00080890: mov.eq r0,r1
  00080892: add.eq sp,#0x2c
  00080894: pop.eq.w {r8,r9,r10,r11}
  00080898: pop.eq {r4,r5,r6,r7,pc}
  0008089a: blx 0x0006e824
```
