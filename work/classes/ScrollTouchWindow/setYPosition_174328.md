# ScrollTouchWindow::setYPosition
elf 0x174328 body 8
Sig: undefined __stdcall setYPosition(int param_1)

## decompile
```c

/* ScrollTouchWindow::setYPosition(int) */

void ScrollTouchWindow::setYPosition(int param_1)

{
  (*(code *)(DAT_001ac6e4 + 0x1ac6e8))(*(undefined4 *)param_1);
  return;
}

```

## target disasm
```
  00184328: ldr r0,[r0,#0x0]
  0018432a: b.w 0x001ac6d8
  001ac6d8: bx pc
```
