# MenuTouchWindow::render3D
elf 0x12cfe4 body 24
Sig: undefined __stdcall render3D(void)

## decompile
```c

/* MenuTouchWindow::render3D() */

void MenuTouchWindow::render3D(void)

{
  int in_r0;
  
  if (*(int *)(in_r0 + 0x16c) != 9) {
    return;
  }
  if (*(int *)(in_r0 + 0xfc) == 0) {
    return;
  }
  (*(code *)(DAT_001ac414 + 0x1ac418))();
  return;
}

```

## target disasm
```
  0013cfe4: ldr.w r1,[r0,#0x16c]
  0013cfe8: cmp r1,#0x9
  0013cfea: it ne
  0013cfec: bx.ne lr
  0013cfee: ldr.w r0,[r0,#0xfc]
  0013cff2: cbz r0,0x0013cff8
  0013cff4: b.w 0x001ac408
  0013cff8: bx lr
  001ac408: bx pc
```
