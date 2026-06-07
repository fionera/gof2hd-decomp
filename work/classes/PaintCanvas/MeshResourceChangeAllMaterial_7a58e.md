# PaintCanvas::MeshResourceChangeAllMaterial
elf 0x7a58e body 46
Sig: undefined __thiscall MeshResourceChangeAllMaterial(PaintCanvas * this, ushort param_1, ushort param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshResourceChangeAllMaterial(unsigned short, unsigned short) */

void __thiscall
AbyssEngine::PaintCanvas::MeshResourceChangeAllMaterial
          (PaintCanvas *this,ushort param_1,ushort param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *(int *)(this + 0x134);
  for (iVar2 = 0; iVar3 != iVar2; iVar2 = iVar2 + 1) {
    iVar1 = *(int *)(*(int *)(this + 0x138) + iVar2 * 4);
    if ((iVar1 != 0) && (iVar1 = *(int *)(iVar1 + 0xc), *(ushort *)(iVar1 + 4) == param_1)) {
      *(ushort *)(iVar1 + 4) = param_2;
    }
  }
  return;
}

```

## target disasm
```
  0008a58e: push {r4,r6,r7,lr}
  0008a590: add r7,sp,#0x8
  0008a592: ldr.w r12,[r0,#0x134]
  0008a596: movs r4,#0x0
  0008a598: b 0x0008a5b6
  0008a59a: ldr.w r3,[r0,#0x138]
  0008a59e: ldr.w r3,[r3,r4,lsl #0x2]
  0008a5a2: cbz r3,0x0008a5b4
  0008a5a4: ldr.w lr,[r3,#0xc]
  0008a5a8: ldrh.w r3,[lr,#0x4]
  0008a5ac: cmp r3,r1
  0008a5ae: it eq
  0008a5b0: strh.eq.w r2,[lr,#0x4]
  0008a5b4: adds r4,#0x1
  0008a5b6: cmp r12,r4
  0008a5b8: bne 0x0008a59a
  0008a5ba: pop {r4,r6,r7,pc}
```
