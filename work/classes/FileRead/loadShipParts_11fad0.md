# FileRead::loadShipParts
elf 0x11fad0 body 352
Sig: undefined __stdcall loadShipParts(int param_1)

## decompile
```c

/* FileRead::loadShipParts(int) */

void FileRead::loadShipParts(int param_1)

{
  int iVar1;
  String *pSVar2;
  int iVar3;
  Array *pAVar4;
  int iVar5;
  int in_r1;
  uint *extraout_r2;
  undefined4 in_r3;
  uint unaff_r4;
  int *piVar6;
  code *pcVar7;
  uint uVar8;
  code *pcVar9;
  uint uVar10;
  undefined4 local_38;
  
  piVar6 = *(int **)(DAT_0012fc48 + 0x12fae2);
  iVar1 = *piVar6;
  local_38 = in_r3;
  pSVar2 = (String *)
           AbyssEngine::String::String
                     ((String *)&stack0xffffffcc,(char *)(DAT_0012fc4c + 0x12faee),false);
  iVar3 = AEFile::FileExist(pSVar2);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  if (iVar3 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_0012fc50 + 0x12fb0c),&stack0xffffffcc,extraout_r2);
    uVar8 = 0;
    pcVar7 = *(code **)(DAT_0012fc54 + 0x12fb20);
    pcVar9 = *(code **)(DAT_0012fc58 + 0x12fb26);
    do {
      if (0x3f < uVar8) {
        AEFile::Close(unaff_r4);
        break;
      }
      AEFile::Read((signed *)((int)&local_38 + 3),unaff_r4);
      AEFile::Read((signed *)((int)&local_38 + 2),unaff_r4);
      pAVar4 = operator_new(0xc);
      Array<int>::Array();
      ArraySetLength<int>(local_38._2_1_ * 10,pAVar4);
      iVar3 = 0;
      for (uVar10 = 0; uVar10 < *(uint *)pAVar4; uVar10 = uVar10 + 10) {
        (*pcVar7)(&local_38,unaff_r4);
        iVar5 = *(int *)(pAVar4 + 4);
        *(int *)(iVar5 + iVar3) = (int)(short)local_38;
        (*pcVar9)((int *)(iVar5 + iVar3) + 1,unaff_r4);
        (*pcVar9)(*(int *)(pAVar4 + 4) + iVar3 + 8,unaff_r4);
        (*pcVar9)(*(int *)(pAVar4 + 4) + iVar3 + 0xc,unaff_r4);
        (*pcVar7)(&local_38,unaff_r4);
        *(int *)(*(int *)(pAVar4 + 4) + iVar3 + 0x10) = (int)(short)local_38;
        (*pcVar7)(&local_38,unaff_r4);
        *(int *)(*(int *)(pAVar4 + 4) + iVar3 + 0x14) = (int)(short)local_38;
        (*pcVar7)(&local_38,unaff_r4);
        *(int *)(*(int *)(pAVar4 + 4) + iVar3 + 0x18) = (int)(short)local_38;
        (*pcVar7)(&local_38,unaff_r4);
        *(int *)(*(int *)(pAVar4 + 4) + iVar3 + 0x1c) = (int)(short)local_38;
        (*pcVar7)(&local_38,unaff_r4);
        *(int *)(*(int *)(pAVar4 + 4) + iVar3 + 0x20) = (int)(short)local_38;
        (*pcVar7)(&local_38,unaff_r4);
        iVar5 = *(int *)(pAVar4 + 4) + iVar3;
        iVar3 = iVar3 + 0x28;
        *(int *)(iVar5 + 0x24) = (int)(short)local_38;
      }
      uVar8 = uVar8 + 1;
    } while (in_r1 + 1 != (int)local_38._3_1_);
  }
  iVar1 = *piVar6 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  0012fad0: push {r4,r5,r6,r7,lr}
  0012fad2: add r7,sp,#0xc
  0012fad4: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0012fad8: ldr r0,[0x0012fc48]
  0012fada: mov r4,r1
  0012fadc: movs r2,#0x0
  0012fade: add r0,pc
  0012fae0: ldr r6,[r0,#0x0]
  0012fae2: ldr r0,[r6,#0x0]
  0012fae4: ldr r1,[0x0012fc4c]
  0012fae6: str r0,[sp,#0x18]
  0012fae8: add r0,sp,#0xc
  0012faea: add r1,pc
  0012faec: blx 0x0006ee18
  0012faf0: blx 0x0006fff4
  0012faf4: mov r5,r0
  0012faf6: add r0,sp,#0xc
  0012faf8: blx 0x0006ee30
  0012fafc: cmp r5,#0x0
  0012fafe: beq.w 0x0012fc0e
  0012fb02: str r6,[sp,#0x0]
  0012fb04: add r1,sp,#0xc
  0012fb06: ldr r0,[0x0012fc50]
  0012fb08: add r0,pc
  0012fb0a: blx 0x0006ecd4
  0012fb0e: adds r0,r4,#0x1
  0012fb10: str r0,[sp,#0x4]
  0012fb12: ldr r0,[0x0012fc54]
  0012fb14: add r5,sp,#0x8
  0012fb16: movs r4,#0x0
  0012fb18: mov.w r8,#0x0
  0012fb1c: add r0,pc
  0012fb1e: ldr r6,[r0,#0x0]
  0012fb20: ldr r0,[0x0012fc58]
  0012fb22: add r0,pc
  0012fb24: ldr.w r9,[r0,#0x0]
  0012fb28: cmp.w r8,#0x40
  0012fb2c: bcs 0x0012fc12
  0012fb2e: ldr r1,[sp,#0xc]
  0012fb30: sub.w r0,r7,#0x2d
  0012fb34: blx 0x00076324
  0012fb38: ldr r1,[sp,#0xc]
  0012fb3a: sub.w r0,r7,#0x2e
  0012fb3e: blx 0x00076324
  0012fb42: movs r0,#0xc
  0012fb44: blx 0x0006eb24
  0012fb48: mov r4,r0
  0012fb4a: blx 0x000701f8
  0012fb4e: ldrsb.w r0,[r7,#-0x2e]
  0012fb52: mov r1,r4
  0012fb54: add.w r0,r0,r0, lsl #0x2
  0012fb58: lsls r0,r0,#0x1
  0012fb5a: blx 0x00071a4c
  0012fb5e: mov.w r10,#0x0
  0012fb62: mov.w r11,#0x0
  0012fb66: b 0x0012fbf8
  0012fb68: ldr r1,[sp,#0xc]
  0012fb6a: mov r0,r5
  0012fb6c: blx r6
  0012fb6e: ldr r0,[r4,#0x4]
  0012fb70: ldrsh.w r1,[sp,#0x8]
  0012fb74: add r0,r10
  0012fb76: stmia r0!,{r1}
  0012fb78: ldr r1,[sp,#0xc]
  0012fb7a: blx r9
  0012fb7c: ldr r0,[r4,#0x4]
  0012fb7e: ldr r1,[sp,#0xc]
  0012fb80: add r0,r10
  0012fb82: adds r0,#0x8
  0012fb84: blx r9
  0012fb86: ldr r0,[r4,#0x4]
  0012fb88: ldr r1,[sp,#0xc]
  0012fb8a: add r0,r10
  0012fb8c: adds r0,#0xc
  0012fb8e: blx r9
  0012fb90: ldr r1,[sp,#0xc]
  0012fb92: mov r0,r5
  0012fb94: blx r6
  0012fb96: ldr r0,[r4,#0x4]
  0012fb98: ldrsh.w r1,[sp,#0x8]
  0012fb9c: add r0,r10
  0012fb9e: str r1,[r0,#0x10]
  0012fba0: mov r0,r5
  0012fba2: ldr r1,[sp,#0xc]
  0012fba4: blx r6
  0012fba6: ldr r0,[r4,#0x4]
  0012fba8: ldrsh.w r1,[sp,#0x8]
  0012fbac: add r0,r10
  0012fbae: str r1,[r0,#0x14]
  0012fbb0: mov r0,r5
  0012fbb2: ldr r1,[sp,#0xc]
  0012fbb4: blx r6
  0012fbb6: ldr r0,[r4,#0x4]
  0012fbb8: ldrsh.w r1,[sp,#0x8]
  0012fbbc: add r0,r10
  0012fbbe: str r1,[r0,#0x18]
  0012fbc0: mov r0,r5
  0012fbc2: ldr r1,[sp,#0xc]
  0012fbc4: blx r6
  0012fbc6: ldr r0,[r4,#0x4]
  0012fbc8: ldrsh.w r1,[sp,#0x8]
  0012fbcc: add r0,r10
  0012fbce: str r1,[r0,#0x1c]
  0012fbd0: mov r0,r5
  0012fbd2: ldr r1,[sp,#0xc]
  0012fbd4: blx r6
  0012fbd6: ldr r0,[r4,#0x4]
  0012fbd8: ldrsh.w r1,[sp,#0x8]
  0012fbdc: add r0,r10
  0012fbde: str r1,[r0,#0x20]
  0012fbe0: mov r0,r5
  0012fbe2: ldr r1,[sp,#0xc]
  0012fbe4: blx r6
  0012fbe6: ldr r0,[r4,#0x4]
  0012fbe8: add.w r11,r11,#0xa
  0012fbec: ldrsh.w r1,[sp,#0x8]
  0012fbf0: add r0,r10
  0012fbf2: add.w r10,r10,#0x28
  0012fbf6: str r1,[r0,#0x24]
  0012fbf8: ldr r0,[r4,#0x0]
  0012fbfa: cmp r11,r0
  0012fbfc: bcc 0x0012fb68
  0012fbfe: ldrsb.w r0,[r7,#-0x2d]
  0012fc02: add.w r8,r8,#0x1
  0012fc06: ldr r1,[sp,#0x4]
  0012fc08: cmp r1,r0
  0012fc0a: bne 0x0012fb28
  0012fc0c: b 0x0012fc18
  0012fc0e: movs r4,#0x0
  0012fc10: b 0x0012fc1a
  0012fc12: ldr r0,[sp,#0xc]
  0012fc14: blx 0x0006ece0
  0012fc18: ldr r6,[sp,#0x0]
  0012fc1a: ldr r0,[sp,#0x18]
  0012fc1c: ldr r1,[r6,#0x0]
  0012fc1e: subs r0,r1,r0
  0012fc20: itttt eq
  0012fc22: mov.eq r0,r4
  0012fc24: add.eq sp,#0x1c
  0012fc26: pop.eq.w {r8,r9,r10,r11}
  0012fc2a: pop.eq {r4,r5,r6,r7,pc}
  0012fc2c: blx 0x0006e824
```
