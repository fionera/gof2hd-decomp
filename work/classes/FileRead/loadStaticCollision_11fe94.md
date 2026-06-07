# FileRead::loadStaticCollision
elf 0x11fe94 body 246
Sig: undefined __stdcall loadStaticCollision(int param_1)

## decompile
```c

/* FileRead::loadStaticCollision(int) */

void FileRead::loadStaticCollision(int param_1)

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
  
  piVar8 = *(int **)(DAT_0012ffa4 + 0x12fea6);
  iVar1 = *piVar8;
  local_38 = in_r2;
  iStack_34 = in_r3;
  pSVar2 = (String *)
           AbyssEngine::String::String
                     ((String *)&stack0xffffffd0,(char *)(DAT_0012ffa8 + 0x12feb6),false);
  iVar3 = AEFile::FileExist(pSVar2);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  uVar7 = 0;
  if (iVar3 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_0012ffac + 0x12fed2),&stack0xffffffd0,extraout_r2);
    local_38 = 0;
    iStack_34 = 0;
    for (; uVar7 < 7; uVar7 = uVar7 + 1) {
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
        goto LAB_0012ff72;
      }
      AEFile::Skip(local_38 * 4,unaff_r4);
    }
    AEFile::Close(unaff_r4);
  }
LAB_0012ff72:
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
  0012fe94: push {r4,r5,r6,r7,lr}
  0012fe96: add r7,sp,#0xc
  0012fe98: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  0012fe9c: ldr r0,[0x0012ffa4]
  0012fe9e: mov r5,r1
  0012fea0: movs r2,#0x0
  0012fea2: add r0,pc
  0012fea4: ldr.w r9,[r0,#0x0]
  0012fea8: ldr.w r0,[r9,#0x0]
  0012feac: ldr r1,[0x0012ffa8]
  0012feae: str r0,[sp,#0x14]
  0012feb0: add r0,sp,#0x8
  0012feb2: add r1,pc
  0012feb4: blx 0x0006ee18
  0012feb8: blx 0x0006fff4
  0012febc: mov r6,r0
  0012febe: add r0,sp,#0x8
  0012fec0: blx 0x0006ee30
  0012fec4: movs r4,#0x0
  0012fec6: cmp r6,#0x0
  0012fec8: beq 0x0012ff72
  0012feca: ldr r0,[0x0012ffac]
  0012fecc: add r1,sp,#0x8
  0012fece: add r0,pc
  0012fed0: blx 0x0006ecd4
  0012fed4: add.w r8,sp,#0x4
  0012fed8: mov r6,sp
  0012feda: strd r4,r4,[sp,#0x0]
  0012fede: b 0x0012ff06
  0012fee0: mov r0,r8
  0012fee2: blx 0x00074818
  0012fee6: ldr r1,[sp,#0x8]
  0012fee8: mov r0,r6
  0012feea: blx 0x00074818
  0012feee: ldrd r0,r1,[sp,#0x0]
  0012fef2: cmp r1,r5
  0012fef4: add.w r0,r0,#0x1
  0012fef8: str r0,[sp,#0x0]
  0012fefa: beq 0x0012ff16
  0012fefc: ldr r1,[sp,#0x8]
  0012fefe: lsls r0,r0,#0x2
  0012ff00: blx 0x0006f1f0
  0012ff04: adds r4,#0x1
  0012ff06: ldr r1,[sp,#0x8]
  0012ff08: cmp r4,#0x7
  0012ff0a: bcc 0x0012fee0
  0012ff0c: mov r0,r1
  0012ff0e: blx 0x0006ece0
  0012ff12: movs r4,#0x0
  0012ff14: b 0x0012ff72
  0012ff16: movs r0,#0xc
  0012ff18: blx 0x0006eb24
  0012ff1c: mov r4,r0
  0012ff1e: blx 0x000701f8
  0012ff22: ldr r6,[sp,#0x0]
  0012ff24: movs r0,#0x4
  0012ff26: umull r0,r1,r6,r0
  0012ff2a: cmp r1,#0x0
  0012ff2c: it ne
  0012ff2e: mov.ne r1,#0x1
  0012ff30: cmp r1,#0x0
  0012ff32: it ne
  0012ff34: mov.ne.w r0,#0xffffffff
  0012ff38: blx 0x0006ec08
  0012ff3c: ldr r2,[sp,#0x8]
  0012ff3e: mov r5,r0
  0012ff40: lsls r0,r6,#0x2
  0012ff42: mov r1,r5
  0012ff44: blx 0x0006eb90
  0012ff48: ldr r0,[sp,#0x0]
  0012ff4a: mov r1,r4
  0012ff4c: blx 0x00071a4c
  0012ff50: movs r2,#0x0
  0012ff52: b 0x0012ff60
  0012ff54: ldr r0,[r4,#0x4]
  0012ff56: ldr.w r1,[r5,r2,lsl #0x2]
  0012ff5a: str.w r1,[r0,r2,lsl #0x2]
  0012ff5e: adds r2,#0x1
  0012ff60: ldr r0,[sp,#0x0]
  0012ff62: cmp r2,r0
  0012ff64: blt 0x0012ff54
  0012ff66: mov r0,r5
  0012ff68: blx 0x0006ebfc
  0012ff6c: ldr r0,[sp,#0x8]
  0012ff6e: blx 0x0006ece0
  0012ff72: ldr r0,[sp,#0x14]
  0012ff74: ldr.w r1,[r9,#0x0]
  0012ff78: subs r0,r1,r0
  0012ff7a: itttt eq
  0012ff7c: mov.eq r0,r4
  0012ff7e: add.eq sp,#0x18
  0012ff80: pop.eq.w {r8,r9,r11}
  0012ff84: pop.eq {r4,r5,r6,r7,pc}
  0012ff86: blx 0x0006e824
```
