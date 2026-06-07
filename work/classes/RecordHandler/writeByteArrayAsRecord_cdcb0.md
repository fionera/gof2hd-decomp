# RecordHandler::writeByteArrayAsRecord
elf 0xcdcb0 body 128
Sig: undefined __thiscall writeByteArrayAsRecord(RecordHandler * this, signed * param_1, int param_2, int param_3, bool param_4)

## decompile
```c

/* RecordHandler::writeByteArrayAsRecord(signed char*, int, int, bool) */

void __thiscall
RecordHandler::writeByteArrayAsRecord
          (RecordHandler *this,signed *param_1,int param_2,int param_3,bool param_4)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined3 in_stack_00000001;
  signed *local_3c;
  int iStack_38;
  int iStack_34;
  
  piVar3 = *(int **)(DAT_000ddd48 + 0xddcc4);
  iVar1 = *piVar3;
  local_3c = param_1;
  iStack_38 = param_2;
  iStack_34 = param_3;
  AbyssEngine::String::String((String *)&local_3c,param_3);
  iVar2 = 0x14;
  if (_param_4 != 0) {
    iVar2 = 0x20;
  }
  AbyssEngine::operator+(&stack0xffffffd0,this + iVar2);
  AbyssEngine::String::~String((String *)&local_3c);
  iVar2 = AEFile::FileExist(&stack0xffffffd0);
  if (iVar2 != 0) {
    AEFile::FileDelete(&stack0xffffffd0);
  }
  AEFile::OpenWrite(&stack0xffffffd0,(uint *)&local_3c);
  AEFile::Write(param_2,param_1,(uint)local_3c);
  AEFile::Close((uint)local_3c);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  iVar1 = *piVar3 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  000ddcb0: push {r4,r5,r6,r7,lr}
  000ddcb2: add r7,sp,#0xc
  000ddcb4: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  000ddcb8: mov r6,r0
  000ddcba: ldr r0,[0x000ddd48]
  000ddcbc: add r5,sp,#0x4
  000ddcbe: mov r8,r1
  000ddcc0: add r0,pc
  000ddcc2: mov r1,r3
  000ddcc4: mov r9,r2
  000ddcc6: ldr r4,[r0,#0x0]
  000ddcc8: ldr r0,[r4,#0x0]
  000ddcca: str r0,[sp,#0x1c]
  000ddccc: mov r0,r5
  000ddcce: blx 0x0006f658
  000ddcd2: ldr r0,[r7,#0x8]
  000ddcd4: add.w r1,r6,#0x14
  000ddcd8: cmp r0,#0x0
  000ddcda: it ne
  000ddcdc: add.ne.w r1,r6,#0x20
  000ddce0: add r0,sp,#0x10
  000ddce2: mov r2,r5
  000ddce4: blx 0x0006ef98
  000ddce8: add r0,sp,#0x4
  000ddcea: blx 0x0006ee30
  000ddcee: add r0,sp,#0x10
  000ddcf0: blx 0x0006fff4
  000ddcf4: cbz r0,0x000ddcfc
  000ddcf6: add r0,sp,#0x10
  000ddcf8: blx 0x0007495c
  000ddcfc: add r0,sp,#0x10
  000ddcfe: add r1,sp,#0x4
  000ddd00: blx 0x0006f3dc
  000ddd04: ldr r2,[sp,#0x4]
  000ddd06: mov r0,r9
  000ddd08: mov r1,r8
  000ddd0a: blx 0x0006f40c
  000ddd0e: ldr r0,[sp,#0x4]
  000ddd10: blx 0x0006ece0
  000ddd14: add r0,sp,#0x10
  000ddd16: blx 0x0006ee30
  000ddd1a: ldr r0,[sp,#0x1c]
  000ddd1c: ldr r1,[r4,#0x0]
  000ddd1e: subs r0,r1,r0
  000ddd20: itttt eq
  000ddd22: mov.eq r0,r9
  000ddd24: add.eq sp,#0x20
  000ddd26: pop.eq.w {r8,r9,r11}
  000ddd2a: pop.eq {r4,r5,r6,r7,pc}
  000ddd2c: blx 0x0006e824
```
