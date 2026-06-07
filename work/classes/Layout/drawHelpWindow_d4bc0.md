# Layout::drawHelpWindow
elf 0xd4bc0 body 10
Sig: undefined __thiscall drawHelpWindow(Layout * this)

## decompile
```c

/* Layout::drawHelpWindow() */

void __thiscall Layout::drawHelpWindow(Layout *this)

{
  (*(code *)(DAT_001ac0c4 + 0x1ac0c8))(*(undefined4 *)(this + 0x3c4));
  return;
}

```

## target disasm
```
  000e4bc0: ldr.w r0,[r0,#0x3c4]
  000e4bc4: b.w 0x001ac0b8
  001ac0b8: bx pc
```
