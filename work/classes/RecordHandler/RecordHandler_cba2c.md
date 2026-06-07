# RecordHandler::RecordHandler
elf 0xcba2c body 150
Sig: undefined __thiscall RecordHandler(RecordHandler * this)

## decompile
```c

/* RecordHandler::RecordHandler() */

void __thiscall RecordHandler::RecordHandler(RecordHandler *this)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_000dbaf8 + 0xdba40);
  iVar1 = *piVar2;
  AbyssEngine::String::String((String *)(this + 8));
  AbyssEngine::String::String((String *)(this + 0x14));
  AbyssEngine::String::String((String *)(this + 0x20));
  AbyssEngine::String::String((String *)&stack0xffffffd0,(char *)(DAT_000dbafc + 0xdba64),false);
  AbyssEngine::String::operator=((String *)(this + 8),(String *)&stack0xffffffd0);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  AbyssEngine::String::String((String *)&stack0xffffffd0,(char *)(DAT_000dbb00 + 0xdba7e),false);
  AbyssEngine::String::operator=((String *)(this + 0x14),(String *)&stack0xffffffd0);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  AbyssEngine::String::String((String *)&stack0xffffffd0,(char *)(DAT_000dbb04 + 0xdba98),false);
  AbyssEngine::String::operator=((String *)(this + 0x20),(String *)&stack0xffffffd0);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  iVar1 = *piVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  000dba2c: push {r4,r5,r6,r7,lr}
  000dba2e: add r7,sp,#0xc
  000dba30: push {r4,r5,r6,r7,r8,r9,r11}
  000dba34: mov r4,r0
  000dba36: ldr r0,[0x000dbaf8]
  000dba38: add.w r8,r4,#0x8
  000dba3c: add r0,pc
  000dba3e: ldr r5,[r0,#0x0]
  000dba40: ldr r0,[r5,#0x0]
  000dba42: str r0,[sp,#0xc]
  000dba44: mov r0,r8
  000dba46: blx 0x0006efbc
  000dba4a: add.w r9,r4,#0x14
  000dba4e: mov r0,r9
  000dba50: blx 0x0006efbc
  000dba54: add.w r6,r4,#0x20
  000dba58: mov r0,r6
  000dba5a: blx 0x0006efbc
  000dba5e: ldr r1,[0x000dbafc]
  000dba60: add r1,pc
  000dba62: mov r0,sp
  000dba64: movs r2,#0x0
  000dba66: blx 0x0006ee18
  000dba6a: mov r1,sp
  000dba6c: mov r0,r8
  000dba6e: blx 0x0006f2b0
  000dba72: mov r0,sp
  000dba74: blx 0x0006ee30
  000dba78: ldr r1,[0x000dbb00]
  000dba7a: add r1,pc
  000dba7c: mov r0,sp
  000dba7e: movs r2,#0x0
  000dba80: blx 0x0006ee18
  000dba84: mov r1,sp
  000dba86: mov r0,r9
  000dba88: blx 0x0006f2b0
  000dba8c: mov r0,sp
  000dba8e: blx 0x0006ee30
  000dba92: ldr r1,[0x000dbb04]
  000dba94: add r1,pc
  000dba96: mov r0,sp
  000dba98: movs r2,#0x0
  000dba9a: blx 0x0006ee18
  000dba9e: mov r1,sp
  000dbaa0: mov r0,r6
  000dbaa2: blx 0x0006f2b0
  000dbaa6: mov r0,sp
  000dbaa8: blx 0x0006ee30
  000dbaac: ldr r0,[sp,#0xc]
  000dbaae: ldr r1,[r5,#0x0]
  000dbab0: subs r0,r1,r0
  000dbab2: itttt eq
  000dbab4: mov.eq r0,r4
  000dbab6: add.eq sp,#0x10
  000dbab8: pop.eq.w {r8,r9,r11}
  000dbabc: pop.eq {r4,r5,r6,r7,pc}
  000dbabe: blx 0x0006e824
```
