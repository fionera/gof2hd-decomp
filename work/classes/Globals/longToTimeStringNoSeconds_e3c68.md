# Globals::longToTimeStringNoSeconds
elf 0xe3c68 body 294
Sig: undefined __stdcall longToTimeStringNoSeconds(longlong param_1, String * param_2)

## decompile
```c

/* Globals::longToTimeStringNoSeconds(long long, AbyssEngine::String&) */

void Globals::longToTimeStringNoSeconds(longlong param_1,String *param_2)

{
  int iVar1;
  char *pcVar2;
  int extraout_r2;
  int extraout_r2_00;
  code *pcVar3;
  int *piVar4;
  undefined8 uVar5;
  String *in_stack_00000000;
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  uVar5 = FUN_001aae54(param_2);
  FUN_001aae54((int)uVar5,(int)((ulonglong)uVar5 >> 0x20),0x3c,0);
  uVar5 = FUN_001aae54(param_2);
  FUN_001aae54((int)uVar5,(int)((ulonglong)uVar5 >> 0x20),0x18,0);
  piVar4 = *(int **)(DAT_000f3dec + 0xf3cac);
  local_24 = *piVar4;
  if (extraout_r2 < 10) {
    pcVar2 = (char *)(DAT_000f3df0 + 0xf3cbc);
  }
  else {
    pcVar2 = (char *)(DAT_000f3df4 + 0xf3cc2);
  }
  AbyssEngine::String::String(aSStack_3c,pcVar2,false);
  AbyssEngine::String::String(aSStack_48,extraout_r2);
  AbyssEngine::operator+(aSStack_30,aSStack_3c);
  AbyssEngine::String::~String(aSStack_48);
  AbyssEngine::String::~String(aSStack_3c);
  iVar1 = FUN_001aae54(param_2);
  iVar1 = extraout_r2_00 + iVar1 * 0x18;
  if (iVar1 < 10) {
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_000f3df8 + 0xf3d04),false);
  }
  else {
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_000f3dfc + 0xf3d12),false);
  }
  AbyssEngine::String::String(aSStack_54,iVar1);
  AbyssEngine::operator+(aSStack_3c,aSStack_48);
  AbyssEngine::String::~String(aSStack_54);
  AbyssEngine::String::~String(aSStack_48);
  AbyssEngine::String::String(aSStack_60,(char *)(DAT_000f3e00 + 0xf3d3c),false);
  AbyssEngine::operator+(aSStack_54,aSStack_3c);
  AbyssEngine::operator+(aSStack_48,aSStack_54);
  AbyssEngine::String::operator=(in_stack_00000000,aSStack_48);
  pcVar3 = *(code **)(DAT_000f3e04 + 0xf3d64);
  (*pcVar3)(aSStack_48);
  (*pcVar3)(aSStack_54);
  (*pcVar3)(aSStack_60);
  (*pcVar3)(aSStack_3c);
  (*pcVar3)(aSStack_30);
  if (*piVar4 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f3c68: push {r4,r5,r6,r7,lr}
  000f3c6a: add r7,sp,#0xc
  000f3c6c: push {r8,r9,r11}
  000f3c70: sub sp,#0x40
  000f3c72: mov r4,r3
  000f3c74: mov r1,r3
  000f3c76: mov r5,r2
  000f3c78: mov r0,r2
  000f3c7a: movw r2,#0xea60
  000f3c7e: movs r3,#0x0
  000f3c80: bl 0x001aae54
  000f3c84: movs r2,#0x3c
  000f3c86: movs r3,#0x0
  000f3c88: bl 0x001aae54
  000f3c8c: mov r6,r2
  000f3c8e: ldr r2,[0x000f3de4]
  000f3c90: mov r0,r5
  000f3c92: mov r1,r4
  000f3c94: movs r3,#0x0
  000f3c96: bl 0x001aae54
  000f3c9a: movs r2,#0x18
  000f3c9c: movs r3,#0x0
  000f3c9e: bl 0x001aae54
  000f3ca2: ldr r0,[0x000f3dec]
  000f3ca4: mov r8,r2
  000f3ca6: cmp r6,#0x9
  000f3ca8: add r0,pc
  000f3caa: ldr.w r9,[r0,#0x0]
  000f3cae: ldr.w r0,[r9,#0x0]
  000f3cb2: str r0,[sp,#0x3c]
  000f3cb4: bgt 0x000f3cbc
  000f3cb6: ldr r1,[0x000f3df0]
  000f3cb8: add r1,pc
  000f3cba: b 0x000f3cc0
  000f3cbc: ldr r1,[0x000f3df4]
  000f3cbe: add r1,pc
  000f3cc0: add r0,sp,#0x24
  000f3cc2: movs r2,#0x0
  000f3cc4: blx 0x0006ee18
  000f3cc8: add r0,sp,#0x18
  000f3cca: mov r1,r6
  000f3ccc: blx 0x0006f658
  000f3cd0: add r0,sp,#0x30
  000f3cd2: add r1,sp,#0x24
  000f3cd4: add r2,sp,#0x18
  000f3cd6: blx 0x0006ef98
  000f3cda: add r0,sp,#0x18
  000f3cdc: blx 0x0006ee30
  000f3ce0: add r0,sp,#0x24
  000f3ce2: blx 0x0006ee30
  000f3ce6: ldr r2,[0x000f3de8]
  000f3ce8: mov r0,r5
  000f3cea: mov r1,r4
  000f3cec: movs r3,#0x0
  000f3cee: bl 0x001aae54
  000f3cf2: add.w r0,r0,r0, lsl #0x1
  000f3cf6: add.w r4,r8,r0, lsl #0x3
  000f3cfa: cmp r4,#0x9
  000f3cfc: bgt 0x000f3d0c
  000f3cfe: ldr r1,[0x000f3df8]
  000f3d00: add r1,pc
  000f3d02: add r0,sp,#0x18
  000f3d04: movs r2,#0x0
  000f3d06: blx 0x0006ee18
  000f3d0a: b 0x000f3d18
  000f3d0c: ldr r1,[0x000f3dfc]
  000f3d0e: add r1,pc
  000f3d10: add r0,sp,#0x18
  000f3d12: movs r2,#0x0
  000f3d14: blx 0x0006ee18
  000f3d18: add r0,sp,#0xc
  000f3d1a: mov r1,r4
  000f3d1c: blx 0x0006f658
  000f3d20: add r0,sp,#0x24
  000f3d22: add r1,sp,#0x18
  000f3d24: add r2,sp,#0xc
  000f3d26: blx 0x0006ef98
  000f3d2a: add r0,sp,#0xc
  000f3d2c: blx 0x0006ee30
  000f3d30: add r0,sp,#0x18
  000f3d32: blx 0x0006ee30
  000f3d36: ldr r1,[0x000f3e00]
  000f3d38: add r1,pc
  000f3d3a: mov r0,sp
  000f3d3c: movs r2,#0x0
  000f3d3e: blx 0x0006ee18
  000f3d42: add r0,sp,#0xc
  000f3d44: add r1,sp,#0x24
  000f3d46: mov r2,sp
  000f3d48: blx 0x0006ef98
  000f3d4c: add r0,sp,#0x18
  000f3d4e: add r1,sp,#0xc
  000f3d50: add r2,sp,#0x30
  000f3d52: blx 0x0006ef98
  000f3d56: ldr r0,[r7,#0x8]
  000f3d58: add r1,sp,#0x18
  000f3d5a: blx 0x0006f2b0
  000f3d5e: ldr r0,[0x000f3e04]
  000f3d60: add r0,pc
  000f3d62: ldr r4,[r0,#0x0]
  000f3d64: add r0,sp,#0x18
  000f3d66: blx r4
  000f3d68: add r0,sp,#0xc
  000f3d6a: blx r4
  000f3d6c: mov r0,sp
  000f3d6e: blx r4
  000f3d70: add r0,sp,#0x24
  000f3d72: blx r4
  000f3d74: add r0,sp,#0x30
  000f3d76: blx r4
  000f3d78: ldr r0,[sp,#0x3c]
  000f3d7a: ldr.w r1,[r9,#0x0]
  000f3d7e: subs r0,r1,r0
  000f3d80: ittt eq
  000f3d82: add.eq sp,#0x40
  000f3d84: pop.eq.w {r8,r9,r11}
  000f3d88: pop.eq {r4,r5,r6,r7,pc}
  000f3d8a: blx 0x0006e824
```
