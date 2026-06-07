# Layout::drawTip
elf 0xd4a14 body 252
Sig: undefined __thiscall drawTip(Layout * this)

## decompile
```c

/* Layout::drawTip() */

void __thiscall Layout::drawTip(Layout *this)

{
  uint *puVar1;
  int *piVar2;
  undefined4 uVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  String aSStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_000e4b20 + 0xe4a26);
  local_28 = *piVar4;
  if (*(int *)(this + 0x3c8) != 0) {
    puVar1 = *(uint **)(DAT_000e4b24 + 0xe4a3e);
    AbyssEngine::PaintCanvas::SetColor(*puVar1);
    piVar2 = *(int **)(DAT_000e4b28 + 0xe4a4e);
    piVar8 = *(int **)(DAT_000e4b2c + 0xe4a52);
    piVar6 = *(int **)(DAT_000e4b30 + 0xe4a56);
    iVar5 = *piVar2;
    iVar9 = *piVar8;
    iVar7 = *piVar6;
    iVar10 = *(int *)(iVar5 + 0x78);
    uVar3 = AbyssEngine::String::String(aSStack_34,(char *)(DAT_000e4b34 + 0xe4a74),false);
    drawBox(iVar5,5,(iVar7 >> 1) - (iVar10 >> 1),(iVar9 >> 1) + 0xd,iVar10,100,uVar3);
    AbyssEngine::String::~String(aSStack_34);
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar1,*(int *)(this + 0x398),*piVar6 >> 1,(char)(*piVar8 >> 1) + '?','\x11');
    Globals::drawLines(**(uint **)(DAT_000e4b3c + 0xe4ad2),
                       (Array *)**(undefined4 **)(DAT_000e4b38 + 0xe4ad0),
                       (int)*(int **)(this + 0x3c8),*piVar6 >> 1,
                       (bool)(((char)(*piVar8 >> 1) + '?') -
                             (char)((uint)(**(int **)(this + 0x3c8) * *(int *)(*piVar2 + 4)) >> 1)))
    ;
  }
  if (*piVar4 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e4a14: push {r4,r5,r6,r7,lr}
  000e4a16: add r7,sp,#0xc
  000e4a18: push {r8,r9,r10,r11}
  000e4a1c: sub sp,#0x2c
  000e4a1e: mov r4,r0
  000e4a20: ldr r0,[0x000e4b20]
  000e4a22: add r0,pc
  000e4a24: ldr r1,[r0,#0x0]
  000e4a26: ldr r0,[r1,#0x0]
  000e4a28: str r0,[sp,#0x28]
  000e4a2a: ldr.w r0,[r4,#0x3c8]
  000e4a2e: cmp r0,#0x0
  000e4a30: beq 0x000e4afc
  000e4a32: str r1,[sp,#0x18]
  000e4a34: mov.w r1,#0xffffffff
  000e4a38: ldr r0,[0x000e4b24]
  000e4a3a: add r0,pc
  000e4a3c: ldr r0,[r0,#0x0]
  000e4a3e: str r0,[sp,#0x10]
  000e4a40: ldr r0,[r0,#0x0]
  000e4a42: blx 0x0006fac0
  000e4a46: ldr r0,[0x000e4b28]
  000e4a48: ldr r1,[0x000e4b2c]
  000e4a4a: add r0,pc
  000e4a4c: ldr r2,[0x000e4b30]
  000e4a4e: add r1,pc
  000e4a50: ldr r0,[r0,#0x0]
  000e4a52: add r2,pc
  000e4a54: ldr.w r9,[r1,#0x0]
  000e4a58: ldr r6,[r2,#0x0]
  000e4a5a: movs r2,#0x0
  000e4a5c: ldr r5,[r0,#0x0]
  000e4a5e: str r0,[sp,#0x14]
  000e4a60: add r0,sp,#0x1c
  000e4a62: ldr.w r10,[r9,#0x0]
  000e4a66: ldr.w r8,[r6,#0x0]
  000e4a6a: ldr.w r11,[r5,#0x78]
  000e4a6e: ldr r1,[0x000e4b34]
  000e4a70: add r1,pc
  000e4a72: blx 0x0006ee18
  000e4a76: movs r1,#0x64
  000e4a78: strd r11,r1,[sp,#0x0]
  000e4a7c: movs r1,#0x5
  000e4a7e: str r0,[sp,#0x8]
  000e4a80: movs r0,#0xd
  000e4a82: add.w r3,r0,r10, asr #0x1
  000e4a86: asr.w r0,r8, asr #0x1
  000e4a8a: sub.w r2,r0,r11, asr #0x1
  000e4a8e: mov r0,r5
  000e4a90: blx 0x0007462c
  000e4a94: add r0,sp,#0x1c
  000e4a96: blx 0x0006ee30
  000e4a9a: ldr r0,[sp,#0x10]
  000e4a9c: mov.w r12,#0x44
  000e4aa0: ldr.w r2,[r9,#0x0]
  000e4aa4: movs r3,#0x11
  000e4aa6: ldr r5,[r6,#0x0]
  000e4aa8: mov.w r8,#0x3f
  000e4aac: ldr.w r1,[r4,#0x398]
  000e4ab0: ldr r0,[r0,#0x0]
  000e4ab2: strd r3,r12,[sp,#0x0]
  000e4ab6: add.w r3,r8,r2, asr #0x1
  000e4aba: asrs r2,r5,#0x1
  000e4abc: blx 0x00071d70
  000e4ac0: ldr r0,[0x000e4b38]
  000e4ac2: mov.w r12,#0x1
  000e4ac6: ldr r1,[0x000e4b3c]
  000e4ac8: ldr.w r2,[r4,#0x3c8]
  000e4acc: add r0,pc
  000e4ace: add r1,pc
  000e4ad0: ldr r0,[r0,#0x0]
  000e4ad2: ldr r3,[r1,#0x0]
  000e4ad4: ldr r1,[sp,#0x14]
  000e4ad6: ldr r4,[r2,#0x0]
  000e4ad8: ldr r5,[r1,#0x0]
  000e4ada: ldr r1,[r0,#0x0]
  000e4adc: ldr r0,[r3,#0x0]
  000e4ade: ldr r5,[r5,#0x4]
  000e4ae0: ldr r3,[r6,#0x0]
  000e4ae2: ldr.w r6,[r9,#0x0]
  000e4ae6: muls r5,r4
  000e4ae8: asrs r3,r3,#0x1
  000e4aea: add.w r6,r8,r6, asr #0x1
  000e4aee: sub.w r6,r6,r5, lsr #0x1
  000e4af2: strd r6,r12,[sp,#0x0]
  000e4af6: blx 0x00074e9c
  000e4afa: ldr r1,[sp,#0x18]
  000e4afc: ldr r0,[sp,#0x28]
  000e4afe: ldr r1,[r1,#0x0]
  000e4b00: subs r0,r1,r0
  000e4b02: ittt eq
  000e4b04: add.eq sp,#0x2c
  000e4b06: pop.eq.w {r8,r9,r10,r11}
  000e4b0a: pop.eq {r4,r5,r6,r7,pc}
  000e4b0c: blx 0x0006e824
```
