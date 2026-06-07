# PaintCanvas::MeshResourceChangeMaterial
elf 0x7a562 body 44
Sig: undefined __thiscall MeshResourceChangeMaterial(PaintCanvas * this, ushort param_1, ushort param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshResourceChangeMaterial(unsigned short, unsigned short) */

void __thiscall
AbyssEngine::PaintCanvas::MeshResourceChangeMaterial
          (PaintCanvas *this,ushort param_1,ushort param_2)

{
  ushort uVar1;
  ushort *puVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = *(int *)(this + 0x134);
  for (iVar4 = 0; iVar3 != iVar4; iVar4 = iVar4 + 1) {
    puVar2 = *(ushort **)(*(int *)(this + 0x138) + iVar4 * 4);
    if (puVar2 != (ushort *)0x0) {
      uVar1 = *puVar2;
      if (uVar1 == param_1) {
        puVar2 = *(ushort **)(puVar2 + 6);
      }
      if (uVar1 == param_1) {
        puVar2[2] = param_2;
      }
    }
  }
  return;
}

```

## target disasm
```
  0008a562: push {r4,r6,r7,lr}
  0008a564: add r7,sp,#0x8
  0008a566: ldr.w r12,[r0,#0x134]
  0008a56a: mov.w lr,#0x0
  0008a56e: b 0x0008a588
  0008a570: ldr.w r3,[r0,#0x138]
  0008a574: ldr.w r3,[r3,lr,lsl #0x2]
  0008a578: cbz r3,0x0008a584
  0008a57a: ldrh r4,[r3,#0x0]
  0008a57c: cmp r4,r1
  0008a57e: itt eq
  0008a580: ldr.eq r3,[r3,#0xc]
  0008a582: strh.eq r2,[r3,#0x4]
  0008a584: add.w lr,lr,#0x1
  0008a588: cmp r12,lr
  0008a58a: bne 0x0008a570
  0008a58c: pop {r4,r6,r7,pc}
```
