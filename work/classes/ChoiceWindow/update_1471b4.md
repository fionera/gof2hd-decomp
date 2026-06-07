# ChoiceWindow::update
elf 0x1471b4 body 8
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* ChoiceWindow::update(int) */

void ChoiceWindow::update(int param_1)

{
  (*(code *)(DAT_001ac484 + 0x1ac488))(*(undefined4 *)(param_1 + 0x1c));
  return;
}

```

## target disasm
```
  001571b4: ldr r0,[r0,#0x1c]
  001571b6: b.w 0x001ac478
  001ac478: bx pc
```
