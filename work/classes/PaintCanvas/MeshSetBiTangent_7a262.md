# PaintCanvas::MeshSetBiTangent
elf 0x7a262 body 48
Sig: undefined __thiscall MeshSetBiTangent(PaintCanvas * this, uint param_1, ushort param_2, Vector * param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetBiTangent(unsigned int, unsigned short,
   AbyssEngine::AEMath::Vector const&) */

void __thiscall
AbyssEngine::PaintCanvas::MeshSetBiTangent
          (PaintCanvas *this,uint param_1,ushort param_2,Vector *param_3)

{
  int iVar1;
  
  if (param_1 < *(uint *)(this + 0x24)) {
    iVar1 = *(int *)(*(int *)(this + 0x28) + param_1 * 4);
    if ((uint)*(ushort *)(iVar1 + 2) <= (uint)param_2) {
      return;
    }
    AEMath::Vector::operator=((Vector *)(*(int *)(iVar1 + 0x18) + (uint)param_2 * 0xc),param_3);
  }
  return;
}

```

## target disasm
```
  0008a262: ldr.w r12,[r0,#0x24]
  0008a266: cmp r12,r1
  0008a268: bls 0x0008a290
  0008a26a: ldr r0,[r0,#0x28]
  0008a26c: ldr.w r0,[r0,r1,lsl #0x2]
  0008a270: ldrh r1,[r0,#0x2]
  0008a272: cmp r1,r2
  0008a274: it ls
  0008a276: bx.ls lr
  0008a278: push {r7,lr}
  0008a27a: mov r7,sp
  0008a27c: ldr r0,[r0,#0x18]
  0008a27e: add.w r1,r2,r2, lsl #0x1
  0008a282: add.w r0,r0,r1, lsl #0x2
  0008a286: mov r1,r3
  0008a288: blx 0x0006eb3c
  0008a28c: pop.w {r7,lr}
  0008a290: bx lr
```
