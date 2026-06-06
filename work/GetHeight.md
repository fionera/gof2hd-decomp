# AbyssEngine::PaintCanvas::GetHeight

ELF vaddr 0x78fba, body 8 bytes
Signature: undefined __stdcall GetHeight(void)

## Ghidra decompile
```c

/* AbyssEngine::PaintCanvas::GetHeight() */

void AbyssEngine::PaintCanvas::GetHeight(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab1e4 + 0x1ab1e8))(*(undefined4 *)(in_r0 + 0x34));
  return;
}

```

## Target disassembly (must match)
```
  00088fba: ldr r0,[r0,#0x34]
  00088fbc: b.w 0x001ab1d8
  001ab1d8: bx pc
```

Verify with: tools/try.sh <yourfile.cpp> GetHeight 78fba 32
