# GlowShader::Init
elf 0x91fc4 body 106
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::GlowShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::GlowShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a2030 + 0xa1fd2));
  iVar2 = DAT_000a2038;
  *(undefined4 *)(param_1 + 4) = uVar1;
  uVar1 = glGetAttribLocation(uVar1,iVar2 + 0xa1fde);
  iVar2 = DAT_000a203c + 0xa1fe8;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = glGetAttribLocation(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_000a2040 + 0xa1ff6);
  iVar2 = DAT_000a2044 + 0xa1ffa;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a2048 + 0xa2008;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a204c + 0xa2014;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x30),0);
  return;
}

```

## target disasm
```
  000a1fc4: push {r4,r5,r7,lr}
  000a1fc6: add r7,sp,#0x8
  000a1fc8: ldr r1,[0x000a2030]
  000a1fca: mov r4,r0
  000a1fcc: ldr r2,[0x000a2034]
  000a1fce: add r1,pc
  000a1fd0: add r2,pc
  000a1fd2: blx 0x00070954
  000a1fd6: ldr r1,[0x000a2038]
  000a1fd8: str r0,[r4,#0x4]
  000a1fda: add r1,pc
  000a1fdc: blx 0x00070ac8
  000a1fe0: ldr r1,[0x000a203c]
  000a1fe2: ldr r2,[r4,#0x4]
  000a1fe4: add r1,pc
  000a1fe6: str r0,[r4,#0x20]
  000a1fe8: mov r0,r2
  000a1fea: blx 0x00070ac8
  000a1fee: ldr r3,[0x000a2040]
  000a1ff0: ldr r1,[0x000a2044]
  000a1ff2: add r3,pc
  000a1ff4: ldr r2,[r4,#0x4]
  000a1ff6: add r1,pc
  000a1ff8: str r0,[r4,#0x24]
  000a1ffa: ldr r5,[r3,#0x0]
  000a1ffc: mov r0,r2
  000a1ffe: blx r5
  000a2000: ldr r1,[0x000a2048]
  000a2002: ldr r2,[r4,#0x4]
  000a2004: add r1,pc
  000a2006: str r0,[r4,#0x28]
  000a2008: mov r0,r2
  000a200a: blx r5
  000a200c: ldr r1,[0x000a204c]
  000a200e: ldr r2,[r4,#0x4]
  000a2010: add r1,pc
  000a2012: str r0,[r4,#0x2c]
  000a2014: mov r0,r2
  000a2016: blx r5
  000a2018: ldr r1,[r4,#0x4]
  000a201a: str r0,[r4,#0x30]
  000a201c: mov r0,r1
  000a201e: blx 0x00070960
  000a2022: ldr r0,[r4,#0x30]
  000a2024: movs r1,#0x0
  000a2026: pop.w {r4,r5,r7,lr}
  000a202a: b.w 0x001ab768
```
