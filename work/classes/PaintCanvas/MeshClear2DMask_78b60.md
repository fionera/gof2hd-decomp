# PaintCanvas::MeshClear2DMask
elf 0x78b60 body 6
Sig: undefined __thiscall MeshClear2DMask(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshClear2DMask() */

void __thiscall AbyssEngine::PaintCanvas::MeshClear2DMask(PaintCanvas *this)

{
  *(undefined4 *)(this + 0x20) = 0;
  return;
}

```

## target disasm
```
  00088b60: movs r1,#0x0
  00088b62: str r1,[r0,#0x20]
  00088b64: bx lr
```
