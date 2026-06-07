# PaintCanvas::HasVibration
elf 0x7c2f0 body 6
Sig: undefined __stdcall HasVibration(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::HasVibration() */

void AbyssEngine::PaintCanvas::HasVibration(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab324 + 0x1ab328))(*(undefined4 *)(in_r0 + 0x34));
  return;
}

```

## target disasm
```
  0008c2f0: ldr r0,[r0,#0x34]
  0008c2f2: b.w 0x001ab318
```
