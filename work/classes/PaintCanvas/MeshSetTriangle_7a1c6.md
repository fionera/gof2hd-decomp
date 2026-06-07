# PaintCanvas::MeshSetTriangle
elf 0x7a1c6 body 60
Sig: undefined __stdcall MeshSetTriangle(uint param_1, ushort param_2, ushort param_3, ushort param_4, ushort param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetTriangle(unsigned int, unsigned short, unsigned short, unsigned
   short, unsigned short) */

void AbyssEngine::PaintCanvas::MeshSetTriangle
               (uint param_1,ushort param_2,ushort param_3,ushort param_4,ushort param_5)

{
  uint uVar1;
  int iVar2;
  undefined2 in_stack_00000004;
  
  uVar1 = (uint)param_3;
  if ((uint)param_2 < *(uint *)(param_1 + 0x24)) {
    iVar2 = *(int *)(*(int *)(param_1 + 0x28) + (uint)param_2 * 4);
    if ((uint)*(ushort *)(iVar2 + 0x28) <= uVar1 * 3) {
      return;
    }
    iVar2 = *(int *)(iVar2 + 0x2c);
    *(ushort *)(iVar2 + uVar1 * 6) = param_4;
    iVar2 = iVar2 + uVar1 * 6;
    *(ushort *)(iVar2 + 2) = param_5;
    *(undefined2 *)(iVar2 + 4) = in_stack_00000004;
  }
  return;
}

```

## target disasm
```
  0008a1c6: ldr.w r12,[r0,#0x24]
  0008a1ca: cmp r12,r1
  0008a1cc: bls 0x0008a200
  0008a1ce: ldr r0,[r0,#0x28]
  0008a1d0: ldr.w r12,[r0,r1,lsl #0x2]
  0008a1d4: add.w r0,r2,r2, lsl #0x1
  0008a1d8: ldrh.w r1,[r12,#0x28]
  0008a1dc: cmp r0,r1
  0008a1de: it cs
  0008a1e0: bx.cs lr
  0008a1e2: push {r7,lr}
  0008a1e4: mov r7,sp
  0008a1e6: ldrd r2,lr,[r7,#0x8]
  0008a1ea: ldr.w r1,[r12,#0x2c]
  0008a1ee: strh.w r3,[r1,r0,lsl #0x1]
  0008a1f2: add.w r0,r1,r0, lsl #0x1
  0008a1f6: strh r2,[r0,#0x2]
  0008a1f8: strh.w lr,[r0,#0x4]
  0008a1fc: pop.w {r7,lr}
  0008a200: bx lr
```
