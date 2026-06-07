# GreenShader::Init
elf 0x882a0 body 216
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::GreenShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::GreenShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_00098378 + 0x982ae));
  iVar2 = DAT_00098384;
  puVar3 = (undefined4 *)(DAT_00098380 + 0x982ba);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x982be);
  iVar2 = DAT_00098388 + 0x982c8;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009838c + 0x982d4;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00098390 + 0x982e0;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00098394 + 0x982ec;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_00098398 + 0x982f8);
  iVar2 = DAT_0009839c + 0x982fc;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983a0 + 0x9830a;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983a4 + 0x98316;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983a8 + 0x98322;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983ac + 0x9832e;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983b0 + 0x9833a;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983b4 + 0x98346;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983b8 + 0x98352;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000983bc + 0x9835e;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x44),0);
  return;
}

```

## target disasm
```
  000982a0: push {r4,r5,r7,lr}
  000982a2: add r7,sp,#0x8
  000982a4: ldr r1,[0x00098378]
  000982a6: mov r4,r0
  000982a8: ldr r2,[0x0009837c]
  000982aa: add r1,pc
  000982ac: add r2,pc
  000982ae: blx 0x00070954
  000982b2: ldr r2,[0x00098380]
  000982b4: ldr r1,[0x00098384]
  000982b6: add r2,pc
  000982b8: str r0,[r4,#0x4]
  000982ba: add r1,pc
  000982bc: ldr r5,[r2,#0x0]
  000982be: blx r5
  000982c0: ldr r1,[0x00098388]
  000982c2: ldr r2,[r4,#0x4]
  000982c4: add r1,pc
  000982c6: str r0,[r4,#0x20]
  000982c8: mov r0,r2
  000982ca: blx r5
  000982cc: ldr r1,[0x0009838c]
  000982ce: ldr r2,[r4,#0x4]
  000982d0: add r1,pc
  000982d2: str r0,[r4,#0x24]
  000982d4: mov r0,r2
  000982d6: blx r5
  000982d8: ldr r1,[0x00098390]
  000982da: ldr r2,[r4,#0x4]
  000982dc: add r1,pc
  000982de: str r0,[r4,#0x28]
  000982e0: mov r0,r2
  000982e2: blx r5
  000982e4: ldr r1,[0x00098394]
  000982e6: ldr r2,[r4,#0x4]
  000982e8: add r1,pc
  000982ea: str r0,[r4,#0x2c]
  000982ec: mov r0,r2
  000982ee: blx r5
  000982f0: ldr r3,[0x00098398]
  000982f2: ldr r1,[0x0009839c]
  000982f4: add r3,pc
  000982f6: ldr r2,[r4,#0x4]
  000982f8: add r1,pc
  000982fa: str r0,[r4,#0x30]
  000982fc: ldr r5,[r3,#0x0]
  000982fe: mov r0,r2
  00098300: blx r5
  00098302: ldr r1,[0x000983a0]
  00098304: ldr r2,[r4,#0x4]
  00098306: add r1,pc
  00098308: str r0,[r4,#0x34]
  0009830a: mov r0,r2
  0009830c: blx r5
  0009830e: ldr r1,[0x000983a4]
  00098310: ldr r2,[r4,#0x4]
  00098312: add r1,pc
  00098314: str r0,[r4,#0x38]
  00098316: mov r0,r2
  00098318: blx r5
  0009831a: ldr r1,[0x000983a8]
  0009831c: ldr r2,[r4,#0x4]
  0009831e: add r1,pc
  00098320: str r0,[r4,#0x3c]
  00098322: mov r0,r2
  00098324: blx r5
  00098326: ldr r1,[0x000983ac]
  00098328: ldr r2,[r4,#0x4]
  0009832a: add r1,pc
  0009832c: str r0,[r4,#0x40]
  0009832e: mov r0,r2
  00098330: blx r5
  00098332: ldr r1,[0x000983b0]
  00098334: ldr r2,[r4,#0x4]
  00098336: add r1,pc
  00098338: str r0,[r4,#0x44]
  0009833a: mov r0,r2
  0009833c: blx r5
  0009833e: ldr r1,[0x000983b4]
  00098340: ldr r2,[r4,#0x4]
  00098342: add r1,pc
  00098344: str r0,[r4,#0x48]
  00098346: mov r0,r2
  00098348: blx r5
  0009834a: ldr r1,[0x000983b8]
  0009834c: ldr r2,[r4,#0x4]
  0009834e: add r1,pc
  00098350: str r0,[r4,#0x4c]
  00098352: mov r0,r2
  00098354: blx r5
  00098356: ldr r1,[0x000983bc]
  00098358: ldr r2,[r4,#0x4]
  0009835a: add r1,pc
  0009835c: str r0,[r4,#0x50]
  0009835e: mov r0,r2
  00098360: blx r5
  00098362: ldr r1,[r4,#0x4]
  00098364: str r0,[r4,#0x54]
  00098366: mov r0,r1
  00098368: blx 0x00070960
  0009836c: ldr r0,[r4,#0x44]
  0009836e: movs r1,#0x0
  00098370: pop.w {r4,r5,r7,lr}
  00098374: b.w 0x001ab768
```
