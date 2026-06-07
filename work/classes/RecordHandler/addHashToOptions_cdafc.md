# RecordHandler::addHashToOptions
elf 0xcdafc body 234
Sig: undefined __thiscall addHashToOptions(RecordHandler * this)

## decompile
```c

/* RecordHandler::addHashToOptions() */

void __thiscall RecordHandler::addHashToOptions(RecordHandler *this)

{
  signed *psVar1;
  int iVar2;
  uchar *md;
  SHA256_CTX *c;
  signed *psVar3;
  undefined8 *puVar4;
  int *piVar5;
  code *pcVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  signed *local_4c;
  undefined8 local_48;
  undefined1 uStack_40;
  undefined7 local_3f;
  undefined1 uStack_38;
  undefined8 uStack_37;
  int local_28;
  
  piVar5 = *(int **)(DAT_000ddbe8 + 0xddb10);
  local_28 = *piVar5;
  local_4c = (signed *)0x0;
  iVar2 = readOptionsFileAsByteArray(this,&local_4c);
  if (-1 < iVar2) {
    md = operator_new__(0x20);
    c = operator_new(0x70);
    SHA256_Init(c);
    psVar1 = local_4c;
    pcVar6 = *(code **)(DAT_000ddbec + 0xddb44);
    (*pcVar6)(c,local_4c,iVar2);
    local_48 = *(undefined8 *)(DAT_000ddbf0 + 0xddb56);
    uStack_37 = *(undefined8 *)(DAT_000ddbf0 + 0xddb67);
    uStack_40 = (undefined1)*(undefined8 *)(DAT_000ddbf0 + 0xddb5e);
    local_3f = (undefined7)*(undefined8 *)(DAT_000ddbf0 + 0xddb5f);
    uStack_38 = (undefined1)((ulonglong)*(undefined8 *)(DAT_000ddbf0 + 0xddb5f) >> 0x38);
    (*pcVar6)(c,&local_48,0x19);
    (*pcVar6)(c,**(undefined4 **)(DAT_000ddbf4 + 0xddb74),0x10);
    SHA256_Final(md,c);
    psVar3 = operator_new__(iVar2 + 0x20U);
    __aeabi_memcpy(psVar3,psVar1,iVar2);
    uVar7 = *(undefined8 *)(md + 8);
    uVar8 = *(undefined8 *)(md + 0x10);
    uVar9 = *(undefined8 *)(md + 0x18);
    puVar4 = (undefined8 *)(psVar3 + iVar2);
    *puVar4 = *(undefined8 *)md;
    puVar4[1] = uVar7;
    puVar4[2] = uVar8;
    puVar4[3] = uVar9;
    writeByteArrayAsOptionsFile(this,psVar3,iVar2 + 0x20U);
    pcVar6 = *(code **)(DAT_000ddbf8 + 0xddbbc);
    (*pcVar6)(psVar1);
    local_4c = (signed *)0x0;
    (*pcVar6)(psVar3);
    (*pcVar6)(md);
    operator_delete(c);
  }
  if (*piVar5 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ddafc: push {r4,r5,r6,r7,lr}
  000ddafe: add r7,sp,#0xc
  000ddb00: push {r8,r9,r10,r11}
  000ddb04: sub sp,#0x2c
  000ddb06: mov r8,r0
  000ddb08: ldr r0,[0x000ddbe8]
  000ddb0a: add r1,sp,#0x4
  000ddb0c: add r0,pc
  000ddb0e: ldr r5,[r0,#0x0]
  000ddb10: ldr r0,[r5,#0x0]
  000ddb12: str r0,[sp,#0x28]
  000ddb14: movs r0,#0x0
  000ddb16: str r0,[sp,#0x4]
  000ddb18: mov r0,r8
  000ddb1a: blx 0x0007498c
  000ddb1e: mov r4,r0
  000ddb20: cmp r0,#0x0
  000ddb22: blt 0x000ddbd2
  000ddb24: movs r0,#0x20
  000ddb26: blx 0x0006ec08
  000ddb2a: mov r9,r0
  000ddb2c: movs r0,#0x70
  000ddb2e: blx 0x0006eb24
  000ddb32: mov r11,r0
  000ddb34: blx 0x00074944
  000ddb38: ldr r0,[0x000ddbec]
  000ddb3a: mov r2,r4
  000ddb3c: ldr.w r10,[sp,#0x4]
  000ddb40: add r0,pc
  000ddb42: str r5,[sp,#0x0]
  000ddb44: ldr r5,[r0,#0x0]
  000ddb46: mov r0,r11
  000ddb48: mov r1,r10
  000ddb4a: blx r5
  000ddb4c: ldr r0,[0x000ddbf0]
  000ddb4e: movs r2,#0x9
  000ddb50: add r1,sp,#0x8
  000ddb52: add r0,pc
  000ddb54: vld1.64 {d16,d17},[r0],r2
  000ddb58: vld1.8 {d18,d19},[r0]
  000ddb5c: mov r0,r1
  000ddb5e: vst1.64 {d16,d17},[r0],r2
  000ddb62: movs r2,#0x19
  000ddb64: vst1.8 {d18,d19},[r0]
  000ddb68: mov r0,r11
  000ddb6a: blx r5
  000ddb6c: ldr r0,[0x000ddbf4]
  000ddb6e: movs r2,#0x10
  000ddb70: add r0,pc
  000ddb72: ldr r0,[r0,#0x0]
  000ddb74: ldr r1,[r0,#0x0]
  000ddb76: mov r0,r11
  000ddb78: blx r5
  000ddb7a: mov r0,r9
  000ddb7c: mov r1,r11
  000ddb7e: blx 0x00074950
  000ddb82: add.w r5,r4,#0x20
  000ddb86: mov r0,r5
  000ddb88: blx 0x0006ec08
  000ddb8c: mov r1,r10
  000ddb8e: mov r2,r4
  000ddb90: mov r6,r0
  000ddb92: blx 0x0006f1e4
  000ddb96: mov r0,r9
  000ddb98: mov r2,r5
  000ddb9a: vld1.8 {d16,d17},[r0]!
  000ddb9e: mov r1,r6
  000ddba0: vld1.8 {d18,d19},[r0]
  000ddba4: adds r0,r6,r4
  000ddba6: vst1.8 {d16,d17},[r0]!
  000ddbaa: vst1.8 {d18,d19},[r0]
  000ddbae: mov r0,r8
  000ddbb0: ldr r5,[sp,#0x0]
  000ddbb2: blx 0x00074998
  000ddbb6: ldr r0,[0x000ddbf8]
  000ddbb8: add r0,pc
  000ddbba: ldr r4,[r0,#0x0]
  000ddbbc: mov r0,r10
  000ddbbe: blx r4
  000ddbc0: movs r0,#0x0
  000ddbc2: str r0,[sp,#0x4]
  000ddbc4: mov r0,r6
  000ddbc6: blx r4
  000ddbc8: mov r0,r9
  000ddbca: blx r4
  000ddbcc: mov r0,r11
  000ddbce: blx 0x0006eb48
  000ddbd2: ldr r0,[sp,#0x28]
  000ddbd4: ldr r1,[r5,#0x0]
  000ddbd6: subs r0,r1,r0
  000ddbd8: ittt eq
  000ddbda: add.eq sp,#0x2c
  000ddbdc: pop.eq.w {r8,r9,r10,r11}
  000ddbe0: pop.eq {r4,r5,r6,r7,pc}
  000ddbe2: blx 0x0006e824
```
