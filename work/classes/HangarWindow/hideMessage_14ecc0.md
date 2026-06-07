# HangarWindow::hideMessage
elf 0x14ecc0 body 8
Sig: undefined __stdcall hideMessage(void)

## decompile
```c

/* HangarWindow::hideMessage() */

void HangarWindow::hideMessage(void)

{
  int in_r0;
  
  *(undefined1 *)(in_r0 + 0x3c) = 0;
  return;
}

```

## target disasm
```
  0015ecc0: movs r1,#0x0
  0015ecc2: strb.w r1,[r0,#0x3c]
  0015ecc6: bx lr
```
