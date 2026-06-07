# RecordHandler::convertSDVersionSaves
elf 0xcffa8 body 488
Sig: undefined __thiscall convertSDVersionSaves(RecordHandler * this)

## decompile
```c

/* RecordHandler::convertSDVersionSaves() */

void __thiscall RecordHandler::convertSDVersionSaves(RecordHandler *this)

{
  Array<signed_char*> *this_00;
  Array<signed_char*> *this_01;
  void *pvVar1;
  void *pvVar2;
  undefined4 uVar3;
  String *this_02;
  int iVar4;
  signed *psVar5;
  int *piVar6;
  uint *puVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar6 = *(int **)(DAT_000e01b8 + 0xdffba);
  local_28 = *piVar6;
  this_00 = operator_new(0xc);
  Array<signed_char*>::Array(this_00);
  puVar7 = *(uint **)(DAT_000e01bc + 0xdffd6);
  ArraySetLength<signed_char*>(*puVar7,(Array *)this_00);
  this_01 = operator_new(0xc);
  Array<signed_char*>::Array(this_01);
  ArraySetLength<signed_char*>(*puVar7,(Array *)this_01);
  uVar11 = *puVar7;
  uVar8 = (uint)((ulonglong)uVar11 * 4);
  if ((int)((ulonglong)uVar11 * 4 >> 0x20) != 0) {
    uVar8 = 0xffffffff;
  }
  pvVar1 = operator_new__(uVar8);
  pvVar2 = operator_new__(uVar8);
  for (iVar10 = 0; iVar10 < (int)uVar11; iVar10 = iVar10 + 1) {
    uVar3 = readRecordAsByteArray(this,(signed **)(*(int *)(this_00 + 4) + iVar10 * 4),iVar10,false)
    ;
    *(undefined4 *)((int)pvVar1 + iVar10 * 4) = uVar3;
    uVar3 = readRecordAsByteArray(this,(signed **)(*(int *)(this_01 + 4) + iVar10 * 4),iVar10,true);
    *(undefined4 *)((int)pvVar2 + iVar10 * 4) = uVar3;
    AbyssEngine::String::String(aSStack_40,iVar10);
    AbyssEngine::operator+(aSStack_34,this + 0x14);
    AEFile::FileDelete(aSStack_34);
    AbyssEngine::String::~String(aSStack_34);
    this_02 = (String *)AbyssEngine::String::~String(aSStack_40);
    AbyssEngine::String::String(this_02,iVar10);
    AbyssEngine::operator+(aSStack_34,this + 0x20);
    AEFile::FileDelete(aSStack_34);
    AbyssEngine::String::~String(aSStack_34);
    AbyssEngine::String::~String(aSStack_40);
    uVar11 = *puVar7;
  }
  iVar10 = 0;
  do {
    iVar4 = uVar11 - 1;
    iVar9 = iVar10;
    while( true ) {
      if (iVar4 <= iVar9) {
        psVar5 = *(signed **)(*(int *)(this_00 + 4) + iVar4 * 4);
        if (psVar5 != (signed *)0x0) {
          writeByteArrayAsRecord(this,psVar5,*(int *)((int)pvVar1 + iVar4 * 4),0,false);
          addHash(this,0);
          iVar10 = *puVar7 * 4 + -4;
          writeByteArrayAsRecord
                    (this,*(signed **)(*(int *)(this_01 + 4) + iVar10),
                     *(int *)((int)pvVar2 + iVar10),0,true);
        }
        ArrayReleaseArrays<signed_char*>((Array *)this_00);
        ArrayReleaseArrays<signed_char*>((Array *)this_01);
        operator_delete__(pvVar1);
        operator_delete__(pvVar2);
        if (*piVar6 == local_28) {
          return;
        }
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      psVar5 = *(signed **)(*(int *)(this_00 + 4) + iVar9 * 4);
      if (psVar5 != (signed *)0x0) break;
      iVar9 = iVar9 + 1;
    }
    iVar10 = iVar9 + 1;
    writeByteArrayAsRecord(this,psVar5,*(int *)((int)pvVar1 + iVar9 * 4),iVar10,false);
    addHash(this,iVar10);
    writeByteArrayAsRecord
              (this,*(signed **)(*(int *)(this_01 + 4) + iVar9 * 4),
               *(int *)((int)pvVar2 + iVar9 * 4),iVar10,true);
    uVar11 = *puVar7;
  } while( true );
}

```

## target disasm
```
  000dffa8: push {r4,r5,r6,r7,lr}
  000dffaa: add r7,sp,#0xc
  000dffac: push {r8,r9,r10,r11}
  000dffb0: sub sp,#0x3c
  000dffb2: mov r5,r0
  000dffb4: ldr r0,[0x000e01b8]
  000dffb6: add r0,pc
  000dffb8: ldr r4,[r0,#0x0]
  000dffba: ldr r0,[r4,#0x0]
  000dffbc: str r0,[sp,#0x38]
  000dffbe: movs r0,#0xc
  000dffc0: blx 0x0006eb24
  000dffc4: mov r6,r0
  000dffc6: blx 0x00074c5c
  000dffca: str r4,[sp,#0x8]
  000dffcc: mov r1,r6
  000dffce: ldr r0,[0x000e01bc]
  000dffd0: str r6,[sp,#0x10]
  000dffd2: add r0,pc
  000dffd4: ldr r4,[r0,#0x0]
  000dffd6: ldr r0,[r4,#0x0]
  000dffd8: blx 0x00074c68
  000dffdc: movs r0,#0xc
  000dffde: blx 0x0006eb24
  000dffe2: mov r6,r0
  000dffe4: blx 0x00074c5c
  000dffe8: ldr r0,[r4,#0x0]
  000dffea: mov r1,r6
  000dffec: str r6,[sp,#0x1c]
  000dffee: blx 0x00074c68
  000dfff2: ldr.w r11,[r4,#0x0]
  000dfff6: movs r0,#0x4
  000dfff8: str r4,[sp,#0xc]
  000dfffa: umull r4,r0,r11,r0
  000dfffe: cmp r0,#0x0
  000e0000: it ne
  000e0002: mov.ne r0,#0x1
  000e0004: cmp r0,#0x0
  000e0006: it ne
  000e0008: mov.ne.w r4,#0xffffffff
  000e000c: mov r0,r4
  000e000e: blx 0x0006ec08
  000e0012: str r0,[sp,#0x18]
  000e0014: mov r0,r4
  000e0016: blx 0x0006ec08
  000e001a: add.w r4,r5,#0x20
  000e001e: add.w r6,r5,#0x14
  000e0022: add.w r9,sp,#0x20
  000e0026: add.w r10,sp,#0x2c
  000e002a: mov.w r8,#0x0
  000e002e: str r0,[sp,#0x14]
  000e0030: b 0x000e00b2
  000e0032: ldr r0,[sp,#0x10]
  000e0034: mov r2,r8
  000e0036: movs r3,#0x0
  000e0038: ldr r0,[r0,#0x4]
  000e003a: add.w r1,r0,r8, lsl #0x2
  000e003e: mov r0,r5
  000e0040: blx 0x000749a4
  000e0044: ldr r1,[sp,#0x18]
  000e0046: mov r2,r8
  000e0048: movs r3,#0x1
  000e004a: str.w r0,[r1,r8,lsl #0x2]
  000e004e: ldr r0,[sp,#0x1c]
  000e0050: ldr r0,[r0,#0x4]
  000e0052: add.w r1,r0,r8, lsl #0x2
  000e0056: mov r0,r5
  000e0058: blx 0x000749a4
  000e005c: ldr r1,[sp,#0x14]
  000e005e: str.w r0,[r1,r8,lsl #0x2]
  000e0062: mov r0,r9
  000e0064: mov r1,r8
  000e0066: blx 0x0006f658
  000e006a: mov r0,r10
  000e006c: mov r1,r6
  000e006e: mov r2,r9
  000e0070: blx 0x0006ef98
  000e0074: mov r0,r10
  000e0076: blx 0x0007495c
  000e007a: mov r0,r10
  000e007c: blx 0x0006ee30
  000e0080: mov r0,r9
  000e0082: blx 0x0006ee30
  000e0086: mov r1,r8
  000e0088: blx 0x0006f658
  000e008c: mov r0,r10
  000e008e: mov r1,r4
  000e0090: mov r2,r9
  000e0092: blx 0x0006ef98
  000e0096: mov r0,r10
  000e0098: blx 0x0007495c
  000e009c: mov r0,r10
  000e009e: blx 0x0006ee30
  000e00a2: mov r0,r9
  000e00a4: blx 0x0006ee30
  000e00a8: ldr r0,[sp,#0xc]
  000e00aa: add.w r8,r8,#0x1
  000e00ae: ldr.w r11,[r0,#0x0]
  000e00b2: cmp r8,r11
  000e00b4: blt 0x000e0032
  000e00b6: mov.w r8,#0x0
  000e00ba: mov.w r9,#0x1
  000e00be: movs r4,#0x0
  000e00c0: ldr.w r10,[sp,#0x10]
  000e00c4: sub.w r0,r11,#0x1
  000e00c8: ldr.w r2,[r10,#0x4]
  000e00cc: ldr.w r11,[sp,#0x1c]
  000e00d0: b 0x000e00d4
  000e00d2: adds r4,#0x1
  000e00d4: cmp r4,r0
  000e00d6: bge 0x000e0120
  000e00d8: ldr.w r1,[r2,r4,lsl #0x2]
  000e00dc: cmp r1,#0x0
  000e00de: beq 0x000e00d2
  000e00e0: ldr r0,[sp,#0x18]
  000e00e2: adds r6,r4,#0x1
  000e00e4: str.w r8,[sp,#0x0]
  000e00e8: mov r3,r6
  000e00ea: ldr.w r2,[r0,r4,lsl #0x2]
  000e00ee: mov r0,r5
  000e00f0: blx 0x000749b0
  000e00f4: mov r0,r5
  000e00f6: mov r1,r6
  000e00f8: blx 0x00074b30
  000e00fc: ldr r1,[sp,#0x14]
  000e00fe: mov r3,r6
  000e0100: ldr.w r0,[r11,#0x4]
  000e0104: ldr.w r2,[r1,r4,lsl #0x2]
  000e0108: ldr.w r1,[r0,r4,lsl #0x2]
  000e010c: mov r0,r5
  000e010e: str.w r9,[sp,#0x0]
  000e0112: blx 0x000749b0
  000e0116: ldr r0,[sp,#0xc]
  000e0118: mov r4,r6
  000e011a: ldr.w r11,[r0,#0x0]
  000e011e: b 0x000e00c4
  000e0120: ldr.w r1,[r2,r0,lsl #0x2]
  000e0124: ldrd r6,r4,[sp,#0x14]
  000e0128: cbz r1,0x000e0162
  000e012a: ldr.w r2,[r4,r0,lsl #0x2]
  000e012e: movs r0,#0x0
  000e0130: str r0,[sp,#0x0]
  000e0132: mov r0,r5
  000e0134: movs r3,#0x0
  000e0136: blx 0x000749b0
  000e013a: mov r0,r5
  000e013c: movs r1,#0x0
  000e013e: blx 0x00074b30
  000e0142: ldr r0,[sp,#0xc]
  000e0144: mvn r1,#0x3
  000e0148: ldr.w r3,[r11,#0x4]
  000e014c: ldr r0,[r0,#0x0]
  000e014e: add.w r0,r1,r0, lsl #0x2
  000e0152: ldr r1,[r3,r0]
  000e0154: movs r3,#0x0
  000e0156: ldr r2,[r6,r0]
  000e0158: movs r0,#0x1
  000e015a: str r0,[sp,#0x0]
  000e015c: mov r0,r5
  000e015e: blx 0x000749b0
  000e0162: mov r0,r10
  000e0164: blx 0x00074c74
  000e0168: mov r0,r11
  000e016a: blx 0x00074c74
  000e016e: mov r0,r4
  000e0170: blx 0x0006ebfc
  000e0174: mov r0,r6
  000e0176: blx 0x0006ebfc
  000e017a: ldr r0,[sp,#0x38]
  000e017c: ldr r1,[sp,#0x8]
  000e017e: ldr r1,[r1,#0x0]
  000e0180: subs r0,r1,r0
  000e0182: ittt eq
  000e0184: add.eq sp,#0x3c
  000e0186: pop.eq.w {r8,r9,r10,r11}
  000e018a: pop.eq {r4,r5,r6,r7,pc}
  000e018c: blx 0x0006e824
```
