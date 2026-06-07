# FileRead::loadStationParts
elf 0x11ffb0 body 368
Sig: undefined __thiscall loadStationParts(FileRead * this, int param_1, int param_2)

## decompile
```c

/* FileRead::loadStationParts(int, int) */

void __thiscall FileRead::loadStationParts(FileRead *this,int param_1,int param_2)

{
  int *piVar1;
  String *pSVar2;
  int iVar3;
  int iVar4;
  Array *pAVar5;
  int *piVar6;
  int iVar7;
  uint *extraout_r2;
  undefined4 *puVar8;
  code *pcVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  code *pcVar13;
  undefined8 in_d5;
  undefined8 in_d6;
  undefined4 in_s14;
  int iVar14;
  int iVar15;
  int iVar16;
  undefined8 local_50;
  undefined8 local_48;
  undefined4 uStack_40;
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  piVar1 = *(int **)(DAT_00130138 + 0x12ffc8);
  _uStack_40 = CONCAT44(*piVar1,in_s14);
  local_50 = in_d5;
  local_48 = in_d6;
  pSVar2 = (String *)
           AbyssEngine::String::String((String *)&local_48,(char *)(DAT_0013013c + 0x12ffd6),false);
  iVar3 = AEFile::FileExist(pSVar2);
  AbyssEngine::String::~String((String *)&local_48);
  if (iVar3 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_00130140 + 0x12fff6),(char *)&local_48,extraout_r2);
    iVar3 = 0;
    iVar14 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    iVar15 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    iVar16 = *(int *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    iVar4 = param_1 + 1;
    if (param_2 == 1) {
      iVar4 = 0x65;
    }
    for (uVar10 = 0; uVar10 < 0x88; uVar10 = uVar10 + 1) {
      AEFile::Read((signed *)((int)&local_50 + 7),(uint)local_48);
      AEFile::ReadSwitched((short *)((int)&local_50 + 4),(uint)local_48);
      AEFile::Read((signed *)((int)&local_50 + 3),(uint)local_48);
      pAVar5 = operator_new(0xc);
      Array<int>::Array();
      ArraySetLength<int>(local_50._3_1_ * 7 + 7,pAVar5);
      piVar6 = *(int **)(pAVar5 + 4);
      iVar12 = 0x1c;
      *piVar6 = (int)local_50._4_2_;
      piVar6[5] = 0x800;
      piVar6[6] = 0;
      piVar6[1] = iVar3;
      piVar6[2] = iVar14;
      piVar6[3] = iVar15;
      piVar6[4] = iVar16;
      for (uVar11 = 7; uVar11 < *(uint *)pAVar5; uVar11 = uVar11 + 7) {
        pcVar9 = *(code **)(DAT_00130144 + 0x130072);
        (*pcVar9)(&local_50,(uint)local_48);
        iVar7 = *(int *)(pAVar5 + 4);
        puVar8 = (undefined4 *)(DAT_00130148 + 0x130084);
        *(int *)(iVar7 + iVar12) = (int)(short)local_50;
        pcVar13 = (code *)*puVar8;
        (*pcVar13)((int *)(iVar7 + iVar12) + 1,(uint)local_48);
        (*pcVar13)(*(int *)(pAVar5 + 4) + iVar12 + 8,(uint)local_48);
        (*pcVar13)(*(int *)(pAVar5 + 4) + iVar12 + 0xc,(uint)local_48);
        (*pcVar9)(&local_50,(uint)local_48);
        *(int *)(*(int *)(pAVar5 + 4) + iVar12 + 0x10) = (int)(short)local_50;
        (*pcVar9)(&local_50,(uint)local_48);
        *(int *)(*(int *)(pAVar5 + 4) + iVar12 + 0x14) = (int)(short)local_50;
        (*pcVar9)(&local_50,(uint)local_48);
        iVar7 = *(int *)(pAVar5 + 4) + iVar12;
        iVar12 = iVar12 + 0x1c;
        *(int *)(iVar7 + 0x18) = (int)(short)local_50;
      }
      if (iVar4 == local_50._7_1_) goto LAB_00130102;
      ArrayRelease<int>(pAVar5);
    }
    AEFile::Close((uint)local_48);
  }
LAB_00130102:
  iVar3 = *piVar1 - local_3c;
  if (iVar3 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar3);
}

```

## target disasm
```
  0012ffb0: push {r4,r5,r6,r7,lr}
  0012ffb2: add r7,sp,#0xc
  0012ffb4: push {r7,r8,r9,r10,r11}
  0012ffb8: vpush {d3,d4,d5,d6,d7,d8,d9}
  0012ffbc: ldr r0,[0x00130138]
  0012ffbe: mov r6,r1
  0012ffc0: mov r8,r2
  0012ffc2: movs r2,#0x0
  0012ffc4: add r0,pc
  0012ffc6: ldr r0,[r0,#0x0]
  0012ffc8: str r0,[sp,#0x4]
  0012ffca: ldr r0,[r0,#0x0]
  0012ffcc: ldr r1,[0x0013013c]
  0012ffce: str r0,[sp,#0x24]
  0012ffd0: add r0,sp,#0x18
  0012ffd2: add r1,pc
  0012ffd4: blx 0x0006ee18
  0012ffd8: blx 0x0006fff4
  0012ffdc: mov r4,r0
  0012ffde: add r0,sp,#0x18
  0012ffe0: blx 0x0006ee30
  0012ffe4: movs r0,#0x0
  0012ffe6: cmp r4,#0x0
  0012ffe8: str r0,[sp,#0x8]
  0012ffea: beq.w 0x00130102
  0012ffee: ldr r0,[0x00130140]
  0012fff0: add r1,sp,#0x18
  0012fff2: add r0,pc
  0012fff4: blx 0x0006ecd4
  0012fff8: vmov.i32 q4,#0x0
  0012fffc: adds r0,r6,#0x1
  0012fffe: add r6,sp,#0x10
  00130000: cmp.w r8,#0x1
  00130004: it eq
  00130006: mov.eq r0,#0x65
  00130008: movs r4,#0x0
  0013000a: mov.w r8,#0x0
  0013000e: str r0,[sp,#0xc]
  00130010: b 0x001300f2
  00130012: sub.w r0,r7,#0x41
  00130016: blx 0x00076324
  0013001a: ldr r1,[sp,#0x18]
  0013001c: add r0,sp,#0x14
  0013001e: blx 0x00076330
  00130022: ldr r1,[sp,#0x18]
  00130024: sub.w r0,r7,#0x45
  00130028: blx 0x00076324
  0013002c: movs r0,#0xc
  0013002e: blx 0x0006eb24
  00130032: mov r4,r0
  00130034: blx 0x000701f8
  00130038: ldrsb.w r0,[r7,#-0x45]
  0013003c: mov r1,r4
  0013003e: rsb r0,r0,r0, lsl #0x3
  00130042: adds r0,#0x7
  00130044: blx 0x00071a4c
  00130048: ldr r0,[r4,#0x4]
  0013004a: mov.w r9,#0x7
  0013004e: ldrsh.w r1,[sp,#0x14]
  00130052: mov.w r10,#0x1c
  00130056: str r1,[r0,#0x0]
  00130058: mov.w r1,#0x800
  0013005c: str r1,[r0,#0x14]
  0013005e: movs r1,#0x0
  00130060: str r1,[r0,#0x18]
  00130062: adds r0,#0x4
  00130064: vst1.32 {d8,d9},[r0]
  00130068: b 0x001300d8
  0013006a: ldr r0,[0x00130144]
  0013006c: ldr r1,[sp,#0x18]
  0013006e: add r0,pc
  00130070: ldr r5,[r0,#0x0]
  00130072: mov r0,r6
  00130074: blx r5
  00130076: ldr r0,[r4,#0x4]
  00130078: ldr r2,[0x00130148]
  0013007a: add r0,r10
  0013007c: ldrsh.w r1,[sp,#0x10]
  00130080: add r2,pc
  00130082: stmia r0!,{r1}
  00130084: ldr r1,[sp,#0x18]
  00130086: ldr.w r11,[r2,#0x0]
  0013008a: blx r11
  0013008c: ldr r0,[r4,#0x4]
  0013008e: ldr r1,[sp,#0x18]
  00130090: add r0,r10
  00130092: adds r0,#0x8
  00130094: blx r11
  00130096: ldr r0,[r4,#0x4]
  00130098: ldr r1,[sp,#0x18]
  0013009a: add r0,r10
  0013009c: adds r0,#0xc
  0013009e: blx r11
  001300a0: ldr r1,[sp,#0x18]
  001300a2: mov r0,r6
  001300a4: blx r5
  001300a6: ldr r0,[r4,#0x4]
  001300a8: ldrsh.w r1,[sp,#0x10]
  001300ac: add r0,r10
  001300ae: str r1,[r0,#0x10]
  001300b0: mov r0,r6
  001300b2: ldr r1,[sp,#0x18]
  001300b4: blx r5
  001300b6: ldr r0,[r4,#0x4]
  001300b8: ldrsh.w r1,[sp,#0x10]
  001300bc: add r0,r10
  001300be: str r1,[r0,#0x14]
  001300c0: mov r0,r6
  001300c2: ldr r1,[sp,#0x18]
  001300c4: blx r5
  001300c6: ldr r0,[r4,#0x4]
  001300c8: add.w r9,r9,#0x7
  001300cc: ldrsh.w r1,[sp,#0x10]
  001300d0: add r0,r10
  001300d2: add.w r10,r10,#0x1c
  001300d6: str r1,[r0,#0x18]
  001300d8: ldr r0,[r4,#0x0]
  001300da: cmp r9,r0
  001300dc: bcc 0x0013006a
  001300de: ldrsb.w r0,[r7,#-0x41]
  001300e2: ldr r1,[sp,#0xc]
  001300e4: cmp r1,r0
  001300e6: beq 0x00130100
  001300e8: mov r0,r4
  001300ea: blx 0x0007369c
  001300ee: add.w r8,r8,#0x1
  001300f2: ldr r1,[sp,#0x18]
  001300f4: cmp.w r8,#0x88
  001300f8: bcc 0x00130012
  001300fa: mov r0,r1
  001300fc: blx 0x0006ece0
  00130100: str r4,[sp,#0x8]
  00130102: ldr r0,[sp,#0x24]
  00130104: ldr r1,[sp,#0x4]
  00130106: ldr r1,[r1,#0x0]
  00130108: subs r0,r1,r0
  0013010a: itttt eq
  0013010c: ldr.eq r0,[sp,#0x8]
  0013010e: vpop.eq {d3,d4,d5,d6,d7,d8,d9}
  00130112: add.eq sp,#0x4
  00130114: pop.eq.w {r8,r9,r10,r11}
  00130118: it eq
  0013011a: pop.eq {r4,r5,r6,r7,pc}
  0013011c: blx 0x0006e824
```
