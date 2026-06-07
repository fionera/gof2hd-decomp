# SpecCubeMapping::Init
elf 0x9286c body 266
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::SpecCubeMapping::Init(AbyssEngine::Engine*) */

void AbyssEngine::SpecCubeMapping::Init(Engine *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  iVar1 = ShaderBaseStruct::LoadBindShader((char *)param_1,(char *)(DAT_000a296c + 0xa287a));
  *(int *)(param_1 + 4) = iVar1;
  if (iVar1 == 0) {
    iVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a2974 + 0xa288e));
    *(int *)(param_1 + 4) = iVar1;
  }
  pcVar4 = *(code **)(DAT_000a297c + 0xa289c);
  uVar2 = (*pcVar4)(iVar1,DAT_000a2980 + 0xa289e);
  iVar1 = DAT_000a2984 + 0xa28a8;
  *(undefined4 *)(param_1 + 0x20) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a2988 + 0xa28b4;
  *(undefined4 *)(param_1 + 0x24) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  puVar3 = (undefined4 *)(DAT_000a298c + 0xa28c0);
  iVar1 = DAT_000a2990 + 0xa28c4;
  *(undefined4 *)(param_1 + 0x28) = uVar2;
  pcVar4 = (code *)*puVar3;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a2994 + 0xa28d2;
  *(undefined4 *)(param_1 + 0x2c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a2998 + 0xa28de;
  *(undefined4 *)(param_1 + 0x30) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a299c + 0xa28ea;
  *(undefined4 *)(param_1 + 0x34) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29a0 + 0xa28f6;
  *(undefined4 *)(param_1 + 0x38) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29a4 + 0xa2902;
  *(undefined4 *)(param_1 + 0x40) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29a8 + 0xa290e;
  *(undefined4 *)(param_1 + 0x3c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29ac + 0xa291a;
  *(undefined4 *)(param_1 + 0x44) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29b0 + 0xa2926;
  *(undefined4 *)(param_1 + 0x48) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29b4 + 0xa2932;
  *(undefined4 *)(param_1 + 0x4c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29b8 + 0xa293e;
  *(undefined4 *)(param_1 + 0x50) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_000a29bc + 0xa294a;
  *(undefined4 *)(param_1 + 0x54) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  *(undefined4 *)(param_1 + 0x58) = uVar2;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  glUniform1i(*(undefined4 *)(param_1 + 0x40),0);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x3c),1);
  return;
}

```

## target disasm
```
  000a286c: push {r4,r5,r7,lr}
  000a286e: add r7,sp,#0x8
  000a2870: ldr r1,[0x000a296c]
  000a2872: mov r4,r0
  000a2874: ldr r2,[0x000a2970]
  000a2876: add r1,pc
  000a2878: add r2,pc
  000a287a: blx 0x00070948
  000a287e: cmp r0,#0x0
  000a2880: str r0,[r4,#0x4]
  000a2882: bne 0x000a2894
  000a2884: ldr r1,[0x000a2974]
  000a2886: mov r0,r4
  000a2888: ldr r2,[0x000a2978]
  000a288a: add r1,pc
  000a288c: add r2,pc
  000a288e: blx 0x00070954
  000a2892: str r0,[r4,#0x4]
  000a2894: ldr r2,[0x000a297c]
  000a2896: ldr r1,[0x000a2980]
  000a2898: add r2,pc
  000a289a: add r1,pc
  000a289c: ldr r5,[r2,#0x0]
  000a289e: blx r5
  000a28a0: ldr r1,[0x000a2984]
  000a28a2: ldr r2,[r4,#0x4]
  000a28a4: add r1,pc
  000a28a6: str r0,[r4,#0x20]
  000a28a8: mov r0,r2
  000a28aa: blx r5
  000a28ac: ldr r1,[0x000a2988]
  000a28ae: ldr r2,[r4,#0x4]
  000a28b0: add r1,pc
  000a28b2: str r0,[r4,#0x24]
  000a28b4: mov r0,r2
  000a28b6: blx r5
  000a28b8: ldr r3,[0x000a298c]
  000a28ba: ldr r1,[0x000a2990]
  000a28bc: add r3,pc
  000a28be: ldr r2,[r4,#0x4]
  000a28c0: add r1,pc
  000a28c2: str r0,[r4,#0x28]
  000a28c4: ldr r5,[r3,#0x0]
  000a28c6: mov r0,r2
  000a28c8: blx r5
  000a28ca: ldr r1,[0x000a2994]
  000a28cc: ldr r2,[r4,#0x4]
  000a28ce: add r1,pc
  000a28d0: str r0,[r4,#0x2c]
  000a28d2: mov r0,r2
  000a28d4: blx r5
  000a28d6: ldr r1,[0x000a2998]
  000a28d8: ldr r2,[r4,#0x4]
  000a28da: add r1,pc
  000a28dc: str r0,[r4,#0x30]
  000a28de: mov r0,r2
  000a28e0: blx r5
  000a28e2: ldr r1,[0x000a299c]
  000a28e4: ldr r2,[r4,#0x4]
  000a28e6: add r1,pc
  000a28e8: str r0,[r4,#0x34]
  000a28ea: mov r0,r2
  000a28ec: blx r5
  000a28ee: ldr r1,[0x000a29a0]
  000a28f0: ldr r2,[r4,#0x4]
  000a28f2: add r1,pc
  000a28f4: str r0,[r4,#0x38]
  000a28f6: mov r0,r2
  000a28f8: blx r5
  000a28fa: ldr r1,[0x000a29a4]
  000a28fc: ldr r2,[r4,#0x4]
  000a28fe: add r1,pc
  000a2900: str r0,[r4,#0x40]
  000a2902: mov r0,r2
  000a2904: blx r5
  000a2906: ldr r1,[0x000a29a8]
  000a2908: ldr r2,[r4,#0x4]
  000a290a: add r1,pc
  000a290c: str r0,[r4,#0x3c]
  000a290e: mov r0,r2
  000a2910: blx r5
  000a2912: ldr r1,[0x000a29ac]
  000a2914: ldr r2,[r4,#0x4]
  000a2916: add r1,pc
  000a2918: str r0,[r4,#0x44]
  000a291a: mov r0,r2
  000a291c: blx r5
  000a291e: ldr r1,[0x000a29b0]
  000a2920: ldr r2,[r4,#0x4]
  000a2922: add r1,pc
  000a2924: str r0,[r4,#0x48]
  000a2926: mov r0,r2
  000a2928: blx r5
  000a292a: ldr r1,[0x000a29b4]
  000a292c: ldr r2,[r4,#0x4]
  000a292e: add r1,pc
  000a2930: str r0,[r4,#0x4c]
  000a2932: mov r0,r2
  000a2934: blx r5
  000a2936: ldr r1,[0x000a29b8]
  000a2938: ldr r2,[r4,#0x4]
  000a293a: add r1,pc
  000a293c: str r0,[r4,#0x50]
  000a293e: mov r0,r2
  000a2940: blx r5
  000a2942: ldr r1,[0x000a29bc]
  000a2944: ldr r2,[r4,#0x4]
  000a2946: add r1,pc
  000a2948: str r0,[r4,#0x54]
  000a294a: mov r0,r2
  000a294c: blx r5
  000a294e: ldr r1,[r4,#0x4]
  000a2950: str r0,[r4,#0x58]
  000a2952: mov r0,r1
  000a2954: blx 0x00070960
  000a2958: ldr r0,[r4,#0x40]
  000a295a: movs r1,#0x0
  000a295c: blx 0x0007096c
  000a2960: ldr r0,[r4,#0x3c]
  000a2962: movs r1,#0x1
  000a2964: pop.w {r4,r5,r7,lr}
  000a2968: b.w 0x001ab768
  001ab768: bx pc
  001ab76c: ldr r12,[0x1ab774]
  001ab770: add pc,r12,pc
```
