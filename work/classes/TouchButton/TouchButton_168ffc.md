# TouchButton::TouchButton
elf 0x168ffc body 170
Sig: undefined __thiscall TouchButton(TouchButton * this, uint param_1, int param_2, int param_3, int param_4, int param_5, uchar param_6, uchar param_7)

## decompile
```c

/* TouchButton::TouchButton(unsigned int, int, int, int, int, unsigned char, unsigned char) */

void __thiscall
TouchButton::TouchButton
          (TouchButton *this,uint param_1,int param_2,int param_3,int param_4,int param_5,
          uchar param_6,uchar param_7)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  int *piVar4;
  String aSStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_001790d4 + 0x179012);
  local_28 = *piVar4;
  AbyssEngine::String::String((String *)(this + 0xc));
  AbyssEngine::String::String((String *)(this + 0x18));
  AbyssEngine::String::String((String *)(this + 0x2c));
  puVar3 = *(uint **)(DAT_001790dc + 0x179046);
  *(undefined4 *)(this + 8) = **(undefined4 **)(DAT_001790d8 + 0x179044);
  uVar1 = AbyssEngine::PaintCanvas::FontGetSpacing(*puVar3);
  iVar2 = DAT_001790e0;
  *(undefined4 *)(this + 0xc4) = uVar1;
  AbyssEngine::String::String(aSStack_34,(char *)(iVar2 + 0x17905c),false);
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
  00178ffc: push {r4,r5,r6,r7,lr}
  00178ffe: add r7,sp,#0xc
  00179000: push {r8,r9,r10,r11}
  00179004: sub sp,#0x34
  00179006: mov r6,r0
  00179008: ldr r0,[0x001790d4]
  0017900a: mov r5,r3
  0017900c: mov r11,r2
  0017900e: add r0,pc
  00179010: mov r9,r1
  00179012: ldr.w r8,[r0,#0x0]
  00179016: ldr.w r0,[r8,#0x0]
  0017901a: str r0,[sp,#0x30]
  0017901c: add.w r0,r6,#0xc
  00179020: str r0,[sp,#0x20]
  00179022: blx 0x0006efbc
  00179026: add.w r4,r6,#0x18
  0017902a: mov r0,r4
  0017902c: blx 0x0006efbc
  00179030: add.w r10,r6,#0x2c
  00179034: mov r0,r10
  00179036: str r4,[sp,#0x1c]
  00179038: blx 0x0006efbc
  0017903c: ldr r0,[0x001790d8]
  0017903e: ldr r1,[0x001790dc]
  00179040: add r0,pc
  00179042: add r1,pc
  00179044: ldr r0,[r0,#0x0]
  00179046: ldr r2,[r1,#0x0]
  00179048: ldr r1,[r0,#0x0]
  0017904a: str r1,[r6,#0x8]
  0017904c: ldr r0,[r2,#0x0]
  0017904e: blx 0x000764a4
  00179052: ldr r1,[0x001790e0]
  00179054: str.w r0,[r6,#0xc4]
  00179058: add r1,pc
  0017905a: add r0,sp,#0x24
  0017905c: movs r2,#0x0
  0017905e: blx 0x0006ee18
  00179062: ldrd r1,r0,[r7,#0x10]
  00179066: ldrd r3,r2,[r7,#0x8]
  0017906a: mov.w r4,#0xffffffff
  0017906e: strd r4,r4,[sp,#0x0]
  00179072: strd r5,r3,[sp,#0x8]
  00179076: mov r3,r11
  00179078: strd r2,r1,[sp,#0x10]
  0017907c: add r1,sp,#0x24
  0017907e: str r0,[sp,#0x18]
  00179080: mov r0,r6
  00179082: mov r2,r9
  00179084: blx 0x00077c80
  00179088: add r0,sp,#0x24
  0017908a: blx 0x0006ee30
  0017908e: ldr r0,[sp,#0x30]
  00179090: ldr.w r1,[r8,#0x0]
  00179094: subs r0,r1,r0
  00179096: itttt eq
  00179098: mov.eq r0,r6
  0017909a: add.eq sp,#0x34
  0017909c: pop.eq.w {r8,r9,r10,r11}
  001790a0: pop.eq {r4,r5,r6,r7,pc}
  001790a2: blx 0x0006e824
```
