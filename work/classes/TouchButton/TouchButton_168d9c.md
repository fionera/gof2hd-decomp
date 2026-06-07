# TouchButton::TouchButton
elf 0x168d9c body 174
Sig: undefined __thiscall TouchButton(TouchButton * this, uint param_1, uint param_2, int param_3, int param_4, int param_5, uchar param_6)

## decompile
```c

/* TouchButton::TouchButton(unsigned int, unsigned int, int, int, int, unsigned char) */

void __thiscall
TouchButton::TouchButton
          (TouchButton *this,uint param_1,uint param_2,int param_3,int param_4,int param_5,
          uchar param_6)

{
  int iVar1;
  undefined4 uVar2;
  uint *puVar3;
  int *piVar4;
  String aSStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_00178e78 + 0x178db2);
  local_28 = *piVar4;
  AbyssEngine::String::String((String *)(this + 0xc));
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x2c));
  puVar3 = *(uint **)(DAT_00178e80 + 0x178de6);
  uVar2 = **(undefined4 **)(DAT_00178e7c + 0x178de4);
  *(uint *)(this + 0x28) = param_2;
  *(undefined4 *)(this + 8) = uVar2;
  uVar2 = AbyssEngine::PaintCanvas::FontGetSpacing(*puVar3);
  iVar1 = DAT_00178e84;
  *(undefined4 *)(this + 0xc4) = uVar2;
  AbyssEngine::String::String(aSStack_34,(char *)(iVar1 + 0x178dfe),false);
  init(this,(EVP_PKEY_CTX *)aSStack_34);
  AbyssEngine::String::~String(aSStack_34);
  iVar1 = *piVar4 - local_28;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  00178d9c: push {r4,r5,r6,r7,lr}
  00178d9e: add r7,sp,#0xc
  00178da0: push {r8,r9,r10,r11}
  00178da4: sub sp,#0x34
  00178da6: mov r6,r0
  00178da8: ldr r0,[0x00178e78]
  00178daa: mov r11,r3
  00178dac: mov r5,r2
  00178dae: add r0,pc
  00178db0: mov r9,r1
  00178db2: ldr.w r8,[r0,#0x0]
  00178db6: ldr.w r0,[r8,#0x0]
  00178dba: str r0,[sp,#0x30]
  00178dbc: add.w r0,r6,#0xc
  00178dc0: str r0,[sp,#0x20]
  00178dc2: blx 0x0006efbc
  00178dc6: add.w r4,r6,#0x18
  00178dca: mov r0,r4
  00178dcc: blx 0x0006efbc
  00178dd0: add.w r10,r6,#0x2c
  00178dd4: mov r0,r10
  00178dd6: str r4,[sp,#0x1c]
  00178dd8: blx 0x0006efbc
  00178ddc: ldr r0,[0x00178e7c]
  00178dde: ldr r1,[0x00178e80]
  00178de0: add r0,pc
  00178de2: add r1,pc
  00178de4: ldr r0,[r0,#0x0]
  00178de6: ldr r2,[r1,#0x0]
  00178de8: ldr r1,[r0,#0x0]
  00178dea: str r5,[r6,#0x28]
  00178dec: str r1,[r6,#0x8]
  00178dee: ldr r0,[r2,#0x0]
  00178df0: blx 0x000764a4
  00178df4: ldr r1,[0x00178e84]
  00178df6: str.w r0,[r6,#0xc4]
  00178dfa: add r1,pc
  00178dfc: add r0,sp,#0x24
  00178dfe: movs r2,#0x0
  00178e00: movs r5,#0x0
  00178e02: blx 0x0006ee18
  00178e06: ldr r0,[r7,#0x10]
  00178e08: ldrd r2,r1,[r7,#0x8]
  00178e0c: mov.w r4,#0xffffffff
  00178e10: movs r3,#0x44
  00178e12: strd r4,r4,[sp,#0x0]
  00178e16: strd r2,r1,[sp,#0x8]
  00178e1a: add r1,sp,#0x24
  00178e1c: strd r5,r0,[sp,#0x10]
  00178e20: mov r0,r6
  00178e22: str r3,[sp,#0x18]
  00178e24: mov r2,r9
  00178e26: mov r3,r11
  00178e28: blx 0x00077c80
  00178e2c: add r0,sp,#0x24
  00178e2e: blx 0x0006ee30
  00178e32: ldr r0,[sp,#0x30]
  00178e34: ldr.w r1,[r8,#0x0]
  00178e38: subs r0,r1,r0
  00178e3a: itttt eq
  00178e3c: mov.eq r0,r6
  00178e3e: add.eq sp,#0x34
  00178e40: pop.eq.w {r8,r9,r10,r11}
  00178e44: pop.eq {r4,r5,r6,r7,pc}
  00178e46: blx 0x0006e824
```
