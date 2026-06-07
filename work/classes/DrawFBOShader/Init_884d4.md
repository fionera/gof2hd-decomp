# DrawFBOShader::Init
elf 0x884d4 body 92
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::DrawFBOShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::DrawFBOShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_00098530 + 0x984e2));
  iVar2 = DAT_00098538;
  *(undefined4 *)(param_1 + 4) = uVar1;
  uVar1 = glGetAttribLocation(uVar1,iVar2 + 0x984ee);
  iVar2 = DAT_0009853c + 0x984f8;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00098540 + 0x98506;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00098544 + 0x98514;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = glGetUniformLocation(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x54),0);
  return;
}

```

## target disasm
```
  000984d4: push {r4,r6,r7,lr}
  000984d6: add r7,sp,#0x8
  000984d8: ldr r1,[0x00098530]
  000984da: mov r4,r0
  000984dc: ldr r2,[0x00098534]
  000984de: add r1,pc
  000984e0: add r2,pc
  000984e2: blx 0x00070954
  000984e6: ldr r1,[0x00098538]
  000984e8: str r0,[r4,#0x4]
  000984ea: add r1,pc
  000984ec: blx 0x00070ac8
  000984f0: ldr r1,[0x0009853c]
  000984f2: ldr r2,[r4,#0x4]
  000984f4: add r1,pc
  000984f6: str r0,[r4,#0x48]
  000984f8: mov r0,r2
  000984fa: blx 0x00070ac8
  000984fe: ldr r1,[0x00098540]
  00098500: ldr r2,[r4,#0x4]
  00098502: add r1,pc
  00098504: str r0,[r4,#0x50]
  00098506: mov r0,r2
  00098508: blx 0x00070ad4
  0009850c: ldr r1,[0x00098544]
  0009850e: ldr r2,[r4,#0x4]
  00098510: add r1,pc
  00098512: str r0,[r4,#0x4c]
  00098514: mov r0,r2
  00098516: blx 0x00070ad4
  0009851a: ldr r1,[r4,#0x4]
  0009851c: str r0,[r4,#0x54]
  0009851e: mov r0,r1
  00098520: blx 0x00070960
  00098524: ldr r0,[r4,#0x54]
  00098526: movs r1,#0x0
  00098528: pop.w {r4,r6,r7,lr}
  0009852c: b.w 0x001ab768
```
