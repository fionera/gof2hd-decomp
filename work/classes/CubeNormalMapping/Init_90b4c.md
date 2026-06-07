# CubeNormalMapping::Init
elf 0x90b4c body 282
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::CubeNormalMapping::Init(AbyssEngine::Engine*) */

void AbyssEngine::CubeNormalMapping::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a0c68 + 0xa0b5a));
  iVar2 = DAT_000a0c74;
  puVar3 = (undefined4 *)(DAT_000a0c70 + 0xa0b66);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0xa0b6a);
  iVar2 = DAT_000a0c78 + 0xa0b74;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0c7c + 0xa0b80;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0c80 + 0xa0b8c;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0c84 + 0xa0b98;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_000a0c88 + 0xa0ba4);
  iVar2 = DAT_000a0c8c + 0xa0ba8;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0c90 + 0xa0bb6;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0c94 + 0xa0bc2;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0c98 + 0xa0bce;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0c9c + 0xa0bda;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0ca0 + 0xa0be6;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0ca4 + 0xa0bf2;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0ca8 + 0xa0bfe;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0cac + 0xa0c0a;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0cb0 + 0xa0c16;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0cb4 + 0xa0c22;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0cb8 + 0xa0c2e;
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a0cbc + 0xa0c3a;
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  pcVar4 = *(code **)(DAT_000a0cc0 + 0xa0c50);
  (*pcVar4)(*(undefined4 *)(param_1 + 0x48),0);
  (*pcVar4)(*(undefined4 *)(param_1 + 0x4c),1);
  (*pcVar4)(*(undefined4 *)(param_1 + 0x44),2);
  return;
}

```

## target disasm
```
  000a0b4c: push {r4,r5,r7,lr}
  000a0b4e: add r7,sp,#0x8
  000a0b50: ldr r1,[0x000a0c68]
  000a0b52: mov r4,r0
  000a0b54: ldr r2,[0x000a0c6c]
  000a0b56: add r1,pc
  000a0b58: add r2,pc
  000a0b5a: blx 0x00070954
  000a0b5e: ldr r2,[0x000a0c70]
  000a0b60: ldr r1,[0x000a0c74]
  000a0b62: add r2,pc
  000a0b64: str r0,[r4,#0x4]
  000a0b66: add r1,pc
  000a0b68: ldr r5,[r2,#0x0]
  000a0b6a: blx r5
  000a0b6c: ldr r1,[0x000a0c78]
  000a0b6e: ldr r2,[r4,#0x4]
  000a0b70: add r1,pc
  000a0b72: str r0,[r4,#0x20]
  000a0b74: mov r0,r2
  000a0b76: blx r5
  000a0b78: ldr r1,[0x000a0c7c]
  000a0b7a: ldr r2,[r4,#0x4]
  000a0b7c: add r1,pc
  000a0b7e: str r0,[r4,#0x24]
  000a0b80: mov r0,r2
  000a0b82: blx r5
  000a0b84: ldr r1,[0x000a0c80]
  000a0b86: ldr r2,[r4,#0x4]
  000a0b88: add r1,pc
  000a0b8a: str r0,[r4,#0x28]
  000a0b8c: mov r0,r2
  000a0b8e: blx r5
  000a0b90: ldr r1,[0x000a0c84]
  000a0b92: ldr r2,[r4,#0x4]
  000a0b94: add r1,pc
  000a0b96: str r0,[r4,#0x2c]
  000a0b98: mov r0,r2
  000a0b9a: blx r5
  000a0b9c: ldr r3,[0x000a0c88]
  000a0b9e: ldr r1,[0x000a0c8c]
  000a0ba0: add r3,pc
  000a0ba2: ldr r2,[r4,#0x4]
  000a0ba4: add r1,pc
  000a0ba6: str r0,[r4,#0x30]
  000a0ba8: ldr r5,[r3,#0x0]
  000a0baa: mov r0,r2
  000a0bac: blx r5
  000a0bae: ldr r1,[0x000a0c90]
  000a0bb0: ldr r2,[r4,#0x4]
  000a0bb2: add r1,pc
  000a0bb4: str r0,[r4,#0x34]
  000a0bb6: mov r0,r2
  000a0bb8: blx r5
  000a0bba: ldr r1,[0x000a0c94]
  000a0bbc: ldr r2,[r4,#0x4]
  000a0bbe: add r1,pc
  000a0bc0: str r0,[r4,#0x38]
  000a0bc2: mov r0,r2
  000a0bc4: blx r5
  000a0bc6: ldr r1,[0x000a0c98]
  000a0bc8: ldr r2,[r4,#0x4]
  000a0bca: add r1,pc
  000a0bcc: str r0,[r4,#0x3c]
  000a0bce: mov r0,r2
  000a0bd0: blx r5
  000a0bd2: ldr r1,[0x000a0c9c]
  000a0bd4: ldr r2,[r4,#0x4]
  000a0bd6: add r1,pc
  000a0bd8: str r0,[r4,#0x40]
  000a0bda: mov r0,r2
  000a0bdc: blx r5
  000a0bde: ldr r1,[0x000a0ca0]
  000a0be0: ldr r2,[r4,#0x4]
  000a0be2: add r1,pc
  000a0be4: str r0,[r4,#0x48]
  000a0be6: mov r0,r2
  000a0be8: blx r5
  000a0bea: ldr r1,[0x000a0ca4]
  000a0bec: ldr r2,[r4,#0x4]
  000a0bee: add r1,pc
  000a0bf0: str r0,[r4,#0x44]
  000a0bf2: mov r0,r2
  000a0bf4: blx r5
  000a0bf6: ldr r1,[0x000a0ca8]
  000a0bf8: ldr r2,[r4,#0x4]
  000a0bfa: add r1,pc
  000a0bfc: str r0,[r4,#0x4c]
  000a0bfe: mov r0,r2
  000a0c00: blx r5
  000a0c02: ldr r1,[0x000a0cac]
  000a0c04: ldr r2,[r4,#0x4]
  000a0c06: add r1,pc
  000a0c08: str r0,[r4,#0x50]
  000a0c0a: mov r0,r2
  000a0c0c: blx r5
  000a0c0e: ldr r1,[0x000a0cb0]
  000a0c10: ldr r2,[r4,#0x4]
  000a0c12: add r1,pc
  000a0c14: str r0,[r4,#0x54]
  000a0c16: mov r0,r2
  000a0c18: blx r5
  000a0c1a: ldr r1,[0x000a0cb4]
  000a0c1c: ldr r2,[r4,#0x4]
  000a0c1e: add r1,pc
  000a0c20: str r0,[r4,#0x58]
  000a0c22: mov r0,r2
  000a0c24: blx r5
  000a0c26: ldr r1,[0x000a0cb8]
  000a0c28: ldr r2,[r4,#0x4]
  000a0c2a: add r1,pc
  000a0c2c: str r0,[r4,#0x60]
  000a0c2e: mov r0,r2
  000a0c30: blx r5
  000a0c32: ldr r1,[0x000a0cbc]
  000a0c34: ldr r2,[r4,#0x4]
  000a0c36: add r1,pc
  000a0c38: str r0,[r4,#0x64]
  000a0c3a: mov r0,r2
  000a0c3c: blx r5
  000a0c3e: ldr r1,[r4,#0x4]
  000a0c40: str r0,[r4,#0x5c]
  000a0c42: mov r0,r1
  000a0c44: blx 0x00070960
  000a0c48: ldr r0,[0x000a0cc0]
  000a0c4a: movs r1,#0x0
  000a0c4c: add r0,pc
  000a0c4e: ldr r5,[r0,#0x0]
  000a0c50: ldr r0,[r4,#0x48]
  000a0c52: blx r5
  000a0c54: ldr r0,[r4,#0x4c]
  000a0c56: movs r1,#0x1
  000a0c58: blx r5
  000a0c5a: ldr r0,[r4,#0x44]
  000a0c5c: movs r1,#0x2
  000a0c5e: mov r2,r5
  000a0c60: pop.w {r4,r5,r7,lr}
  000a0c64: bx r2
```
