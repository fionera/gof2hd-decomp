# Trail::translate
elf 0x15c308 body 98
Sig: undefined __thiscall translate(Trail * this, Vector * param_1)

## decompile
```c

/* Trail::translate(AbyssEngine::AEMath::Vector const&) */

void __thiscall Trail::translate(Trail *this,Vector *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  uint in_fpscr;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  fVar3 = *(float *)param_1;
  fVar4 = *(float *)(param_1 + 4);
  fVar5 = *(float *)(param_1 + 8);
  for (iVar1 = 0; iVar1 < *(int *)(this + 0x20); iVar1 = iVar1 + 3) {
    puVar2 = (undefined4 *)(*(int *)(this + 0x18) + iVar1 * 4);
    fVar7 = (float)VectorSignedToFloat(puVar2[1],(byte)(in_fpscr >> 0x16) & 3);
    fVar8 = (float)VectorSignedToFloat(puVar2[2],(byte)(in_fpscr >> 0x16) & 3);
    fVar6 = (float)VectorSignedToFloat(*puVar2,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(*(int *)(this + 0x18) + iVar1 * 4) = (int)(fVar3 + fVar6);
    puVar2[1] = (int)(fVar4 + fVar7);
    puVar2[2] = (int)(fVar5 + fVar8);
  }
  return;
}

```

## target disasm
```
  0016c308: vldr.32 s0,[r1]
  0016c30c: vldr.32 s2,[r1,#0x4]
  0016c310: vldr.32 s4,[r1,#0x8]
  0016c314: movs r1,#0x0
  0016c316: ldr r2,[r0,#0x20]
  0016c318: cmp r1,r2
  0016c31a: it ge
  0016c31c: bx.ge lr
  0016c31e: ldr.w r12,[r0,#0x18]
  0016c322: add.w r3,r12,r1, lsl #0x2
  0016c326: vldr.32 s6,[r3]
  0016c32a: vldr.32 s8,[r3,#0x4]
  0016c32e: vldr.32 s10,[r3,#0x8]
  0016c332: vcvt.f32.s32 s8,s8
  0016c336: vcvt.f32.s32 s10,s10
  0016c33a: vcvt.f32.s32 s6,s6
  0016c33e: vadd.f32 s8,s2,s8
  0016c342: vadd.f32 s10,s4,s10
  0016c346: vadd.f32 s6,s0,s6
  0016c34a: vcvt.s32.f32 s8,s8
  0016c34e: vcvt.s32.f32 s10,s10
  0016c352: vcvt.s32.f32 s6,s6
  0016c356: vmov r2,s6
  0016c35a: str.w r2,[r12,r1,lsl #0x2]
  0016c35e: adds r1,#0x3
  0016c360: vstr.32 s8,[r3,#0x4]
  0016c364: vstr.32 s10,[r3,#0x8]
  0016c368: b 0x0016c316
```
