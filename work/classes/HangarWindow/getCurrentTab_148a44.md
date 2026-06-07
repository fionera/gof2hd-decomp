# HangarWindow::getCurrentTab
elf 0x148a44 body 8
Sig: undefined __thiscall getCurrentTab(HangarWindow * this)

## decompile
```c

/* HangarWindow::getCurrentTab() */

void __thiscall HangarWindow::getCurrentTab(HangarWindow *this)

{
  (*(code *)(DAT_001ac4a4 + 0x1ac4a8))(*(undefined4 *)(this + 0x14));
  return;
}

```

## target disasm
```
  00158a44: ldr r0,[r0,#0x14]
  00158a46: b.w 0x001ac498
  001ac498: bx pc
```
