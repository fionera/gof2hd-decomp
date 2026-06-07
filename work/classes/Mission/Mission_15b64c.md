# Mission::Mission
elf 0x15b64c body 214
Sig: undefined __thiscall Mission(Mission * this, int param_1, String param_2, int * param_3, int param_4, int param_5, int param_6, int param_7)

## decompile
```c

/* Mission::Mission(int, AbyssEngine::String, int*, int, int, int, int) */

void __thiscall
Mission::Mission(Mission *this,undefined4 param_1,String *param_3,undefined4 param_4,
                undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_0016b760 + 0x16b660);
  iVar3 = *piVar5;
  *(int *)this = *(int *)(DAT_0016b764 + 0x16b664) + 8;
  AbyssEngine::String::String((String *)(this + 0x10));
  AbyssEngine::String::String((String *)(this + 0x1c));
  AbyssEngine::String::String((String *)(this + 0x40));
  AbyssEngine::String::String((String *)(this + 0x4c));
  *(undefined4 *)(this + 0xc) = param_1;
  AbyssEngine::String::operator=((String *)(this + 0x10),param_3);
  puVar4 = (undefined4 *)(DAT_0016b768 + 0x16b6b4);
  *(undefined4 *)(this + 0x28) = param_4;
  *(undefined4 *)(this + 0x2c) = param_5;
  *(undefined4 *)(this + 0x30) = param_6;
  piVar2 = (int *)*puVar4;
  *(undefined4 *)(this + 0x3c) = param_7;
  Galaxy::getStation(*piVar2);
  Station::getName();
  AbyssEngine::String::operator=((String *)(this + 0x40),&stack0xffffffcc);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  iVar1 = DAT_0016b76c;
  *(undefined4 *)(this + 0x58) = param_8;
  AbyssEngine::String::String((String *)&stack0xffffffcc,(char *)(iVar1 + 0x16b6e2),false);
  AbyssEngine::String::operator=((String *)(this + 0x4c),&stack0xffffffcc);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  this[0x74] = (Mission)0x1;
  *(undefined4 *)(this + 100) = 0;
  this[0x5c] = (Mission)0x0;
  *(undefined2 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  iVar3 = *piVar5 - iVar3;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  0016b64c: push {r4,r5,r6,r7,lr}
  0016b64e: add r7,sp,#0xc
  0016b650: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0016b654: mov r5,r0
  0016b656: ldr r0,[0x0016b760]
  0016b658: mov r8,r1
  0016b65a: ldr r1,[0x0016b764]
  0016b65c: add r0,pc
  0016b65e: mov r11,r5
  0016b660: add r1,pc
  0016b662: mov r6,r3
  0016b664: ldr.w r9,[r0,#0x0]
  0016b668: mov r4,r2
  0016b66a: ldr r0,[r1,#0x0]
  0016b66c: ldr.w r1,[r9,#0x0]
  0016b670: adds r0,#0x8
  0016b672: str r1,[sp,#0x18]
  0016b674: str.w r0,[r11],#0x1c
  0016b678: add.w r0,r5,#0x10
  0016b67c: str r0,[sp,#0x8]
  0016b67e: blx 0x0006efbc
  0016b682: mov r0,r11
  0016b684: blx 0x0006efbc
  0016b688: add.w r0,r5,#0x40
  0016b68c: str r0,[sp,#0x4]
  0016b68e: blx 0x0006efbc
  0016b692: add.w r10,r5,#0x4c
  0016b696: mov r0,r10
  0016b698: blx 0x0006efbc
  0016b69c: str.w r8,[r5,#0xc]
  0016b6a0: ldr r0,[sp,#0x8]
  0016b6a2: mov r1,r4
  0016b6a4: blx 0x0006f2b0
  0016b6a8: ldrd r2,r0,[r7,#0x8]
  0016b6ac: ldr r3,[0x0016b768]
  0016b6ae: ldr r1,[r7,#0x10]
  0016b6b0: add r3,pc
  0016b6b2: strd r6,r2,[r5,#0x28]
  0016b6b6: str r0,[r5,#0x30]
  0016b6b8: ldr r0,[r3,#0x0]
  0016b6ba: str r1,[r5,#0x3c]
  0016b6bc: ldr r0,[r0,#0x0]
  0016b6be: blx 0x00071c44
  0016b6c2: mov r1,r0
  0016b6c4: add r0,sp,#0xc
  0016b6c6: blx 0x000736a8
  0016b6ca: ldr r0,[sp,#0x4]
  0016b6cc: add r1,sp,#0xc
  0016b6ce: blx 0x0006f2b0
  0016b6d2: add r0,sp,#0xc
  0016b6d4: ldr r4,[r7,#0x14]
  0016b6d6: blx 0x0006ee30
  0016b6da: ldr r1,[0x0016b76c]
  0016b6dc: str r4,[r5,#0x58]
  0016b6de: add r1,pc
  0016b6e0: add r0,sp,#0xc
  0016b6e2: movs r2,#0x0
  0016b6e4: blx 0x0006ee18
  0016b6e8: add r1,sp,#0xc
  0016b6ea: mov r0,r10
  0016b6ec: blx 0x0006f2b0
  0016b6f0: add r0,sp,#0xc
  0016b6f2: blx 0x0006ee30
  0016b6f6: movs r0,#0x1
  0016b6f8: strb.w r0,[r5,#0x74]
  0016b6fc: movs r0,#0x0
  0016b6fe: str r0,[r5,#0x64]
  0016b700: strb.w r0,[r5,#0x5c]
  0016b704: strh r0,[r5,#0x4]
  0016b706: str r0,[r5,#0x8]
  0016b708: str r0,[r5,#0x38]
  0016b70a: ldr r0,[sp,#0x18]
  0016b70c: ldr.w r1,[r9,#0x0]
  0016b710: subs r0,r1,r0
  0016b712: itttt eq
  0016b714: mov.eq r0,r5
  0016b716: add.eq sp,#0x1c
  0016b718: pop.eq.w {r8,r9,r10,r11}
  0016b71c: pop.eq {r4,r5,r6,r7,pc}
  0016b71e: blx 0x0006e824
```
