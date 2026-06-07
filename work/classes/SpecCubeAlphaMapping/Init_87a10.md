# SpecCubeAlphaMapping::Init
elf 0x87a10 body 256
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::SpecCubeAlphaMapping::Init(AbyssEngine::Engine*) */

void AbyssEngine::SpecCubeAlphaMapping::Init(Engine *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  iVar1 = ShaderBaseStruct::LoadBindShader((char *)param_1,(char *)(DAT_00097b10 + 0x97a1e));
  *(int *)(param_1 + 4) = iVar1;
  if (iVar1 == 0) {
    iVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_00097b18 + 0x97a32));
    *(int *)(param_1 + 4) = iVar1;
  }
  pcVar4 = *(code **)(DAT_00097b20 + 0x97a40);
  uVar2 = (*pcVar4)(iVar1,DAT_00097b24 + 0x97a42);
  iVar1 = DAT_00097b28 + 0x97a4c;
  *(undefined4 *)(param_1 + 0x20) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b2c + 0x97a58;
  *(undefined4 *)(param_1 + 0x24) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  puVar3 = (undefined4 *)(DAT_00097b30 + 0x97a64);
  iVar1 = DAT_00097b34 + 0x97a68;
  *(undefined4 *)(param_1 + 0x28) = uVar2;
  pcVar4 = (code *)*puVar3;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b38 + 0x97a76;
  *(undefined4 *)(param_1 + 0x2c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b3c + 0x97a82;
  *(undefined4 *)(param_1 + 0x30) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b40 + 0x97a8e;
  *(undefined4 *)(param_1 + 0x34) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b44 + 0x97a9a;
  *(undefined4 *)(param_1 + 0x38) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b48 + 0x97aa6;
  *(undefined4 *)(param_1 + 0x40) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b4c + 0x97ab2;
  *(undefined4 *)(param_1 + 0x3c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b50 + 0x97abe;
  *(undefined4 *)(param_1 + 0x44) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b54 + 0x97aca;
  *(undefined4 *)(param_1 + 0x48) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b58 + 0x97ad6;
  *(undefined4 *)(param_1 + 0x4c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b5c + 0x97ae2;
  *(undefined4 *)(param_1 + 0x50) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_00097b60 + 0x97aee;
  *(undefined4 *)(param_1 + 0x54) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  *(undefined4 *)(param_1 + 0x58) = uVar2;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  glUniform1i(*(undefined4 *)(param_1 + 0x40),0);
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x3c),1);
  return;
}

```

## target disasm
```
  00097a10: push {r4,r5,r7,lr}
  00097a12: add r7,sp,#0x8
  00097a14: ldr r1,[0x00097b10]
  00097a16: mov r4,r0
  00097a18: ldr r2,[0x00097b14]
  00097a1a: add r1,pc
  00097a1c: add r2,pc
  00097a1e: blx 0x00070948
  00097a22: cmp r0,#0x0
  00097a24: str r0,[r4,#0x4]
  00097a26: bne 0x00097a38
  00097a28: ldr r1,[0x00097b18]
  00097a2a: mov r0,r4
  00097a2c: ldr r2,[0x00097b1c]
  00097a2e: add r1,pc
  00097a30: add r2,pc
  00097a32: blx 0x00070954
  00097a36: str r0,[r4,#0x4]
  00097a38: ldr r2,[0x00097b20]
  00097a3a: ldr r1,[0x00097b24]
  00097a3c: add r2,pc
  00097a3e: add r1,pc
  00097a40: ldr r5,[r2,#0x0]
  00097a42: blx r5
  00097a44: ldr r1,[0x00097b28]
  00097a46: ldr r2,[r4,#0x4]
  00097a48: add r1,pc
  00097a4a: str r0,[r4,#0x20]
  00097a4c: mov r0,r2
  00097a4e: blx r5
  00097a50: ldr r1,[0x00097b2c]
  00097a52: ldr r2,[r4,#0x4]
  00097a54: add r1,pc
  00097a56: str r0,[r4,#0x24]
  00097a58: mov r0,r2
  00097a5a: blx r5
  00097a5c: ldr r3,[0x00097b30]
  00097a5e: ldr r1,[0x00097b34]
  00097a60: add r3,pc
  00097a62: ldr r2,[r4,#0x4]
  00097a64: add r1,pc
  00097a66: str r0,[r4,#0x28]
  00097a68: ldr r5,[r3,#0x0]
  00097a6a: mov r0,r2
  00097a6c: blx r5
  00097a6e: ldr r1,[0x00097b38]
  00097a70: ldr r2,[r4,#0x4]
  00097a72: add r1,pc
  00097a74: str r0,[r4,#0x2c]
  00097a76: mov r0,r2
  00097a78: blx r5
  00097a7a: ldr r1,[0x00097b3c]
  00097a7c: ldr r2,[r4,#0x4]
  00097a7e: add r1,pc
  00097a80: str r0,[r4,#0x30]
  00097a82: mov r0,r2
  00097a84: blx r5
  00097a86: ldr r1,[0x00097b40]
  00097a88: ldr r2,[r4,#0x4]
  00097a8a: add r1,pc
  00097a8c: str r0,[r4,#0x34]
  00097a8e: mov r0,r2
  00097a90: blx r5
  00097a92: ldr r1,[0x00097b44]
  00097a94: ldr r2,[r4,#0x4]
  00097a96: add r1,pc
  00097a98: str r0,[r4,#0x38]
  00097a9a: mov r0,r2
  00097a9c: blx r5
  00097a9e: ldr r1,[0x00097b48]
  00097aa0: ldr r2,[r4,#0x4]
  00097aa2: add r1,pc
  00097aa4: str r0,[r4,#0x40]
  00097aa6: mov r0,r2
  00097aa8: blx r5
  00097aaa: ldr r1,[0x00097b4c]
  00097aac: ldr r2,[r4,#0x4]
  00097aae: add r1,pc
  00097ab0: str r0,[r4,#0x3c]
  00097ab2: mov r0,r2
  00097ab4: blx r5
  00097ab6: ldr r1,[0x00097b50]
  00097ab8: ldr r2,[r4,#0x4]
  00097aba: add r1,pc
  00097abc: str r0,[r4,#0x44]
  00097abe: mov r0,r2
  00097ac0: blx r5
  00097ac2: ldr r1,[0x00097b54]
  00097ac4: ldr r2,[r4,#0x4]
  00097ac6: add r1,pc
  00097ac8: str r0,[r4,#0x48]
  00097aca: mov r0,r2
  00097acc: blx r5
  00097ace: ldr r1,[0x00097b58]
  00097ad0: ldr r2,[r4,#0x4]
  00097ad2: add r1,pc
  00097ad4: str r0,[r4,#0x4c]
  00097ad6: mov r0,r2
  00097ad8: blx r5
  00097ada: ldr r1,[0x00097b5c]
  00097adc: ldr r2,[r4,#0x4]
  00097ade: add r1,pc
  00097ae0: str r0,[r4,#0x50]
  00097ae2: mov r0,r2
  00097ae4: blx r5
  00097ae6: ldr r1,[0x00097b60]
  00097ae8: ldr r2,[r4,#0x4]
  00097aea: add r1,pc
  00097aec: str r0,[r4,#0x54]
  00097aee: mov r0,r2
  00097af0: blx r5
  00097af2: ldr r1,[r4,#0x4]
  00097af4: str r0,[r4,#0x58]
  00097af6: mov r0,r1
  00097af8: blx 0x00070960
  00097afc: ldr r0,[r4,#0x40]
  00097afe: movs r1,#0x0
  00097b00: blx 0x0007096c
  00097b04: ldr r0,[r4,#0x3c]
  00097b06: movs r1,#0x1
  00097b08: pop.w {r4,r5,r7,lr}
  00097b0c: b.w 0x001ab768
```
