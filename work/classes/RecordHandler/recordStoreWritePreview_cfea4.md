# RecordHandler::recordStoreWritePreview
elf 0xcfea4 body 218
Sig: undefined __thiscall recordStoreWritePreview(RecordHandler * this, GameRecord * param_1, int param_2)

## decompile
```c

/* RecordHandler::recordStoreWritePreview(GameRecord*, int) */

void __thiscall
RecordHandler::recordStoreWritePreview(RecordHandler *this,GameRecord *param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 extraout_r3;
  int *piVar3;
  float fVar4;
  uint in_stack_ffffffc8;
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_000dffa0 + 0xdfeba);
  local_1c = *piVar3;
  if (param_1 == (GameRecord *)0x0) {
    uVar2 = 0;
  }
  else {
    AbyssEngine::String::String(aSStack_34,param_2);
    AbyssEngine::operator+(aSStack_28,this + 0x20);
    AbyssEngine::String::~String(aSStack_34);
    iVar1 = AEFile::FileExist(aSStack_28);
    if (iVar1 != 0) {
      AEFile::FileDelete(aSStack_28);
    }
    AEFile::OpenWrite(aSStack_28,(uint *)&stack0xffffffc8);
    AEFile::Write(*(AEFile **)(param_1 + 0x10),CONCAT44(extraout_r3,in_stack_ffffffc8),
                  in_stack_ffffffc8);
    AEFile::Write(*(int *)(param_1 + 8),in_stack_ffffffc8);
    Station::getName();
    AEFile::Write(aSStack_34,in_stack_ffffffc8,true);
    AbyssEngine::String::~String(aSStack_34);
    AEFile::Write(param_1 + 0x188,in_stack_ffffffc8,true);
    AEFile::Write(*(int *)(param_1 + 0x40),in_stack_ffffffc8);
    fVar4 = (float)AEFile::Write(*(int *)(param_1 + 0x20),in_stack_ffffffc8);
    AEFile::Write(fVar4,*(uint *)(param_1 + 0x11c));
    iVar1 = Ship::getIndex();
    AEFile::Write(iVar1,in_stack_ffffffc8);
    AEFile::Close(in_stack_ffffffc8);
    AbyssEngine::String::~String(aSStack_28);
    uVar2 = 1;
  }
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar2);
  }
  return;
}

```

## target disasm
```
  000dfea4: push {r4,r5,r6,r7,lr}
  000dfea6: add r7,sp,#0xc
  000dfea8: push.w r8
  000dfeac: sub sp,#0x20
  000dfeae: mov r5,r0
  000dfeb0: ldr r0,[0x000dffa0]
  000dfeb2: mov r4,r1
  000dfeb4: cmp r1,#0x0
  000dfeb6: add r0,pc
  000dfeb8: ldr.w r8,[r0,#0x0]
  000dfebc: ldr.w r0,[r8,#0x0]
  000dfec0: str r0,[sp,#0x1c]
  000dfec2: beq 0x000dff66
  000dfec4: add r6,sp,#0x4
  000dfec6: mov r1,r2
  000dfec8: mov r0,r6
  000dfeca: blx 0x0006f658
  000dfece: add.w r1,r5,#0x20
  000dfed2: add r0,sp,#0x10
  000dfed4: mov r2,r6
  000dfed6: blx 0x0006ef98
  000dfeda: add r0,sp,#0x4
  000dfedc: blx 0x0006ee30
  000dfee0: add r0,sp,#0x10
  000dfee2: blx 0x0006fff4
  000dfee6: cbz r0,0x000dfeee
  000dfee8: add r0,sp,#0x10
  000dfeea: blx 0x0007495c
  000dfeee: add r0,sp,#0x10
  000dfef0: mov r1,sp
  000dfef2: blx 0x0006f3dc
  000dfef6: ldrd r0,r1,[r4,#0x10]
  000dfefa: ldr r2,[sp,#0x0]
  000dfefc: blx 0x00074a40
  000dff00: ldr r1,[sp,#0x0]
  000dff02: ldr r0,[r4,#0x8]
  000dff04: blx 0x00074a1c
  000dff08: ldr.w r1,[r4,#0x138]
  000dff0c: add r0,sp,#0x4
  000dff0e: blx 0x000736a8
  000dff12: ldr r1,[sp,#0x0]
  000dff14: add r0,sp,#0x4
  000dff16: movs r2,#0x1
  000dff18: blx 0x00074968
  000dff1c: add r0,sp,#0x4
  000dff1e: blx 0x0006ee30
  000dff22: ldr r1,[sp,#0x0]
  000dff24: add.w r0,r4,#0x188
  000dff28: movs r2,#0x1
  000dff2a: blx 0x00074968
  000dff2e: ldr r1,[sp,#0x0]
  000dff30: ldr r0,[r4,#0x40]
  000dff32: blx 0x00074a1c
  000dff36: ldr r1,[sp,#0x0]
  000dff38: ldr r0,[r4,#0x20]
  000dff3a: blx 0x00074a1c
  000dff3e: ldr r1,[sp,#0x0]
  000dff40: ldr.w r0,[r4,#0x11c]
  000dff44: blx 0x00074af4
  000dff48: ldr.w r0,[r4,#0x130]
  000dff4c: blx 0x000719c8
  000dff50: ldr r1,[sp,#0x0]
  000dff52: blx 0x00074a1c
  000dff56: ldr r0,[sp,#0x0]
  000dff58: blx 0x0006ece0
  000dff5c: add r0,sp,#0x10
  000dff5e: blx 0x0006ee30
  000dff62: movs r0,#0x1
  000dff64: b 0x000dff68
  000dff66: movs r0,#0x0
  000dff68: ldr r1,[sp,#0x1c]
  000dff6a: ldr.w r2,[r8,#0x0]
  000dff6e: subs r1,r2,r1
  000dff70: ittt eq
  000dff72: add.eq sp,#0x20
  000dff74: pop.eq.w r8
  000dff78: pop.eq {r4,r5,r6,r7,pc}
  000dff7a: blx 0x0006e824
```
