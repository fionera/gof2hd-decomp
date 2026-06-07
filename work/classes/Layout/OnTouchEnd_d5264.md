# Layout::OnTouchEnd
elf 0xd5264 body 108
Sig: undefined __thiscall OnTouchEnd(Layout * this, int param_1, int param_2)

## decompile
```c

/* Layout::OnTouchEnd(int, int) */

uint __thiscall Layout::OnTouchEnd(Layout *this,int param_1,int param_2)

{
  Layout LVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  
  if ((*this == (Layout)0x0) && (this[0x3cc] != (Layout)0x0)) {
    LVar1 = (Layout)TouchButton::OnTouchEnd(*(int *)(this + 0x3bc),param_1);
    this[0x3c0] = LVar1;
  }
  if ((*(int *)(this + 0x3c4) != 0) && (*this != (Layout)0x0)) {
    iVar2 = ChoiceWindow::OnTouchEnd(*(int *)(this + 0x3c4),param_1);
    return (uint)(iVar2 == 0);
  }
  iVar2 = TouchButton::isVisible(*(TouchButton **)(this + 0x3b4));
  if (iVar2 == 0) {
    uVar3 = *(undefined4 *)(this + 0x3b8);
  }
  else {
    uVar3 = *(undefined4 *)(this + 0x3b4);
  }
  uVar4 = (*(code *)(DAT_001ac0f4 + 0x1ac0f8))(uVar3,param_1,param_2);
  return uVar4;
}

```

## target disasm
```
  000e5264: push {r4,r5,r6,r7,lr}
  000e5266: add r7,sp,#0xc
  000e5268: push.w r11
  000e526c: mov r6,r0
  000e526e: ldrb r0,[r0,#0x0]
  000e5270: mov r4,r2
  000e5272: mov r5,r1
  000e5274: cbnz r0,0x000e528c
  000e5276: ldrb.w r0,[r6,#0x3cc]
  000e527a: cbz r0,0x000e528c
  000e527c: ldr.w r0,[r6,#0x3bc]
  000e5280: mov r1,r5
  000e5282: mov r2,r4
  000e5284: blx 0x00074788
  000e5288: strb.w r0,[r6,#0x3c0]
  000e528c: ldr.w r0,[r6,#0x3c4]
  000e5290: cbz r0,0x000e52aa
  000e5292: ldrb r1,[r6,#0x0]
  000e5294: cbz r1,0x000e52aa
  000e5296: mov r1,r5
  000e5298: mov r2,r4
  000e529a: blx 0x00074770
  000e529e: clz r0,r0
  000e52a2: lsrs r0,r0,#0x5
  000e52a4: pop.w r11
  000e52a8: pop {r4,r5,r6,r7,pc}
  000e52aa: ldr.w r0,[r6,#0x3b4]
  000e52ae: blx 0x00074ec0
  000e52b2: cmp r0,#0x0
  000e52b4: ite ne
  000e52b6: ldr.ne.w r0,[r6,#0x3b4]
  000e52ba: ldr.eq.w r0,[r6,#0x3b8]
  000e52be: mov r1,r5
  000e52c0: mov r2,r4
  000e52c2: pop.w r11
  000e52c6: pop.w {r4,r5,r6,r7,lr}
  000e52ca: b.w 0x001ac0e8
  001ac0e8: bx pc
```
