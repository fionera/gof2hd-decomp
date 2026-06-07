# Mission::Mission
elf 0x15b89c body 154
Sig: undefined __thiscall Mission(Mission * this)

## decompile
```c

/* Mission::Mission() */

void __thiscall Mission::Mission(Mission *this)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_0016b96c + 0x16b8b2);
  iVar1 = *piVar2;
  *(int *)this = *(int *)(DAT_0016b970 + 0x16b8b6) + 8;
  AbyssEngine::String::String((String *)(this + 0x10));
  AbyssEngine::String::String((String *)(this + 0x1c));
  AbyssEngine::String::String((String *)(this + 0x40));
  AbyssEngine::String::String((String *)(this + 0x4c));
  AbyssEngine::String::String((String *)&stack0xffffffd0,(char *)(DAT_0016b974 + 0x16b8e8),false);
  AbyssEngine::String::operator=((String *)(this + 0x10),(String *)&stack0xffffffd0);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  *(undefined4 *)(this + 100) = 0;
  this[0x74] = (Mission)0x0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0xffffffff;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  this[0x5c] = (Mission)0x0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined2 *)(this + 4) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
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
  0016b89c: push {r4,r5,r6,r7,lr}
  0016b89e: add r7,sp,#0xc
  0016b8a0: push {r4,r5,r6,r7,r8,r9,r10}
  0016b8a4: mov r6,r0
  0016b8a6: ldr r0,[0x0016b96c]
  0016b8a8: ldr r1,[0x0016b970]
  0016b8aa: add.w r9,r6,#0x10
  0016b8ae: add r0,pc
  0016b8b0: mov r5,r6
  0016b8b2: add r1,pc
  0016b8b4: ldr r4,[r0,#0x0]
  0016b8b6: ldr r0,[r1,#0x0]
  0016b8b8: ldr r1,[r4,#0x0]
  0016b8ba: adds r0,#0x8
  0016b8bc: str r1,[sp,#0xc]
  0016b8be: str.w r0,[r5],#0x1c
  0016b8c2: mov r0,r9
  0016b8c4: blx 0x0006efbc
  0016b8c8: mov r0,r5
  0016b8ca: blx 0x0006efbc
  0016b8ce: add.w r8,r6,#0x40
  0016b8d2: mov r0,r8
  0016b8d4: blx 0x0006efbc
  0016b8d8: add.w r10,r6,#0x4c
  0016b8dc: mov r0,r10
  0016b8de: blx 0x0006efbc
  0016b8e2: ldr r1,[0x0016b974]
  0016b8e4: add r1,pc
  0016b8e6: mov r0,sp
  0016b8e8: movs r2,#0x0
  0016b8ea: blx 0x0006ee18
  0016b8ee: mov r1,sp
  0016b8f0: mov r0,r9
  0016b8f2: blx 0x0006f2b0
  0016b8f6: mov r0,sp
  0016b8f8: blx 0x0006ee30
  0016b8fc: movs r0,#0x0
  0016b8fe: mov.w r1,#0xffffffff
  0016b902: str r0,[r6,#0x64]
  0016b904: strb.w r0,[r6,#0x74]
  0016b908: strd r0,r1,[r6,#0x8]
  0016b90c: strd r0,r0,[r6,#0x28]
  0016b910: str r0,[r6,#0x30]
  0016b912: str r0,[r6,#0x58]
  0016b914: strb.w r0,[r6,#0x5c]
  0016b918: str r0,[r6,#0x70]
  0016b91a: strh r0,[r6,#0x4]
  0016b91c: strd r0,r0,[r6,#0x38]
  0016b920: ldr r0,[sp,#0xc]
  0016b922: ldr r1,[r4,#0x0]
  0016b924: subs r0,r1,r0
  0016b926: itttt eq
  0016b928: mov.eq r0,r6
  0016b92a: add.eq sp,#0x10
  0016b92c: pop.eq.w {r8,r9,r10}
  0016b930: pop.eq {r4,r5,r6,r7,pc}
  0016b932: blx 0x0006e824
```
