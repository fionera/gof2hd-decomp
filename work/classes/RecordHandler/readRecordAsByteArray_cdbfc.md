# RecordHandler::readRecordAsByteArray
elf 0xcdbfc body 152
Sig: undefined __thiscall readRecordAsByteArray(RecordHandler * this, signed * * param_1, int param_2, bool param_3)

## decompile
```c

/* RecordHandler::readRecordAsByteArray(signed char**, int, bool) */

void __thiscall
RecordHandler::readRecordAsByteArray(RecordHandler *this,signed **param_1,int param_2,bool param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  signed *psVar4;
  int *piVar5;
  signed **local_3c;
  int iStack_38;
  uint uStack_34;
  
  piVar5 = *(int **)(DAT_000ddcac + 0xddc10);
  iVar1 = *piVar5;
  local_3c = param_1;
  iStack_38 = param_2;
  uStack_34 = (uint)param_3;
  AbyssEngine::String::String((String *)&local_3c,param_2);
  iVar2 = 0x14;
  if (param_3 != 0) {
    iVar2 = 0x20;
  }
  AbyssEngine::operator+(&stack0xffffffd0,this + iVar2);
  AbyssEngine::String::~String((String *)&local_3c);
  iVar2 = AEFile::FileExist(&stack0xffffffd0);
  if (iVar2 != 0) {
    AEFile::OpenRead(&stack0xffffffd0,(uint *)&local_3c);
    uVar3 = AEFile::GetFileSize((uint)local_3c);
    psVar4 = operator_new__(uVar3 | (int)uVar3 >> 0x1f);
    *param_1 = psVar4;
    AEFile::Read(uVar3,psVar4,(uint)local_3c);
    AEFile::Close((uint)local_3c);
  }
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  iVar1 = *piVar5 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  000ddbfc: push {r4,r5,r6,r7,lr}
  000ddbfe: add r7,sp,#0xc
  000ddc00: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  000ddc04: mov r6,r0
  000ddc06: ldr r0,[0x000ddcac]
  000ddc08: add r4,sp,#0x4
  000ddc0a: mov r8,r1
  000ddc0c: add r0,pc
  000ddc0e: mov r1,r2
  000ddc10: mov r5,r3
  000ddc12: ldr.w r9,[r0,#0x0]
  000ddc16: ldr.w r0,[r9,#0x0]
  000ddc1a: str r0,[sp,#0x1c]
  000ddc1c: mov r0,r4
  000ddc1e: blx 0x0006f658
  000ddc22: add.w r1,r6,#0x14
  000ddc26: cmp r5,#0x0
  000ddc28: it ne
  000ddc2a: add.ne.w r1,r6,#0x20
  000ddc2e: add r0,sp,#0x10
  000ddc30: mov r2,r4
  000ddc32: blx 0x0006ef98
  000ddc36: add r0,sp,#0x4
  000ddc38: blx 0x0006ee30
  000ddc3c: add r0,sp,#0x10
  000ddc3e: blx 0x0006fff4
  000ddc42: cbz r0,0x000ddc72
  000ddc44: add r0,sp,#0x10
  000ddc46: add r1,sp,#0x4
  000ddc48: blx 0x0006f34c
  000ddc4c: ldr r0,[sp,#0x4]
  000ddc4e: blx 0x00071044
  000ddc52: mov r5,r0
  000ddc54: orr.w r0,r0,r0, asr #0x1f
  000ddc58: blx 0x0006ec08
  000ddc5c: mov r1,r0
  000ddc5e: str.w r1,[r8,#0x0]
  000ddc62: ldr r2,[sp,#0x4]
  000ddc64: mov r0,r5
  000ddc66: blx 0x0006eb90
  000ddc6a: ldr r0,[sp,#0x4]
  000ddc6c: blx 0x0006ece0
  000ddc70: b 0x000ddc76
  000ddc72: mov.w r5,#0xffffffff
  000ddc76: add r0,sp,#0x10
  000ddc78: blx 0x0006ee30
  000ddc7c: ldr r0,[sp,#0x1c]
  000ddc7e: ldr.w r1,[r9,#0x0]
  000ddc82: subs r0,r1,r0
  000ddc84: itttt eq
  000ddc86: mov.eq r0,r5
  000ddc88: add.eq sp,#0x20
  000ddc8a: pop.eq.w {r8,r9,r11}
  000ddc8e: pop.eq {r4,r5,r6,r7,pc}
  000ddc90: blx 0x0006e824
```
