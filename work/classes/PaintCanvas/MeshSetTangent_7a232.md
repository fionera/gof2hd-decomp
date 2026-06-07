# PaintCanvas::MeshSetTangent
elf 0x7a232 body 48
Sig: undefined __thiscall MeshSetTangent(PaintCanvas * this, uint param_1, ushort param_2, Vector * param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetTangent(unsigned int, unsigned short,
   AbyssEngine::AEMath::Vector const&) */

void __thiscall
AbyssEngine::PaintCanvas::MeshSetTangent
          (PaintCanvas *this,uint param_1,ushort param_2,Vector *param_3)

{
  int iVar1;
  
  if (param_1 < *(uint *)(this + 0x24)) {
    iVar1 = *(int *)(*(int *)(this + 0x28) + param_1 * 4);
    if ((uint)*(ushort *)(iVar1 + 2) <= (uint)param_2) {
      return;
    }
    AEMath::Vector::operator=((Vector *)(*(int *)(iVar1 + 0x14) + (uint)param_2 * 0xc),param_3);
  }
  return;
}

```

## target disasm
```
  0008a232: ldr.w r12,[r0,#0x24]
  0008a236: cmp r12,r1
  0008a238: bls 0x0008a260
  0008a23a: ldr r0,[r0,#0x28]
  0008a23c: ldr.w r0,[r0,r1,lsl #0x2]
  0008a240: ldrh r1,[r0,#0x2]
  0008a242: cmp r1,r2
  0008a244: it ls
  0008a246: bx.ls lr
  0008a248: push {r7,lr}
  0008a24a: mov r7,sp
  0008a24c: ldr r0,[r0,#0x14]
  0008a24e: add.w r1,r2,r2, lsl #0x1
  0008a252: add.w r0,r0,r1, lsl #0x2
  0008a256: mov r1,r3
  0008a258: blx 0x0006eb3c
  0008a25c: pop.w {r7,lr}
  0008a260: bx lr
```
