# TextureShader::ConnectShaderComponents
elf 0x90f94 body 182
Sig: undefined __stdcall ConnectShaderComponents(uint param_1, int param_2)

## decompile
```c

/* AbyssEngine::TextureShader::ConnectShaderComponents(unsigned int, int) */

void AbyssEngine::TextureShader::ConnectShaderComponents(uint param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  int in_r2;
  int iVar3;
  code *pcVar4;
  
  pcVar4 = *(code **)(DAT_000a104c + 0xa0fa8);
  uVar1 = (*pcVar4)(param_2,DAT_000a1050 + 0xa0fac);
  iVar3 = param_1 + in_r2 * 4;
  iVar2 = DAT_000a1054 + 0xa0fba;
  *(undefined4 *)(iVar3 + 0x44) = uVar1;
  uVar1 = glGetAttribLocation(param_2,iVar2);
  iVar2 = DAT_000a1058;
  *(undefined4 *)(iVar3 + 0x24) = uVar1;
  uVar1 = glGetAttribLocation(param_2,iVar2 + 0xa0fca);
  iVar2 = DAT_000a105c;
  *(undefined4 *)(iVar3 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa0fd6);
  iVar2 = DAT_000a1060;
  *(undefined4 *)(iVar3 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa0fe0);
  iVar2 = DAT_000a1064;
  *(undefined4 *)(iVar3 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa0fea);
  iVar2 = DAT_000a1068;
  *(undefined4 *)(iVar3 + 0x5c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa0ff4);
  iVar2 = DAT_000a106c;
  *(undefined4 *)(iVar3 + 100) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa0ffe);
  iVar2 = DAT_000a1070;
  *(undefined4 *)(iVar3 + 0x6c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa1008);
  iVar2 = DAT_000a1074;
  *(undefined4 *)(iVar3 + 0x74) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa1012);
  iVar2 = DAT_000a1078;
  *(undefined4 *)(iVar3 + 0x7c) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa101c);
  iVar2 = DAT_000a107c;
  *(undefined4 *)(iVar3 + 0x84) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa1028);
  iVar2 = DAT_000a1080;
  *(undefined4 *)(iVar3 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(param_2,iVar2 + 0xa1032);
  *(undefined4 *)(iVar3 + 0x4c) = uVar1;
  glUseProgram(param_2);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(iVar3 + 0x44),0);
  return;
}

```

## target disasm
```
  000a0f94: push {r4,r5,r6,r7,lr}
  000a0f96: add r7,sp,#0xc
  000a0f98: push.w r8
  000a0f9c: mov r5,r0
  000a0f9e: ldr r0,[0x000a104c]
  000a0fa0: mov r4,r1
  000a0fa2: ldr r1,[0x000a1050]
  000a0fa4: add r0,pc
  000a0fa6: mov r8,r2
  000a0fa8: add r1,pc
  000a0faa: ldr r6,[r0,#0x0]
  000a0fac: mov r0,r4
  000a0fae: blx r6
  000a0fb0: ldr r1,[0x000a1054]
  000a0fb2: add.w r5,r5,r8, lsl #0x2
  000a0fb6: add r1,pc
  000a0fb8: str r0,[r5,#0x44]
  000a0fba: mov r0,r4
  000a0fbc: blx 0x00070ac8
  000a0fc0: ldr r1,[0x000a1058]
  000a0fc2: str r0,[r5,#0x24]
  000a0fc4: mov r0,r4
  000a0fc6: add r1,pc
  000a0fc8: blx 0x00070ac8
  000a0fcc: ldr r1,[0x000a105c]
  000a0fce: str r0,[r5,#0x2c]
  000a0fd0: mov r0,r4
  000a0fd2: add r1,pc
  000a0fd4: blx r6
  000a0fd6: ldr r1,[0x000a1060]
  000a0fd8: str r0,[r5,#0x34]
  000a0fda: mov r0,r4
  000a0fdc: add r1,pc
  000a0fde: blx r6
  000a0fe0: ldr r1,[0x000a1064]
  000a0fe2: str r0,[r5,#0x3c]
  000a0fe4: mov r0,r4
  000a0fe6: add r1,pc
  000a0fe8: blx r6
  000a0fea: ldr r1,[0x000a1068]
  000a0fec: str r0,[r5,#0x5c]
  000a0fee: mov r0,r4
  000a0ff0: add r1,pc
  000a0ff2: blx r6
  000a0ff4: ldr r1,[0x000a106c]
  000a0ff6: str r0,[r5,#0x64]
  000a0ff8: mov r0,r4
  000a0ffa: add r1,pc
  000a0ffc: blx r6
  000a0ffe: ldr r1,[0x000a1070]
  000a1000: str r0,[r5,#0x6c]
  000a1002: mov r0,r4
  000a1004: add r1,pc
  000a1006: blx r6
  000a1008: ldr r1,[0x000a1074]
  000a100a: str r0,[r5,#0x74]
  000a100c: mov r0,r4
  000a100e: add r1,pc
  000a1010: blx r6
  000a1012: ldr r1,[0x000a1078]
  000a1014: str r0,[r5,#0x7c]
  000a1016: mov r0,r4
  000a1018: add r1,pc
  000a101a: blx r6
  000a101c: ldr r1,[0x000a107c]
  000a101e: str.w r0,[r5,#0x84]
  000a1022: mov r0,r4
  000a1024: add r1,pc
  000a1026: blx r6
  000a1028: ldr r1,[0x000a1080]
  000a102a: str r0,[r5,#0x54]
  000a102c: mov r0,r4
  000a102e: add r1,pc
  000a1030: blx r6
  000a1032: str r0,[r5,#0x4c]
  000a1034: mov r0,r4
  000a1036: blx 0x00070960
  000a103a: ldr r0,[r5,#0x44]
  000a103c: movs r1,#0x0
  000a103e: pop.w r8
  000a1042: pop.w {r4,r5,r6,r7,lr}
  000a1046: b.w 0x001ab768
```
