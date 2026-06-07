# ScrollTouchWindow::OnTouchBegin
elf 0x174552 body 8
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* ScrollTouchWindow::OnTouchBegin(int, int) */

void ScrollTouchWindow::OnTouchBegin(int param_1,int param_2)

{
  (*(code *)(DAT_001ac714 + 0x1ac718))(*(undefined4 *)param_1);
  return;
}

```

## target disasm
```
  00184552: ldr r0,[r0,#0x0]
  00184554: b.w 0x001ac708
  001ac708: bx pc
```
