# FileRead::loadStationCollision
elf 0x11fc5c body 246
Sig: undefined __stdcall loadStationCollision(int param_1)

## decompile
```c

/* FileRead::loadStationCollision(int) */

void FileRead::loadStationCollision(int param_1)

{
  int iVar1;
  String *pSVar2;
  int iVar3;
  Array *pAVar4;
  uint uVar5;
  void *pvVar6;
  int in_r1;
  uint in_r2;
  uint *extraout_r2;
  int in_r3;
  uint unaff_r4;
  uint uVar7;
  int *piVar8;
  uint local_38;
  int iStack_34;
  
  piVar8 = *(int **)(DAT_0012fd6c + 0x12fc6e);
  iVar1 = *piVar8;
  local_38 = in_r2;
  iStack_34 = in_r3;
  pSVar2 = (String *)
           AbyssEngine::String::String
                     ((String *)&stack0xffffffd0,(char *)(DAT_0012fd70 + 0x12fc7e),false);
  iVar3 = AEFile::FileExist(pSVar2);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  uVar7 = 0;
  if (iVar3 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_0012fd74 + 0x12fc9a),&stack0xffffffd0,extraout_r2);
    local_38 = 0;
    iStack_34 = 0;
    for (; uVar7 < 0x88; uVar7 = uVar7 + 1) {
      AEFile::Read(&iStack_34,unaff_r4);
      AEFile::Read((int *)&local_38,unaff_r4);
      local_38 = local_38 + 1;
      if (iStack_34 == in_r1) {
        pAVar4 = operator_new(0xc);
        Array<int>::Array();
        uVar7 = local_38;
        uVar5 = (uint)((ulonglong)local_38 * 4);
        if ((int)((ulonglong)local_38 * 4 >> 0x20) != 0) {
          uVar5 = 0xffffffff;
        }
        pvVar6 = operator_new__(uVar5);
        AEFile::Read(uVar7 << 2,pvVar6,unaff_r4);
        ArraySetLength<int>(local_38,pAVar4);
        for (iVar3 = 0; iVar3 < (int)local_38; iVar3 = iVar3 + 1) {
          *(undefined4 *)(*(int *)(pAVar4 + 4) + iVar3 * 4) =
               *(undefined4 *)((int)pvVar6 + iVar3 * 4);
        }
        operator_delete__(pvVar6);
        AEFile::Close(unaff_r4);
        goto LAB_0012fd3a;
      }
      AEFile::Skip(local_38 * 4,unaff_r4);
    }
    AEFile::Close(unaff_r4);
  }
LAB_0012fd3a:
  iVar1 = *piVar8 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  0012fc5c: push {r4,r5,r6,r7,lr}
  0012fc5e: add r7,sp,#0xc
  0012fc60: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  0012fc64: ldr r0,[0x0012fd6c]
  0012fc66: mov r5,r1
  0012fc68: movs r2,#0x0
  0012fc6a: add r0,pc
  0012fc6c: ldr.w r9,[r0,#0x0]
  0012fc70: ldr.w r0,[r9,#0x0]
  0012fc74: ldr r1,[0x0012fd70]
  0012fc76: str r0,[sp,#0x14]
  0012fc78: add r0,sp,#0x8
  0012fc7a: add r1,pc
  0012fc7c: blx 0x0006ee18
  0012fc80: blx 0x0006fff4
  0012fc84: mov r6,r0
  0012fc86: add r0,sp,#0x8
  0012fc88: blx 0x0006ee30
  0012fc8c: movs r4,#0x0
  0012fc8e: cmp r6,#0x0
  0012fc90: beq 0x0012fd3a
  0012fc92: ldr r0,[0x0012fd74]
  0012fc94: add r1,sp,#0x8
  0012fc96: add r0,pc
  0012fc98: blx 0x0006ecd4
  0012fc9c: add.w r8,sp,#0x4
  0012fca0: mov r6,sp
  0012fca2: strd r4,r4,[sp,#0x0]
  0012fca6: b 0x0012fcce
  0012fca8: mov r0,r8
  0012fcaa: blx 0x00074818
  0012fcae: ldr r1,[sp,#0x8]
  0012fcb0: mov r0,r6
  0012fcb2: blx 0x00074818
  0012fcb6: ldrd r0,r1,[sp,#0x0]
  0012fcba: cmp r1,r5
  0012fcbc: add.w r0,r0,#0x1
  0012fcc0: str r0,[sp,#0x0]
  0012fcc2: beq 0x0012fcde
  0012fcc4: ldr r1,[sp,#0x8]
  0012fcc6: lsls r0,r0,#0x2
  0012fcc8: blx 0x0006f1f0
  0012fccc: adds r4,#0x1
  0012fcce: ldr r1,[sp,#0x8]
  0012fcd0: cmp r4,#0x88
  0012fcd2: bcc 0x0012fca8
  0012fcd4: mov r0,r1
  0012fcd6: blx 0x0006ece0
  0012fcda: movs r4,#0x0
  0012fcdc: b 0x0012fd3a
  0012fcde: movs r0,#0xc
  0012fce0: blx 0x0006eb24
  0012fce4: mov r4,r0
  0012fce6: blx 0x000701f8
  0012fcea: ldr r6,[sp,#0x0]
  0012fcec: movs r0,#0x4
  0012fcee: umull r0,r1,r6,r0
  0012fcf2: cmp r1,#0x0
  0012fcf4: it ne
  0012fcf6: mov.ne r1,#0x1
  0012fcf8: cmp r1,#0x0
  0012fcfa: it ne
  0012fcfc: mov.ne.w r0,#0xffffffff
  0012fd00: blx 0x0006ec08
  0012fd04: ldr r2,[sp,#0x8]
  0012fd06: mov r5,r0
  0012fd08: lsls r0,r6,#0x2
  0012fd0a: mov r1,r5
  0012fd0c: blx 0x0006eb90
  0012fd10: ldr r0,[sp,#0x0]
  0012fd12: mov r1,r4
  0012fd14: blx 0x00071a4c
  0012fd18: movs r2,#0x0
  0012fd1a: b 0x0012fd28
  0012fd1c: ldr r0,[r4,#0x4]
  0012fd1e: ldr.w r1,[r5,r2,lsl #0x2]
  0012fd22: str.w r1,[r0,r2,lsl #0x2]
  0012fd26: adds r2,#0x1
  0012fd28: ldr r0,[sp,#0x0]
  0012fd2a: cmp r2,r0
  0012fd2c: blt 0x0012fd1c
  0012fd2e: mov r0,r5
  0012fd30: blx 0x0006ebfc
  0012fd34: ldr r0,[sp,#0x8]
  0012fd36: blx 0x0006ece0
  0012fd3a: ldr r0,[sp,#0x14]
  0012fd3c: ldr.w r1,[r9,#0x0]
  0012fd40: subs r0,r1,r0
  0012fd42: itttt eq
  0012fd44: mov.eq r0,r4
  0012fd46: add.eq sp,#0x18
  0012fd48: pop.eq.w {r8,r9,r11}
  0012fd4c: pop.eq {r4,r5,r6,r7,pc}
  0012fd4e: blx 0x0006e824
```
