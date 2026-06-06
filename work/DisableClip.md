# AbyssEngine::PaintCanvas::DisableClip

ELF vaddr 0x7cb36, body 8 bytes
Signature: undefined __stdcall DisableClip(void)

## Ghidra decompile
```c

/* AbyssEngine::PaintCanvas::DisableClip() */

void AbyssEngine::PaintCanvas::DisableClip(void)

{
  (*(code *)(DAT_001ab394 + 0x1ab398))(0xc11);
  return;
}

```

## Target disassembly (must match)
```
  0008cb36: movw r0,#0xc11
  0008cb3a: b.w 0x001ab388
```

Verify with: tools/try.sh <yourfile.cpp> DisableClip 7cb36 32
