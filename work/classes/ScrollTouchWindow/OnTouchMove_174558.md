# ScrollTouchWindow::OnTouchMove
elf 0x174558 body 18
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* ScrollTouchWindow::OnTouchMove(int, int) */

void ScrollTouchWindow::OnTouchMove(int param_1,int param_2)

{
  int in_r2;
  
  ScrollTouchBox::OnTouchMove(*(ScrollTouchBox **)param_1,param_2,in_r2);
  *(undefined1 *)(param_1 + 0x10) = 1;
  return;
}

```

## target disasm
```
  00184558: push {r4,r6,r7,lr}
  0018455a: add r7,sp,#0x8
  0018455c: mov r4,r0
  0018455e: ldr r0,[r0,#0x0]
  00184560: blx 0x00077e54
  00184564: movs r0,#0x1
  00184566: strb r0,[r4,#0x10]
  00184568: pop {r4,r6,r7,pc}
```
