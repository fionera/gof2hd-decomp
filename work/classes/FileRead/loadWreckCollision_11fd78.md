# FileRead::loadWreckCollision
elf 0x11fd78 body 246
Sig: undefined __stdcall loadWreckCollision(int param_1)

## decompile
```c

/* FileRead::loadWreckCollision(int) */

void FileRead::loadWreckCollision(int param_1)

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
  
  piVar8 = *(int **)(DAT_0012fe88 + 0x12fd8a);
  iVar1 = *piVar8;
  local_38 = in_r2;
  iStack_34 = in_r3;
  pSVar2 = (String *)
           AbyssEngine::String::String
                     ((String *)&stack0xffffffd0,(char *)(DAT_0012fe8c + 0x12fd9a),false);
  iVar3 = AEFile::FileExist(pSVar2);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  uVar7 = 0;
  if (iVar3 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_0012fe90 + 0x12fdb6),&stack0xffffffd0,extraout_r2);
    local_38 = 0;
    iStack_34 = 0;
    for (; uVar7 < 6; uVar7 = uVar7 + 1) {
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
        goto LAB_0012fe56;
      }
      AEFile::Skip(local_38 * 4,unaff_r4);
    }
    AEFile::Close(unaff_r4);
  }
LAB_0012fe56:
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
  0012fd78: push {r4,r5,r6,r7,lr}
  0012fd7a: add r7,sp,#0xc
  0012fd7c: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  0012fd80: ldr r0,[0x0012fe88]
  0012fd82: mov r5,r1
  0012fd84: movs r2,#0x0
  0012fd86: add r0,pc
  0012fd88: ldr.w r9,[r0,#0x0]
  0012fd8c: ldr.w r0,[r9,#0x0]
  0012fd90: ldr r1,[0x0012fe8c]
  0012fd92: str r0,[sp,#0x14]
  0012fd94: add r0,sp,#0x8
  0012fd96: add r1,pc
  0012fd98: blx 0x0006ee18
  0012fd9c: blx 0x0006fff4
  0012fda0: mov r6,r0
  0012fda2: add r0,sp,#0x8
  0012fda4: blx 0x0006ee30
  0012fda8: movs r4,#0x0
  0012fdaa: cmp r6,#0x0
  0012fdac: beq 0x0012fe56
  0012fdae: ldr r0,[0x0012fe90]
  0012fdb0: add r1,sp,#0x8
  0012fdb2: add r0,pc
  0012fdb4: blx 0x0006ecd4
  0012fdb8: add.w r8,sp,#0x4
  0012fdbc: mov r6,sp
  0012fdbe: strd r4,r4,[sp,#0x0]
  0012fdc2: b 0x0012fdea
  0012fdc4: mov r0,r8
  0012fdc6: blx 0x00074818
  0012fdca: ldr r1,[sp,#0x8]
  0012fdcc: mov r0,r6
  0012fdce: blx 0x00074818
  0012fdd2: ldrd r0,r1,[sp,#0x0]
  0012fdd6: cmp r1,r5
  0012fdd8: add.w r0,r0,#0x1
  0012fddc: str r0,[sp,#0x0]
  0012fdde: beq 0x0012fdfa
  0012fde0: ldr r1,[sp,#0x8]
  0012fde2: lsls r0,r0,#0x2
  0012fde4: blx 0x0006f1f0
  0012fde8: adds r4,#0x1
  0012fdea: ldr r1,[sp,#0x8]
  0012fdec: cmp r4,#0x6
  0012fdee: bcc 0x0012fdc4
  0012fdf0: mov r0,r1
  0012fdf2: blx 0x0006ece0
  0012fdf6: movs r4,#0x0
  0012fdf8: b 0x0012fe56
  0012fdfa: movs r0,#0xc
  0012fdfc: blx 0x0006eb24
  0012fe00: mov r4,r0
  0012fe02: blx 0x000701f8
  0012fe06: ldr r6,[sp,#0x0]
  0012fe08: movs r0,#0x4
  0012fe0a: umull r0,r1,r6,r0
  0012fe0e: cmp r1,#0x0
  0012fe10: it ne
  0012fe12: mov.ne r1,#0x1
  0012fe14: cmp r1,#0x0
  0012fe16: it ne
  0012fe18: mov.ne.w r0,#0xffffffff
  0012fe1c: blx 0x0006ec08
  0012fe20: ldr r2,[sp,#0x8]
  0012fe22: mov r5,r0
  0012fe24: lsls r0,r6,#0x2
  0012fe26: mov r1,r5
  0012fe28: blx 0x0006eb90
  0012fe2c: ldr r0,[sp,#0x0]
  0012fe2e: mov r1,r4
  0012fe30: blx 0x00071a4c
  0012fe34: movs r2,#0x0
  0012fe36: b 0x0012fe44
  0012fe38: ldr r0,[r4,#0x4]
  0012fe3a: ldr.w r1,[r5,r2,lsl #0x2]
  0012fe3e: str.w r1,[r0,r2,lsl #0x2]
  0012fe42: adds r2,#0x1
  0012fe44: ldr r0,[sp,#0x0]
  0012fe46: cmp r2,r0
  0012fe48: blt 0x0012fe38
  0012fe4a: mov r0,r5
  0012fe4c: blx 0x0006ebfc
  0012fe50: ldr r0,[sp,#0x8]
  0012fe52: blx 0x0006ece0
  0012fe56: ldr r0,[sp,#0x14]
  0012fe58: ldr.w r1,[r9,#0x0]
  0012fe5c: subs r0,r1,r0
  0012fe5e: itttt eq
  0012fe60: mov.eq r0,r4
  0012fe62: add.eq sp,#0x18
  0012fe64: pop.eq.w {r8,r9,r11}
  0012fe68: pop.eq {r4,r5,r6,r7,pc}
  0012fe6a: blx 0x0006e824
```
