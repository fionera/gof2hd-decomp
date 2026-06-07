# PaintCanvas::MeshSetNormal
elf 0x7a202 body 48
Sig: undefined __thiscall MeshSetNormal(PaintCanvas * this, uint param_1, ushort param_2, Vector * param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetNormal(unsigned int, unsigned short, AbyssEngine::AEMath::Vector
   const&) */

void __thiscall
AbyssEngine::PaintCanvas::MeshSetNormal
          (PaintCanvas *this,uint param_1,ushort param_2,Vector *param_3)

{
  int iVar1;
  
  if (param_1 < *(uint *)(this + 0x24)) {
    iVar1 = *(int *)(*(int *)(this + 0x28) + param_1 * 4);
    if ((uint)*(ushort *)(iVar1 + 2) <= (uint)param_2) {
      return;
    }
    AEMath::Vector::operator=((Vector *)(*(int *)(iVar1 + 0x10) + (uint)param_2 * 0xc),param_3);
  }
  return;
}

```

## target disasm
```
  0008a202: ldr.w r12,[r0,#0x24]
  0008a206: cmp r12,r1
  0008a208: bls 0x0008a230
  0008a20a: ldr r0,[r0,#0x28]
  0008a20c: ldr.w r0,[r0,r1,lsl #0x2]
  0008a210: ldrh r1,[r0,#0x2]
  0008a212: cmp r1,r2
  0008a214: it ls
  0008a216: bx.ls lr
  0008a218: push {r7,lr}
  0008a21a: mov r7,sp
  0008a21c: ldr r0,[r0,#0x10]
  0008a21e: add.w r1,r2,r2, lsl #0x1
  0008a222: add.w r0,r0,r1, lsl #0x2
  0008a226: mov r1,r3
  0008a228: blx 0x0006eb3c
  0008a22c: pop.w {r7,lr}
  0008a230: bx lr
```
