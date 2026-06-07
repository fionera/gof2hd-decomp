# ScrollTouchWindow::OnTouchEnd
elf 0x17456a body 18
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* ScrollTouchWindow::OnTouchEnd(int, int) */

void ScrollTouchWindow::OnTouchEnd(int param_1,int param_2)

{
  ScrollTouchBox::OnTouchEnd(*(int *)param_1,param_2);
  *(undefined1 *)(param_1 + 0x10) = 0;
  return;
}

```

## target disasm
```
  0018456a: push {r4,r6,r7,lr}
  0018456c: add r7,sp,#0x8
  0018456e: mov r4,r0
  00184570: ldr r0,[r0,#0x0]
  00184572: blx 0x00077e60
  00184576: movs r0,#0x0
  00184578: strb r0,[r4,#0x10]
  0018457a: pop {r4,r6,r7,pc}
```
