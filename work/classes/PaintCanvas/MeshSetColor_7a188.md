# PaintCanvas::MeshSetColor
elf 0x7a188 body 62
Sig: undefined __thiscall MeshSetColor(PaintCanvas * this, uint param_1, ushort param_2, float param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetColor(unsigned int, unsigned short, float, float, float, float)
    */

float __thiscall
AbyssEngine::PaintCanvas::MeshSetColor
          (PaintCanvas *this,uint param_1,ushort param_2,float param_3,float param_4,float param_5,
          float param_6)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 in_r3;
  undefined4 in_stack_00000000;
  undefined4 in_stack_00000004;
  float in_stack_00000008;
  
  if (param_1 < *(uint *)(this + 0x24)) {
    iVar1 = *(int *)(*(int *)(this + 0x28) + param_1 * 4);
    if ((uint)*(ushort *)(iVar1 + 2) <= (uint)param_2) {
      return param_3;
    }
    puVar2 = (undefined4 *)(*(int *)(iVar1 + 0xc) + (uint)param_2 * 0x10);
    *puVar2 = in_r3;
    puVar2[1] = in_stack_00000000;
    puVar2[2] = in_stack_00000004;
    puVar2[3] = in_stack_00000008;
    param_3 = in_stack_00000008;
  }
  return param_3;
}

```

## target disasm
```
  0008a188: ldr.w r12,[r0,#0x24]
  0008a18c: cmp r12,r1
  0008a18e: bls 0x0008a1c4
  0008a190: ldr r0,[r0,#0x28]
  0008a192: ldr.w r0,[r0,r1,lsl #0x2]
  0008a196: ldrh r1,[r0,#0x2]
  0008a198: cmp r1,r2
  0008a19a: it ls
  0008a19c: bx.ls lr
  0008a19e: ldr r0,[r0,#0xc]
  0008a1a0: vmov s6,r3
  0008a1a4: vldr.32 s0,[sp,#0x8]
  0008a1a8: add.w r0,r0,r2, lsl #0x4
  0008a1ac: vldr.32 s2,[sp,#0x4]
  0008a1b0: vldr.32 s4,[sp]
  0008a1b4: vstr.32 s6,[r0]
  0008a1b8: vstr.32 s4,[r0,#0x4]
  0008a1bc: vstr.32 s2,[r0,#0x8]
  0008a1c0: vstr.32 s0,[r0,#0xc]
  0008a1c4: bx lr
```
