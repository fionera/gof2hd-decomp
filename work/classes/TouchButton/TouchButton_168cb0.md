# TouchButton::TouchButton
elf 0x168cb0 body 172
Sig: undefined __thiscall TouchButton(TouchButton * this, uint param_1, int param_2, int param_3, int param_4, uchar param_5)

## decompile
```c

/* TouchButton::TouchButton(unsigned int, int, int, int, unsigned char) */

void __thiscall
TouchButton::TouchButton
          (TouchButton *this,uint param_1,int param_2,int param_3,int param_4,uchar param_5)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  int *piVar4;
  String aSStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_00178d8c + 0x178cc6);
  local_28 = *piVar4;
  AbyssEngine::String::String((String *)(this + 0xc));
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x2c));
  puVar3 = *(uint **)(DAT_00178d94 + 0x178cfa);
  *(undefined4 *)(this + 8) = **(undefined4 **)(DAT_00178d90 + 0x178cf8);
  uVar1 = AbyssEngine::PaintCanvas::FontGetSpacing(*puVar3);
  iVar2 = DAT_00178d98;
  *(undefined4 *)(this + 0xc4) = uVar1;
  AbyssEngine::String::String(aSStack_34,(char *)(iVar2 + 0x178d10),false);
  init(this,(EVP_PKEY_CTX *)aSStack_34);
  AbyssEngine::String::~String(aSStack_34);
  iVar2 = *piVar4 - local_28;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  00178cb0: push {r4,r5,r6,r7,lr}
  00178cb2: add r7,sp,#0xc
  00178cb4: push {r8,r9,r10,r11}
  00178cb8: sub sp,#0x3c
  00178cba: mov r6,r0
  00178cbc: ldr r0,[0x00178d8c]
  00178cbe: mov r5,r3
  00178cc0: mov r11,r2
  00178cc2: add r0,pc
  00178cc4: mov r10,r1
  00178cc6: ldr.w r8,[r0,#0x0]
  00178cca: ldr.w r0,[r8,#0x0]
  00178cce: str r0,[sp,#0x38]
  00178cd0: add.w r0,r6,#0xc
  00178cd4: str r0,[sp,#0x28]
  00178cd6: blx 0x0006efbc
  00178cda: add.w r4,r6,#0x18
  00178cde: mov r0,r4
  00178ce0: blx 0x0006efbc
  00178ce4: add.w r0,r6,#0x2c
  00178ce8: str r4,[sp,#0x24]
  00178cea: blx 0x0006efbc
  00178cee: str r0,[sp,#0x20]
  00178cf0: ldr r0,[0x00178d90]
  00178cf2: ldr r1,[0x00178d94]
  00178cf4: add r0,pc
  00178cf6: add r1,pc
  00178cf8: ldr r0,[r0,#0x0]
  00178cfa: ldr r2,[r1,#0x0]
  00178cfc: ldr r1,[r0,#0x0]
  00178cfe: str r1,[r6,#0x8]
  00178d00: ldr r0,[r2,#0x0]
  00178d02: blx 0x000764a4
  00178d06: ldr r1,[0x00178d98]
  00178d08: str.w r0,[r6,#0xc4]
  00178d0c: add r1,pc
  00178d0e: add r0,sp,#0x2c
  00178d10: movs r2,#0x0
  00178d12: mov.w r9,#0x0
  00178d16: blx 0x0006ee18
  00178d1a: ldrd r1,r0,[r7,#0x8]
  00178d1e: mov.w r3,#0xffffffff
  00178d22: movs r2,#0x44
  00178d24: strd r3,r3,[sp,#0x0]
  00178d28: mov r3,r11
  00178d2a: strd r5,r1,[sp,#0x8]
  00178d2e: add r1,sp,#0x2c
  00178d30: strd r9,r0,[sp,#0x10]
  00178d34: mov r0,r6
  00178d36: str r2,[sp,#0x18]
  00178d38: mov r2,r10
  00178d3a: blx 0x00077c80
  00178d3e: add r0,sp,#0x2c
  00178d40: blx 0x0006ee30
  00178d44: ldr r0,[sp,#0x38]
  00178d46: ldr.w r1,[r8,#0x0]
  00178d4a: subs r0,r1,r0
  00178d4c: itttt eq
  00178d4e: mov.eq r0,r6
  00178d50: add.eq sp,#0x3c
  00178d52: pop.eq.w {r8,r9,r10,r11}
  00178d56: pop.eq {r4,r5,r6,r7,pc}
  00178d58: blx 0x0006e824
```
