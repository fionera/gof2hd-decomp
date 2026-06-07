# PaintCanvas::MeshConvertToVBO
elf 0x7a292 body 20
Sig: undefined __thiscall MeshConvertToVBO(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshConvertToVBO(unsigned int) */

void __thiscall AbyssEngine::PaintCanvas::MeshConvertToVBO(PaintCanvas *this,uint param_1)

{
  if (param_1 < *(uint *)(this + 0x24)) {
    (*(code *)(DAT_001ab244 + 0x1ab248))(*(undefined4 *)(*(int *)(this + 0x28) + param_1 * 4));
    return;
  }
  return;
}

```

## target disasm
```
  0008a292: ldr r2,[r0,#0x24]
  0008a294: cmp r2,r1
  0008a296: it ls
  0008a298: bx.ls lr
  0008a29a: ldr r0,[r0,#0x28]
  0008a29c: ldr.w r0,[r0,r1,lsl #0x2]
  0008a2a0: b.w 0x001ab238
  001ab238: bx pc
```
