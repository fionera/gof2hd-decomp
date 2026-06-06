# AbyssEngine::PaintCanvas::End3d

ELF vaddr 0x7d53c, body 8 bytes
Signature: undefined __stdcall End3d(void)

## Ghidra decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AbyssEngine::PaintCanvas::End3d() */

void AbyssEngine::PaintCanvas::End3d(void)

{
  char in_NG;
  bool in_ZR;
  char in_OV;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0xe92d);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## Target disassembly (must match)
```
  0008d53c: swige 0x3b5f0
  0008d540: swieq 0xe92d
```

Verify: tools/try.sh work/src/End3d.cpp End3d 7d53c 32
