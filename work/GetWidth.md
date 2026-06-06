# AbyssEngine::PaintCanvas::GetWidth

ELF vaddr 0x78fb4, body 8 bytes
Signature: undefined __stdcall GetWidth(void)

## Ghidra decompile
```c

/* AbyssEngine::PaintCanvas::GetWidth() */

void AbyssEngine::PaintCanvas::GetWidth(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab1d4 + 0x1ab1d8))(*(undefined4 *)(in_r0 + 0x34));
  return;
}

```

## Target disassembly (must match)
```
  00088fb4: ldr r0,[r0,#0x34]
  00088fb6: b.w 0x001ab1c8
  001ab1c8: bx pc
```

Verify with: tools/try.sh <yourfile.cpp> GetWidth 78fb4 32
