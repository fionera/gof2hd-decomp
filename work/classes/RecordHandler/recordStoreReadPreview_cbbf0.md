# RecordHandler::recordStoreReadPreview
elf 0xcbbf0 body 200
Sig: undefined __stdcall recordStoreReadPreview(int param_1)

## decompile
```c

/* RecordHandler::recordStoreReadPreview(int) */

void RecordHandler::recordStoreReadPreview(int param_1)

{
  int iVar1;
  GameRecord *this;
  int in_r1;
  int *piVar2;
  uint local_34 [3];
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000dbcd8 + 0xdbc04);
  local_1c = *piVar2;
  AbyssEngine::String::String((String *)local_34,in_r1);
  AbyssEngine::operator+(aSStack_28,(String *)(param_1 + 0x20));
  AbyssEngine::String::~String((String *)local_34);
  iVar1 = AEFile::FileExist(aSStack_28);
  if (iVar1 != 0) {
    AEFile::OpenRead(aSStack_28,local_34);
    this = operator_new(0x1c8);
    GameRecord::GameRecord(this);
    AEFile::Read((longlong *)(this + 0x10),local_34[0]);
    AEFile::Read((int *)(this + 8),local_34[0]);
    AEFile::Read(this + 0x194,local_34[0],true);
    AEFile::Read(this + 0x188,local_34[0],true);
    AEFile::Read((int *)(this + 0x40),local_34[0]);
    AEFile::Read((int *)(this + 0x20),local_34[0]);
    AEFile::Read((float *)(this + 0x11c),local_34[0]);
    AEFile::Read((int *)(this + 0x1a0),local_34[0]);
    AEFile::Close(local_34[0]);
  }
  AbyssEngine::String::~String(aSStack_28);
  iVar1 = *piVar2 - local_1c;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  000dbbf0: push {r4,r5,r6,r7,lr}
  000dbbf2: add r7,sp,#0xc
  000dbbf4: push.w r11
  000dbbf8: sub sp,#0x20
  000dbbfa: mov r4,r0
  000dbbfc: ldr r0,[0x000dbcd8]
  000dbbfe: add r5,sp,#0x4
  000dbc00: add r0,pc
  000dbc02: ldr r6,[r0,#0x0]
  000dbc04: ldr r0,[r6,#0x0]
  000dbc06: str r0,[sp,#0x1c]
  000dbc08: mov r0,r5
  000dbc0a: blx 0x0006f658
  000dbc0e: add.w r1,r4,#0x20
  000dbc12: add r0,sp,#0x10
  000dbc14: mov r2,r5
  000dbc16: blx 0x0006ef98
  000dbc1a: add r0,sp,#0x4
  000dbc1c: blx 0x0006ee30
  000dbc20: add r0,sp,#0x10
  000dbc22: blx 0x0006fff4
  000dbc26: cbz r0,0x000dbc9a
  000dbc28: add r0,sp,#0x10
  000dbc2a: add r1,sp,#0x4
  000dbc2c: blx 0x0006f34c
  000dbc30: mov.w r0,#0x1c8
  000dbc34: blx 0x0006eb24
  000dbc38: mov r4,r0
  000dbc3a: blx 0x00074800
  000dbc3e: ldr r1,[sp,#0x4]
  000dbc40: add.w r0,r4,#0x10
  000dbc44: blx 0x0007480c
  000dbc48: ldr r1,[sp,#0x4]
  000dbc4a: add.w r0,r4,#0x8
  000dbc4e: blx 0x00074818
  000dbc52: ldr r1,[sp,#0x4]
  000dbc54: add.w r0,r4,#0x194
  000dbc58: movs r2,#0x1
  000dbc5a: blx 0x00074824
  000dbc5e: ldr r1,[sp,#0x4]
  000dbc60: add.w r0,r4,#0x188
  000dbc64: movs r2,#0x1
  000dbc66: blx 0x00074824
  000dbc6a: ldr r1,[sp,#0x4]
  000dbc6c: add.w r0,r4,#0x40
  000dbc70: blx 0x00074818
  000dbc74: ldr r1,[sp,#0x4]
  000dbc76: add.w r0,r4,#0x20
  000dbc7a: blx 0x00074818
  000dbc7e: ldr r1,[sp,#0x4]
  000dbc80: add.w r0,r4,#0x11c
  000dbc84: blx 0x00074830
  000dbc88: ldr r1,[sp,#0x4]
  000dbc8a: add.w r0,r4,#0x1a0
  000dbc8e: blx 0x00074818
  000dbc92: ldr r0,[sp,#0x4]
  000dbc94: blx 0x0006ece0
  000dbc98: b 0x000dbc9c
  000dbc9a: movs r4,#0x0
  000dbc9c: add r0,sp,#0x10
  000dbc9e: blx 0x0006ee30
  000dbca2: ldr r0,[sp,#0x1c]
  000dbca4: ldr r1,[r6,#0x0]
  000dbca6: subs r0,r1,r0
  000dbca8: itttt eq
  000dbcaa: mov.eq r0,r4
  000dbcac: add.eq sp,#0x20
  000dbcae: pop.eq.w r11
  000dbcb2: pop.eq {r4,r5,r6,r7,pc}
  000dbcb4: blx 0x0006e824
```
