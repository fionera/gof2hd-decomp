# RecordHandler::writeByteArrayAsOptionsFile
elf 0xcddd8 body 100
Sig: undefined __thiscall writeByteArrayAsOptionsFile(RecordHandler * this, signed * param_1, int param_2)

## decompile
```c

/* RecordHandler::writeByteArrayAsOptionsFile(signed char*, int) */

void __thiscall
RecordHandler::writeByteArrayAsOptionsFile(RecordHandler *this,signed *param_1,int param_2)

{
  String *pSVar1;
  int iVar2;
  int *piVar3;
  uint local_2c;
  String aSStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_000dde4c + 0xdddee);
  local_1c = *piVar3;
  pSVar1 = (String *)AbyssEngine::String::String(aSStack_28,this + 8,false);
  iVar2 = AEFile::FileExist(pSVar1);
  if (iVar2 != 0) {
    AEFile::FileDelete(aSStack_28);
  }
  AEFile::OpenWrite(aSStack_28,&local_2c);
  AEFile::Write(param_2,param_1,local_2c);
  AEFile::Close(local_2c);
  AbyssEngine::String::~String(aSStack_28);
  iVar2 = *piVar3 - local_1c;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  000dddd8: push {r4,r5,r6,r7,lr}
  000dddda: add r7,sp,#0xc
  000ddddc: push.w r11
  000ddde0: sub sp,#0x18
  000ddde2: mov r5,r1
  000ddde4: ldr r1,[0x000dde4c]
  000ddde6: mov r4,r2
  000ddde8: movs r2,#0x0
  000dddea: add r1,pc
  000dddec: ldr r6,[r1,#0x0]
  000dddee: ldr r1,[r6,#0x0]
  000dddf0: str r1,[sp,#0x14]
  000dddf2: add.w r1,r0,#0x8
  000dddf6: add r0,sp,#0x8
  000dddf8: blx 0x0006f028
  000dddfc: blx 0x0006fff4
  000dde00: cbz r0,0x000dde08
  000dde02: add r0,sp,#0x8
  000dde04: blx 0x0007495c
  000dde08: add r0,sp,#0x8
  000dde0a: add r1,sp,#0x4
  000dde0c: blx 0x0006f3dc
  000dde10: ldr r2,[sp,#0x4]
  000dde12: mov r0,r4
  000dde14: mov r1,r5
  000dde16: blx 0x0006f40c
  000dde1a: ldr r0,[sp,#0x4]
  000dde1c: blx 0x0006ece0
  000dde20: add r0,sp,#0x8
  000dde22: blx 0x0006ee30
  000dde26: ldr r0,[sp,#0x14]
  000dde28: ldr r1,[r6,#0x0]
  000dde2a: subs r0,r1,r0
  000dde2c: itttt eq
  000dde2e: mov.eq r0,r4
  000dde30: add.eq sp,#0x18
  000dde32: pop.eq.w r11
  000dde36: pop.eq {r4,r5,r6,r7,pc}
  000dde38: blx 0x0006e824
```
