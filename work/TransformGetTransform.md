# AbyssEngine::PaintCanvas::TransformGetTransform

ELF vaddr 0x7b67c, body 8 bytes
Signature: undefined __stdcall TransformGetTransform(uint param_1)

## Ghidra decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AbyssEngine::PaintCanvas::TransformGetTransform(unsigned int) */

void AbyssEngine::PaintCanvas::TransformGetTransform(uint param_1)

{
  char in_NG;
  char in_OV;
  
  if (in_NG != in_OV) {
    software_interrupt(0x92428a);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## Target disassembly (must match)
```
  0008b67c: ldrsbcs pc,[r8,#-0x80]
  0008b680: swilt 0x92428a
```

Verify with: tools/try.sh <yourfile.cpp> TransformGetTransform 7b67c 32
