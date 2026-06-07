# PaintCanvas::Vibrate
elf 0x7c2f6 body 6
Sig: undefined __stdcall Vibrate(ushort param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::Vibrate(unsigned short) */

void AbyssEngine::PaintCanvas::Vibrate(ushort param_1)

{
  (*(code *)(DAT_001ab334 + 0x1ab338))(*(undefined4 *)(param_1 + 0x34));
  return;
}

```

## target disasm
```
  0008c2f6: ldr r0,[r0,#0x34]
  0008c2f8: b.w 0x001ab328
```
