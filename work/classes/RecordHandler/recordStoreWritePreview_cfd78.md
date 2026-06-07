# RecordHandler::recordStoreWritePreview
elf 0xcfd78 body 248
Sig: undefined __thiscall recordStoreWritePreview(RecordHandler * this, int param_1)

## decompile
```c

/* RecordHandler::recordStoreWritePreview(int) */

void __thiscall RecordHandler::recordStoreWritePreview(RecordHandler *this,int param_1)

{
  int iVar1;
  AEFile *this_00;
  undefined4 extraout_r3;
  int *piVar2;
  float fVar3;
  uint in_stack_ffffffc8;
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000dfe98 + 0xdfd8c);
  local_1c = *piVar2;
  AbyssEngine::String::String(aSStack_34,param_1);
  AbyssEngine::operator+(aSStack_28,this + 0x20);
  AbyssEngine::String::~String(aSStack_34);
  iVar1 = AEFile::FileExist(aSStack_28);
  if (iVar1 != 0) {
    AEFile::FileDelete(aSStack_28);
  }
  AEFile::OpenWrite(aSStack_28,(uint *)&stack0xffffffc8);
  this_00 = (AEFile *)Status::getPlayingTime();
  AEFile::Write(this_00,CONCAT44(extraout_r3,in_stack_ffffffc8),in_stack_ffffffc8);
  iVar1 = Status::getCredits();
  AEFile::Write(iVar1,in_stack_ffffffc8);
  Status::getStation();
  Station::getName();
  AEFile::Write(aSStack_34,in_stack_ffffffc8,true);
  AbyssEngine::String::~String(aSStack_34);
  Status::getSystem();
  SolarSystem::getName();
  AEFile::Write(aSStack_34,in_stack_ffffffc8,true);
  AbyssEngine::String::~String(aSStack_34);
  iVar1 = Status::getCurrentCampaignMission();
  AEFile::Write(iVar1,in_stack_ffffffc8);
  iVar1 = Status::getLevel();
  fVar3 = (float)AEFile::Write(iVar1,in_stack_ffffffc8);
  AEFile::Write(fVar3,*(uint *)(*(int *)(DAT_000dfea0 + 0xdfe38) + 0x2c));
  Status::getShip();
  iVar1 = Ship::getIndex();
  AEFile::Write(iVar1,in_stack_ffffffc8);
  AEFile::Close(in_stack_ffffffc8);
  AbyssEngine::String::~String(aSStack_28);
  iVar1 = *piVar2 - local_1c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  000dfd78: push {r4,r5,r6,r7,lr}
  000dfd7a: add r7,sp,#0xc
  000dfd7c: push.w r11
  000dfd80: sub sp,#0x20
  000dfd82: mov r4,r0
  000dfd84: ldr r0,[0x000dfe98]
  000dfd86: add r5,sp,#0x4
  000dfd88: add r0,pc
  000dfd8a: ldr r6,[r0,#0x0]
  000dfd8c: ldr r0,[r6,#0x0]
  000dfd8e: str r0,[sp,#0x1c]
  000dfd90: mov r0,r5
  000dfd92: blx 0x0006f658
  000dfd96: add.w r1,r4,#0x20
  000dfd9a: add r0,sp,#0x10
  000dfd9c: mov r2,r5
  000dfd9e: blx 0x0006ef98
  000dfda2: add r0,sp,#0x4
  000dfda4: blx 0x0006ee30
  000dfda8: add r0,sp,#0x10
  000dfdaa: blx 0x0006fff4
  000dfdae: cbz r0,0x000dfdb6
  000dfdb0: add r0,sp,#0x10
  000dfdb2: blx 0x0007495c
  000dfdb6: add r0,sp,#0x10
  000dfdb8: mov r1,sp
  000dfdba: blx 0x0006f3dc
  000dfdbe: ldr r0,[0x000dfe9c]
  000dfdc0: add r0,pc
  000dfdc2: ldr r4,[r0,#0x0]
  000dfdc4: ldr r0,[r4,#0x0]
  000dfdc6: blx 0x00071710
  000dfdca: ldr r2,[sp,#0x0]
  000dfdcc: blx 0x00074a40
  000dfdd0: ldr r0,[r4,#0x0]
  000dfdd2: blx 0x000733d8
  000dfdd6: ldr r1,[sp,#0x0]
  000dfdd8: blx 0x00074a1c
  000dfddc: ldr r0,[r4,#0x0]
  000dfdde: blx 0x00071c14
  000dfde2: mov r1,r0
  000dfde4: add r0,sp,#0x4
  000dfde6: blx 0x000736a8
  000dfdea: ldr r1,[sp,#0x0]
  000dfdec: add r0,sp,#0x4
  000dfdee: movs r2,#0x1
  000dfdf0: blx 0x00074968
  000dfdf4: add r0,sp,#0x4
  000dfdf6: blx 0x0006ee30
  000dfdfa: ldr r0,[r4,#0x0]
  000dfdfc: blx 0x00071a10
  000dfe00: mov r1,r0
  000dfe02: add r0,sp,#0x4
  000dfe04: blx 0x00071cc8
  000dfe08: ldr r1,[sp,#0x0]
  000dfe0a: add r0,sp,#0x4
  000dfe0c: movs r2,#0x1
  000dfe0e: blx 0x00074968
  000dfe12: add r0,sp,#0x4
  000dfe14: blx 0x0006ee30
  000dfe18: ldr r0,[r4,#0x0]
  000dfe1a: blx 0x00071770
  000dfe1e: ldr r1,[sp,#0x0]
  000dfe20: blx 0x00074a1c
  000dfe24: ldr r0,[r4,#0x0]
  000dfe26: blx 0x00071c5c
  000dfe2a: ldr r1,[sp,#0x0]
  000dfe2c: blx 0x00074a1c
  000dfe30: ldr r0,[0x000dfea0]
  000dfe32: ldr r1,[sp,#0x0]
  000dfe34: add r0,pc
  000dfe36: ldr r0,[r0,#0x0]
  000dfe38: ldr r0,[r0,#0x2c]
  000dfe3a: blx 0x00074af4
  000dfe3e: ldr r0,[r4,#0x0]
  000dfe40: blx 0x00071a58
  000dfe44: blx 0x000719c8
  000dfe48: ldr r1,[sp,#0x0]
  000dfe4a: blx 0x00074a1c
  000dfe4e: ldr r0,[sp,#0x0]
  000dfe50: blx 0x0006ece0
  000dfe54: add r0,sp,#0x10
  000dfe56: blx 0x0006ee30
  000dfe5a: ldr r0,[sp,#0x1c]
  000dfe5c: ldr r1,[r6,#0x0]
  000dfe5e: subs r0,r1,r0
  000dfe60: itttt eq
  000dfe62: mov.eq r0,#0x1
  000dfe64: add.eq sp,#0x20
  000dfe66: pop.eq.w r11
  000dfe6a: pop.eq {r4,r5,r6,r7,pc}
  000dfe6c: blx 0x0006e824
```
