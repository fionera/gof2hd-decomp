# PaintCanvas::TransformGetTransform
elf 0x7b67c body 8
Sig: undefined __stdcall TransformGetTransform(uint param_1)

## decompile
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

## target disasm
```
  0008b67c: ldrsbcs pc,[r8,#-0x80]
  0008b680: swilt 0x92428a
```
