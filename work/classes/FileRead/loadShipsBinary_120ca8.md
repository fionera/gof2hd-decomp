# FileRead::loadShipsBinary
elf 0x120ca8 body 250
Sig: undefined __stdcall loadShipsBinary(void)

## decompile
```c

/* FileRead::loadShipsBinary() */

void FileRead::loadShipsBinary(void)

{
  String *pSVar1;
  int iVar2;
  Array<Ship*> *this;
  Ship *this_00;
  uint *extraout_r2;
  int *piVar3;
  code *pcVar4;
  uint uVar5;
  uint in_fpscr;
  float fVar6;
  undefined4 local_58;
  int local_54;
  int iStack_50;
  int local_4c;
  int iStack_48;
  int local_44;
  int iStack_40;
  int local_3c;
  int iStack_38;
  uint local_34 [3];
  int local_28;
  
  piVar3 = *(int **)(DAT_00130dc0 + 0x130cba);
  local_28 = *piVar3;
  pSVar1 = (String *)
           AbyssEngine::String::String((String *)local_34,(char *)(DAT_00130dc4 + 0x130cc6),false);
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String((String *)local_34);
  if (iVar2 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_00130dc8 + 0x130ce2),(char *)local_34,extraout_r2);
    this = operator_new(0xc);
    Array<Ship*>::Array(this);
    ArraySetLength<Ship*>(0x40,(Array *)this);
    pcVar4 = *(code **)(DAT_00130dcc + 0x130d06);
    for (uVar5 = 0; uVar5 < 0x40; uVar5 = uVar5 + 1) {
      (*pcVar4)(&iStack_38);
      (*pcVar4)(&local_3c,local_34[0]);
      (*pcVar4)(&iStack_40,local_34[0]);
      (*pcVar4)(&local_44,local_34[0]);
      (*pcVar4)(&iStack_48,local_34[0]);
      (*pcVar4)(&local_4c,local_34[0]);
      (*pcVar4)(&iStack_50,local_34[0]);
      (*pcVar4)(&local_54,local_34[0]);
      (*pcVar4)(&local_58,local_34[0]);
      this_00 = operator_new(0x80);
      fVar6 = (float)VectorSignedToFloat(local_58,(byte)(in_fpscr >> 0x16) & 3);
      Ship::Ship(this_00,iStack_38,local_3c,iStack_40,local_44,iStack_48,local_4c,iStack_50,local_54
                 ,fVar6);
      *(Ship **)(*(int *)(this + 4) + uVar5 * 4) = this_00;
    }
    AEFile::Close(local_34[0]);
  }
  iVar2 = *piVar3 - local_28;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  00130ca8: push {r4,r5,r6,r7,lr}
  00130caa: add r7,sp,#0xc
  00130cac: push {r8,r9,r10,r11}
  00130cb0: sub sp,#0x54
  00130cb2: ldr r0,[0x00130dc0]
  00130cb4: movs r2,#0x0
  00130cb6: add r0,pc
  00130cb8: ldr r5,[r0,#0x0]
  00130cba: ldr r0,[r5,#0x0]
  00130cbc: ldr r1,[0x00130dc4]
  00130cbe: str r0,[sp,#0x50]
  00130cc0: add r0,sp,#0x44
  00130cc2: add r1,pc
  00130cc4: blx 0x0006ee18
  00130cc8: blx 0x0006fff4
  00130ccc: mov r4,r0
  00130cce: add r0,sp,#0x44
  00130cd0: blx 0x0006ee30
  00130cd4: cmp r4,#0x0
  00130cd6: beq 0x00130d88
  00130cd8: str r5,[sp,#0x1c]
  00130cda: add r1,sp,#0x44
  00130cdc: ldr r0,[0x00130dc8]
  00130cde: add r0,pc
  00130ce0: blx 0x0006ecd4
  00130ce4: movs r0,#0xc
  00130ce6: blx 0x0006eb24
  00130cea: mov r8,r0
  00130cec: blx 0x000719b0
  00130cf0: movs r0,#0x40
  00130cf2: mov r1,r8
  00130cf4: blx 0x00071a28
  00130cf8: ldr r0,[0x00130dcc]
  00130cfa: add.w r10,sp,#0x20
  00130cfe: mov.w r9,#0x0
  00130d02: add r0,pc
  00130d04: ldr r6,[r0,#0x0]
  00130d06: b 0x00130d76
  00130d08: add r0,sp,#0x40
  00130d0a: blx r6
  00130d0c: ldr r1,[sp,#0x44]
  00130d0e: add r0,sp,#0x3c
  00130d10: blx r6
  00130d12: ldr r1,[sp,#0x44]
  00130d14: add r0,sp,#0x38
  00130d16: blx r6
  00130d18: ldr r1,[sp,#0x44]
  00130d1a: add r0,sp,#0x34
  00130d1c: blx r6
  00130d1e: ldr r1,[sp,#0x44]
  00130d20: add r0,sp,#0x30
  00130d22: blx r6
  00130d24: ldr r1,[sp,#0x44]
  00130d26: add r0,sp,#0x2c
  00130d28: blx r6
  00130d2a: ldr r1,[sp,#0x44]
  00130d2c: add r0,sp,#0x28
  00130d2e: blx r6
  00130d30: ldr r1,[sp,#0x44]
  00130d32: add r0,sp,#0x24
  00130d34: blx r6
  00130d36: ldr r1,[sp,#0x44]
  00130d38: mov r0,r10
  00130d3a: blx r6
  00130d3c: movs r0,#0x80
  00130d3e: blx 0x0006eb24
  00130d42: vldr.32 s0,[sp,#0x20]
  00130d46: mov r11,r0
  00130d48: ldrd r2,r1,[sp,#0x3c]
  00130d4c: ldrd r0,r3,[sp,#0x34]
  00130d50: ldrd r5,r4,[sp,#0x2c]
  00130d54: ldrd lr,r12,[sp,#0x24]
  00130d58: vcvt.f32.s32 s0,s0
  00130d5c: stm sp,{r0,r4,r5,r12,lr}
  00130d60: mov r0,r11
  00130d62: vstr.32 s0,[sp,#0x14]
  00130d66: blx 0x00076378
  00130d6a: ldr.w r0,[r8,#0x4]
  00130d6e: str.w r11,[r0,r9,lsl #0x2]
  00130d72: add.w r9,r9,#0x1
  00130d76: ldr r1,[sp,#0x44]
  00130d78: cmp.w r9,#0x40
  00130d7c: bcc 0x00130d08
  00130d7e: mov r0,r1
  00130d80: blx 0x0006ece0
  00130d84: ldr r5,[sp,#0x1c]
  00130d86: b 0x00130d8c
  00130d88: mov.w r8,#0x0
  00130d8c: ldr r0,[sp,#0x50]
  00130d8e: ldr r1,[r5,#0x0]
  00130d90: subs r0,r1,r0
  00130d92: itttt eq
  00130d94: mov.eq r0,r8
  00130d96: add.eq sp,#0x54
  00130d98: pop.eq.w {r8,r9,r10,r11}
  00130d9c: pop.eq {r4,r5,r6,r7,pc}
  00130d9e: blx 0x0006e824
```
