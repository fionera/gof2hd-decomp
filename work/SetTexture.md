# AbyssEngine::PaintCanvas::SetTexture

ELF vaddr 0x78212, body 8 bytes
Signature: undefined __stdcall SetTexture(uint param_1, uint param_2)

## Ghidra decompile
```c

/* AbyssEngine::PaintCanvas::SetTexture(unsigned int, unsigned int) */

void AbyssEngine::PaintCanvas::SetTexture(uint param_1,uint param_2)

{
  (*(code *)(DAT_001ab1a4 + 0x1ab1a8))(*(undefined4 *)(param_1 + 0x34));
  return;
}

```

## Target disassembly (must match)
```
  00088212: ldr r0,[r0,#0x34]
  00088214: b.w 0x001ab198
  001ab198: bx pc
```

Verify with: tools/try.sh <yourfile.cpp> SetTexture 78212 32
