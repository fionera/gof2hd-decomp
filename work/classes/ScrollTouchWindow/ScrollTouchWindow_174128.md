# ScrollTouchWindow::ScrollTouchWindow
elf 0x174128 body 124
Sig: undefined __thiscall ScrollTouchWindow(ScrollTouchWindow * this, int param_1, int param_2, int param_3, int param_4, bool param_5)

## decompile
```c

/* ScrollTouchWindow::ScrollTouchWindow(int, int, int, int, bool) */

undefined8 __thiscall
ScrollTouchWindow::ScrollTouchWindow
          (ScrollTouchWindow *this,int param_1,int param_2,int param_3,int param_4,bool param_5)

{
  ScrollTouchBox *this_00;
  int iVar1;
  int iVar2;
  int iVar3;
  undefined3 in_stack_00000005;
  
  AbyssEngine::String::String((String *)(this + 4));
  *(int *)(this + 0x14) = param_1;
  *(int *)(this + 0x18) = param_2;
  *(int *)(this + 0x1c) = param_3;
  *(int *)(this + 0x20) = param_4;
  this_00 = operator_new(0x40);
  iVar1 = *(int *)(**(int **)(DAT_001841bc + 0x184162) + 0x4c);
  if (_param_5 == 0) {
    iVar2 = iVar1 + param_2;
    iVar3 = 0;
  }
  else {
    iVar3 = *(int *)(**(int **)(DAT_001841bc + 0x184162) + 8);
    iVar2 = iVar1 + param_2 + iVar3;
    iVar3 = -iVar3;
  }
  iVar3 = iVar3 + param_4 + iVar1 * -2;
  ScrollTouchBox::ScrollTouchBox(this_00,iVar1 + param_1,iVar2,param_3 + iVar1 * -2,iVar3);
  this[0x10] = (ScrollTouchWindow)0x0;
  *(ScrollTouchBox **)this = this_00;
  this[0x11] = (ScrollTouchWindow)param_5;
  return CONCAT44(iVar3,this);
}

```

## target disasm
```
  00184128: push {r4,r5,r6,r7,lr}
  0018412a: add r7,sp,#0xc
  0018412c: push {r5,r6,r7,r8,r9,r10,r11}
  00184130: mov r5,r0
  00184132: adds r0,#0x4
  00184134: mov r9,r3
  00184136: mov r6,r2
  00184138: mov r4,r1
  0018413a: str r0,[sp,#0x8]
  0018413c: blx 0x0006efbc
  00184140: ldr.w r8,[r7,#0x8]
  00184144: add.w r0,r5,#0x14
  00184148: stm r0,{r4,r6,r9}
  0018414c: str.w r8,[r5,#0x20]
  00184150: movs r0,#0x40
  00184152: blx 0x0006eb24
  00184156: mov r10,r0
  00184158: ldr r0,[0x001841bc]
  0018415a: ldr.w r11,[r7,#0xc]
  0018415e: add r0,pc
  00184160: cmp.w r11,#0x0
  00184164: ldr r0,[r0,#0x0]
  00184166: ldr r2,[r0,#0x0]
  00184168: ldr r0,[r2,#0x4c]
  0018416a: add.w r1,r0,r4
  0018416e: beq 0x0018417a
  00184170: ldr r3,[r2,#0x8]
  00184172: adds r2,r0,r6
  00184174: add r2,r3
  00184176: rsbs r3,r3
  00184178: b 0x0018417e
  0018417a: adds r2,r0,r6
  0018417c: movs r3,#0x0
  0018417e: sub.w r6,r8,r0, lsl #0x1
  00184182: add r3,r6
  00184184: str r3,[sp,#0x0]
  00184186: sub.w r3,r9,r0, lsl #0x1
  0018418a: mov r0,r10
  0018418c: blx 0x0007516c
  00184190: movs r0,#0x0
  00184192: strb r0,[r5,#0x10]
  00184194: mov r0,r5
  00184196: str.w r10,[r5,#0x0]
  0018419a: strb.w r11,[r5,#0x11]
  0018419e: pop.w {r1,r2,r3,r8,r9,r10,r11}
  001841a2: pop {r4,r5,r6,r7,pc}
```
