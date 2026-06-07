# VertexColorAlphaTextureShader::Init
elf 0x91b6c body 228
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::VertexColorAlphaTextureShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::VertexColorAlphaTextureShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a1c50 + 0xa1b7a));
  iVar2 = DAT_000a1c5c;
  puVar3 = (undefined4 *)(DAT_000a1c58 + 0xa1b86);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0xa1b8a);
  iVar2 = DAT_000a1c60 + 0xa1b94;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c64 + 0xa1ba0;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c68 + 0xa1bac;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c6c + 0xa1bb8;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c70 + 0xa1bc4;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_000a1c74 + 0xa1bd0);
  iVar2 = DAT_000a1c78 + 0xa1bd4;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c7c + 0xa1be2;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c80 + 0xa1bee;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c84 + 0xa1bfa;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c88 + 0xa1c06;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c8c + 0xa1c12;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c90 + 0xa1c1e;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c94 + 0xa1c2a;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1c98 + 0xa1c36;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x48),0);
  return;
}

```

## target disasm
```
  000a1b6c: push {r4,r5,r7,lr}
  000a1b6e: add r7,sp,#0x8
  000a1b70: ldr r1,[0x000a1c50]
  000a1b72: mov r4,r0
  000a1b74: ldr r2,[0x000a1c54]
  000a1b76: add r1,pc
  000a1b78: add r2,pc
  000a1b7a: blx 0x00070954
  000a1b7e: ldr r2,[0x000a1c58]
  000a1b80: ldr r1,[0x000a1c5c]
  000a1b82: add r2,pc
  000a1b84: str r0,[r4,#0x4]
  000a1b86: add r1,pc
  000a1b88: ldr r5,[r2,#0x0]
  000a1b8a: blx r5
  000a1b8c: ldr r1,[0x000a1c60]
  000a1b8e: ldr r2,[r4,#0x4]
  000a1b90: add r1,pc
  000a1b92: str r0,[r4,#0x34]
  000a1b94: mov r0,r2
  000a1b96: blx r5
  000a1b98: ldr r1,[0x000a1c64]
  000a1b9a: ldr r2,[r4,#0x4]
  000a1b9c: add r1,pc
  000a1b9e: str r0,[r4,#0x20]
  000a1ba0: mov r0,r2
  000a1ba2: blx r5
  000a1ba4: ldr r1,[0x000a1c68]
  000a1ba6: ldr r2,[r4,#0x4]
  000a1ba8: add r1,pc
  000a1baa: str r0,[r4,#0x24]
  000a1bac: mov r0,r2
  000a1bae: blx r5
  000a1bb0: ldr r1,[0x000a1c6c]
  000a1bb2: ldr r2,[r4,#0x4]
  000a1bb4: add r1,pc
  000a1bb6: str r0,[r4,#0x28]
  000a1bb8: mov r0,r2
  000a1bba: blx r5
  000a1bbc: ldr r1,[0x000a1c70]
  000a1bbe: ldr r2,[r4,#0x4]
  000a1bc0: add r1,pc
  000a1bc2: str r0,[r4,#0x2c]
  000a1bc4: mov r0,r2
  000a1bc6: blx r5
  000a1bc8: ldr r3,[0x000a1c74]
  000a1bca: ldr r1,[0x000a1c78]
  000a1bcc: add r3,pc
  000a1bce: ldr r2,[r4,#0x4]
  000a1bd0: add r1,pc
  000a1bd2: str r0,[r4,#0x30]
  000a1bd4: ldr r5,[r3,#0x0]
  000a1bd6: mov r0,r2
  000a1bd8: blx r5
  000a1bda: ldr r1,[0x000a1c7c]
  000a1bdc: ldr r2,[r4,#0x4]
  000a1bde: add r1,pc
  000a1be0: str r0,[r4,#0x38]
  000a1be2: mov r0,r2
  000a1be4: blx r5
  000a1be6: ldr r1,[0x000a1c80]
  000a1be8: ldr r2,[r4,#0x4]
  000a1bea: add r1,pc
  000a1bec: str r0,[r4,#0x3c]
  000a1bee: mov r0,r2
  000a1bf0: blx r5
  000a1bf2: ldr r1,[0x000a1c84]
  000a1bf4: ldr r2,[r4,#0x4]
  000a1bf6: add r1,pc
  000a1bf8: str r0,[r4,#0x40]
  000a1bfa: mov r0,r2
  000a1bfc: blx r5
  000a1bfe: ldr r1,[0x000a1c88]
  000a1c00: ldr r2,[r4,#0x4]
  000a1c02: add r1,pc
  000a1c04: str r0,[r4,#0x44]
  000a1c06: mov r0,r2
  000a1c08: blx r5
  000a1c0a: ldr r1,[0x000a1c8c]
  000a1c0c: ldr r2,[r4,#0x4]
  000a1c0e: add r1,pc
  000a1c10: str r0,[r4,#0x48]
  000a1c12: mov r0,r2
  000a1c14: blx r5
  000a1c16: ldr r1,[0x000a1c90]
  000a1c18: ldr r2,[r4,#0x4]
  000a1c1a: add r1,pc
  000a1c1c: str r0,[r4,#0x4c]
  000a1c1e: mov r0,r2
  000a1c20: blx r5
  000a1c22: ldr r1,[0x000a1c94]
  000a1c24: ldr r2,[r4,#0x4]
  000a1c26: add r1,pc
  000a1c28: str r0,[r4,#0x50]
  000a1c2a: mov r0,r2
  000a1c2c: blx r5
  000a1c2e: ldr r1,[0x000a1c98]
  000a1c30: ldr r2,[r4,#0x4]
  000a1c32: add r1,pc
  000a1c34: str r0,[r4,#0x54]
  000a1c36: mov r0,r2
  000a1c38: blx r5
  000a1c3a: ldr r1,[r4,#0x4]
  000a1c3c: str r0,[r4,#0x58]
  000a1c3e: mov r0,r1
  000a1c40: blx 0x00070960
  000a1c44: ldr r0,[r4,#0x48]
  000a1c46: movs r1,#0x0
  000a1c48: pop.w {r4,r5,r7,lr}
  000a1c4c: b.w 0x001ab768
```
