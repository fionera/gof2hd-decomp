# ChoiceWindow::setHeight
elf 0x146dc4 body 192
Sig: undefined __thiscall setHeight(ChoiceWindow * this, int param_1)

## decompile
```c

/* ChoiceWindow::setHeight(int) */

void __thiscall ChoiceWindow::setHeight(ChoiceWindow *this,int param_1)

{
  int *piVar1;
  TouchButton *this_00;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  
  piVar5 = *(int **)(DAT_00156e84 + 0x156dd8);
  iVar6 = *piVar5;
  piVar1 = *(int **)(DAT_00156e88 + 0x156de4);
  iVar4 = *(int *)(iVar6 + 8);
  *(int *)(this + 0xc) = iVar4 + param_1;
  this_00 = *(TouchButton **)(this + 0x10);
  iVar2 = *piVar1 / 2 - param_1 / 2;
  *(int *)(this + 4) = iVar2 - iVar4;
  if (this[0x58] == (ChoiceWindow)0x0) {
    if (this_00 == (TouchButton *)0x0) {
      return;
    }
    uVar3 = 0x24;
    iVar4 = (param_1 + iVar2) - *(int *)(this + 0x48);
    iVar2 = *(int *)this + *(int *)(this + 8) / 2;
  }
  else {
    if (this_00 != (TouchButton *)0x0) {
      TouchButton::setPosition
                (this_00,(*(int *)this + *(int *)(this + 8) / 2) - *(int *)(iVar6 + 0x4c) / 2,
                 (param_1 + iVar2) - *(int *)(this + 0x48),'\"');
    }
    this_00 = *(TouchButton **)(this + 0x14);
    if (this_00 == (TouchButton *)0x0) {
      return;
    }
    iVar4 = (*(int *)(this + 4) + *(int *)(this + 0xc)) - *(int *)(this + 0x48);
    iVar2 = *(int *)this + *(int *)(this + 8) / 2 + *(int *)(*piVar5 + 0x4c) / 2;
    uVar3 = 0x21;
  }
  (*(code *)(DAT_001ac464 + 0x1ac468))(this_00,iVar2,iVar4,uVar3);
  return;
}

```

## target disasm
```
  00156dc4: push {r4,r5,r6,r7,lr}
  00156dc6: add r7,sp,#0xc
  00156dc8: push.w r8
  00156dcc: mov r4,r0
  00156dce: ldr r0,[0x00156e84]
  00156dd0: add.w r2,r1,r1, lsr #0x1f
  00156dd4: add r0,pc
  00156dd6: ldr.w r8,[r0,#0x0]
  00156dda: ldr r0,[0x00156e88]
  00156ddc: ldr.w r12,[r8,#0x0]
  00156de0: add r0,pc
  00156de2: ldr r0,[r0,#0x0]
  00156de4: ldr.w r5,[r12,#0x8]
  00156de8: adds r3,r5,r1
  00156dea: str r3,[r4,#0xc]
  00156dec: ldr r3,[r0,#0x0]
  00156dee: ldrb.w r6,[r4,#0x58]
  00156df2: ldr r0,[r4,#0x10]
  00156df4: add.w r3,r3,r3, lsr #0x1f
  00156df8: cmp r6,#0x0
  00156dfa: asr.w r3,r3, asr #0x1
  00156dfe: sub.w r3,r3,r2, asr #0x1
  00156e02: sub.w r2,r3,r5
  00156e06: str r2,[r4,#0x4]
  00156e08: beq 0x00156e5c
  00156e0a: cbz r0,0x00156e30
  00156e0c: ldr r5,[r4,#0x8]
  00156e0e: add r1,r3
  00156e10: ldr r2,[r4,#0x48]
  00156e12: ldr.w r3,[r12,#0x4c]
  00156e16: subs r2,r1,r2
  00156e18: ldr r6,[r4,#0x0]
  00156e1a: add.w r1,r5,r5, lsr #0x1f
  00156e1e: add.w r3,r3,r3, lsr #0x1f
  00156e22: add.w r1,r6,r1, asr #0x1
  00156e26: sub.w r1,r1,r3, asr #0x1
  00156e2a: movs r3,#0x22
  00156e2c: blx 0x00076d80
  00156e30: ldr r0,[r4,#0x14]
  00156e32: cbz r0,0x00156e7e
  00156e34: ldr.w r1,[r8,#0x0]
  00156e38: ldrd r3,r2,[r4,#0x0]
  00156e3c: ldrd r6,r5,[r4,#0x8]
  00156e40: add r2,r5
  00156e42: ldr r1,[r1,#0x4c]
  00156e44: ldr r4,[r4,#0x48]
  00156e46: add.w r6,r6,r6, lsr #0x1f
  00156e4a: add.w r1,r1,r1, lsr #0x1f
  00156e4e: subs r2,r2,r4
  00156e50: add.w r3,r3,r6, asr #0x1
  00156e54: add.w r1,r3,r1, asr #0x1
  00156e58: movs r3,#0x21
  00156e5a: b 0x00156e72
  00156e5c: cbz r0,0x00156e7e
  00156e5e: ldr r5,[r4,#0x8]
  00156e60: add r1,r3
  00156e62: ldr r2,[r4,#0x48]
  00156e64: movs r3,#0x24
  00156e66: ldr r6,[r4,#0x0]
  00156e68: subs r2,r1,r2
  00156e6a: add.w r1,r5,r5, lsr #0x1f
  00156e6e: add.w r1,r6,r1, asr #0x1
  00156e72: pop.w r8
  00156e76: pop.w {r4,r5,r6,r7,lr}
  00156e7a: b.w 0x001ac458
  00156e7e: pop.w r8
  00156e82: pop {r4,r5,r6,r7,pc}
```
