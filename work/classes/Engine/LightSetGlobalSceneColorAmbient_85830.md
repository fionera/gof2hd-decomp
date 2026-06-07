# Engine::LightSetGlobalSceneColorAmbient
elf 0x85830 body 154
Sig: undefined __stdcall LightSetGlobalSceneColorAmbient(float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::Engine::LightSetGlobalSceneColorAmbient(float, float, float) */

void AbyssEngine::Engine::LightSetGlobalSceneColorAmbient(float param_1,float param_2,float param_3)

{
  int in_r0;
  float in_r1;
  int iVar1;
  float in_r2;
  int iVar2;
  float in_r3;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  puVar4 = (undefined4 *)(DAT_000958cc + 0x95842);
  *(float *)(in_r0 + 0x288) = in_r1;
  *(float *)(in_r0 + 0x28c) = in_r2;
  *(float *)(in_r0 + 0x290) = in_r3;
  *(undefined4 *)(in_r0 + 0x294) = 0x3f800000;
  if (*(char *)*puVar4 == '\0') {
    (*(code *)(&LAB_001ab688 + DAT_001ab684))(0xb53,in_r0 + 0x288);
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
    *(float *)(iVar2 + 0x2cc) = (*(float *)(iVar5 + 0x268) + in_r1) * *(float *)(in_r0 + 0x2a8);
    *(float *)(iVar2 + 0x2d0) = (*(float *)(iVar5 + 0x26c) + in_r2) * *(float *)(in_r0 + 0x2ac);
    *(float *)(iVar2 + 0x2d4) = (*(float *)(iVar5 + 0x270) + in_r3) * *(float *)(in_r0 + 0x2b0);
  }
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  00095830: push {r4,r6,r7,lr}
  00095832: add r7,sp,#0x8
  00095834: ldr r4,[0x000958cc]
  00095836: mov.w r12,#0x3f800000
  0009583a: add.w lr,r0,#0x288
  0009583e: add r4,pc
  00095840: stm lr,{r1,r2,r3,r12}
  00095844: ldr r4,[r4,#0x0]
  00095846: ldrb r4,[r4,#0x0]
  00095848: cbz r4,0x000958ba
  0009584a: ldr.w r12,[r0,#0x32c]
  0009584e: vmov s0,r3
  00095852: vmov s4,r1
  00095856: mov.w lr,#0x0
  0009585a: vmov s2,r2
  0009585e: movs r3,#0x0
  00095860: movs r1,#0x0
  00095862: b 0x000958ae
  00095864: add.w r4,r0,lr
  00095868: vldr.32 s8,[r0,#0x2a8]
  0009586c: adds r2,r0,r3
  0009586e: add.w lr,lr,#0x10
  00095872: vldr.32 s6,[r4,#0x268]
  00095876: adds r3,#0xc
  00095878: adds r1,#0x1
  0009587a: vadd.f32 s6,s6,s4
  0009587e: vmul.f32 s6,s6,s8
  00095882: vstr.32 s6,[r2,#0x2cc]
  00095886: vldr.32 s6,[r4,#0x26c]
  0009588a: vldr.32 s8,[r0,#0x2ac]
  0009588e: vadd.f32 s6,s6,s2
  00095892: vmul.f32 s6,s6,s8
  00095896: vstr.32 s6,[r2,#0x2d0]
  0009589a: vldr.32 s6,[r4,#0x270]
  0009589e: vldr.32 s8,[r0,#0x2b0]
  000958a2: vadd.f32 s6,s6,s0
  000958a6: vmul.f32 s6,s6,s8
  000958aa: vstr.32 s6,[r2,#0x2d4]
  000958ae: cmp r1,r12
  000958b0: blt 0x00095864
  000958b2: pop.w {r4,r6,r7,lr}
  000958b6: b.w 0x001ab658
  000958ba: add.w r1,r0,#0x288
  000958be: movw r0,#0xb53
  000958c2: pop.w {r4,r6,r7,lr}
  000958c6: b.w 0x001ab678
```
