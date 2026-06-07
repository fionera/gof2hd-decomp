# BumpShader::Init
elf 0x8b880 body 358
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009b9e8 + 0x9b88e));
  iVar2 = DAT_0009b9f4;
  puVar3 = (undefined4 *)(DAT_0009b9f0 + 0x9b89a);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9b89e);
  iVar2 = DAT_0009b9f8 + 0x9b8a8;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b9fc + 0x9b8b4;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009ba00 + 0x9b8c0);
  iVar2 = DAT_0009ba04 + 0x9b8c4;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba08 + 0x9b8d2;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba0c + 0x9b8de;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba10 + 0x9b8ea;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba14 + 0x9b8f6;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba18 + 0x9b902;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba1c + 0x9b90e;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba20 + 0x9b91a;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba24 + 0x9b926;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba28 + 0x9b932;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba2c + 0x9b93e;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba30 + 0x9b94a;
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba34 + 0x9b956;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba38 + 0x9b962;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba3c + 0x9b96e;
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba40 + 0x9b97a;
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba44 + 0x9b986;
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba48 + 0x9b992;
  *(undefined4 *)(param_1 + 0x6c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba4c + 0x9b99e;
  *(undefined4 *)(param_1 + 0x70) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba50 + 0x9b9aa;
  *(undefined4 *)(param_1 + 0x74) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba54 + 0x9b9b6;
  *(undefined4 *)(param_1 + 0x78) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ba58 + 0x9b9c2;
  *(undefined4 *)(param_1 + 0x7c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x80) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  glUniform1i(*(undefined4 *)(param_1 + 0x40),0);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x44),7);
  return;
}

```

## target disasm
```
  0009b880: push {r4,r5,r7,lr}
  0009b882: add r7,sp,#0x8
  0009b884: ldr r1,[0x0009b9e8]
  0009b886: mov r4,r0
  0009b888: ldr r2,[0x0009b9ec]
  0009b88a: add r1,pc
  0009b88c: add r2,pc
  0009b88e: blx 0x00070954
  0009b892: ldr r2,[0x0009b9f0]
  0009b894: ldr r1,[0x0009b9f4]
  0009b896: add r2,pc
  0009b898: str r0,[r4,#0x4]
  0009b89a: add r1,pc
  0009b89c: ldr r5,[r2,#0x0]
  0009b89e: blx r5
  0009b8a0: ldr r1,[0x0009b9f8]
  0009b8a2: ldr r2,[r4,#0x4]
  0009b8a4: add r1,pc
  0009b8a6: str r0,[r4,#0x20]
  0009b8a8: mov r0,r2
  0009b8aa: blx r5
  0009b8ac: ldr r1,[0x0009b9fc]
  0009b8ae: ldr r2,[r4,#0x4]
  0009b8b0: add r1,pc
  0009b8b2: str r0,[r4,#0x24]
  0009b8b4: mov r0,r2
  0009b8b6: blx r5
  0009b8b8: ldr r3,[0x0009ba00]
  0009b8ba: ldr r1,[0x0009ba04]
  0009b8bc: add r3,pc
  0009b8be: ldr r2,[r4,#0x4]
  0009b8c0: add r1,pc
  0009b8c2: str r0,[r4,#0x28]
  0009b8c4: ldr r5,[r3,#0x0]
  0009b8c6: mov r0,r2
  0009b8c8: blx r5
  0009b8ca: ldr r1,[0x0009ba08]
  0009b8cc: ldr r2,[r4,#0x4]
  0009b8ce: add r1,pc
  0009b8d0: str r0,[r4,#0x2c]
  0009b8d2: mov r0,r2
  0009b8d4: blx r5
  0009b8d6: ldr r1,[0x0009ba0c]
  0009b8d8: ldr r2,[r4,#0x4]
  0009b8da: add r1,pc
  0009b8dc: str r0,[r4,#0x30]
  0009b8de: mov r0,r2
  0009b8e0: blx r5
  0009b8e2: ldr r1,[0x0009ba10]
  0009b8e4: ldr r2,[r4,#0x4]
  0009b8e6: add r1,pc
  0009b8e8: str r0,[r4,#0x34]
  0009b8ea: mov r0,r2
  0009b8ec: blx r5
  0009b8ee: ldr r1,[0x0009ba14]
  0009b8f0: ldr r2,[r4,#0x4]
  0009b8f2: add r1,pc
  0009b8f4: str r0,[r4,#0x38]
  0009b8f6: mov r0,r2
  0009b8f8: blx r5
  0009b8fa: ldr r1,[0x0009ba18]
  0009b8fc: ldr r2,[r4,#0x4]
  0009b8fe: add r1,pc
  0009b900: str r0,[r4,#0x3c]
  0009b902: mov r0,r2
  0009b904: blx r5
  0009b906: ldr r1,[0x0009ba1c]
  0009b908: ldr r2,[r4,#0x4]
  0009b90a: add r1,pc
  0009b90c: str r0,[r4,#0x40]
  0009b90e: mov r0,r2
  0009b910: blx r5
  0009b912: ldr r1,[0x0009ba20]
  0009b914: ldr r2,[r4,#0x4]
  0009b916: add r1,pc
  0009b918: str r0,[r4,#0x44]
  0009b91a: mov r0,r2
  0009b91c: blx r5
  0009b91e: ldr r1,[0x0009ba24]
  0009b920: ldr r2,[r4,#0x4]
  0009b922: add r1,pc
  0009b924: str r0,[r4,#0x48]
  0009b926: mov r0,r2
  0009b928: blx r5
  0009b92a: ldr r1,[0x0009ba28]
  0009b92c: ldr r2,[r4,#0x4]
  0009b92e: add r1,pc
  0009b930: str r0,[r4,#0x4c]
  0009b932: mov r0,r2
  0009b934: blx r5
  0009b936: ldr r1,[0x0009ba2c]
  0009b938: ldr r2,[r4,#0x4]
  0009b93a: add r1,pc
  0009b93c: str r0,[r4,#0x54]
  0009b93e: mov r0,r2
  0009b940: blx r5
  0009b942: ldr r1,[0x0009ba30]
  0009b944: ldr r2,[r4,#0x4]
  0009b946: add r1,pc
  0009b948: str r0,[r4,#0x5c]
  0009b94a: mov r0,r2
  0009b94c: blx r5
  0009b94e: ldr r1,[0x0009ba34]
  0009b950: ldr r2,[r4,#0x4]
  0009b952: add r1,pc
  0009b954: str r0,[r4,#0x50]
  0009b956: mov r0,r2
  0009b958: blx r5
  0009b95a: ldr r1,[0x0009ba38]
  0009b95c: ldr r2,[r4,#0x4]
  0009b95e: add r1,pc
  0009b960: str r0,[r4,#0x58]
  0009b962: mov r0,r2
  0009b964: blx r5
  0009b966: ldr r1,[0x0009ba3c]
  0009b968: ldr r2,[r4,#0x4]
  0009b96a: add r1,pc
  0009b96c: str r0,[r4,#0x60]
  0009b96e: mov r0,r2
  0009b970: blx r5
  0009b972: ldr r1,[0x0009ba40]
  0009b974: ldr r2,[r4,#0x4]
  0009b976: add r1,pc
  0009b978: str r0,[r4,#0x64]
  0009b97a: mov r0,r2
  0009b97c: blx r5
  0009b97e: ldr r1,[0x0009ba44]
  0009b980: ldr r2,[r4,#0x4]
  0009b982: add r1,pc
  0009b984: str r0,[r4,#0x68]
  0009b986: mov r0,r2
  0009b988: blx r5
  0009b98a: ldr r1,[0x0009ba48]
  0009b98c: ldr r2,[r4,#0x4]
  0009b98e: add r1,pc
  0009b990: str r0,[r4,#0x6c]
  0009b992: mov r0,r2
  0009b994: blx r5
  0009b996: ldr r1,[0x0009ba4c]
  0009b998: ldr r2,[r4,#0x4]
  0009b99a: add r1,pc
  0009b99c: str r0,[r4,#0x70]
  0009b99e: mov r0,r2
  0009b9a0: blx r5
  0009b9a2: ldr r1,[0x0009ba50]
  0009b9a4: ldr r2,[r4,#0x4]
  0009b9a6: add r1,pc
  0009b9a8: str r0,[r4,#0x74]
  0009b9aa: mov r0,r2
  0009b9ac: blx r5
  0009b9ae: ldr r1,[0x0009ba54]
  0009b9b0: ldr r2,[r4,#0x4]
  0009b9b2: add r1,pc
  0009b9b4: str r0,[r4,#0x78]
  0009b9b6: mov r0,r2
  0009b9b8: blx r5
  0009b9ba: ldr r1,[0x0009ba58]
  0009b9bc: ldr r2,[r4,#0x4]
  0009b9be: add r1,pc
  0009b9c0: str r0,[r4,#0x7c]
  0009b9c2: mov r0,r2
  0009b9c4: blx r5
  0009b9c6: ldr r1,[r4,#0x4]
  0009b9c8: str.w r0,[r4,#0x80]
  0009b9cc: mov r0,r1
  0009b9ce: blx 0x00070960
  0009b9d2: ldr r0,[r4,#0x40]
  0009b9d4: movs r1,#0x0
  0009b9d6: blx 0x0007096c
  0009b9da: ldr r0,[r4,#0x44]
  0009b9dc: movs r1,#0x7
  0009b9de: pop.w {r4,r5,r7,lr}
  0009b9e2: b.w 0x001ab768
```
