# AbyssEngine::PaintCanvas::StartDraw2FBO

ELF vaddr 0x7d088, body 8 bytes
Signature: undefined __thiscall StartDraw2FBO(PaintCanvas * this)

## Ghidra decompile
```c

/* AbyssEngine::PaintCanvas::StartDraw2FBO() */

void __thiscall AbyssEngine::PaintCanvas::StartDraw2FBO(PaintCanvas *this)

{
  (*(code *)(DAT_001ab3f4 + 0x1ab3f8))(*(undefined4 *)(this + 0x34));
  return;
}

```

## Target disassembly (must match)
```
  0008d088: ldr r0,[r0,#0x34]
  0008d08a: b.w 0x001ab3e8
  001ab3e8: bx pc
```

Verify: tools/try.sh work/src/StartDraw2FBO.cpp StartDraw2FBO 7d088 32
