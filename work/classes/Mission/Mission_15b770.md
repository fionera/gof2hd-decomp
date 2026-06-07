# Mission::Mission
elf 0x15b770 body 220
Sig: undefined __thiscall Mission(Mission * this, int param_1, int param_2, int param_3)

## decompile
```c

/* Mission::Mission(int, int, int) */

void __thiscall Mission::Mission(Mission *this,int param_1,int param_2,int param_3)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_0016b888 + 0x16b784);
  iVar1 = *piVar2;
  *(int *)this = *(int *)(DAT_0016b88c + 0x16b788) + 8;
  AbyssEngine::String::String((String *)(this + 0x10));
  AbyssEngine::String::String((String *)(this + 0x1c));
  AbyssEngine::String::String((String *)(this + 0x40));
  AbyssEngine::String::String((String *)(this + 0x4c));
  *(int *)(this + 0x30) = param_2;
  *(int *)(this + 0x3c) = param_3;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(int *)(this + 0xc) = param_1;
  if (param_3 < 0) {
    AbyssEngine::String::String((String *)&stack0xffffffcc,(char *)(DAT_0016b894 + 0x16b7f2),false);
  }
  else {
    Galaxy::getStation(**(int **)(DAT_0016b890 + 0x16b7da));
    Station::getName();
  }
  AbyssEngine::String::operator=((String *)(this + 0x40),&stack0xffffffcc);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  AbyssEngine::String::String((String *)&stack0xffffffcc,(char *)(DAT_0016b898 + 0x16b80c),false);
  AbyssEngine::String::operator=((String *)(this + 0x4c),&stack0xffffffcc);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  this[0x74] = (Mission)0x1;
  *(undefined4 *)(this + 100) = 1;
  *(undefined2 *)(this + 4) = 0;
  this[0x5c] = (Mission)0x0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x38) = 0;
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
  0016b770: push {r4,r5,r6,r7,lr}
  0016b772: add r7,sp,#0xc
  0016b774: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0016b778: mov r5,r0
  0016b77a: ldr r0,[0x0016b888]
  0016b77c: mov r4,r1
  0016b77e: ldr r1,[0x0016b88c]
  0016b780: add r0,pc
  0016b782: mov r11,r5
  0016b784: add r1,pc
  0016b786: mov r6,r3
  0016b788: ldr.w r9,[r0,#0x0]
  0016b78c: mov r8,r2
  0016b78e: ldr r0,[r1,#0x0]
  0016b790: ldr.w r1,[r9,#0x0]
  0016b794: adds r0,#0x8
  0016b796: str r1,[sp,#0x18]
  0016b798: str.w r0,[r11],#0x1c
  0016b79c: add.w r0,r5,#0x10
  0016b7a0: str r0,[sp,#0x4]
  0016b7a2: blx 0x0006efbc
  0016b7a6: mov r0,r11
  0016b7a8: blx 0x0006efbc
  0016b7ac: add.w r0,r5,#0x40
  0016b7b0: str r0,[sp,#0x8]
  0016b7b2: blx 0x0006efbc
  0016b7b6: add.w r10,r5,#0x4c
  0016b7ba: mov r0,r10
  0016b7bc: blx 0x0006efbc
  0016b7c0: cmp r6,#0x0
  0016b7c2: mov.w r0,#0x0
  0016b7c6: str.w r8,[r5,#0x30]
  0016b7ca: str r6,[r5,#0x3c]
  0016b7cc: str r0,[r5,#0x28]
  0016b7ce: strd r0,r4,[r5,#0x8]
  0016b7d2: blt 0x0016b7ec
  0016b7d4: ldr r0,[0x0016b890]
  0016b7d6: add r0,pc
  0016b7d8: ldr r0,[r0,#0x0]
  0016b7da: ldr r0,[r0,#0x0]
  0016b7dc: mov r1,r6
  0016b7de: blx 0x00071c44
  0016b7e2: mov r1,r0
  0016b7e4: add r0,sp,#0xc
  0016b7e6: blx 0x000736a8
  0016b7ea: b 0x0016b7f8
  0016b7ec: ldr r1,[0x0016b894]
  0016b7ee: add r1,pc
  0016b7f0: add r0,sp,#0xc
  0016b7f2: movs r2,#0x0
  0016b7f4: blx 0x0006ee18
  0016b7f8: ldr r0,[sp,#0x8]
  0016b7fa: add r1,sp,#0xc
  0016b7fc: blx 0x0006f2b0
  0016b800: add r0,sp,#0xc
  0016b802: blx 0x0006ee30
  0016b806: ldr r1,[0x0016b898]
  0016b808: add r1,pc
  0016b80a: add r0,sp,#0xc
  0016b80c: movs r2,#0x0
  0016b80e: blx 0x0006ee18
  0016b812: add r1,sp,#0xc
  0016b814: mov r0,r10
  0016b816: blx 0x0006f2b0
  0016b81a: add r0,sp,#0xc
  0016b81c: blx 0x0006ee30
  0016b820: movs r0,#0x1
  0016b822: strb.w r0,[r5,#0x74]
  0016b826: str r0,[r5,#0x64]
  0016b828: movs r0,#0x0
  0016b82a: strh r0,[r5,#0x4]
  0016b82c: strb.w r0,[r5,#0x5c]
  0016b830: str r0,[r5,#0x70]
  0016b832: str r0,[r5,#0x38]
  0016b834: ldr r0,[sp,#0x18]
  0016b836: ldr.w r1,[r9,#0x0]
  0016b83a: subs r0,r1,r0
  0016b83c: itttt eq
  0016b83e: mov.eq r0,r5
  0016b840: add.eq sp,#0x1c
  0016b842: pop.eq.w {r8,r9,r10,r11}
  0016b846: pop.eq {r4,r5,r6,r7,pc}
  0016b848: blx 0x0006e824
```
