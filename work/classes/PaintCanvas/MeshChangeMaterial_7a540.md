# PaintCanvas::MeshChangeMaterial
elf 0x7a540 body 36
Sig: undefined __thiscall MeshChangeMaterial(PaintCanvas * this, uint param_1, ushort param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshChangeMaterial(unsigned int, unsigned short) */

void __thiscall
AbyssEngine::PaintCanvas::MeshChangeMaterial(PaintCanvas *this,uint param_1,ushort param_2)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  
  uVar1 = (uint)param_2;
  bVar3 = uVar1 <= *(uint *)(this + 0x174);
  bVar2 = *(uint *)(this + 0x174) == uVar1;
  if (bVar3 && !bVar2) {
    bVar3 = param_1 <= *(uint *)(this + 0x24);
    bVar2 = *(uint *)(this + 0x24) == param_1;
  }
  if (!bVar3 || bVar2) {
    return;
  }
  (*(code *)(DAT_001ab274 + 0x1ab278))
            (this,*(undefined4 *)(*(int *)(this + 0x28) + param_1 * 4),
             *(undefined4 *)(*(int *)(this + 0x178) + uVar1 * 4));
  return;
}

```

## target disasm
```
  0008a540: ldr.w r3,[r0,#0x174]
  0008a544: cmp r3,r2
  0008a546: itte hi
  0008a548: ldr.hi r3,[r0,#0x24]
  0008a54a: cmp.hi r3,r1
  0008a54c: bx.ls lr
  0008a54e: ldr.w r12,[r0,#0x28]
  0008a552: ldr.w r3,[r0,#0x178]
  0008a556: ldr.w r1,[r12,r1,lsl #0x2]
  0008a55a: ldr.w r2,[r3,r2,lsl #0x2]
  0008a55e: b.w 0x001ab268
  001ab268: bx pc
```
