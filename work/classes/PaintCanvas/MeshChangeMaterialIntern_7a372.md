# PaintCanvas::MeshChangeMaterialIntern
elf 0x7a372 body 16
Sig: undefined __thiscall MeshChangeMaterialIntern(PaintCanvas * this, Mesh * param_1, Material * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshChangeMaterialIntern(AbyssEngine::Mesh*, AbyssEngine::Material*) */

void __thiscall
AbyssEngine::PaintCanvas::MeshChangeMaterialIntern
          (PaintCanvas *this,Mesh *param_1,Material *param_2)

{
  if ((param_1 != (Mesh *)0x0) && (param_2 != (Material *)0x0)) {
    *(Material **)(param_1 + 0x30) = param_2;
    (*(code *)(DAT_001ab264 + 0x1ab268))(this,*(undefined4 *)(param_1 + 0x34));
    return;
  }
  return;
}

```

## target disasm
```
  0008a372: cbz r1,0x0008a37e
  0008a374: cbz r2,0x0008a37e
  0008a376: str r2,[r1,#0x30]
  0008a378: ldr r1,[r1,#0x34]
  0008a37a: b.w 0x001ab258
  0008a37e: bx lr
  001ab258: bx pc
```
