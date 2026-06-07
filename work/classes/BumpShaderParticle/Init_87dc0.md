# BumpShaderParticle::Init
elf 0x87dc0 body 294
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpShaderParticle::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpShaderParticle::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_00097ee8 + 0x97dce));
  iVar2 = DAT_00097ef4;
  puVar3 = (undefined4 *)(DAT_00097ef0 + 0x97dda);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x97dde);
  iVar2 = DAT_00097ef8 + 0x97de8;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097efc + 0x97df4;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f00 + 0x97e00;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f04 + 0x97e0c;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f08 + 0x97e18;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_00097f0c + 0x97e24);
  iVar2 = DAT_00097f10 + 0x97e28;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f14 + 0x97e36;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f18 + 0x97e42;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f1c + 0x97e4e;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f20 + 0x97e5a;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f24 + 0x97e66;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f28 + 0x97e72;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f2c + 0x97e7e;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f30 + 0x97e8a;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f34 + 0x97e96;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f38 + 0x97ea2;
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f3c + 0x97eae;
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_00097f40 + 0x97eba;
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 2; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x48)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x48),iVar2);
    }
  }
  return;
}

```

## target disasm
```
  00097dc0: push {r4,r5,r7,lr}
  00097dc2: add r7,sp,#0x8
  00097dc4: ldr r1,[0x00097ee8]
  00097dc6: mov r4,r0
  00097dc8: ldr r2,[0x00097eec]
  00097dca: add r1,pc
  00097dcc: add r2,pc
  00097dce: blx 0x00070954
  00097dd2: ldr r2,[0x00097ef0]
  00097dd4: ldr r1,[0x00097ef4]
  00097dd6: add r2,pc
  00097dd8: str r0,[r4,#0x4]
  00097dda: add r1,pc
  00097ddc: ldr r5,[r2,#0x0]
  00097dde: blx r5
  00097de0: ldr r1,[0x00097ef8]
  00097de2: ldr r2,[r4,#0x4]
  00097de4: add r1,pc
  00097de6: str r0,[r4,#0x20]
  00097de8: mov r0,r2
  00097dea: blx r5
  00097dec: ldr r1,[0x00097efc]
  00097dee: ldr r2,[r4,#0x4]
  00097df0: add r1,pc
  00097df2: str r0,[r4,#0x24]
  00097df4: mov r0,r2
  00097df6: blx r5
  00097df8: ldr r1,[0x00097f00]
  00097dfa: ldr r2,[r4,#0x4]
  00097dfc: add r1,pc
  00097dfe: str r0,[r4,#0x28]
  00097e00: mov r0,r2
  00097e02: blx r5
  00097e04: ldr r1,[0x00097f04]
  00097e06: ldr r2,[r4,#0x4]
  00097e08: add r1,pc
  00097e0a: str r0,[r4,#0x2c]
  00097e0c: mov r0,r2
  00097e0e: blx r5
  00097e10: ldr r1,[0x00097f08]
  00097e12: ldr r2,[r4,#0x4]
  00097e14: add r1,pc
  00097e16: str r0,[r4,#0x30]
  00097e18: mov r0,r2
  00097e1a: blx r5
  00097e1c: ldr r3,[0x00097f0c]
  00097e1e: ldr r1,[0x00097f10]
  00097e20: add r3,pc
  00097e22: ldr r2,[r4,#0x4]
  00097e24: add r1,pc
  00097e26: str r0,[r4,#0x34]
  00097e28: ldr r5,[r3,#0x0]
  00097e2a: mov r0,r2
  00097e2c: blx r5
  00097e2e: ldr r1,[0x00097f14]
  00097e30: ldr r2,[r4,#0x4]
  00097e32: add r1,pc
  00097e34: str r0,[r4,#0x38]
  00097e36: mov r0,r2
  00097e38: blx r5
  00097e3a: ldr r1,[0x00097f18]
  00097e3c: ldr r2,[r4,#0x4]
  00097e3e: add r1,pc
  00097e40: str r0,[r4,#0x3c]
  00097e42: mov r0,r2
  00097e44: blx r5
  00097e46: ldr r1,[0x00097f1c]
  00097e48: ldr r2,[r4,#0x4]
  00097e4a: add r1,pc
  00097e4c: str r0,[r4,#0x40]
  00097e4e: mov r0,r2
  00097e50: blx r5
  00097e52: ldr r1,[0x00097f20]
  00097e54: ldr r2,[r4,#0x4]
  00097e56: add r1,pc
  00097e58: str r0,[r4,#0x44]
  00097e5a: mov r0,r2
  00097e5c: blx r5
  00097e5e: ldr r1,[0x00097f24]
  00097e60: ldr r2,[r4,#0x4]
  00097e62: add r1,pc
  00097e64: str r0,[r4,#0x48]
  00097e66: mov r0,r2
  00097e68: blx r5
  00097e6a: ldr r1,[0x00097f28]
  00097e6c: ldr r2,[r4,#0x4]
  00097e6e: add r1,pc
  00097e70: str r0,[r4,#0x4c]
  00097e72: mov r0,r2
  00097e74: blx r5
  00097e76: ldr r1,[0x00097f2c]
  00097e78: ldr r2,[r4,#0x4]
  00097e7a: add r1,pc
  00097e7c: str r0,[r4,#0x50]
  00097e7e: mov r0,r2
  00097e80: blx r5
  00097e82: ldr r1,[0x00097f30]
  00097e84: ldr r2,[r4,#0x4]
  00097e86: add r1,pc
  00097e88: str r0,[r4,#0x54]
  00097e8a: mov r0,r2
  00097e8c: blx r5
  00097e8e: ldr r1,[0x00097f34]
  00097e90: ldr r2,[r4,#0x4]
  00097e92: add r1,pc
  00097e94: str r0,[r4,#0x58]
  00097e96: mov r0,r2
  00097e98: blx r5
  00097e9a: ldr r1,[0x00097f38]
  00097e9c: ldr r2,[r4,#0x4]
  00097e9e: add r1,pc
  00097ea0: str r0,[r4,#0x5c]
  00097ea2: mov r0,r2
  00097ea4: blx r5
  00097ea6: ldr r1,[0x00097f3c]
  00097ea8: ldr r2,[r4,#0x4]
  00097eaa: add r1,pc
  00097eac: str r0,[r4,#0x60]
  00097eae: mov r0,r2
  00097eb0: blx r5
  00097eb2: ldr r1,[0x00097f40]
  00097eb4: ldr r2,[r4,#0x4]
  00097eb6: add r1,pc
  00097eb8: str r0,[r4,#0x64]
  00097eba: mov r0,r2
  00097ebc: blx r5
  00097ebe: ldr r1,[r4,#0x4]
  00097ec0: str r0,[r4,#0x68]
  00097ec2: mov r0,r1
  00097ec4: blx 0x00070960
  00097ec8: add.w r5,r4,#0x48
  00097ecc: movs r4,#0x0
  00097ece: b 0x00097ee0
  00097ed0: ldr.w r0,[r5,r4,lsl #0x2]
  00097ed4: cmp r0,#0x0
  00097ed6: blt 0x00097ede
  00097ed8: mov r1,r4
  00097eda: blx 0x0007096c
  00097ede: adds r4,#0x1
  00097ee0: cmp r4,#0x2
  00097ee2: bne 0x00097ed0
  00097ee4: pop {r4,r5,r7,pc}
```
