# PaintCanvas::SpriteSystemGetPosition
elf 0x7a9a2 body 52
Sig: undefined __thiscall SpriteSystemGetPosition(PaintCanvas * this, uint param_1, ushort param_2, Vector * param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemGetPosition(unsigned int, unsigned short,
   AbyssEngine::AEMath::Vector&) */

void __thiscall
AbyssEngine::PaintCanvas::SpriteSystemGetPosition
          (PaintCanvas *this,uint param_1,ushort param_2,Vector *param_3)

{
  ushort *puVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = (uint)param_2;
  if ((param_1 < *(uint *)(this + 0x180)) &&
     (puVar1 = *(ushort **)(*(int *)(this + 0x184) + param_1 * 4), puVar1 != (ushort *)0x0)) {
    if (*puVar1 <= uVar3) {
      return;
    }
    iVar2 = *(int *)(puVar1 + 2) + uVar3 * 0xc;
    *(undefined4 *)param_3 = *(undefined4 *)(*(int *)(puVar1 + 2) + uVar3 * 0xc);
    *(undefined4 *)(param_3 + 4) = *(undefined4 *)(iVar2 + 4);
    *(undefined4 *)(param_3 + 8) = *(undefined4 *)(iVar2 + 8);
  }
  return;
}

```

## target disasm
```
  0008a9a2: ldr.w r12,[r0,#0x180]
  0008a9a6: cmp r12,r1
  0008a9a8: bls 0x0008a9d4
  0008a9aa: ldr.w r0,[r0,#0x184]
  0008a9ae: ldr.w r0,[r0,r1,lsl #0x2]
  0008a9b2: cbz r0,0x0008a9d4
  0008a9b4: ldrh r1,[r0,#0x0]
  0008a9b6: cmp r1,r2
  0008a9b8: it ls
  0008a9ba: bx.ls lr
  0008a9bc: ldr r0,[r0,#0x4]
  0008a9be: add.w r1,r2,r2, lsl #0x1
  0008a9c2: ldr.w r2,[r0,r1,lsl #0x2]
  0008a9c6: add.w r0,r0,r1, lsl #0x2
  0008a9ca: str r2,[r3,#0x0]
  0008a9cc: ldr r1,[r0,#0x4]
  0008a9ce: str r1,[r3,#0x4]
  0008a9d0: ldr r0,[r0,#0x8]
  0008a9d2: str r0,[r3,#0x8]
  0008a9d4: bx lr
```
