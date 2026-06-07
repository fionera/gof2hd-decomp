# GameText::ReadLangFile
elf 0x7f64c body 298
Sig: undefined __thiscall ReadLangFile(GameText * this, uint param_1, int param_2)

## decompile
```c

/* GameText::ReadLangFile(unsigned int, int) */

void __thiscall GameText::ReadLangFile(GameText *this,uint param_1,int param_2)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  short *psVar4;
  int iVar5;
  char *pcVar6;
  ushort *puVar7;
  String *this_00;
  int *piVar8;
  undefined4 extraout_r1;
  uint uVar9;
  int iVar10;
  String aSStack_38 [14];
  ushort uStack_2a;
  int local_28;
  
  piVar8 = *(int **)(DAT_0008f790 + 0x8f666);
  local_28 = *piVar8;
  if (param_1 == 0) {
    if (*piVar8 - local_28 == 0) {
      (*(code *)(DAT_001ab454 + 0x1ab458))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*piVar8 - local_28);
  }
  iVar10 = 0;
  psVar4 = *(short **)(DAT_0008f794 + 0x8f67a);
  do {
    if (param_2 <= iVar10) {
LAB_0008f758:
      AEFile::Close(param_1);
      if (*piVar8 == local_28) {
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
    iVar5 = AEFile::Read(2,&uStack_2a,param_1);
    uVar3 = uStack_2a;
    if (iVar5 == 0) {
LAB_0008f752:
      release(this);
      goto LAB_0008f758;
    }
    uVar9 = (uint)uStack_2a;
    uVar2 = uStack_2a >> 8;
    uStack_2a = uStack_2a >> 8 | uStack_2a << 8;
    pcVar6 = operator_new__(((uVar9 << 0x18 | (uint)uVar2 << 0x10) >> 0x10) + 1);
    iVar5 = AEFile::Read((uVar9 & 0xff) << 8 | (uint)(uVar3 >> 8),pcVar6,param_1);
    if (iVar5 == 0) {
      operator_delete__(pcVar6);
      goto LAB_0008f752;
    }
    pcVar6[uStack_2a] = '\0';
    puVar7 = (ushort *)AbyssEngine::String::getWCharFromUtf8(pcVar6,(uint)uStack_2a);
    sVar1 = *psVar4;
    this_00 = operator_new(0xc);
    if (sVar1 == 9) {
      AbyssEngine::String::String(aSStack_38,puVar7,false);
      convertStringFromArabic(this_00,extraout_r1,aSStack_38);
      *(String **)(*(int *)(this + 0xc) + iVar10 * 4) = this_00;
      AbyssEngine::String::~String(aSStack_38);
    }
    else {
      AbyssEngine::String::String(this_00,puVar7,false);
      *(String **)(*(int *)(this + 0xc) + iVar10 * 4) = this_00;
    }
    if (puVar7 != (ushort *)0x0) {
      operator_delete__(puVar7);
    }
    operator_delete__(pcVar6);
    iVar10 = iVar10 + 1;
  } while( true );
}

```

## target disasm
```
  0008f64c: push {r4,r5,r6,r7,lr}
  0008f64e: add r7,sp,#0xc
  0008f650: push {r8,r9,r10,r11}
  0008f654: sub sp,#0x24
  0008f656: str r2,[sp,#0x8]
  0008f658: mov r11,r1
  0008f65a: str r0,[sp,#0xc]
  0008f65c: cmp.w r11,#0x0
  0008f660: ldr r0,[0x0008f790]
  0008f662: add r0,pc
  0008f664: ldr r1,[r0,#0x0]
  0008f666: ldr r0,[r1,#0x0]
  0008f668: str r0,[sp,#0x20]
  0008f66a: beq 0x0008f72e
  0008f66c: str r1,[sp,#0x0]
  0008f66e: add r5,sp,#0x10
  0008f670: ldr r0,[0x0008f794]
  0008f672: mov.w r10,#0x0
  0008f676: add r0,pc
  0008f678: ldr r0,[r0,#0x0]
  0008f67a: str r0,[sp,#0x4]
  0008f67c: b 0x0008f726
  0008f67e: sub.w r1,r7,#0x22
  0008f682: movs r0,#0x2
  0008f684: mov r2,r11
  0008f686: blx 0x0006eb90
  0008f68a: cmp r0,#0x0
  0008f68c: beq 0x0008f752
  0008f68e: ldrh.w r6,[r7,#-0x22]
  0008f692: movs r1,#0x1
  0008f694: lsrs r0,r6,#0x8
  0008f696: orr.w r0,r0,r6, lsl #0x8
  0008f69a: strh r0,[r7,#-0x22]
  0008f69e: rev r0,r6
  0008f6a0: add.w r0,r1,r0, ror #0x10
  0008f6a4: blx 0x0006ec08
  0008f6a8: mov r4,r0
  0008f6aa: rev16 r0,r6
  0008f6ac: mov r1,r4
  0008f6ae: mov r2,r11
  0008f6b0: blx 0x0006eb90
  0008f6b4: cmp r0,#0x0
  0008f6b6: beq 0x0008f74c
  0008f6b8: ldrh.w r1,[r7,#-0x22]
  0008f6bc: movs r0,#0x0
  0008f6be: strb r0,[r4,r1]
  0008f6c0: mov r0,r4
  0008f6c2: blx 0x0006f598
  0008f6c6: mov r8,r0
  0008f6c8: ldr r0,[sp,#0x4]
  0008f6ca: ldrh.w r9,[r0,#0x0]
  0008f6ce: movs r0,#0xc
  0008f6d0: blx 0x0006eb24
  0008f6d4: mov r6,r0
  0008f6d6: cmp.w r9,#0x9
  0008f6da: bne 0x0008f6fe
  0008f6dc: mov r0,r5
  0008f6de: mov r1,r8
  0008f6e0: movs r2,#0x0
  0008f6e2: blx 0x0006ef50
  0008f6e6: mov r0,r6
  0008f6e8: mov r2,r5
  0008f6ea: blx 0x0007000c
  0008f6ee: ldr r0,[sp,#0xc]
  0008f6f0: ldr r0,[r0,#0xc]
  0008f6f2: str.w r6,[r0,r10,lsl #0x2]
  0008f6f6: mov r0,r5
  0008f6f8: blx 0x0006ee30
  0008f6fc: b 0x0008f710
  0008f6fe: mov r0,r6
  0008f700: mov r1,r8
  0008f702: movs r2,#0x0
  0008f704: blx 0x0006ef50
  0008f708: ldr r0,[sp,#0xc]
  0008f70a: ldr r0,[r0,#0xc]
  0008f70c: str.w r6,[r0,r10,lsl #0x2]
  0008f710: cmp.w r8,#0x0
  0008f714: itt ne
  0008f716: mov.ne r0,r8
  0008f718: blx.ne 0x0006ebfc
  0008f71c: mov r0,r4
  0008f71e: blx 0x0006ebfc
  0008f722: add.w r10,r10,#0x1
  0008f726: ldr r0,[sp,#0x8]
  0008f728: cmp r10,r0
  0008f72a: blt 0x0008f67e
  0008f72c: b 0x0008f758
  0008f72e: ldr r0,[sp,#0x20]
  0008f730: ldr r1,[r1,#0x0]
  0008f732: subs r0,r1,r0
  0008f734: itttt eq
  0008f736: ldr.eq r0,[sp,#0xc]
  0008f738: add.eq sp,#0x24
  0008f73a: pop.eq.w {r8,r9,r10,r11}
  0008f73e: pop.eq.w {r4,r5,r6,r7,lr}
  0008f742: it eq
  0008f744: b.eq.w 0x001ab448
  0008f748: blx 0x0006e824
  0008f74c: mov r0,r4
  0008f74e: blx 0x0006ebfc
  0008f752: ldr r0,[sp,#0xc]
  0008f754: blx 0x0006ffe8
  0008f758: mov r0,r11
  0008f75a: blx 0x0006ece0
  0008f75e: ldr r0,[sp,#0x20]
  0008f760: ldr r1,[sp,#0x0]
  0008f762: ldr r1,[r1,#0x0]
  0008f764: subs r0,r1,r0
  0008f766: ittt eq
  0008f768: add.eq sp,#0x24
  0008f76a: pop.eq.w {r8,r9,r10,r11}
  0008f76e: pop.eq {r4,r5,r6,r7,pc}
  0008f770: blx 0x0006e824
  001ab448: bx pc
```
