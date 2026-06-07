# FileRead::loadItemsBinary
elf 0x120afc body 368
Sig: undefined __stdcall loadItemsBinary(void)

## decompile
```c

/* FileRead::loadItemsBinary() */

void FileRead::loadItemsBinary(void)

{
  String *pSVar1;
  int iVar2;
  Array<Item*> *this;
  Array *pAVar3;
  Array *pAVar4;
  Array *pAVar5;
  Item *this_00;
  uint *extraout_r2;
  int *piVar6;
  int iVar7;
  uint uVar8;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34 [3];
  int local_28;
  
  piVar6 = *(int **)(DAT_00130c9c + 0x130b0e);
  local_28 = *piVar6;
  pSVar1 = (String *)
           AbyssEngine::String::String((String *)local_34,(char *)(DAT_00130ca0 + 0x130b1a),false);
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String((String *)local_34);
  if (iVar2 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_00130ca4 + 0x130b38),(char *)local_34,extraout_r2);
    this = operator_new(0xc);
    Array<Item*>::Array(this);
    ArraySetLength<Item*>(0xe9,(Array *)this);
    local_3c = 0;
    local_38 = 0;
    local_40 = 0;
    for (uVar8 = 0; uVar8 < 0xe9; uVar8 = uVar8 + 1) {
      AEFile::ReadSwitched((int *)&local_38,local_34[0]);
      if ((int)local_38 < 1) {
        pAVar3 = (Array *)0x0;
      }
      else {
        pAVar3 = operator_new(0xc);
        Array<int>::Array();
        ArraySetLength<int>(local_38,pAVar3);
        iVar2 = 0;
        for (iVar7 = 0; iVar7 < (int)local_38; iVar7 = iVar7 + 1) {
          AEFile::ReadSwitched((int *)(*(int *)(pAVar3 + 4) + iVar2),local_34[0]);
          iVar2 = iVar2 + 4;
        }
      }
      AEFile::ReadSwitched((int *)&local_3c,local_34[0]);
      if ((int)local_3c < 1) {
        pAVar4 = (Array *)0x0;
      }
      else {
        pAVar4 = operator_new(0xc);
        Array<int>::Array();
        ArraySetLength<int>(local_3c,pAVar4);
        iVar2 = 0;
        for (iVar7 = 0; iVar7 < (int)local_3c; iVar7 = iVar7 + 1) {
          AEFile::ReadSwitched((int *)(*(int *)(pAVar4 + 4) + iVar2),local_34[0]);
          iVar2 = iVar2 + 4;
        }
      }
      AEFile::ReadSwitched((int *)&local_40,local_34[0]);
      if ((int)local_40 < 1) {
        pAVar5 = (Array *)0x0;
      }
      else {
        pAVar5 = operator_new(0xc);
        Array<int>::Array();
        ArraySetLength<int>(local_40,pAVar5);
        iVar7 = 0;
        for (iVar2 = 0; iVar2 < (int)local_40; iVar2 = iVar2 + 1) {
          AEFile::ReadSwitched((int *)(*(int *)(pAVar5 + 4) + iVar7),local_34[0]);
          iVar7 = iVar7 + 4;
        }
      }
      this_00 = operator_new(0x48);
      Item::Item(this_00,pAVar3,pAVar4,pAVar5);
      *(Item **)(*(int *)(this + 4) + uVar8 * 4) = this_00;
    }
    AEFile::Close(local_34[0]);
  }
  iVar2 = *piVar6 - local_28;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  00130afc: push {r4,r5,r6,r7,lr}
  00130afe: add r7,sp,#0xc
  00130b00: push {r8,r9,r10,r11}
  00130b04: sub sp,#0x24
  00130b06: ldr r0,[0x00130c9c]
  00130b08: movs r2,#0x0
  00130b0a: add r0,pc
  00130b0c: ldr r5,[r0,#0x0]
  00130b0e: ldr r0,[r5,#0x0]
  00130b10: ldr r1,[0x00130ca0]
  00130b12: str r0,[sp,#0x20]
  00130b14: add r0,sp,#0x14
  00130b16: add r1,pc
  00130b18: blx 0x0006ee18
  00130b1c: blx 0x0006fff4
  00130b20: mov r4,r0
  00130b22: add r0,sp,#0x14
  00130b24: blx 0x0006ee30
  00130b28: cmp r4,#0x0
  00130b2a: beq.w 0x00130c54
  00130b2e: str r5,[sp,#0x0]
  00130b30: add r1,sp,#0x14
  00130b32: ldr r0,[0x00130ca4]
  00130b34: add r0,pc
  00130b36: blx 0x0006ecd4
  00130b3a: movs r0,#0xc
  00130b3c: blx 0x0006eb24
  00130b40: mov r6,r0
  00130b42: blx 0x00071860
  00130b46: movs r0,#0xe9
  00130b48: mov r1,r6
  00130b4a: blx 0x0007186c
  00130b4e: mov.w r8,#0x0
  00130b52: strd r8,r8,[sp,#0xc]
  00130b56: strd r6,r8,[sp,#0x4]
  00130b5a: b 0x00130c40
  00130b5c: add r0,sp,#0x10
  00130b5e: blx 0x0007630c
  00130b62: ldr r0,[sp,#0x10]
  00130b64: cmp r0,#0x1
  00130b66: blt 0x00130b9a
  00130b68: movs r0,#0xc
  00130b6a: blx 0x0006eb24
  00130b6e: mov r10,r0
  00130b70: blx 0x000701f8
  00130b74: ldr r0,[sp,#0x10]
  00130b76: mov r1,r10
  00130b78: blx 0x00071a4c
  00130b7c: movs r4,#0x0
  00130b7e: movs r5,#0x0
  00130b80: b 0x00130b92
  00130b82: ldr.w r0,[r10,#0x4]
  00130b86: ldr r1,[sp,#0x14]
  00130b88: add r0,r4
  00130b8a: blx 0x0007630c
  00130b8e: adds r4,#0x4
  00130b90: adds r5,#0x1
  00130b92: ldr r0,[sp,#0x10]
  00130b94: cmp r5,r0
  00130b96: blt 0x00130b82
  00130b98: b 0x00130b9e
  00130b9a: mov.w r10,#0x0
  00130b9e: ldr r1,[sp,#0x14]
  00130ba0: add r0,sp,#0xc
  00130ba2: blx 0x0007630c
  00130ba6: ldr r0,[sp,#0xc]
  00130ba8: cmp r0,#0x1
  00130baa: blt 0x00130bde
  00130bac: movs r0,#0xc
  00130bae: blx 0x0006eb24
  00130bb2: mov r11,r0
  00130bb4: blx 0x000701f8
  00130bb8: ldr r0,[sp,#0xc]
  00130bba: mov r1,r11
  00130bbc: blx 0x00071a4c
  00130bc0: movs r4,#0x0
  00130bc2: movs r6,#0x0
  00130bc4: b 0x00130bd6
  00130bc6: ldr.w r0,[r11,#0x4]
  00130bca: ldr r1,[sp,#0x14]
  00130bcc: add r0,r4
  00130bce: blx 0x0007630c
  00130bd2: adds r4,#0x4
  00130bd4: adds r6,#0x1
  00130bd6: ldr r0,[sp,#0xc]
  00130bd8: cmp r6,r0
  00130bda: blt 0x00130bc6
  00130bdc: b 0x00130be2
  00130bde: mov.w r11,#0x0
  00130be2: ldr r1,[sp,#0x14]
  00130be4: add r0,sp,#0x8
  00130be6: blx 0x0007630c
  00130bea: ldr r0,[sp,#0x8]
  00130bec: cmp r0,#0x1
  00130bee: blt 0x00130c20
  00130bf0: movs r0,#0xc
  00130bf2: blx 0x0006eb24
  00130bf6: mov r4,r0
  00130bf8: blx 0x000701f8
  00130bfc: ldr r0,[sp,#0x8]
  00130bfe: mov r1,r4
  00130c00: blx 0x00071a4c
  00130c04: movs r6,#0x0
  00130c06: movs r5,#0x0
  00130c08: b 0x00130c18
  00130c0a: ldr r0,[r4,#0x4]
  00130c0c: ldr r1,[sp,#0x14]
  00130c0e: add r0,r6
  00130c10: blx 0x0007630c
  00130c14: adds r6,#0x4
  00130c16: adds r5,#0x1
  00130c18: ldr r0,[sp,#0x8]
  00130c1a: cmp r5,r0
  00130c1c: blt 0x00130c0a
  00130c1e: b 0x00130c22
  00130c20: movs r4,#0x0
  00130c22: movs r0,#0x48
  00130c24: blx 0x0006eb24
  00130c28: mov r9,r0
  00130c2a: mov r1,r10
  00130c2c: mov r2,r11
  00130c2e: mov r3,r4
  00130c30: blx 0x0007636c
  00130c34: ldr r6,[sp,#0x4]
  00130c36: ldr r0,[r6,#0x4]
  00130c38: str.w r9,[r0,r8,lsl #0x2]
  00130c3c: add.w r8,r8,#0x1
  00130c40: ldr r1,[sp,#0x14]
  00130c42: cmp.w r8,#0xe9
  00130c46: bcc.w 0x00130b5c
  00130c4a: mov r0,r1
  00130c4c: blx 0x0006ece0
  00130c50: ldr r5,[sp,#0x0]
  00130c52: b 0x00130c56
  00130c54: movs r6,#0x0
  00130c56: ldr r0,[sp,#0x20]
  00130c58: ldr r1,[r5,#0x0]
  00130c5a: subs r0,r1,r0
  00130c5c: itttt eq
  00130c5e: mov.eq r0,r6
  00130c60: add.eq sp,#0x24
  00130c62: pop.eq.w {r8,r9,r10,r11}
  00130c66: pop.eq {r4,r5,r6,r7,pc}
  00130c68: blx 0x0006e824
```
