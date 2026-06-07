# PaintCanvas::DisableClip
elf 0x7cb36 body 8
Sig: undefined __stdcall DisableClip(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::DisableClip() */

void AbyssEngine::PaintCanvas::DisableClip(void)

{
  (*(code *)(DAT_001ab394 + 0x1ab398))(0xc11);
  return;
}

```

## target disasm
```
  0008cb36: movw r0,#0xc11
  0008cb3a: b.w 0x001ab388
```
