# PaintCanvas::StartDraw2FBO
elf 0x7d088 body 8
Sig: undefined __thiscall StartDraw2FBO(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::StartDraw2FBO() */

void __thiscall AbyssEngine::PaintCanvas::StartDraw2FBO(PaintCanvas *this)

{
  (*(code *)(DAT_001ab3f4 + 0x1ab3f8))(*(undefined4 *)(this + 0x34));
  return;
}

```

## target disasm
```
  0008d088: ldr r0,[r0,#0x34]
  0008d08a: b.w 0x001ab3e8
  001ab3e8: bx pc
```
