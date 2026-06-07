# Engine::LightSetMaterialColorSpecular
elf 0x85d44 body 136
Sig: undefined __stdcall LightSetMaterialColorSpecular(float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::Engine::LightSetMaterialColorSpecular(float, float, float) */

void AbyssEngine::Engine::LightSetMaterialColorSpecular(float param_1,float param_2,float param_3)

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
  
  pcVar4 = *(char **)(DAT_00095dcc + 0x95d56);
  *(float *)(in_r0 + 0x2b8) = in_r1;
  *(float *)(in_r0 + 700) = in_r2;
  *(float *)(in_r0 + 0x2c0) = in_r3;
  *(undefined4 *)(in_r0 + 0x2c4) = *(undefined4 *)(in_r0 + 0x488);
  if (*pcVar4 == '\0') {
    (*(code *)(DAT_001ab6a4 + 0x1ab6a8))(0x408,0x1202,in_r0 + 0x2b8);
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
    *(float *)(iVar2 + 0x2e4) = *(float *)(iVar5 + 0x248) * in_r1;
    *(float *)(iVar2 + 0x2e8) = *(float *)(iVar5 + 0x24c) * in_r2;
    *(float *)(iVar2 + 0x2ec) = *(float *)(iVar5 + 0x250) * in_r3;
  }
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  00095d44: push {r4,r6,r7,lr}
  00095d46: add r7,sp,#0x8
  00095d48: ldr r4,[0x00095dcc]
  00095d4a: add.w lr,r0,#0x2b8
  00095d4e: ldr.w r12,[r0,#0x488]
  00095d52: add r4,pc
  00095d54: ldr r4,[r4,#0x0]
  00095d56: stm lr,{r1,r2,r3,r12}
  00095d5a: ldrb r4,[r4,#0x0]
  00095d5c: cbz r4,0x00095db6
  00095d5e: ldr.w r12,[r0,#0x32c]
  00095d62: vmov s0,r3
  00095d66: vmov s4,r1
  00095d6a: mov.w lr,#0x0
  00095d6e: vmov s2,r2
  00095d72: movs r3,#0x0
  00095d74: movs r1,#0x0
  00095d76: b 0x00095daa
  00095d78: add.w r4,r0,lr
  00095d7c: adds r2,r0,r3
  00095d7e: add.w lr,lr,#0x10
  00095d82: adds r3,#0xc
  00095d84: vldr.32 s6,[r4,#0x248]
  00095d88: adds r1,#0x1
  00095d8a: vmul.f32 s6,s6,s4
  00095d8e: vstr.32 s6,[r2,#0x2e4]
  00095d92: vldr.32 s6,[r4,#0x24c]
  00095d96: vmul.f32 s6,s6,s2
  00095d9a: vstr.32 s6,[r2,#0x2e8]
  00095d9e: vldr.32 s6,[r4,#0x250]
  00095da2: vmul.f32 s6,s6,s0
  00095da6: vstr.32 s6,[r2,#0x2ec]
  00095daa: cmp r1,r12
  00095dac: blt 0x00095d78
  00095dae: pop.w {r4,r6,r7,lr}
  00095db2: b.w 0x001ab658
  00095db6: add.w r2,r0,#0x2b8
  00095dba: mov.w r0,#0x408
  00095dbe: movw r1,#0x1202
  00095dc2: pop.w {r4,r6,r7,lr}
  00095dc6: b.w 0x001ab698
  001ab698: bx pc
```
