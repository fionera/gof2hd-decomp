# PaintCanvas::SetShaderMode
elf 0x7cb3e body 8
Sig: undefined __thiscall SetShaderMode(PaintCanvas * this, int param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetShaderMode(int) */

void __thiscall AbyssEngine::PaintCanvas::SetShaderMode(PaintCanvas *this,int param_1)

{
  *(int *)(*(int *)(this + 0x34) + 0x4a8) = param_1;
  return;
}

```

## target disasm
```
  0008cb3e: ldr r0,[r0,#0x34]
  0008cb40: str.w r1,[r0,#0x4a8]
  0008cb44: bx lr
```
