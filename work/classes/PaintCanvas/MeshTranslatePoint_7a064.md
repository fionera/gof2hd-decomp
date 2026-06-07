# PaintCanvas::MeshTranslatePoint
elf 0x7a064 body 82
Sig: undefined __thiscall MeshTranslatePoint(PaintCanvas * this, uint param_1, ushort param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshTranslatePoint(unsigned int, unsigned short, float, float, float)
    */

float __thiscall
AbyssEngine::PaintCanvas::MeshTranslatePoint
          (PaintCanvas *this,uint param_1,ushort param_2,float param_3,float param_4,float param_5)

{
  int iVar1;
  float *pfVar2;
  float in_r3;
  float in_stack_00000000;
  float in_stack_00000004;
  
  if (param_1 < *(uint *)(this + 0x24)) {
    iVar1 = *(int *)(*(int *)(this + 0x28) + param_1 * 4);
    if ((uint)*(ushort *)(iVar1 + 2) <= (uint)param_2) {
      return param_3;
    }
    pfVar2 = (float *)(*(int *)(iVar1 + 4) + (uint)param_2 * 0xc);
    param_3 = pfVar2[2] + in_stack_00000004;
    *pfVar2 = *pfVar2 + in_r3;
    pfVar2[1] = pfVar2[1] + in_stack_00000000;
    pfVar2[2] = param_3;
  }
  return param_3;
}

```

## target disasm
```
  0008a064: ldr.w r12,[r0,#0x24]
  0008a068: cmp r12,r1
  0008a06a: bls 0x0008a0b4
  0008a06c: ldr r0,[r0,#0x28]
  0008a06e: ldr.w r0,[r0,r1,lsl #0x2]
  0008a072: ldrh r1,[r0,#0x2]
  0008a074: cmp r1,r2
  0008a076: it ls
  0008a078: bx.ls lr
  0008a07a: ldr r0,[r0,#0x4]
  0008a07c: add.w r1,r2,r2, lsl #0x1
  0008a080: vldr.32 s0,[sp,#0x4]
  0008a084: vmov s4,r3
  0008a088: add.w r0,r0,r1, lsl #0x2
  0008a08c: vldr.32 s2,[sp]
  0008a090: vldr.32 s6,[r0]
  0008a094: vldr.32 s8,[r0,#0x4]
  0008a098: vldr.32 s10,[r0,#0x8]
  0008a09c: vadd.f32 s4,s6,s4
  0008a0a0: vadd.f32 s2,s8,s2
  0008a0a4: vadd.f32 s0,s10,s0
  0008a0a8: vstr.32 s4,[r0]
  0008a0ac: vstr.32 s2,[r0,#0x4]
  0008a0b0: vstr.32 s0,[r0,#0x8]
  0008a0b4: bx lr
```
