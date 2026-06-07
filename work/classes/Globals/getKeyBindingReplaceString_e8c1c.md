# Globals::getKeyBindingReplaceString
elf 0xe8c1c body 56
Sig: undefined __stdcall getKeyBindingReplaceString(int param_1)

## decompile
```c

/* Globals::getKeyBindingReplaceString(int) */

undefined8 Globals::getKeyBindingReplaceString(int param_1)

{
  String *this;
  String *pSVar1;
  undefined4 in_r1;
  int *piVar2;
  int iStack_20;
  undefined4 uStack_1c;
  int local_14;
  
  piVar2 = *(int **)(DAT_000f8c64 + 0xf8c28);
  local_14 = *piVar2;
  iStack_20 = param_1;
  uStack_1c = in_r1;
  this = (String *)AbyssEngine::String::String((String *)&iStack_20);
  pSVar1 = (String *)AbyssEngine::String::ToUpperCase(this);
  AbyssEngine::String::String((String *)param_1,pSVar1,false);
  AbyssEngine::String::~String((String *)&iStack_20);
  if (*piVar2 == local_14) {
    return CONCAT44(uStack_1c,iStack_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000f8c1c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000f8c1e: add r7,sp,#0x18
  000f8c20: mov r4,r0
  000f8c22: ldr r0,[0x000f8c64]
  000f8c24: add r0,pc
  000f8c26: ldr r5,[r0,#0x0]
  000f8c28: ldr r0,[r5,#0x0]
  000f8c2a: str r0,[sp,#0xc]
  000f8c2c: mov r0,sp
  000f8c2e: blx 0x0006efbc
  000f8c32: blx 0x00075e50
  000f8c36: mov r1,r0
  000f8c38: mov r0,r4
  000f8c3a: movs r2,#0x0
  000f8c3c: blx 0x0006f028
  000f8c40: mov r0,sp
  000f8c42: blx 0x0006ee30
  000f8c46: ldr r0,[sp,#0xc]
  000f8c48: ldr r1,[r5,#0x0]
  000f8c4a: subs r0,r1,r0
  000f8c4c: it eq
  000f8c4e: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  000f8c50: blx 0x0006e824
```
