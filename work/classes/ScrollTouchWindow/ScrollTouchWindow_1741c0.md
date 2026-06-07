# ScrollTouchWindow::ScrollTouchWindow
elf 0x1741c0 body 102
Sig: undefined __thiscall ScrollTouchWindow(ScrollTouchWindow * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* ScrollTouchWindow::ScrollTouchWindow(int, int, int, int) */

ScrollTouchWindow * __thiscall
ScrollTouchWindow::ScrollTouchWindow
          (ScrollTouchWindow *this,int param_1,int param_2,int param_3,int param_4)

{
  ScrollTouchBox *this_00;
  int iVar1;
  int iVar2;
  
  AbyssEngine::String::String((String *)(this + 4));
  *(int *)(this + 0x14) = param_1;
  *(int *)(this + 0x18) = param_2;
  *(int *)(this + 0x1c) = param_3;
  *(int *)(this + 0x20) = param_4;
  this_00 = operator_new(0x40);
  iVar2 = *(int *)(**(int **)(DAT_00184240 + 0x1841f4) + 8);
  iVar1 = *(int *)(**(int **)(DAT_00184240 + 0x1841f4) + 0x4c);
  ScrollTouchBox::ScrollTouchBox
            (this_00,iVar1 + param_1,iVar2 + iVar1 + param_2,param_3 + iVar1 * -2,
             (param_4 - iVar2) + iVar1 * -2);
  *(ScrollTouchBox **)this = this_00;
  *(undefined2 *)(this + 0x10) = 0x100;
  return this;
}

```

## target disasm
```
  001841c0: push {r4,r5,r6,r7,lr}
  001841c2: add r7,sp,#0xc
  001841c4: push {r7,r8,r9,r10,r11}
  001841c8: add.w r8,r0,#0x4
  001841cc: mov r5,r0
  001841ce: mov r9,r3
  001841d0: mov r10,r2
  001841d2: mov r0,r8
  001841d4: mov r6,r1
  001841d6: blx 0x0006efbc
  001841da: ldr.w r11,[r7,#0x8]
  001841de: strd r6,r10,[r5,#0x14]
  001841e2: strd r9,r11,[r5,#0x1c]
  001841e6: movs r0,#0x40
  001841e8: blx 0x0006eb24
  001841ec: mov r4,r0
  001841ee: ldr r0,[0x00184240]
  001841f0: add r0,pc
  001841f2: ldr r0,[r0,#0x0]
  001841f4: ldr r0,[r0,#0x0]
  001841f6: ldr r2,[r0,#0x8]
  001841f8: ldr r0,[r0,#0x4c]
  001841fa: sub.w r1,r11,r2
  001841fe: sub.w r1,r1,r0, lsl #0x1
  00184202: add.w r3,r0,r10
  00184206: str r1,[sp,#0x0]
  00184208: add r2,r3
  0018420a: adds r1,r0,r6
  0018420c: sub.w r3,r9,r0, lsl #0x1
  00184210: mov r0,r4
  00184212: blx 0x0007516c
  00184216: mov.w r0,#0x100
  0018421a: str r4,[r5,#0x0]
  0018421c: strh r0,[r5,#0x10]
  0018421e: mov r0,r5
  00184220: pop.w {r3,r8,r9,r10,r11}
  00184224: pop {r4,r5,r6,r7,pc}
```
