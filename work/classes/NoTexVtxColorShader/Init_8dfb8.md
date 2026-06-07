# NoTexVtxColorShader::Init
elf 0x8dfb8 body 84
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::NoTexVtxColorShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::NoTexVtxColorShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009e00c + 0x9dfc6));
  iVar2 = DAT_0009e014;
  *(undefined4 *)(param_1 + 4) = uVar1;
  uVar1 = glGetAttribLocation(uVar1,iVar2 + 0x9dfd2);
  iVar2 = DAT_0009e018 + 0x9dfdc;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e01c + 0x9dfea;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e020 + 0x9dff8;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  (*(code *)(DAT_001ab7a4 + 0x1ab7a8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  0009dfb8: push {r4,r6,r7,lr}
  0009dfba: add r7,sp,#0x8
  0009dfbc: ldr r1,[0x0009e00c]
  0009dfbe: mov r4,r0
  0009dfc0: ldr r2,[0x0009e010]
  0009dfc2: add r1,pc
  0009dfc4: add r2,pc
  0009dfc6: blx 0x00070954
  0009dfca: ldr r1,[0x0009e014]
  0009dfcc: str r0,[r4,#0x4]
  0009dfce: add r1,pc
  0009dfd0: blx 0x00070ac8
  0009dfd4: ldr r1,[0x0009e018]
  0009dfd6: ldr r2,[r4,#0x4]
  0009dfd8: add r1,pc
  0009dfda: str r0,[r4,#0x20]
  0009dfdc: mov r0,r2
  0009dfde: blx 0x00070ac8
  0009dfe2: ldr r1,[0x0009e01c]
  0009dfe4: ldr r2,[r4,#0x4]
  0009dfe6: add r1,pc
  0009dfe8: str r0,[r4,#0x24]
  0009dfea: mov r0,r2
  0009dfec: blx 0x00070ad4
  0009dff0: ldr r1,[0x0009e020]
  0009dff2: ldr r2,[r4,#0x4]
  0009dff4: add r1,pc
  0009dff6: str r0,[r4,#0x28]
  0009dff8: mov r0,r2
  0009dffa: blx 0x00070ad4
  0009dffe: ldr r1,[r4,#0x4]
  0009e000: str r0,[r4,#0x2c]
  0009e002: mov r0,r1
  0009e004: pop.w {r4,r6,r7,lr}
  0009e008: b.w 0x001ab798
```
