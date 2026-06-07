# BlurShader::Init
elf 0x8a68c body 142
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BlurShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::BlurShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009a71c + 0x9a69a));
  iVar2 = DAT_0009a724;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = glGetAttribLocation(uVar1,iVar2 + 0x9a6a6);
  iVar2 = DAT_0009a728 + 0x9a6b0;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 0x24),iVar2);
  puVar3 = (undefined4 *)(DAT_0009a72c + 0x9a6be);
  iVar2 = DAT_0009a730 + 0x9a6c2;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 0x24),iVar2);
  iVar2 = DAT_0009a734 + 0x9a6d0;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 0x24),iVar2);
  iVar2 = DAT_0009a738 + 0x9a6dc;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 0x24),iVar2);
  iVar2 = DAT_0009a73c + 0x9a6e8;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 0x24),iVar2);
  iVar2 = DAT_0009a740 + 0x9a6f4;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 0x24),iVar2);
  iVar2 = DAT_0009a744 + 0x9a700;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 0x24),iVar2);
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 0x24));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x34),0);
  return;
}

```

## target disasm
```
  0009a68c: push {r4,r5,r7,lr}
  0009a68e: add r7,sp,#0x8
  0009a690: ldr r1,[0x0009a71c]
  0009a692: mov r4,r0
  0009a694: ldr r2,[0x0009a720]
  0009a696: add r1,pc
  0009a698: add r2,pc
  0009a69a: blx 0x00070954
  0009a69e: ldr r1,[0x0009a724]
  0009a6a0: str r0,[r4,#0x24]
  0009a6a2: add r1,pc
  0009a6a4: blx 0x00070ac8
  0009a6a8: ldr r1,[0x0009a728]
  0009a6aa: ldr r2,[r4,#0x24]
  0009a6ac: add r1,pc
  0009a6ae: str r0,[r4,#0x28]
  0009a6b0: mov r0,r2
  0009a6b2: blx 0x00070ac8
  0009a6b6: ldr r3,[0x0009a72c]
  0009a6b8: ldr r1,[0x0009a730]
  0009a6ba: add r3,pc
  0009a6bc: ldr r2,[r4,#0x24]
  0009a6be: add r1,pc
  0009a6c0: str r0,[r4,#0x30]
  0009a6c2: ldr r5,[r3,#0x0]
  0009a6c4: mov r0,r2
  0009a6c6: blx r5
  0009a6c8: ldr r1,[0x0009a734]
  0009a6ca: ldr r2,[r4,#0x24]
  0009a6cc: add r1,pc
  0009a6ce: str r0,[r4,#0x2c]
  0009a6d0: mov r0,r2
  0009a6d2: blx r5
  0009a6d4: ldr r1,[0x0009a738]
  0009a6d6: ldr r2,[r4,#0x24]
  0009a6d8: add r1,pc
  0009a6da: str r0,[r4,#0x34]
  0009a6dc: mov r0,r2
  0009a6de: blx r5
  0009a6e0: ldr r1,[0x0009a73c]
  0009a6e2: ldr r2,[r4,#0x24]
  0009a6e4: add r1,pc
  0009a6e6: str r0,[r4,#0x38]
  0009a6e8: mov r0,r2
  0009a6ea: blx r5
  0009a6ec: ldr r1,[0x0009a740]
  0009a6ee: ldr r2,[r4,#0x24]
  0009a6f0: add r1,pc
  0009a6f2: str r0,[r4,#0x44]
  0009a6f4: mov r0,r2
  0009a6f6: blx r5
  0009a6f8: ldr r1,[0x0009a744]
  0009a6fa: ldr r2,[r4,#0x24]
  0009a6fc: add r1,pc
  0009a6fe: str r0,[r4,#0x3c]
  0009a700: mov r0,r2
  0009a702: blx r5
  0009a704: ldr r1,[r4,#0x24]
  0009a706: str r0,[r4,#0x40]
  0009a708: mov r0,r1
  0009a70a: blx 0x00070960
  0009a70e: ldr r0,[r4,#0x34]
  0009a710: movs r1,#0x0
  0009a712: pop.w {r4,r5,r7,lr}
  0009a716: b.w 0x001ab768
```
