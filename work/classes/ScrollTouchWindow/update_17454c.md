# ScrollTouchWindow::update
elf 0x17454c body 8
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* ScrollTouchWindow::update(int) */

void ScrollTouchWindow::update(int param_1)

{
  (*(code *)(DAT_001ac704 + 0x1ac708))(*(undefined4 *)param_1);
  return;
}

```

## target disasm
```
  0018454c: ldr r0,[r0,#0x0]
  0018454e: b.w 0x001ac6f8
  001ac6f8: bx pc
```
