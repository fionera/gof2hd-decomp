# HangarWindow::render3D
elf 0x1490c8 body 14
Sig: undefined __stdcall render3D(void)

## decompile
```c

/* HangarWindow::render3D() */

void HangarWindow::render3D(void)

{
  int in_r0;
  
  if (*(int *)(in_r0 + 0x58) == 1) {
    (*(code *)(DAT_001ac4c4 + 0x1ac4c8))();
    return;
  }
  return;
}

```

## target disasm
```
  001590c8: ldr r1,[r0,#0x58]
  001590ca: cmp r1,#0x1
  001590cc: itt eq
  001590ce: ldr.eq r0,[r0,#0x18]
  001590d0: b.eq.w 0x001ac4b8
  001590d4: bx lr
```
