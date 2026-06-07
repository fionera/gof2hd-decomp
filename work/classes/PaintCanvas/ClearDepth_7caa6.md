# PaintCanvas::ClearDepth
elf 0x7caa6 body 8
Sig: undefined __stdcall ClearDepth(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::ClearDepth() */

void AbyssEngine::PaintCanvas::ClearDepth(void)

{
  (*(code *)(DAT_001ab094 + 0x1ab098))(0x100);
  return;
}

```

## target disasm
```
  0008caa6: mov.w r0,#0x100
  0008caaa: b.w 0x001ab088
```
