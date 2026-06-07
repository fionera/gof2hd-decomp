# Engine::LightSetMaterialColorDiffuse
elf 0x85c14 body 134
Sig: undefined __stdcall LightSetMaterialColorDiffuse(float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::Engine::LightSetMaterialColorDiffuse(float, float, float) */

void AbyssEngine::Engine::LightSetMaterialColorDiffuse(float param_1,float param_2,float param_3)

{
  int in_r0;
  float in_r1;
  int iVar1;
  float in_r2;
  int iVar2;
  float in_r3;
  int iVar3;
  char *pcVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  pcVar4 = *(char **)(DAT_00095c9c + 0x95c26);
  *(float *)(in_r0 + 0x298) = in_r1;
  *(float *)(in_r0 + 0x29c) = in_r2;
  *(float *)(in_r0 + 0x2a0) = in_r3;
  *(undefined4 *)(in_r0 + 0x2a4) = *(undefined4 *)(in_r0 + 0x488);
  if (*pcVar4 == '\0') {
    (*(code *)(DAT_001ab6a4 + 0x1ab6a8))(0x408,0x1201,in_r0 + 0x298);
    return;
  }
  iVar6 = *(int *)(in_r0 + 0x32c);
  iVar7 = 0;
  iVar3 = 0;
  for (iVar1 = 0; iVar1 < iVar6; iVar1 = iVar1 + 1) {
    iVar5 = in_r0 + iVar7;
    iVar2 = in_r0 + iVar3;
    iVar7 = iVar7 + 0x10;
    iVar3 = iVar3 + 0xc;
    *(float *)(iVar2 + 0x2fc) = *(float *)(iVar5 + 0x228) * in_r1;
    *(float *)(iVar2 + 0x300) = *(float *)(iVar5 + 0x22c) * in_r2;
    *(float *)(iVar2 + 0x304) = *(float *)(iVar5 + 0x230) * in_r3;
  }
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  00095c14: push {r4,r6,r7,lr}
  00095c16: add r7,sp,#0x8
  00095c18: ldr r4,[0x00095c9c]
  00095c1a: add.w lr,r0,#0x298
  00095c1e: ldr.w r12,[r0,#0x488]
  00095c22: add r4,pc
  00095c24: ldr r4,[r4,#0x0]
  00095c26: stm lr,{r1,r2,r3,r12}
  00095c2a: ldrb r4,[r4,#0x0]
  00095c2c: cbz r4,0x00095c86
  00095c2e: ldr.w r12,[r0,#0x32c]
  00095c32: vmov s0,r3
  00095c36: vmov s4,r1
  00095c3a: mov.w lr,#0x0
  00095c3e: vmov s2,r2
  00095c42: movs r3,#0x0
  00095c44: movs r1,#0x0
  00095c46: b 0x00095c7a
  00095c48: add.w r4,r0,lr
  00095c4c: adds r2,r0,r3
  00095c4e: add.w lr,lr,#0x10
  00095c52: adds r3,#0xc
  00095c54: vldr.32 s6,[r4,#0x228]
  00095c58: adds r1,#0x1
  00095c5a: vmul.f32 s6,s6,s4
  00095c5e: vstr.32 s6,[r2,#0x2fc]
  00095c62: vldr.32 s6,[r4,#0x22c]
  00095c66: vmul.f32 s6,s6,s2
  00095c6a: vstr.32 s6,[r2,#0x300]
  00095c6e: vldr.32 s6,[r4,#0x230]
  00095c72: vmul.f32 s6,s6,s0
  00095c76: vstr.32 s6,[r2,#0x304]
  00095c7a: cmp r1,r12
  00095c7c: blt 0x00095c48
  00095c7e: pop.w {r4,r6,r7,lr}
  00095c82: b.w 0x001ab658
  00095c86: add.w r2,r0,#0x298
  00095c8a: mov.w r0,#0x408
  00095c8e: movw r1,#0x1201
  00095c92: pop.w {r4,r6,r7,lr}
  00095c96: b.w 0x001ab698
```
