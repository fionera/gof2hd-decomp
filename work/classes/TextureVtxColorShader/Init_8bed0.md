# TextureVtxColorShader::Init
elf 0x8bed0 body 70
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::TextureVtxColorShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::TextureVtxColorShader::Init(Engine *param_1)

{
  int iVar1;
  undefined4 uVar2;
  char *pcVar3;
  
  pcVar3 = (char *)(DAT_0009bf0c + 0x9bede);
  iVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,pcVar3);
  *(int *)(param_1 + 4) = iVar1;
  ConnectShaderComponents((uint)param_1,iVar1);
  uVar2 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,pcVar3);
  *(undefined4 *)(param_1 + 0x20) = uVar2;
  (*(code *)(DAT_001ab7c4 + 0x1ab7c8))(param_1,uVar2,1);
  return;
}

```

## target disasm
```
  0009bed0: push {r4,r5,r7,lr}
  0009bed2: add r7,sp,#0x8
  0009bed4: ldr r4,[0x0009bf0c]
  0009bed6: mov r5,r0
  0009bed8: ldr r2,[0x0009bf10]
  0009beda: add r4,pc
  0009bedc: add r2,pc
  0009bede: mov r1,r4
  0009bee0: blx 0x00070954
  0009bee4: mov r1,r0
  0009bee6: str r0,[r5,#0x4]
  0009bee8: mov r0,r5
  0009beea: movs r2,#0x0
  0009beec: blx 0x00070da4
  0009bef0: ldr r2,[0x0009bf14]
  0009bef2: mov r0,r5
  0009bef4: mov r1,r4
  0009bef6: add r2,pc
  0009bef8: blx 0x00070954
  0009befc: mov r1,r0
  0009befe: str r0,[r5,#0x20]
  0009bf00: mov r0,r5
  0009bf02: movs r2,#0x1
  0009bf04: pop.w {r4,r5,r7,lr}
  0009bf08: b.w 0x001ab7b8
  001ab7b8: bx pc
  001ab7bc: ldr r12,[0x1ab7c4]
  001ab7c0: add pc,r12,pc
```
