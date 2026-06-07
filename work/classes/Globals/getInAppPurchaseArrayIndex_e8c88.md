# Globals::getInAppPurchaseArrayIndex
elf 0xe8c88 body 546
Sig: undefined __stdcall getInAppPurchaseArrayIndex(int param_1, Array * param_2)

## decompile
```c

/* Globals::getInAppPurchaseArrayIndex(int, Array<AbyssEngine::String*>*) */

void Globals::getInAppPurchaseArrayIndex(int param_1,Array *param_2)

{
  bool bVar1;
  int *piVar2;
  char *pcVar3;
  char *pcVar4;
  char *pcVar5;
  int iVar6;
  uint *in_r2;
  String *this;
  uint uVar7;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar2 = *(int **)(DAT_000f8f20 + 0xf8c9e);
  local_28 = *piVar2;
  if (in_r2 != (uint *)0x0) {
    pcVar3 = (char *)(DAT_000f8f24 + 0xf8cb8);
    uVar7 = 0;
    pcVar4 = (char *)(DAT_000f8f28 + 0xf8cc2);
    pcVar5 = (char *)(DAT_000f8f2c + 0xf8cc8);
    do {
      if (*in_r2 <= uVar7) break;
      this = *(String **)(in_r2[1] + uVar7 * 4);
      AbyssEngine::String::String(aSStack_40,pcVar3,false);
      AbyssEngine::String::String(aSStack_4c,pcVar4,false);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::String::~String(aSStack_4c);
      AbyssEngine::String::~String(aSStack_40);
      if (this == (String *)0x0) {
        bVar1 = false;
      }
      else {
        switch(param_2) {
        case (Array *)0x0:
          AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f30 + 0xf8d54),false);
          AbyssEngine::operator+(aSStack_40,aSStack_34);
          iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
          break;
        case (Array *)0x1:
          AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f34 + 0xf8df4),false);
          AbyssEngine::operator+(aSStack_40,aSStack_34);
          iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
          break;
        case (Array *)0x2:
          AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f38 + 0xf8e14),false);
          AbyssEngine::operator+(aSStack_40,aSStack_34);
          iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
          break;
        case (Array *)0x3:
          AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f3c + 0xf8e34),false);
          AbyssEngine::operator+(aSStack_40,aSStack_34);
          iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
          break;
        case (Array *)0x4:
          AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f40 + 0xf8e54),false);
          AbyssEngine::operator+(aSStack_40,aSStack_34);
          iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
          break;
        default:
          switch(param_2) {
          case (Array *)0x32:
            AbyssEngine::String::String(aSStack_4c,pcVar5,false);
            AbyssEngine::operator+(aSStack_40,aSStack_34);
            iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
            break;
          case (Array *)0x33:
            AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f44 + 0xf8d74),false);
            AbyssEngine::operator+(aSStack_40,aSStack_34);
            iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
            break;
          case (Array *)0x34:
            AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f48 + 0xf8d94),false);
            AbyssEngine::operator+(aSStack_40,aSStack_34);
            iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
            break;
          case (Array *)0x35:
            AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f4c + 0xf8db4),false);
            AbyssEngine::operator+(aSStack_40,aSStack_34);
            iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
            break;
          case (Array *)0x36:
            AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f8f50 + 0xf8dd4),false);
            AbyssEngine::operator+(aSStack_40,aSStack_34);
            iVar6 = AbyssEngine::String::Compare(this,aSStack_40);
            break;
          default:
            goto switchD_000f8d12_default;
          }
        }
        AbyssEngine::String::~String(aSStack_40);
        AbyssEngine::String::~String(aSStack_4c);
        if (iVar6 == 0) {
          bVar1 = false;
        }
        else {
switchD_000f8d12_default:
          bVar1 = true;
        }
      }
      AbyssEngine::String::~String(aSStack_34);
      uVar7 = uVar7 + 1;
    } while (bVar1);
  }
  iVar6 = *piVar2 - local_28;
  if (iVar6 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar6);
}

```

## target disasm
```
  000f8c88: push {r4,r5,r6,r7,lr}
  000f8c8a: add r7,sp,#0xc
  000f8c8c: push {r8,r9,r10,r11}
  000f8c90: sub sp,#0x3c
  000f8c92: ldr r0,[0x000f8f20]
  000f8c94: mov r4,r2
  000f8c96: mov r8,r1
  000f8c98: cmp r2,#0x0
  000f8c9a: add r0,pc
  000f8c9c: ldr r0,[r0,#0x0]
  000f8c9e: str r0,[sp,#0x4]
  000f8ca0: ldr r0,[r0,#0x0]
  000f8ca2: str r0,[sp,#0x38]
  000f8ca4: beq.w 0x000f8e98
  000f8ca8: ldr r0,[0x000f8f24]
  000f8caa: add.w r11,sp,#0x20
  000f8cae: add r5,sp,#0x14
  000f8cb0: add.w r9,sp,#0x2c
  000f8cb4: add r0,pc
  000f8cb6: str r0,[sp,#0x10]
  000f8cb8: ldr r0,[0x000f8f28]
  000f8cba: mov.w r10,#0x0
  000f8cbe: add r0,pc
  000f8cc0: str r0,[sp,#0xc]
  000f8cc2: ldr r0,[0x000f8f2c]
  000f8cc4: add r0,pc
  000f8cc6: str r0,[sp,#0x0]
  000f8cc8: ldr r0,[r4,#0x0]
  000f8cca: cmp r10,r0
  000f8ccc: bcs.w 0x000f8e98
  000f8cd0: ldr r0,[r4,#0x4]
  000f8cd2: movs r2,#0x0
  000f8cd4: ldr r1,[sp,#0x10]
  000f8cd6: ldr.w r6,[r0,r10,lsl #0x2]
  000f8cda: mov r0,r11
  000f8cdc: blx 0x0006ee18
  000f8ce0: ldr r1,[sp,#0xc]
  000f8ce2: mov r0,r5
  000f8ce4: movs r2,#0x0
  000f8ce6: blx 0x0006ee18
  000f8cea: mov r0,r9
  000f8cec: mov r1,r11
  000f8cee: mov r2,r5
  000f8cf0: blx 0x0006ef98
  000f8cf4: mov r0,r5
  000f8cf6: blx 0x0006ee30
  000f8cfa: mov r0,r11
  000f8cfc: blx 0x0006ee30
  000f8d00: cbz r6,0x000f8d3a
  000f8d02: cmp.w r8,#0x4
  000f8d06: bls 0x000f8d44
  000f8d08: sub.w r0,r8,#0x32
  000f8d0c: cmp r0,#0x4
  000f8d0e: bhi.w 0x000f8e7c
  000f8d12: tbb [pc,r0]
  000f8d1c: ldr r1,[sp,#0x0]
  000f8d1e: mov r0,r5
  000f8d20: movs r2,#0x0
  000f8d22: blx 0x0006ee18
  000f8d26: mov r0,r11
  000f8d28: mov r1,r9
  000f8d2a: mov r2,r5
  000f8d2c: blx 0x0006ef98
  000f8d30: mov r0,r6
  000f8d32: mov r1,r11
  000f8d34: blx 0x0006f664
  000f8d38: b 0x000f8e6c
  000f8d3a: mov.w r0,#0xffffffff
  000f8d3e: movs r6,#0x0
  000f8d40: str r0,[sp,#0x8]
  000f8d42: b 0x000f8e86
  000f8d44: tbb [pc,r8]
  000f8d4e: ldr r1,[0x000f8f30]
  000f8d50: add r1,pc
  000f8d52: mov r0,r5
  000f8d54: movs r2,#0x0
  000f8d56: blx 0x0006ee18
  000f8d5a: mov r0,r11
  000f8d5c: mov r1,r9
  000f8d5e: mov r2,r5
  000f8d60: blx 0x0006ef98
  000f8d64: mov r0,r6
  000f8d66: mov r1,r11
  000f8d68: blx 0x0006f664
  000f8d6c: b 0x000f8e6c
  000f8d6e: ldr r1,[0x000f8f44]
  000f8d70: add r1,pc
  000f8d72: mov r0,r5
  000f8d74: movs r2,#0x0
  000f8d76: blx 0x0006ee18
  000f8d7a: mov r0,r11
  000f8d7c: mov r1,r9
  000f8d7e: mov r2,r5
  000f8d80: blx 0x0006ef98
  000f8d84: mov r0,r6
  000f8d86: mov r1,r11
  000f8d88: blx 0x0006f664
  000f8d8c: b 0x000f8e6c
  000f8d8e: ldr r1,[0x000f8f48]
  000f8d90: add r1,pc
  000f8d92: mov r0,r5
  000f8d94: movs r2,#0x0
  000f8d96: blx 0x0006ee18
  000f8d9a: mov r0,r11
  000f8d9c: mov r1,r9
  000f8d9e: mov r2,r5
  000f8da0: blx 0x0006ef98
  000f8da4: mov r0,r6
  000f8da6: mov r1,r11
  000f8da8: blx 0x0006f664
  000f8dac: b 0x000f8e6c
  000f8dae: ldr r1,[0x000f8f4c]
  000f8db0: add r1,pc
  000f8db2: mov r0,r5
  000f8db4: movs r2,#0x0
  000f8db6: blx 0x0006ee18
  000f8dba: mov r0,r11
  000f8dbc: mov r1,r9
  000f8dbe: mov r2,r5
  000f8dc0: blx 0x0006ef98
  000f8dc4: mov r0,r6
  000f8dc6: mov r1,r11
  000f8dc8: blx 0x0006f664
  000f8dcc: b 0x000f8e6c
  000f8dce: ldr r1,[0x000f8f50]
  000f8dd0: add r1,pc
  000f8dd2: mov r0,r5
  000f8dd4: movs r2,#0x0
  000f8dd6: blx 0x0006ee18
  000f8dda: mov r0,r11
  000f8ddc: mov r1,r9
  000f8dde: mov r2,r5
  000f8de0: blx 0x0006ef98
  000f8de4: mov r0,r6
  000f8de6: mov r1,r11
  000f8de8: blx 0x0006f664
  000f8dec: b 0x000f8e6c
  000f8dee: ldr r1,[0x000f8f34]
  000f8df0: add r1,pc
  000f8df2: mov r0,r5
  000f8df4: movs r2,#0x0
  000f8df6: blx 0x0006ee18
  000f8dfa: mov r0,r11
  000f8dfc: mov r1,r9
  000f8dfe: mov r2,r5
  000f8e00: blx 0x0006ef98
  000f8e04: mov r0,r6
  000f8e06: mov r1,r11
  000f8e08: blx 0x0006f664
  000f8e0c: b 0x000f8e6c
  000f8e0e: ldr r1,[0x000f8f38]
  000f8e10: add r1,pc
  000f8e12: mov r0,r5
  000f8e14: movs r2,#0x0
  000f8e16: blx 0x0006ee18
  000f8e1a: mov r0,r11
  000f8e1c: mov r1,r9
  000f8e1e: mov r2,r5
  000f8e20: blx 0x0006ef98
  000f8e24: mov r0,r6
  000f8e26: mov r1,r11
  000f8e28: blx 0x0006f664
  000f8e2c: b 0x000f8e6c
  000f8e2e: ldr r1,[0x000f8f3c]
  000f8e30: add r1,pc
  000f8e32: mov r0,r5
  000f8e34: movs r2,#0x0
  000f8e36: blx 0x0006ee18
  000f8e3a: mov r0,r11
  000f8e3c: mov r1,r9
  000f8e3e: mov r2,r5
  000f8e40: blx 0x0006ef98
  000f8e44: mov r0,r6
  000f8e46: mov r1,r11
  000f8e48: blx 0x0006f664
  000f8e4c: b 0x000f8e6c
  000f8e4e: ldr r1,[0x000f8f40]
  000f8e50: add r1,pc
  000f8e52: mov r0,r5
  000f8e54: movs r2,#0x0
  000f8e56: blx 0x0006ee18
  000f8e5a: mov r0,r11
  000f8e5c: mov r1,r9
  000f8e5e: mov r2,r5
  000f8e60: blx 0x0006ef98
  000f8e64: mov r0,r6
  000f8e66: mov r1,r11
  000f8e68: blx 0x0006f664
  000f8e6c: mov r6,r0
  000f8e6e: mov r0,r11
  000f8e70: blx 0x0006ee30
  000f8e74: mov r0,r5
  000f8e76: blx 0x0006ee30
  000f8e7a: cbz r6,0x000f8e80
  000f8e7c: movs r6,#0x1
  000f8e7e: b 0x000f8e86
  000f8e80: movs r6,#0x0
  000f8e82: str.w r10,[sp,#0x8]
  000f8e86: mov r0,r9
  000f8e88: blx 0x0006ee30
  000f8e8c: add.w r10,r10,#0x1
  000f8e90: cmp r6,#0x0
  000f8e92: bne.w 0x000f8cc8
  000f8e96: b 0x000f8e9e
  000f8e98: mov.w r0,#0xffffffff
  000f8e9c: str r0,[sp,#0x8]
  000f8e9e: ldr r0,[sp,#0x38]
  000f8ea0: ldr r1,[sp,#0x4]
  000f8ea2: ldr r1,[r1,#0x0]
  000f8ea4: subs r0,r1,r0
  000f8ea6: itttt eq
  000f8ea8: ldr.eq r0,[sp,#0x8]
  000f8eaa: add.eq sp,#0x3c
  000f8eac: pop.eq.w {r8,r9,r10,r11}
  000f8eb0: pop.eq {r4,r5,r6,r7,pc}
  000f8eb2: blx 0x0006e824
```
