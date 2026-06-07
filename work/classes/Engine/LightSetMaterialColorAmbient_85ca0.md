# Engine::LightSetMaterialColorAmbient
elf 0x85ca0 body 158
Sig: undefined __stdcall LightSetMaterialColorAmbient(float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::Engine::LightSetMaterialColorAmbient(float, float, float) */

void AbyssEngine::Engine::LightSetMaterialColorAmbient(float param_1,float param_2,float param_3)

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
  
  pcVar4 = *(char **)(DAT_00095d40 + 0x95cb2);
  *(float *)(in_r0 + 0x2a8) = in_r1;
  *(float *)(in_r0 + 0x2ac) = in_r2;
  *(float *)(in_r0 + 0x2b0) = in_r3;
  *(undefined4 *)(in_r0 + 0x2b4) = *(undefined4 *)(in_r0 + 0x488);
  if (*pcVar4 == '\0') {
    (*(code *)(DAT_001ab6a4 + 0x1ab6a8))(0x408,0x1200,in_r0 + 0x2a8);
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
    *(float *)(iVar2 + 0x2cc) = (*(float *)(in_r0 + 0x288) + *(float *)(iVar5 + 0x268)) * in_r1;
    *(float *)(iVar2 + 0x2d0) = (*(float *)(in_r0 + 0x28c) + *(float *)(iVar5 + 0x26c)) * in_r2;
    *(float *)(iVar2 + 0x2d4) = (*(float *)(in_r0 + 0x290) + *(float *)(iVar5 + 0x270)) * in_r3;
  }
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  00095ca0: push {r4,r6,r7,lr}
  00095ca2: add r7,sp,#0x8
  00095ca4: ldr r4,[0x00095d40]
  00095ca6: add.w lr,r0,#0x2a8
  00095caa: ldr.w r12,[r0,#0x488]
  00095cae: add r4,pc
  00095cb0: ldr r4,[r4,#0x0]
  00095cb2: stm lr,{r1,r2,r3,r12}
  00095cb6: ldrb r4,[r4,#0x0]
  00095cb8: cbz r4,0x00095d2a
  00095cba: ldr.w r12,[r0,#0x32c]
  00095cbe: vmov s0,r3
  00095cc2: vmov s4,r1
  00095cc6: mov.w lr,#0x0
  00095cca: vmov s2,r2
  00095cce: movs r3,#0x0
  00095cd0: movs r1,#0x0
  00095cd2: b 0x00095d1e
  00095cd4: add.w r4,r0,lr
  00095cd8: vldr.32 s6,[r0,#0x288]
  00095cdc: adds r2,r0,r3
  00095cde: add.w lr,lr,#0x10
  00095ce2: vldr.32 s8,[r4,#0x268]
  00095ce6: adds r3,#0xc
  00095ce8: adds r1,#0x1
  00095cea: vadd.f32 s6,s6,s8
  00095cee: vmul.f32 s6,s6,s4
  00095cf2: vstr.32 s6,[r2,#0x2cc]
  00095cf6: vldr.32 s6,[r4,#0x26c]
  00095cfa: vldr.32 s8,[r0,#0x28c]
  00095cfe: vadd.f32 s6,s8,s6
  00095d02: vmul.f32 s6,s6,s2
  00095d06: vstr.32 s6,[r2,#0x2d0]
  00095d0a: vldr.32 s6,[r4,#0x270]
  00095d0e: vldr.32 s8,[r0,#0x290]
  00095d12: vadd.f32 s6,s8,s6
  00095d16: vmul.f32 s6,s6,s0
  00095d1a: vstr.32 s6,[r2,#0x2d4]
  00095d1e: cmp r1,r12
  00095d20: blt 0x00095cd4
  00095d22: pop.w {r4,r6,r7,lr}
  00095d26: b.w 0x001ab658
  00095d2a: add.w r2,r0,#0x2a8
  00095d2e: mov.w r0,#0x408
  00095d32: mov.w r1,#0x1200
  00095d36: pop.w {r4,r6,r7,lr}
  00095d3a: b.w 0x001ab698
```
