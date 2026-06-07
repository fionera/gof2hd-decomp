# RecordHandler::addHash
elf 0xcdea0 body 254
Sig: undefined __thiscall addHash(RecordHandler * this, int param_1)

## decompile
```c

/* RecordHandler::addHash(int) */

void __thiscall RecordHandler::addHash(RecordHandler *this,int param_1)

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
  
  piVar5 = *(int **)(DAT_000ddfa0 + 0xddeb6);
  local_28 = *piVar5;
  local_4c = (signed *)0x0;
  iVar2 = readRecordAsByteArray(this,&local_4c,param_1,false);
  if (-1 < iVar2) {
    md = operator_new__(0x20);
    c = operator_new(0x70);
    SHA256_Init(c);
    psVar1 = local_4c;
    pcVar6 = *(code **)(DAT_000ddfa4 + 0xddef0);
    (*pcVar6)(c,local_4c,iVar2);
    local_48 = *(undefined8 *)(DAT_000ddfa8 + 0xddf02);
    uStack_37 = *(undefined8 *)(DAT_000ddfa8 + 0xddf13);
    uStack_40 = (undefined1)*(undefined8 *)(DAT_000ddfa8 + 0xddf0a);
    local_3f = (undefined7)*(undefined8 *)(DAT_000ddfa8 + 0xddf0b);
    uStack_38 = (undefined1)((ulonglong)*(undefined8 *)(DAT_000ddfa8 + 0xddf0b) >> 0x38);
    (*pcVar6)(c,&local_48,0x19);
    (*pcVar6)(c,**(undefined4 **)(DAT_000ddfac + 0xddf20),0x10);
    SHA256_Final(md,c);
    psVar3 = operator_new__(iVar2 + 0x20U);
    __aeabi_memcpy(psVar3,psVar1,iVar2);
    uVar7 = *(undefined8 *)(md + 8);
    puVar4 = (undefined8 *)(psVar3 + iVar2);
    uVar8 = *(undefined8 *)(md + 0x10);
    uVar9 = *(undefined8 *)(md + 0x18);
    *puVar4 = *(undefined8 *)md;
    puVar4[1] = uVar7;
    puVar4[2] = uVar8;
    puVar4[3] = uVar9;
    writeByteArrayAsRecord(this,psVar3,iVar2 + 0x20U,param_1,false);
    pcVar6 = *(code **)(DAT_000ddfb0 + 0xddf72);
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
  000ddea0: push {r4,r5,r6,r7,lr}
  000ddea2: add r7,sp,#0xc
  000ddea4: push {r8,r9,r10,r11}
  000ddea8: sub sp,#0x3c
  000ddeaa: mov r10,r0
  000ddeac: ldr r0,[0x000ddfa0]
  000ddeae: mov r6,r1
  000ddeb0: add r1,sp,#0x14
  000ddeb2: add r0,pc
  000ddeb4: mov r2,r6
  000ddeb6: movs r3,#0x0
  000ddeb8: ldr r4,[r0,#0x0]
  000ddeba: ldr r0,[r4,#0x0]
  000ddebc: str r0,[sp,#0x38]
  000ddebe: movs r0,#0x0
  000ddec0: str r0,[sp,#0x14]
  000ddec2: mov r0,r10
  000ddec4: blx 0x000749a4
  000ddec8: mov r5,r0
  000ddeca: cmp r0,#0x0
  000ddecc: blt 0x000ddf8a
  000ddece: movs r0,#0x20
  000dded0: blx 0x0006ec08
  000dded4: mov r9,r0
  000dded6: movs r0,#0x70
  000dded8: blx 0x0006eb24
  000ddedc: str r4,[sp,#0x10]
  000ddede: mov r4,r0
  000ddee0: blx 0x00074944
  000ddee4: ldr r0,[0x000ddfa4]
  000ddee6: mov r2,r5
  000ddee8: ldr.w r11,[sp,#0x14]
  000ddeec: add r0,pc
  000ddeee: str r6,[sp,#0xc]
  000ddef0: ldr r6,[r0,#0x0]
  000ddef2: mov r0,r4
  000ddef4: mov r1,r11
  000ddef6: blx r6
  000ddef8: ldr r0,[0x000ddfa8]
  000ddefa: movs r2,#0x9
  000ddefc: add r1,sp,#0x18
  000ddefe: add r0,pc
  000ddf00: vld1.64 {d16,d17},[r0],r2
  000ddf04: vld1.8 {d18,d19},[r0]
  000ddf08: mov r0,r1
  000ddf0a: vst1.64 {d16,d17},[r0],r2
  000ddf0e: movs r2,#0x19
  000ddf10: vst1.8 {d18,d19},[r0]
  000ddf14: mov r0,r4
  000ddf16: blx r6
  000ddf18: ldr r0,[0x000ddfac]
  000ddf1a: movs r2,#0x10
  000ddf1c: add r0,pc
  000ddf1e: ldr r0,[r0,#0x0]
  000ddf20: ldr r1,[r0,#0x0]
  000ddf22: mov r0,r4
  000ddf24: blx r6
  000ddf26: mov r0,r9
  000ddf28: mov r1,r4
  000ddf2a: str.w r9,[sp,#0x8]
  000ddf2e: blx 0x00074950
  000ddf32: add.w r8,r5,#0x20
  000ddf36: mov r0,r8
  000ddf38: blx 0x0006ec08
  000ddf3c: mov r1,r11
  000ddf3e: mov r2,r5
  000ddf40: mov r6,r0
  000ddf42: blx 0x0006f1e4
  000ddf46: vld1.8 {d16,d17},[r9]!
  000ddf4a: adds r0,r6,r5
  000ddf4c: mov r1,r6
  000ddf4e: mov r2,r8
  000ddf50: vld1.8 {d18,d19},[r9]
  000ddf54: mov.w r9,#0x0
  000ddf58: vst1.8 {d16,d17},[r0]!
  000ddf5c: ldr r3,[sp,#0xc]
  000ddf5e: vst1.8 {d18,d19},[r0]
  000ddf62: mov r0,r10
  000ddf64: str.w r9,[sp,#0x0]
  000ddf68: blx 0x000749b0
  000ddf6c: ldr r0,[0x000ddfb0]
  000ddf6e: add r0,pc
  000ddf70: ldr r5,[r0,#0x0]
  000ddf72: mov r0,r11
  000ddf74: blx r5
  000ddf76: mov r0,r6
  000ddf78: str.w r9,[sp,#0x14]
  000ddf7c: blx r5
  000ddf7e: ldr r0,[sp,#0x8]
  000ddf80: blx r5
  000ddf82: mov r0,r4
  000ddf84: ldr r4,[sp,#0x10]
  000ddf86: blx 0x0006eb48
  000ddf8a: ldr r0,[sp,#0x38]
  000ddf8c: ldr r1,[r4,#0x0]
  000ddf8e: subs r0,r1,r0
  000ddf90: ittt eq
  000ddf92: add.eq sp,#0x3c
  000ddf94: pop.eq.w {r8,r9,r10,r11}
  000ddf98: pop.eq {r4,r5,r6,r7,pc}
  000ddf9a: blx 0x0006e824
```
