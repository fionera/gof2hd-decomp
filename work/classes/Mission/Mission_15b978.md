# Mission::Mission
elf 0x15b978 body 152
Sig: undefined __thiscall Mission(Mission * this, int param_1)

## decompile
```c

/* Mission::Mission(int) */

void __thiscall Mission::Mission(Mission *this,int param_1)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_0016ba48 + 0x16b98c);
  iVar1 = *piVar2;
  *(int *)this = *(int *)(DAT_0016ba4c + 0x16b992) + 8;
  AbyssEngine::String::String((String *)(this + 0x10));
  AbyssEngine::String::String((String *)(this + 0x1c));
  AbyssEngine::String::String((String *)(this + 0x40));
  AbyssEngine::String::String((String *)(this + 0x4c));
  AbyssEngine::String::String((String *)&stack0xffffffcc,(char *)(DAT_0016ba50 + 0x16b9c6),false);
  AbyssEngine::String::operator=((String *)(this + 0x10),&stack0xffffffcc);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  *(undefined4 *)(this + 100) = 0;
  this[0x74] = (Mission)0x0;
  *(undefined4 *)(this + 8) = 0;
  *(int *)(this + 0xc) = param_1;
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
  0016b978: push {r4,r5,r6,r7,lr}
  0016b97a: add r7,sp,#0xc
  0016b97c: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0016b980: mov r6,r0
  0016b982: ldr r0,[0x0016ba48]
  0016b984: mov r8,r1
  0016b986: ldr r1,[0x0016ba4c]
  0016b988: add r0,pc
  0016b98a: add.w r10,r6,#0x10
  0016b98e: add r1,pc
  0016b990: mov r5,r6
  0016b992: ldr r4,[r0,#0x0]
  0016b994: ldr r0,[r1,#0x0]
  0016b996: ldr r1,[r4,#0x0]
  0016b998: adds r0,#0x8
  0016b99a: str r1,[sp,#0x10]
  0016b99c: str.w r0,[r5],#0x1c
  0016b9a0: mov r0,r10
  0016b9a2: blx 0x0006efbc
  0016b9a6: mov r0,r5
  0016b9a8: blx 0x0006efbc
  0016b9ac: add.w r9,r6,#0x40
  0016b9b0: mov r0,r9
  0016b9b2: blx 0x0006efbc
  0016b9b6: add.w r11,r6,#0x4c
  0016b9ba: mov r0,r11
  0016b9bc: blx 0x0006efbc
  0016b9c0: ldr r1,[0x0016ba50]
  0016b9c2: add r1,pc
  0016b9c4: add r0,sp,#0x4
  0016b9c6: movs r2,#0x0
  0016b9c8: blx 0x0006ee18
  0016b9cc: add r1,sp,#0x4
  0016b9ce: mov r0,r10
  0016b9d0: blx 0x0006f2b0
  0016b9d4: add r0,sp,#0x4
  0016b9d6: blx 0x0006ee30
  0016b9da: movs r0,#0x0
  0016b9dc: str r0,[r6,#0x64]
  0016b9de: strb.w r0,[r6,#0x74]
  0016b9e2: strd r0,r8,[r6,#0x8]
  0016b9e6: strd r0,r0,[r6,#0x28]
  0016b9ea: str r0,[r6,#0x30]
  0016b9ec: str r0,[r6,#0x58]
  0016b9ee: strb.w r0,[r6,#0x5c]
  0016b9f2: str r0,[r6,#0x70]
  0016b9f4: strh r0,[r6,#0x4]
  0016b9f6: strd r0,r0,[r6,#0x38]
  0016b9fa: ldr r0,[sp,#0x10]
  0016b9fc: ldr r1,[r4,#0x0]
  0016b9fe: subs r0,r1,r0
  0016ba00: itttt eq
  0016ba02: mov.eq r0,r6
  0016ba04: add.eq sp,#0x14
  0016ba06: pop.eq.w {r8,r9,r10,r11}
  0016ba0a: pop.eq {r4,r5,r6,r7,pc}
  0016ba0c: blx 0x0006e824
```
