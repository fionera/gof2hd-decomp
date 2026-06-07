# Globals::longToTimeString
elf 0xe3994 body 498
Sig: undefined __stdcall longToTimeString(longlong param_1, String * param_2)

## decompile
```c

/* Globals::longToTimeString(long long, AbyssEngine::String&) */

void Globals::longToTimeString(longlong param_1,String *param_2)

{
  undefined1 *puVar1;
  int iVar2;
  char *pcVar3;
  int extraout_r2;
  int extraout_r2_00;
  int extraout_r2_01;
  code *pcVar4;
  int *piVar5;
  undefined8 uVar6;
  String *in_stack_00000000;
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  uVar6 = FUN_001aae54(param_2);
  FUN_001aae54((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),0x3c,0);
  uVar6 = FUN_001aae54(param_2);
  FUN_001aae54((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),0x3c,0);
  uVar6 = FUN_001aae54(param_2);
  FUN_001aae54((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),0x18,0);
  piVar5 = *(int **)(DAT_000f3c38 + 0xf39f0);
  local_28 = *piVar5;
  if (extraout_r2 < 10) {
    pcVar3 = (char *)(DAT_000f3c3c + 0xf3a00);
  }
  else {
    pcVar3 = (char *)(DAT_000f3c40 + 0xf3a06);
  }
  AbyssEngine::String::String(aSStack_40,pcVar3,false);
  AbyssEngine::String::String(aSStack_4c,extraout_r2);
  AbyssEngine::operator+(aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_4c);
  AbyssEngine::String::~String(aSStack_40);
  if (extraout_r2_00 < 10) {
    AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f3c44 + 0xf3a34),false);
  }
  else {
    AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f3c48 + 0xf3a42),false);
  }
  AbyssEngine::String::String(aSStack_58,extraout_r2_00);
  AbyssEngine::operator+(aSStack_40,aSStack_4c);
  AbyssEngine::String::~String(aSStack_58);
  AbyssEngine::String::~String(aSStack_4c);
  if (extraout_r2_01 == 0) {
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_000f3c4c + 0xf3a9c),false);
    AbyssEngine::operator+(aSStack_58,aSStack_40);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    AbyssEngine::String::operator=(in_stack_00000000,aSStack_4c);
    pcVar4 = *(code **)(DAT_000f3c50 + 0xf3ac4);
    (*pcVar4)(aSStack_4c);
    (*pcVar4)(aSStack_58);
    puVar1 = &stack0x00000010;
  }
  else {
    iVar2 = FUN_001aae54(param_2);
    iVar2 = extraout_r2_01 + iVar2 * 0x18;
    if (iVar2 < 10) {
      AbyssEngine::String::String(aSStack_58,(char *)(DAT_000f3c54 + 0xf3a8e),false);
    }
    else {
      AbyssEngine::String::String(aSStack_58,(char *)(DAT_000f3c58 + 0xf3ad6),false);
    }
    AbyssEngine::String::String(aSStack_64,iVar2);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    AbyssEngine::String::~String(aSStack_64);
    AbyssEngine::String::~String(aSStack_58);
    AbyssEngine::String::String(aSStack_88,(char *)(DAT_000f3c5c + 0xf3b00),false);
    AbyssEngine::operator+(aSStack_7c,aSStack_4c);
    AbyssEngine::operator+(aSStack_70,aSStack_7c);
    AbyssEngine::String::String(aSStack_94,(char *)(DAT_000f3c60 + 0xf3b20),false);
    AbyssEngine::operator+(aSStack_64,aSStack_70);
    AbyssEngine::operator+(aSStack_58,aSStack_64);
    AbyssEngine::String::operator=(in_stack_00000000,aSStack_58);
    pcVar4 = *(code **)(DAT_000f3c64 + 0xf3b48);
    (*pcVar4)(aSStack_58);
    (*pcVar4)(aSStack_64);
    (*pcVar4)(aSStack_94);
    (*pcVar4)(aSStack_70);
    (*pcVar4)(aSStack_7c);
    (*pcVar4)(aSStack_88);
    puVar1 = &stack0x00000028;
  }
  (*pcVar4)(puVar1 + -0x74);
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_34);
  if (*piVar5 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f3994: push {r4,r5,r6,r7,lr}
  000f3996: add r7,sp,#0xc
  000f3998: push {r8,r9,r10,r11}
  000f399c: sub sp,#0x74
  000f399e: mov r10,r3
  000f39a0: mov r1,r3
  000f39a2: mov r6,r2
  000f39a4: mov r0,r2
  000f39a6: mov.w r2,#0x3e8
  000f39aa: movs r3,#0x0
  000f39ac: bl 0x001aae54
  000f39b0: movs r2,#0x3c
  000f39b2: movs r3,#0x0
  000f39b4: bl 0x001aae54
  000f39b8: mov r5,r2
  000f39ba: mov r0,r6
  000f39bc: mov r1,r10
  000f39be: movw r2,#0xea60
  000f39c2: movs r3,#0x0
  000f39c4: bl 0x001aae54
  000f39c8: movs r2,#0x3c
  000f39ca: movs r3,#0x0
  000f39cc: bl 0x001aae54
  000f39d0: mov r4,r2
  000f39d2: ldr r2,[0x000f3c30]
  000f39d4: mov r0,r6
  000f39d6: mov r1,r10
  000f39d8: movs r3,#0x0
  000f39da: bl 0x001aae54
  000f39de: movs r2,#0x18
  000f39e0: movs r3,#0x0
  000f39e2: bl 0x001aae54
  000f39e6: ldr r0,[0x000f3c38]
  000f39e8: mov r9,r2
  000f39ea: cmp r5,#0x9
  000f39ec: add r0,pc
  000f39ee: ldr.w r11,[r0,#0x0]
  000f39f2: ldr.w r0,[r11,#0x0]
  000f39f6: str r0,[sp,#0x70]
  000f39f8: bgt 0x000f3a00
  000f39fa: ldr r1,[0x000f3c3c]
  000f39fc: add r1,pc
  000f39fe: b 0x000f3a04
  000f3a00: ldr r1,[0x000f3c40]
  000f3a02: add r1,pc
  000f3a04: add r0,sp,#0x58
  000f3a06: movs r2,#0x0
  000f3a08: blx 0x0006ee18
  000f3a0c: add r0,sp,#0x4c
  000f3a0e: mov r1,r5
  000f3a10: blx 0x0006f658
  000f3a14: add r0,sp,#0x64
  000f3a16: add r1,sp,#0x58
  000f3a18: add r2,sp,#0x4c
  000f3a1a: blx 0x0006ef98
  000f3a1e: add r0,sp,#0x4c
  000f3a20: blx 0x0006ee30
  000f3a24: add r0,sp,#0x58
  000f3a26: blx 0x0006ee30
  000f3a2a: cmp r4,#0x9
  000f3a2c: bgt 0x000f3a3c
  000f3a2e: ldr r1,[0x000f3c44]
  000f3a30: add r1,pc
  000f3a32: add r0,sp,#0x4c
  000f3a34: movs r2,#0x0
  000f3a36: blx 0x0006ee18
  000f3a3a: b 0x000f3a48
  000f3a3c: ldr r1,[0x000f3c48]
  000f3a3e: add r1,pc
  000f3a40: add r0,sp,#0x4c
  000f3a42: movs r2,#0x0
  000f3a44: blx 0x0006ee18
  000f3a48: add r0,sp,#0x40
  000f3a4a: mov r1,r4
  000f3a4c: blx 0x0006f658
  000f3a50: add r0,sp,#0x58
  000f3a52: add r1,sp,#0x4c
  000f3a54: add r2,sp,#0x40
  000f3a56: blx 0x0006ef98
  000f3a5a: add r0,sp,#0x40
  000f3a5c: ldr.w r8,[r7,#0x8]
  000f3a60: blx 0x0006ee30
  000f3a64: add r0,sp,#0x4c
  000f3a66: blx 0x0006ee30
  000f3a6a: cmp.w r9,#0x0
  000f3a6e: beq 0x000f3a96
  000f3a70: ldr r2,[0x000f3c34]
  000f3a72: mov r0,r6
  000f3a74: mov r1,r10
  000f3a76: movs r3,#0x0
  000f3a78: bl 0x001aae54
  000f3a7c: add.w r0,r0,r0, lsl #0x1
  000f3a80: add.w r4,r9,r0, lsl #0x3
  000f3a84: cmp r4,#0x9
  000f3a86: bgt 0x000f3ad0
  000f3a88: ldr r1,[0x000f3c54]
  000f3a8a: add r1,pc
  000f3a8c: add r0,sp,#0x40
  000f3a8e: movs r2,#0x0
  000f3a90: blx 0x0006ee18
  000f3a94: b 0x000f3adc
  000f3a96: ldr r1,[0x000f3c4c]
  000f3a98: add r1,pc
  000f3a9a: add r0,sp,#0x34
  000f3a9c: movs r2,#0x0
  000f3a9e: blx 0x0006ee18
  000f3aa2: add r0,sp,#0x40
  000f3aa4: add r1,sp,#0x58
  000f3aa6: add r2,sp,#0x34
  000f3aa8: blx 0x0006ef98
  000f3aac: add r0,sp,#0x4c
  000f3aae: add r1,sp,#0x40
  000f3ab0: add r2,sp,#0x64
  000f3ab2: blx 0x0006ef98
  000f3ab6: add r1,sp,#0x4c
  000f3ab8: mov r0,r8
  000f3aba: blx 0x0006f2b0
  000f3abe: ldr r0,[0x000f3c50]
  000f3ac0: add r0,pc
  000f3ac2: ldr r4,[r0,#0x0]
  000f3ac4: add r0,sp,#0x4c
  000f3ac6: blx r4
  000f3ac8: add r0,sp,#0x40
  000f3aca: blx r4
  000f3acc: add r0,sp,#0x34
  000f3ace: b 0x000f3b62
  000f3ad0: ldr r1,[0x000f3c58]
  000f3ad2: add r1,pc
  000f3ad4: add r0,sp,#0x40
  000f3ad6: movs r2,#0x0
  000f3ad8: blx 0x0006ee18
  000f3adc: add r0,sp,#0x34
  000f3ade: mov r1,r4
  000f3ae0: blx 0x0006f658
  000f3ae4: add r0,sp,#0x4c
  000f3ae6: add r1,sp,#0x40
  000f3ae8: add r2,sp,#0x34
  000f3aea: blx 0x0006ef98
  000f3aee: add r0,sp,#0x34
  000f3af0: blx 0x0006ee30
  000f3af4: add r0,sp,#0x40
  000f3af6: blx 0x0006ee30
  000f3afa: ldr r1,[0x000f3c5c]
  000f3afc: add r1,pc
  000f3afe: add r0,sp,#0x10
  000f3b00: movs r2,#0x0
  000f3b02: blx 0x0006ee18
  000f3b06: add r0,sp,#0x1c
  000f3b08: add r1,sp,#0x4c
  000f3b0a: add r2,sp,#0x10
  000f3b0c: blx 0x0006ef98
  000f3b10: add r0,sp,#0x28
  000f3b12: add r1,sp,#0x1c
  000f3b14: add r2,sp,#0x58
  000f3b16: blx 0x0006ef98
  000f3b1a: ldr r1,[0x000f3c60]
  000f3b1c: add r1,pc
  000f3b1e: add r0,sp,#0x4
  000f3b20: movs r2,#0x0
  000f3b22: blx 0x0006ee18
  000f3b26: add r0,sp,#0x34
  000f3b28: add r1,sp,#0x28
  000f3b2a: add r2,sp,#0x4
  000f3b2c: blx 0x0006ef98
  000f3b30: add r0,sp,#0x40
  000f3b32: add r1,sp,#0x34
  000f3b34: add r2,sp,#0x64
  000f3b36: blx 0x0006ef98
  000f3b3a: add r1,sp,#0x40
  000f3b3c: mov r0,r8
  000f3b3e: blx 0x0006f2b0
  000f3b42: ldr r0,[0x000f3c64]
  000f3b44: add r0,pc
  000f3b46: ldr r4,[r0,#0x0]
  000f3b48: add r0,sp,#0x40
  000f3b4a: blx r4
  000f3b4c: add r0,sp,#0x34
  000f3b4e: blx r4
  000f3b50: add r0,sp,#0x4
  000f3b52: blx r4
  000f3b54: add r0,sp,#0x28
  000f3b56: blx r4
  000f3b58: add r0,sp,#0x1c
  000f3b5a: blx r4
  000f3b5c: add r0,sp,#0x10
  000f3b5e: blx r4
  000f3b60: add r0,sp,#0x4c
  000f3b62: blx r4
  000f3b64: add r0,sp,#0x58
  000f3b66: blx 0x0006ee30
  000f3b6a: add r0,sp,#0x64
  000f3b6c: blx 0x0006ee30
  000f3b70: ldr r0,[sp,#0x70]
  000f3b72: ldr.w r1,[r11,#0x0]
  000f3b76: subs r0,r1,r0
  000f3b78: ittt eq
  000f3b7a: add.eq sp,#0x74
  000f3b7c: pop.eq.w {r8,r9,r10,r11}
  000f3b80: pop.eq {r4,r5,r6,r7,pc}
  000f3b82: blx 0x0006e824
```
