# String::Set
elf 0x72cb0 body 334
Sig: undefined __thiscall Set(String * this, float param_1)

## decompile
```c

/* AbyssEngine::String::Set(float) */

void __thiscall AbyssEngine::String::Set(String *this,float param_1)

{
  ushort *puVar1;
  int in_r1;
  code *pcVar2;
  char *pcVar3;
  int iVar4;
  int *piVar5;
  undefined1 auStack_78 [12];
  String aSStack_6c [12];
  undefined1 auStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  int local_3c;
  ushort *local_38;
  undefined4 local_34;
  int local_30;
  int local_2c;
  int local_28;
  
  piVar5 = *(int **)(DAT_00082e60 + 0x82cc6);
  local_28 = *piVar5;
  puVar1 = (ushort *)computeFloatString(param_1,in_r1,(int *)0xa,&local_2c,(int)&local_30);
  local_38 = (ushort *)0x0;
  local_3c = *(int *)(DAT_00082e64 + 0x82ce6) + 8;
  local_34 = 0;
  if (local_2c < 1) {
    Set((String *)&local_3c,(char *)(DAT_00082e68 + 0x82dc8));
    pcVar3 = (char *)(DAT_00082e6c + 0x82ddc);
    for (iVar4 = 0; local_2c < iVar4; iVar4 = iVar4 + -1) {
      String(aSStack_48,pcVar3,false);
      operator+=((String *)&local_3c,aSStack_48);
      ~String(aSStack_48);
    }
  }
  String(aSStack_48,puVar1,false);
  operator+=((String *)&local_3c,aSStack_48);
  ~String(aSStack_48);
  if (0 < local_2c) {
    SubString((uint)auStack_60,(uint)&local_3c);
    String(aSStack_6c,(char *)(DAT_00082e70 + 0x82d22),false);
    operator+(aSStack_54,auStack_60);
    SubString((uint)auStack_78,(uint)&local_3c);
    operator+(aSStack_48,aSStack_54);
    operator=((String *)&local_3c,aSStack_48);
    pcVar2 = *(code **)(DAT_00082e74 + 0x82d56);
    (*pcVar2)(aSStack_48);
    (*pcVar2)(auStack_78);
    (*pcVar2)(aSStack_54);
    (*pcVar2)(aSStack_6c);
    (*pcVar2)(auStack_60);
  }
  if (local_30 != 0) {
    String(aSStack_54,(char *)(DAT_00082e78 + 0x82d74),false);
    operator+(aSStack_48,aSStack_54);
    operator=((String *)&local_3c,aSStack_48);
    ~String(aSStack_48);
    ~String(aSStack_54);
  }
  operator_delete__(puVar1);
  Set(this,local_38);
  ~String((String *)&local_3c);
  if (*piVar5 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00082cb0: push {r4,r5,r6,r7,lr}
  00082cb2: add r7,sp,#0xc
  00082cb4: push {r8,r9,r10,r11}
  00082cb8: sub sp,#0x5c
  00082cba: str r0,[sp,#0x4]
  00082cbc: add r2,sp,#0x54
  00082cbe: ldr r0,[0x00082e60]
  00082cc0: add r3,sp,#0x50
  00082cc2: add r0,pc
  00082cc4: ldr.w r11,[r0,#0x0]
  00082cc8: ldr.w r0,[r11,#0x0]
  00082ccc: str r0,[sp,#0x58]
  00082cce: movs r0,#0x1
  00082cd0: str r0,[sp,#0x0]
  00082cd2: mov r0,r1
  00082cd4: movs r1,#0xa
  00082cd6: blx 0x0006f5f8
  00082cda: mov r9,r0
  00082cdc: ldr r0,[0x00082e64]
  00082cde: ldr r6,[sp,#0x54]
  00082ce0: movs r1,#0x0
  00082ce2: add r0,pc
  00082ce4: str r1,[sp,#0x48]
  00082ce6: cmp r6,#0x0
  00082ce8: ldr r0,[r0,#0x0]
  00082cea: add.w r0,r0,#0x8
  00082cee: str r0,[sp,#0x44]
  00082cf0: str r1,[sp,#0x4c]
  00082cf2: ble 0x00082dc2
  00082cf4: add r0,sp,#0x38
  00082cf6: mov r1,r9
  00082cf8: movs r2,#0x0
  00082cfa: blx 0x0006f574
  00082cfe: add r0,sp,#0x44
  00082d00: add r1,sp,#0x38
  00082d02: blx 0x0006ef5c
  00082d06: add r0,sp,#0x38
  00082d08: blx 0x0006f580
  00082d0c: cmp r6,#0x1
  00082d0e: blt 0x00082d6a
  00082d10: add r0,sp,#0x20
  00082d12: add r1,sp,#0x44
  00082d14: movs r2,#0x0
  00082d16: mov r3,r6
  00082d18: blx 0x0006f604
  00082d1c: ldr r1,[0x00082e70]
  00082d1e: add r1,pc
  00082d20: add r0,sp,#0x14
  00082d22: movs r2,#0x0
  00082d24: blx 0x0006f610
  00082d28: add r0,sp,#0x2c
  00082d2a: add r1,sp,#0x20
  00082d2c: add r2,sp,#0x14
  00082d2e: blx 0x0006ef98
  00082d32: ldr r3,[sp,#0x4c]
  00082d34: add r0,sp,#0x8
  00082d36: add r1,sp,#0x44
  00082d38: mov r2,r6
  00082d3a: blx 0x0006f604
  00082d3e: add r0,sp,#0x38
  00082d40: add r1,sp,#0x2c
  00082d42: add r2,sp,#0x8
  00082d44: blx 0x0006ef98
  00082d48: add r0,sp,#0x44
  00082d4a: add r1,sp,#0x38
  00082d4c: blx 0x0006f2b0
  00082d50: ldr r0,[0x00082e74]
  00082d52: add r0,pc
  00082d54: ldr r4,[r0,#0x0]
  00082d56: add r0,sp,#0x38
  00082d58: blx r4
  00082d5a: add r0,sp,#0x8
  00082d5c: blx r4
  00082d5e: add r0,sp,#0x2c
  00082d60: blx r4
  00082d62: add r0,sp,#0x14
  00082d64: blx r4
  00082d66: add r0,sp,#0x20
  00082d68: blx r4
  00082d6a: ldr r0,[sp,#0x50]
  00082d6c: cbz r0,0x00082d98
  00082d6e: ldr r1,[0x00082e78]
  00082d70: add r1,pc
  00082d72: add r0,sp,#0x2c
  00082d74: movs r2,#0x0
  00082d76: blx 0x0006f610
  00082d7a: add r0,sp,#0x38
  00082d7c: add r1,sp,#0x2c
  00082d7e: add r2,sp,#0x44
  00082d80: blx 0x0006ef98
  00082d84: add r0,sp,#0x44
  00082d86: add r1,sp,#0x38
  00082d88: blx 0x0006f2b0
  00082d8c: add r0,sp,#0x38
  00082d8e: blx 0x0006f580
  00082d92: add r0,sp,#0x2c
  00082d94: blx 0x0006f580
  00082d98: mov r0,r9
  00082d9a: blx 0x0006ebfc
  00082d9e: ldr r1,[sp,#0x48]
  00082da0: ldr r0,[sp,#0x4]
  00082da2: blx 0x0006f3f4
  00082da6: add r0,sp,#0x44
  00082da8: blx 0x0006f580
  00082dac: ldr r0,[sp,#0x58]
  00082dae: ldr.w r1,[r11,#0x0]
  00082db2: subs r0,r1,r0
  00082db4: ittt eq
  00082db6: add.eq sp,#0x5c
  00082db8: pop.eq.w {r8,r9,r10,r11}
  00082dbc: pop.eq {r4,r5,r6,r7,pc}
  00082dbe: blx 0x0006e824
  00082dc2: ldr r1,[0x00082e68]
  00082dc4: add r1,pc
  00082dc6: add r0,sp,#0x44
  00082dc8: blx 0x0006f2bc
  00082dcc: ldr r4,[0x00082e6c]
  00082dce: add r5,sp,#0x38
  00082dd0: add.w r10,sp,#0x44
  00082dd4: mov.w r8,#0x0
  00082dd8: add r4,pc
  00082dda: b 0x00082df8
  00082ddc: mov r0,r5
  00082dde: mov r1,r4
  00082de0: movs r2,#0x0
  00082de2: blx 0x0006f610
  00082de6: mov r0,r10
  00082de8: mov r1,r5
  00082dea: blx 0x0006ef5c
  00082dee: mov r0,r5
  00082df0: blx 0x0006f580
  00082df4: sub.w r8,r8,#0x1
  00082df8: cmp r8,r6
  00082dfa: bgt 0x00082ddc
  00082dfc: b 0x00082cf4
```
