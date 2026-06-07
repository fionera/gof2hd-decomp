# SimpleShader::Init
elf 0x88fdc body 70
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::SimpleShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::SimpleShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_00099024 + 0x98fea));
  iVar2 = DAT_0009902c;
  *(undefined4 *)(param_1 + 4) = uVar1;
  uVar1 = glGetAttribLocation(uVar1,iVar2 + 0x98ff6);
  iVar2 = DAT_00099030 + 0x99000;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00099034 + 0x9900e;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  (*(code *)(DAT_001ab7a4 + 0x1ab7a8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  00098fdc: push {r4,r6,r7,lr}
  00098fde: add r7,sp,#0x8
  00098fe0: ldr r1,[0x00099024]
  00098fe2: mov r4,r0
  00098fe4: ldr r2,[0x00099028]
  00098fe6: add r1,pc
  00098fe8: add r2,pc
  00098fea: blx 0x00070954
  00098fee: ldr r1,[0x0009902c]
  00098ff0: str r0,[r4,#0x4]
  00098ff2: add r1,pc
  00098ff4: blx 0x00070ac8
  00098ff8: ldr r1,[0x00099030]
  00098ffa: ldr r2,[r4,#0x4]
  00098ffc: add r1,pc
  00098ffe: str r0,[r4,#0x20]
  00099000: mov r0,r2
  00099002: blx 0x00070ad4
  00099006: ldr r1,[0x00099034]
  00099008: ldr r2,[r4,#0x4]
  0009900a: add r1,pc
  0009900c: str r0,[r4,#0x24]
  0009900e: mov r0,r2
  00099010: blx 0x00070ad4
  00099014: ldr r1,[r4,#0x4]
  00099016: str r0,[r4,#0x28]
  00099018: mov r0,r1
  0009901a: pop.w {r4,r6,r7,lr}
  0009901e: b.w 0x001ab798
```
