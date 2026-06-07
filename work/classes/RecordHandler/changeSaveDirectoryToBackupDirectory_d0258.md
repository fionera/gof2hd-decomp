# RecordHandler::changeSaveDirectoryToBackupDirectory
elf 0xd0258 body 282
Sig: undefined __thiscall changeSaveDirectoryToBackupDirectory(RecordHandler * this)

## decompile
```c

/* RecordHandler::changeSaveDirectoryToBackupDirectory() */

void __thiscall RecordHandler::changeSaveDirectoryToBackupDirectory(RecordHandler *this)

{
  Array<signed_char*> *this_00;
  Array<signed_char*> *this_01;
  void *pvVar1;
  void *pvVar2;
  undefined4 uVar3;
  signed *psVar4;
  undefined4 unaff_r5;
  int unaff_r6;
  uint uVar5;
  int iVar6;
  uint *puVar7;
  uint uVar8;
  
  this_00 = operator_new(0xc);
  Array<signed_char*>::Array(this_00);
  puVar7 = *(uint **)(DAT_000e0388 + 0xe0276);
  ArraySetLength<signed_char*>(*puVar7,(Array *)this_00);
  this_01 = operator_new(0xc);
  Array<signed_char*>::Array(this_01);
  ArraySetLength<signed_char*>(*puVar7,(Array *)this_01);
  uVar8 = *puVar7;
  uVar5 = (uint)((ulonglong)uVar8 * 4);
  if ((int)((ulonglong)uVar8 * 4 >> 0x20) != 0) {
    uVar5 = 0xffffffff;
  }
  pvVar1 = operator_new__(uVar5);
  pvVar2 = operator_new__(uVar5);
  for (iVar6 = 0; iVar6 < (int)uVar8; iVar6 = iVar6 + 1) {
    uVar3 = readRecordAsByteArray(this,(signed **)(*(int *)(this_00 + 4) + iVar6 * 4),iVar6,false);
    *(undefined4 *)((int)pvVar1 + iVar6 * 4) = uVar3;
    uVar3 = readRecordAsByteArray(this,(signed **)(*(int *)(this_01 + 4) + iVar6 * 4),iVar6,true);
    *(undefined4 *)((int)pvVar2 + iVar6 * 4) = uVar3;
    uVar8 = *puVar7;
  }
  **(undefined1 **)(DAT_000e038c + 0xe0306) = 1;
  iVar6 = 0;
  while (iVar6 < (int)uVar8) {
    psVar4 = *(signed **)(*(int *)(this_00 + 4) + iVar6 * 4);
    if (psVar4 == (signed *)0x0) {
      iVar6 = iVar6 + 1;
    }
    else {
      writeByteArrayAsRecord(this,psVar4,*(int *)((int)pvVar1 + iVar6 * 4),iVar6,false);
      unaff_r6 = iVar6 + 1;
      addHash(this,unaff_r6);
      unaff_r5 = 1;
      writeByteArrayAsRecord
                (this,*(signed **)(*(int *)(this_01 + 4) + iVar6 * 4),
                 *(int *)((int)pvVar2 + iVar6 * 4),iVar6,true);
      uVar8 = *puVar7;
      iVar6 = unaff_r6;
    }
  }
  ArrayReleaseArrays<signed_char*>((Array *)this_00);
  ArrayReleaseArrays<signed_char*>((Array *)this_01);
  operator_delete__(pvVar1);
  (*(code *)(DAT_001ab114 + 0x1ab118))(pvVar2,unaff_r5,unaff_r6,this_01);
  return;
}

```

## target disasm
```
  000e0258: push {r4,r5,r6,r7,lr}
  000e025a: add r7,sp,#0xc
  000e025c: push {r5,r6,r7,r8,r9,r10,r11}
  000e0260: mov r5,r0
  000e0262: movs r0,#0xc
  000e0264: blx 0x0006eb24
  000e0268: mov r4,r0
  000e026a: blx 0x00074c5c
  000e026e: ldr r0,[0x000e0388]
  000e0270: mov r1,r4
  000e0272: add r0,pc
  000e0274: ldr.w r9,[r0,#0x0]
  000e0278: ldr.w r0,[r9,#0x0]
  000e027c: blx 0x00074c68
  000e0280: movs r0,#0xc
  000e0282: blx 0x0006eb24
  000e0286: mov r6,r0
  000e0288: blx 0x00074c5c
  000e028c: ldr.w r0,[r9,#0x0]
  000e0290: mov r1,r6
  000e0292: str r6,[sp,#0x8]
  000e0294: blx 0x00074c68
  000e0298: ldr.w r11,[r9,#0x0]
  000e029c: movs r0,#0x4
  000e029e: mov r8,r9
  000e02a0: umull r6,r0,r11,r0
  000e02a4: cmp r0,#0x0
  000e02a6: it ne
  000e02a8: mov.ne r0,#0x1
  000e02aa: cmp r0,#0x0
  000e02ac: it ne
  000e02ae: mov.ne.w r6,#0xffffffff
  000e02b2: mov r0,r6
  000e02b4: blx 0x0006ec08
  000e02b8: mov r9,r0
  000e02ba: mov r0,r6
  000e02bc: blx 0x0006ec08
  000e02c0: mov r10,r0
  000e02c2: movs r6,#0x0
  000e02c4: b 0x000e02f6
  000e02c6: ldr r0,[r4,#0x4]
  000e02c8: mov r2,r6
  000e02ca: movs r3,#0x0
  000e02cc: add.w r1,r0,r6, lsl #0x2
  000e02d0: mov r0,r5
  000e02d2: blx 0x000749a4
  000e02d6: str.w r0,[r9,r6,lsl #0x2]
  000e02da: mov r2,r6
  000e02dc: movs r3,#0x1
  000e02de: ldr r0,[sp,#0x8]
  000e02e0: ldr r0,[r0,#0x4]
  000e02e2: add.w r1,r0,r6, lsl #0x2
  000e02e6: mov r0,r5
  000e02e8: blx 0x000749a4
  000e02ec: str.w r0,[r10,r6,lsl #0x2]
  000e02f0: adds r6,#0x1
  000e02f2: ldr.w r11,[r8,#0x0]
  000e02f6: cmp r6,r11
  000e02f8: blt 0x000e02c6
  000e02fa: ldr r0,[0x000e038c]
  000e02fc: movs r1,#0x1
  000e02fe: movs r3,#0x0
  000e0300: movs r6,#0x0
  000e0302: add r0,pc
  000e0304: ldr r0,[r0,#0x0]
  000e0306: strb r1,[r0,#0x0]
  000e0308: b 0x000e034e
  000e030a: ldr r0,[r4,#0x4]
  000e030c: ldr.w r1,[r0,r6,lsl #0x2]
  000e0310: cbnz r1,0x000e0316
  000e0312: adds r6,#0x1
  000e0314: b 0x000e034e
  000e0316: ldr.w r2,[r9,r6,lsl #0x2]
  000e031a: mov r0,r5
  000e031c: str r3,[sp,#0x0]
  000e031e: mov r3,r6
  000e0320: blx 0x000749b0
  000e0324: adds r1,r6,#0x1
  000e0326: mov r0,r5
  000e0328: str r1,[sp,#0x4]
  000e032a: blx 0x00074b30
  000e032e: ldr r0,[sp,#0x8]
  000e0330: mov r3,r6
  000e0332: ldr.w r2,[r10,r6,lsl #0x2]
  000e0336: ldr r0,[r0,#0x4]
  000e0338: ldr.w r1,[r0,r6,lsl #0x2]
  000e033c: movs r0,#0x1
  000e033e: str r0,[sp,#0x0]
  000e0340: mov r0,r5
  000e0342: blx 0x000749b0
  000e0346: ldr.w r11,[r8,#0x0]
  000e034a: movs r3,#0x0
  000e034c: ldr r6,[sp,#0x4]
  000e034e: cmp r6,r11
  000e0350: blt 0x000e030a
  000e0352: mov r0,r4
  000e0354: blx 0x00074c74
  000e0358: ldr r0,[sp,#0x8]
  000e035a: blx 0x00074c74
  000e035e: mov r0,r9
  000e0360: blx 0x0006ebfc
  000e0364: mov r0,r10
  000e0366: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000e036a: pop.w {r4,r5,r6,r7,lr}
  000e036e: b.w 0x001ab108
```
