# TextureVtxColorShader::ConnectShaderComponents
elf 0x8bdc8 body 198
Sig: undefined __stdcall ConnectShaderComponents(uint param_1, int param_2)

## decompile
```c

/* AbyssEngine::TextureVtxColorShader::ConnectShaderComponents(unsigned int, int) */

void AbyssEngine::TextureVtxColorShader::ConnectShaderComponents(uint param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  int in_r2;
  undefined4 *puVar3;
  code *pcVar4;
  int iVar5;
  code *pcVar6;
  
  pcVar4 = *(code **)(DAT_0009be90 + 0x9bddc);
  uVar2 = (*pcVar4)(param_2,DAT_0009be94 + 0x9bde0);
  iVar1 = DAT_0009be9c;
  iVar5 = param_1 + in_r2 * 4;
  puVar3 = (undefined4 *)(DAT_0009be98 + 0x9bdf0);
  *(undefined4 *)(iVar5 + 0x4c) = uVar2;
  pcVar6 = (code *)*puVar3;
  uVar2 = (*pcVar6)(param_2,iVar1 + 0x9bdf4);
  iVar1 = DAT_0009bea0;
  *(undefined4 *)(iVar5 + 0x24) = uVar2;
  uVar2 = (*pcVar6)(param_2,iVar1 + 0x9be04);
  iVar1 = DAT_0009bea4;
  *(undefined4 *)(iVar5 + 0x2c) = uVar2;
  uVar2 = (*pcVar6)(param_2,iVar1 + 0x9be0e);
  iVar1 = DAT_0009bea8;
  *(undefined4 *)(iVar5 + 0x34) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be18);
  iVar1 = DAT_0009beac;
  *(undefined4 *)(iVar5 + 0x3c) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be22);
  iVar1 = DAT_0009beb0;
  *(undefined4 *)(iVar5 + 0x44) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be2c);
  iVar1 = DAT_0009beb4;
  *(undefined4 *)(iVar5 + 0x8c) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be38);
  iVar1 = DAT_0009beb8;
  *(undefined4 *)(iVar5 + 100) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be42);
  iVar1 = DAT_0009bebc;
  *(undefined4 *)(iVar5 + 0x6c) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be4c);
  iVar1 = DAT_0009bec0;
  *(undefined4 *)(iVar5 + 0x74) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be56);
  iVar1 = DAT_0009bec4;
  *(undefined4 *)(iVar5 + 0x7c) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be60);
  iVar1 = DAT_0009bec8;
  *(undefined4 *)(iVar5 + 0x84) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be6c);
  iVar1 = DAT_0009becc;
  *(undefined4 *)(iVar5 + 0x54) = uVar2;
  uVar2 = (*pcVar4)(param_2,iVar1 + 0x9be76);
  *(undefined4 *)(iVar5 + 0x5c) = uVar2;
  glUseProgram(param_2);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(iVar5 + 0x4c),0);
  return;
}

```

## target disasm
```
  0009bdc8: push {r4,r5,r6,r7,lr}
  0009bdca: add r7,sp,#0xc
  0009bdcc: push {r8,r9,r11}
  0009bdd0: mov r6,r0
  0009bdd2: ldr r0,[0x0009be90]
  0009bdd4: mov r4,r1
  0009bdd6: ldr r1,[0x0009be94]
  0009bdd8: add r0,pc
  0009bdda: mov r8,r2
  0009bddc: add r1,pc
  0009bdde: ldr r5,[r0,#0x0]
  0009bde0: mov r0,r4
  0009bde2: blx r5
  0009bde4: ldr r2,[0x0009be98]
  0009bde6: add.w r6,r6,r8, lsl #0x2
  0009bdea: ldr r1,[0x0009be9c]
  0009bdec: add r2,pc
  0009bdee: str r0,[r6,#0x4c]
  0009bdf0: add r1,pc
  0009bdf2: mov r0,r4
  0009bdf4: ldr.w r9,[r2,#0x0]
  0009bdf8: blx r9
  0009bdfa: ldr r1,[0x0009bea0]
  0009bdfc: str r0,[r6,#0x24]
  0009bdfe: mov r0,r4
  0009be00: add r1,pc
  0009be02: blx r9
  0009be04: ldr r1,[0x0009bea4]
  0009be06: str r0,[r6,#0x2c]
  0009be08: mov r0,r4
  0009be0a: add r1,pc
  0009be0c: blx r9
  0009be0e: ldr r1,[0x0009bea8]
  0009be10: str r0,[r6,#0x34]
  0009be12: mov r0,r4
  0009be14: add r1,pc
  0009be16: blx r5
  0009be18: ldr r1,[0x0009beac]
  0009be1a: str r0,[r6,#0x3c]
  0009be1c: mov r0,r4
  0009be1e: add r1,pc
  0009be20: blx r5
  0009be22: ldr r1,[0x0009beb0]
  0009be24: str r0,[r6,#0x44]
  0009be26: mov r0,r4
  0009be28: add r1,pc
  0009be2a: blx r5
  0009be2c: ldr r1,[0x0009beb4]
  0009be2e: str.w r0,[r6,#0x8c]
  0009be32: mov r0,r4
  0009be34: add r1,pc
  0009be36: blx r5
  0009be38: ldr r1,[0x0009beb8]
  0009be3a: str r0,[r6,#0x64]
  0009be3c: mov r0,r4
  0009be3e: add r1,pc
  0009be40: blx r5
  0009be42: ldr r1,[0x0009bebc]
  0009be44: str r0,[r6,#0x6c]
  0009be46: mov r0,r4
  0009be48: add r1,pc
  0009be4a: blx r5
  0009be4c: ldr r1,[0x0009bec0]
  0009be4e: str r0,[r6,#0x74]
  0009be50: mov r0,r4
  0009be52: add r1,pc
  0009be54: blx r5
  0009be56: ldr r1,[0x0009bec4]
  0009be58: str r0,[r6,#0x7c]
  0009be5a: mov r0,r4
  0009be5c: add r1,pc
  0009be5e: blx r5
  0009be60: ldr r1,[0x0009bec8]
  0009be62: str.w r0,[r6,#0x84]
  0009be66: mov r0,r4
  0009be68: add r1,pc
  0009be6a: blx r5
  0009be6c: ldr r1,[0x0009becc]
  0009be6e: str r0,[r6,#0x54]
  0009be70: mov r0,r4
  0009be72: add r1,pc
  0009be74: blx r5
  0009be76: str r0,[r6,#0x5c]
  0009be78: mov r0,r4
  0009be7a: blx 0x00070960
  0009be7e: ldr r0,[r6,#0x4c]
  0009be80: movs r1,#0x0
  0009be82: pop.w {r8,r9,r11}
  0009be86: pop.w {r4,r5,r6,r7,lr}
  0009be8a: b.w 0x001ab768
```
