# PaintCanvas::SpriteSystemGetPosition
elf 0x7a8e6 body 188
Sig: undefined __thiscall SpriteSystemGetPosition(PaintCanvas * this, uint param_1, ushort param_2, Matrix * param_3, Vector * param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemGetPosition(unsigned int, unsigned short,
   AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector&) */

void __thiscall
AbyssEngine::PaintCanvas::SpriteSystemGetPosition
          (PaintCanvas *this,uint param_1,ushort param_2,Matrix *param_3,Vector *param_4)

{
  float *pfVar1;
  ushort *puVar2;
  uint in_fpscr;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  
  if ((param_1 < *(uint *)(this + 0x180)) &&
     (puVar2 = *(ushort **)(*(int *)(this + 0x184) + param_1 * 4), puVar2 != (ushort *)0x0)) {
    if ((uint)*puVar2 <= (uint)param_2) {
      return;
    }
    fVar3 = *(float *)(param_3 + 0x14);
    pfVar1 = (float *)(*(int *)(puVar2 + 2) + (uint)param_2 * 0xc);
    fVar4 = *(float *)(param_3 + 0x24);
    fVar10 = pfVar1[1];
    fVar6 = *pfVar1;
    fVar9 = *(float *)(param_3 + 0x10);
    fVar11 = pfVar1[2];
    fVar5 = *(float *)(param_3 + 0x18);
    fVar12 = *(float *)(param_3 + 0x20);
    fVar7 = *(float *)(param_3 + 0x28);
    fVar13 = *(float *)(param_3 + 0x1c);
    fVar8 = (float)VectorSignedToFloat((int)**(short **)(puVar2 + 4) >> 1,
                                       (byte)(in_fpscr >> 0x16) & 3);
    fVar14 = *(float *)(param_3 + 0x2c);
    *(float *)param_4 =
         (*(float *)(param_3 + 0xc) +
         *(float *)(param_3 + 4) * fVar10 + *(float *)param_3 * fVar6 +
         *(float *)(param_3 + 8) * fVar11) - fVar8;
    *(float *)(param_4 + 4) = fVar13 + fVar10 * fVar3 + fVar6 * fVar9 + fVar11 * fVar5 + fVar8;
    *(float *)(param_4 + 8) = fVar14 + fVar10 * fVar4 + fVar6 * fVar12 + fVar11 * fVar7;
  }
  return;
}

```

## target disasm
```
  0008a8e6: ldr.w r12,[r0,#0x180]
  0008a8ea: cmp r12,r1
  0008a8ec: bls 0x0008a9a0
  0008a8ee: ldr.w r0,[r0,#0x184]
  0008a8f2: ldr.w r1,[r0,r1,lsl #0x2]
  0008a8f6: cmp r1,#0x0
  0008a8f8: beq 0x0008a9a0
  0008a8fa: ldrh r0,[r1,#0x0]
  0008a8fc: cmp r0,r2
  0008a8fe: it ls
  0008a900: bx.ls lr
  0008a902: ldrd r0,r1,[r1,#0x4]
  0008a906: add.w r2,r2,r2, lsl #0x1
  0008a90a: vldr.32 s0,[r3,#0x14]
  0008a90e: add.w r0,r0,r2, lsl #0x2
  0008a912: vldr.32 s10,[r3,#0x4]
  0008a916: vldr.32 s1,[r3,#0x24]
  0008a91a: vldr.32 s4,[r0,#0x4]
  0008a91e: vldr.32 s2,[r0]
  0008a922: vmul.f32 s0,s4,s0
  0008a926: vldr.32 s3,[r3,#0x10]
  0008a92a: vmul.f32 s10,s10,s4
  0008a92e: vldr.32 s8,[r3]
  0008a932: vmul.f32 s4,s4,s1
  0008a936: vldr.32 s6,[r0,#0x8]
  0008a93a: vldr.32 s12,[r3,#0x8]
  0008a93e: vldr.32 s1,[r3,#0x18]
  0008a942: ldrsh.w r0,[r1,#0x0]
  0008a946: vmla.f32 s0,s2,s3
  0008a94a: vldr.32 s14,[r3,#0xc]
  0008a94e: vmla.f32 s10,s8,s2
  0008a952: vldr.32 s8,[r3,#0x20]
  0008a956: asrs r0,r0,#0x1
  0008a958: ldr.w r12,[sp,#0x0]
  0008a95c: vmla.f32 s4,s2,s8
  0008a960: vldr.32 s2,[r3,#0x28]
  0008a964: vldr.32 s8,[r3,#0x1c]
  0008a968: vmla.f32 s0,s6,s1
  0008a96c: vmla.f32 s10,s12,s6
  0008a970: vmla.f32 s4,s6,s2
  0008a974: vmov s2,r0
  0008a978: vcvt.f32.s32 s2,s2
  0008a97c: vadd.f32 s0,s8,s0
  0008a980: vldr.32 s8,[r3,#0x2c]
  0008a984: vadd.f32 s6,s14,s10
  0008a988: vadd.f32 s4,s8,s4
  0008a98c: vadd.f32 s0,s0,s2
  0008a990: vsub.f32 s2,s6,s2
  0008a994: vstr.32 s2,[r12]
  0008a998: vstr.32 s0,[r12,#0x4]
  0008a99c: vstr.32 s4,[r12,#0x8]
  0008a9a0: bx lr
```
