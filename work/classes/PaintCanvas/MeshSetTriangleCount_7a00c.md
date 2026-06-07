# PaintCanvas::MeshSetTriangleCount
elf 0x7a00c body 30
Sig: undefined __thiscall MeshSetTriangleCount(PaintCanvas * this, uint param_1, ushort param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetTriangleCount(unsigned int, unsigned short) */

void __thiscall
AbyssEngine::PaintCanvas::MeshSetTriangleCount(PaintCanvas *this,uint param_1,ushort param_2)

{
  ushort uVar1;
  int iVar2;
  
  if (*(uint *)(this + 0x24) <= param_1) {
    return;
  }
  iVar2 = *(int *)(*(int *)(this + 0x28) + param_1 * 4);
  uVar1 = *(ushort *)(iVar2 + 0x2a);
  if (uVar1 < param_2) {
    param_2 = uVar1;
  }
  *(ushort *)(iVar2 + 0x28) = param_2 * 3;
  return;
}

```

## target disasm
```
  0008a00c: ldr r3,[r0,#0x24]
  0008a00e: cmp r3,r1
  0008a010: it ls
  0008a012: bx.ls lr
  0008a014: ldr r0,[r0,#0x28]
  0008a016: ldr.w r0,[r0,r1,lsl #0x2]
  0008a01a: ldrh r1,[r0,#0x2a]
  0008a01c: cmp r1,r2
  0008a01e: it cc
  0008a020: mov.cc r2,r1
  0008a022: add.w r1,r2,r2, lsl #0x1
  0008a026: strh r1,[r0,#0x28]
  0008a028: bx lr
```
