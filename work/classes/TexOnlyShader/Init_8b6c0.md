# TexOnlyShader::Init
elf 0x8b6c0 body 92
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::TexOnlyShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::TexOnlyShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009b71c + 0x9b6ce));
  iVar2 = DAT_0009b724;
  *(undefined4 *)(param_1 + 4) = uVar1;
  uVar1 = glGetAttribLocation(uVar1,iVar2 + 0x9b6da);
  iVar2 = DAT_0009b728 + 0x9b6e4;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b72c + 0x9b6f2;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b730 + 0x9b700;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x2c),0);
  return;
}

```

## target disasm
```
  0009b6c0: push {r4,r6,r7,lr}
  0009b6c2: add r7,sp,#0x8
  0009b6c4: ldr r1,[0x0009b71c]
  0009b6c6: mov r4,r0
  0009b6c8: ldr r2,[0x0009b720]
  0009b6ca: add r1,pc
  0009b6cc: add r2,pc
  0009b6ce: blx 0x00070954
  0009b6d2: ldr r1,[0x0009b724]
  0009b6d4: str r0,[r4,#0x4]
  0009b6d6: add r1,pc
  0009b6d8: blx 0x00070ac8
  0009b6dc: ldr r1,[0x0009b728]
  0009b6de: ldr r2,[r4,#0x4]
  0009b6e0: add r1,pc
  0009b6e2: str r0,[r4,#0x20]
  0009b6e4: mov r0,r2
  0009b6e6: blx 0x00070ac8
  0009b6ea: ldr r1,[0x0009b72c]
  0009b6ec: ldr r2,[r4,#0x4]
  0009b6ee: add r1,pc
  0009b6f0: str r0,[r4,#0x24]
  0009b6f2: mov r0,r2
  0009b6f4: blx 0x00070ad4
  0009b6f8: ldr r1,[0x0009b730]
  0009b6fa: ldr r2,[r4,#0x4]
  0009b6fc: add r1,pc
  0009b6fe: str r0,[r4,#0x28]
  0009b700: mov r0,r2
  0009b702: blx 0x00070ad4
  0009b706: ldr r1,[r4,#0x4]
  0009b708: str r0,[r4,#0x2c]
  0009b70a: mov r0,r1
  0009b70c: blx 0x00070960
  0009b710: ldr r0,[r4,#0x2c]
  0009b712: movs r1,#0x0
  0009b714: pop.w {r4,r6,r7,lr}
  0009b718: b.w 0x001ab768
```
