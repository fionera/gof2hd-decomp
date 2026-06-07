# BumpRimCubeShader_new::UpdateMeshData
elf 0x88b58 body 962
Sig: undefined __stdcall UpdateMeshData(Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader_new::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void AbyssEngine::BumpRimCubeShader_new::UpdateMeshData(Mesh *param_1,Engine *param_2)

{
  int iVar1;
  float *pfVar2;
  int in_r2;
  int iVar3;
  code *pcVar4;
  code *pcVar5;
  int *piVar6;
  undefined4 uVar7;
  
  piVar6 = *(int **)(DAT_00098e78 + 0x98b6c);
  iVar1 = *piVar6;
  if (-1 < *(int *)(param_1 + 0x34)) {
    glUniformMatrix4fv(*(int *)(param_1 + 0x34),1,0,in_r2 + 0x104);
  }
  if (-1 < *(int *)(param_1 + 0x38)) {
    glUniformMatrix3fv(*(int *)(param_1 + 0x38),1,0,in_r2 + 0x204);
  }
  if (-1 < *(int *)(param_1 + 0x3c)) {
    glUniformMatrix4fv(*(int *)(param_1 + 0x3c),1,0,in_r2 + 0x144);
  }
  if (-1 < *(int *)(param_1 + 0x78)) {
    glUniform1f(*(int *)(param_1 + 0x78),**(undefined4 **)(DAT_00098f24 + 0x98bb6));
  }
  if (-1 < *(int *)(param_1 + 0x7c)) {
    glUniform1f(*(int *)(param_1 + 0x7c),**(undefined4 **)(DAT_00098f28 + 0x98bc8));
  }
  if (-1 < *(int *)(param_1 + 0x94)) {
    glUniform1f(*(int *)(param_1 + 0x94),**(undefined4 **)(DAT_00098f2c + 0x98bdc));
  }
  if (param_1[9] != (Mesh)0x0) {
    glUniform3f(*(undefined4 *)(param_1 + 0x40),*(undefined4 *)(in_r2 + 0x330),
                *(undefined4 *)(in_r2 + 0x334),*(undefined4 *)(in_r2 + 0x338));
    if (-1 < *(int *)(param_1 + 0x48)) {
      glUniform3f(*(int *)(param_1 + 0x48),*(undefined4 *)(in_r2 + 0x34c),
                  *(undefined4 *)(in_r2 + 0x350),*(undefined4 *)(in_r2 + 0x354));
    }
    if (-1 < *(int *)(param_1 + 0x58)) {
      glUniform4fv(*(int *)(param_1 + 0x58),1,in_r2 + 0xd0);
    }
    if (-1 < *(int *)(param_1 + 0x5c)) {
      glUniform3fv(*(int *)(param_1 + 0x5c),1,&stack0xffffffd0);
    }
    if (-1 < *(int *)(param_1 + 0x60)) {
      glUniform3fv(*(int *)(param_1 + 0x60),1,in_r2 + 0x2fc);
    }
    if (-1 < *(int *)(param_1 + 0x68)) {
      glUniform3fv(*(int *)(param_1 + 0x68),1,in_r2 + 0x2e4);
    }
    if (-1 < *(int *)(param_1 + 0x70)) {
      glUniform1f(*(int *)(param_1 + 0x70),*(undefined4 *)(in_r2 + 0x2c8));
    }
    if (-1 < *(int *)(param_1 + 0x74)) {
      glUniform3fv(*(int *)(param_1 + 0x74),1,in_r2 + 800);
    }
    iVar3 = *(int *)(param_1 + 0x84);
    if (-1 < iVar3) {
      pfVar2 = AEMath::Vector::operator_cast_to_float_((Vector *)(in_r2 + 0x3f0));
      glUniform3fv(iVar3,1,pfVar2);
    }
    if (-1 < *(int *)(param_1 + 0x8c)) {
      glUniform1f(*(int *)(param_1 + 0x8c),*(undefined4 *)(in_r2 + 1000));
    }
    if (-1 < *(int *)(param_1 + 0x88)) {
      glUniform1f(*(int *)(param_1 + 0x88),*(undefined4 *)(in_r2 + 0x3ec));
    }
    if (-1 < *(int *)(param_1 + 0x90)) {
      glUniform1i(*(int *)(param_1 + 0x90),**(undefined1 **)(DAT_00098f30 + 0x98cda));
    }
    if (-1 < *(int *)(param_1 + 0x80)) {
      uVar7 = DAT_00098f20;
      if ((*(int *)(param_2 + 0x30) != 0) &&
         (uVar7 = 0x3f800000, *(int *)(*(int *)(param_2 + 0x30) + 0x24) == 0)) {
        uVar7 = DAT_00098f20;
      }
      glUniform1f(*(int *)(param_1 + 0x80),uVar7);
    }
    iVar3 = *(int *)(param_1 + 0x5c);
    if (*(int *)(in_r2 + 0x32c) < 2) {
      pcVar4 = *(code **)(DAT_00098f34 + 0x98d84);
      (*pcVar4)(iVar3,0,0,0);
      (*pcVar4)(*(undefined4 *)(param_1 + 100),0,0,0);
      (*pcVar4)(*(undefined4 *)(param_1 + 0x6c),0,0,0);
      (*pcVar4)(*(undefined4 *)(param_1 + 0x44),*(undefined4 *)(in_r2 + 0x33c),
                *(undefined4 *)(in_r2 + 0x340),*(undefined4 *)(in_r2 + 0x344));
    }
    else {
      if (-1 < iVar3) {
        glUniform3fv(iVar3,1,&stack0xffffffd0);
      }
      glUniform3fv(*(undefined4 *)(param_1 + 100),1,in_r2 + 0x308);
      glUniform3fv(*(undefined4 *)(param_1 + 0x6c),1,in_r2 + 0x2f0);
      glUniform3f(*(undefined4 *)(param_1 + 0x44),*(undefined4 *)(in_r2 + 0x33c),
                  *(undefined4 *)(in_r2 + 0x340),*(undefined4 *)(in_r2 + 0x344));
    }
    param_1[9] = (Mesh)0x0;
  }
  if (-1 < *(int *)(param_1 + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x28)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x2c)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x30)) {
    glEnableVertexAttribArray();
  }
  if (param_2[0x5c] == (Engine)0x0) {
    if (-1 < *(int *)(param_1 + 0x20)) {
      glVertexAttribPointer(*(int *)(param_1 + 0x20),3,0x1406,0,0,*(undefined4 *)(param_2 + 4));
    }
    if (-1 < *(int *)(param_1 + 0x24)) {
      glVertexAttribPointer(*(int *)(param_1 + 0x24),2,0x1406,0,0,*(undefined4 *)(param_2 + 8));
    }
    if (-1 < *(int *)(param_1 + 0x28)) {
      glVertexAttribPointer(*(int *)(param_1 + 0x28),3,0x1406,0,0,*(undefined4 *)(param_2 + 0x10));
    }
    if (-1 < *(int *)(param_1 + 0x2c)) {
      glVertexAttribPointer(*(int *)(param_1 + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_2 + 0x14));
    }
    if (-1 < *(int *)(param_1 + 0x30)) {
      glVertexAttribPointer(*(int *)(param_1 + 0x30),3,0x1406,0,0,*(undefined4 *)(param_2 + 0x18));
    }
  }
  else {
    pcVar5 = *(code **)(DAT_00098f38 + 0x98dee);
    (*pcVar5)(0x8892,*(undefined4 *)(param_2 + 0x60));
    pcVar4 = *(code **)(DAT_00098f3c + 0x98e04);
    (*pcVar4)(*(undefined4 *)(param_1 + 0x20),3,0x1406,0,0,0);
    (*pcVar5)(0x8892,*(undefined4 *)(param_2 + 0x68));
    (*pcVar4)(*(undefined4 *)(param_1 + 0x24),2,0x1406,0,0,0);
    (*pcVar5)(0x8892,*(undefined4 *)(param_2 + 0x6c));
    (*pcVar4)(*(undefined4 *)(param_1 + 0x28),3,0x1406,0,0,0);
    (*pcVar5)(0x8892,*(undefined4 *)(param_2 + 0x70));
    (*pcVar4)(*(undefined4 *)(param_1 + 0x2c),3,0x1406,0,0,0);
    (*pcVar5)(0x8892,*(undefined4 *)(param_2 + 0x74));
    (*pcVar4)(*(undefined4 *)(param_1 + 0x30),3,0x1406,0,0,0);
  }
  if (*piVar6 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00098b58: push {r4,r5,r6,r7,lr}
  00098b5a: add r7,sp,#0xc
  00098b5c: push {r2,r3,r4,r5,r6,r7,r8,r9,r10}
  00098b60: mov r4,r0
  00098b62: ldr r0,[0x00098e78]
  00098b64: mov r6,r2
  00098b66: mov r8,r1
  00098b68: add r0,pc
  00098b6a: ldr.w r9,[r0,#0x0]
  00098b6e: ldr.w r0,[r9,#0x0]
  00098b72: str r0,[sp,#0x14]
  00098b74: ldr r0,[r4,#0x34]
  00098b76: cmp r0,#0x0
  00098b78: blt 0x00098b86
  00098b7a: add.w r3,r6,#0x104
  00098b7e: movs r1,#0x1
  00098b80: movs r2,#0x0
  00098b82: blx 0x00070990
  00098b86: ldr r0,[r4,#0x38]
  00098b88: cmp r0,#0x0
  00098b8a: blt 0x00098b98
  00098b8c: add.w r3,r6,#0x204
  00098b90: movs r1,#0x1
  00098b92: movs r2,#0x0
  00098b94: blx 0x0007099c
  00098b98: ldr r0,[r4,#0x3c]
  00098b9a: cmp r0,#0x0
  00098b9c: blt 0x00098baa
  00098b9e: add.w r3,r6,#0x144
  00098ba2: movs r1,#0x1
  00098ba4: movs r2,#0x0
  00098ba6: blx 0x00070990
  00098baa: ldr r0,[r4,#0x78]
  00098bac: cmp r0,#0x0
  00098bae: blt 0x00098bbc
  00098bb0: ldr r1,[0x00098f24]
  00098bb2: add r1,pc
  00098bb4: ldr r1,[r1,#0x0]
  00098bb6: ldr r1,[r1,#0x0]
  00098bb8: blx 0x00070978
  00098bbc: ldr r0,[r4,#0x7c]
  00098bbe: cmp r0,#0x0
  00098bc0: blt 0x00098bce
  00098bc2: ldr r1,[0x00098f28]
  00098bc4: add r1,pc
  00098bc6: ldr r1,[r1,#0x0]
  00098bc8: ldr r1,[r1,#0x0]
  00098bca: blx 0x00070978
  00098bce: ldr.w r0,[r4,#0x94]
  00098bd2: cmp r0,#0x0
  00098bd4: blt 0x00098be2
  00098bd6: ldr r1,[0x00098f2c]
  00098bd8: add r1,pc
  00098bda: ldr r1,[r1,#0x0]
  00098bdc: ldr r1,[r1,#0x0]
  00098bde: blx 0x00070978
  00098be2: ldrb r0,[r4,#0x9]
  00098be4: cmp r0,#0x0
  00098be6: beq.w 0x00098daa
  00098bea: add.w r3,r6,#0x330
  00098bee: ldmia r3,{r1,r2,r3}
  00098bf0: ldr r0,[r4,#0x40]
  00098bf2: blx 0x000709b4
  00098bf6: ldr r0,[r4,#0x48]
  00098bf8: cmp r0,#0x0
  00098bfa: blt 0x00098c06
  00098bfc: add.w r3,r6,#0x34c
  00098c00: ldmia r3,{r1,r2,r3}
  00098c02: blx 0x000709b4
  00098c06: ldr r0,[r4,#0x58]
  00098c08: cmp r0,#0x0
  00098c0a: blt 0x00098c16
  00098c0c: add.w r2,r6,#0xd0
  00098c10: movs r1,#0x1
  00098c12: blx 0x00070984
  00098c16: ldr r0,[r4,#0x5c]
  00098c18: cmp r0,#0x0
  00098c1a: blt 0x00098c54
  00098c1c: vldr.32 s0,[r6,#0x2cc]
  00098c20: add r2,sp,#0x8
  00098c22: vldr.32 s6,[r6,#0x2d8]
  00098c26: movs r1,#0x1
  00098c28: vldr.32 s2,[r6,#0x2d0]
  00098c2c: vadd.f32 s0,s0,s6
  00098c30: vldr.32 s8,[r6,#0x2dc]
  00098c34: vldr.32 s4,[r6,#0x2d4]
  00098c38: vldr.32 s10,[r6,#0x2e0]
  00098c3c: vstr.32 s0,[sp,#0x8]
  00098c40: vadd.f32 s0,s2,s8
  00098c44: vstr.32 s0,[sp,#0xc]
  00098c48: vadd.f32 s0,s4,s10
  00098c4c: vstr.32 s0,[sp,#0x10]
  00098c50: blx 0x00070a50
  00098c54: ldr r0,[r4,#0x60]
  00098c56: cmp r0,#0x0
  00098c58: blt 0x00098c64
  00098c5a: add.w r2,r6,#0x2fc
  00098c5e: movs r1,#0x1
  00098c60: blx 0x00070a50
  00098c64: ldr r0,[r4,#0x68]
  00098c66: cmp r0,#0x0
  00098c68: blt 0x00098c74
  00098c6a: add.w r2,r6,#0x2e4
  00098c6e: movs r1,#0x1
  00098c70: blx 0x00070a50
  00098c74: ldr r0,[r4,#0x70]
  00098c76: cmp r0,#0x0
  00098c78: blt 0x00098c82
  00098c7a: ldr.w r1,[r6,#0x2c8]
  00098c7e: blx 0x00070978
  00098c82: ldr r0,[r4,#0x74]
  00098c84: cmp r0,#0x0
  00098c86: blt 0x00098c92
  00098c88: add.w r2,r6,#0x320
  00098c8c: movs r1,#0x1
  00098c8e: blx 0x00070a50
  00098c92: ldr.w r5,[r4,#0x84]
  00098c96: cmp r5,#0x0
  00098c98: blt 0x00098cac
  00098c9a: add.w r0,r6,#0x3f0
  00098c9e: blx 0x00070b28
  00098ca2: mov r2,r0
  00098ca4: mov r0,r5
  00098ca6: movs r1,#0x1
  00098ca8: blx 0x00070a50
  00098cac: ldr.w r0,[r4,#0x8c]
  00098cb0: cmp r0,#0x0
  00098cb2: blt 0x00098cbc
  00098cb4: ldr.w r1,[r6,#0x3e8]
  00098cb8: blx 0x00070978
  00098cbc: ldr.w r0,[r4,#0x88]
  00098cc0: cmp r0,#0x0
  00098cc2: blt 0x00098ccc
  00098cc4: ldr.w r1,[r6,#0x3ec]
  00098cc8: blx 0x00070978
  00098ccc: ldr.w r0,[r4,#0x90]
  00098cd0: cmp r0,#0x0
  00098cd2: blt 0x00098ce0
  00098cd4: ldr r1,[0x00098f30]
  00098cd6: add r1,pc
  00098cd8: ldr r1,[r1,#0x0]
  00098cda: ldrb r1,[r1,#0x0]
  00098cdc: blx 0x0007096c
  00098ce0: ldr.w r0,[r4,#0x80]
  00098ce4: cmp r0,#0x0
  00098ce6: blt 0x00098d0e
  00098ce8: ldr.w r1,[r8,#0x30]
  00098cec: cbz r1,0x00098d02
  00098cee: ldr r1,[r1,#0x24]
  00098cf0: vmov.f32 s0,0x3f800000
  00098cf4: vldr.32 s2,[pc,#0x228]
  00098cf8: cmp r1,#0x0
  00098cfa: it eq
  00098cfc: vmov.eq.f32 s0,s2
  00098d00: b 0x00098d06
  00098d02: vldr.32 s0,[pc,#0x21c]
  00098d06: vmov r1,s0
  00098d0a: blx 0x00070978
  00098d0e: ldr.w r1,[r6,#0x32c]
  00098d12: ldr r0,[r4,#0x5c]
  00098d14: cmp r1,#0x2
  00098d16: blt 0x00098d7a
  00098d18: cmp r0,#0x0
  00098d1a: blt 0x00098d54
  00098d1c: vldr.32 s0,[r6,#0x2cc]
  00098d20: add r2,sp,#0x8
  00098d22: vldr.32 s6,[r6,#0x2d8]
  00098d26: movs r1,#0x1
  00098d28: vldr.32 s2,[r6,#0x2d0]
  00098d2c: vadd.f32 s0,s0,s6
  00098d30: vldr.32 s8,[r6,#0x2dc]
  00098d34: vldr.32 s4,[r6,#0x2d4]
  00098d38: vldr.32 s10,[r6,#0x2e0]
  00098d3c: vstr.32 s0,[sp,#0x8]
  00098d40: vadd.f32 s0,s2,s8
  00098d44: vstr.32 s0,[sp,#0xc]
  00098d48: vadd.f32 s0,s4,s10
  00098d4c: vstr.32 s0,[sp,#0x10]
  00098d50: blx 0x00070a50
  00098d54: ldr r0,[r4,#0x64]
  00098d56: add.w r2,r6,#0x308
  00098d5a: movs r1,#0x1
  00098d5c: blx 0x00070a50
  00098d60: ldr r0,[r4,#0x6c]
  00098d62: add.w r2,r6,#0x2f0
  00098d66: movs r1,#0x1
  00098d68: blx 0x00070a50
  00098d6c: add.w r3,r6,#0x33c
  00098d70: ldmia r3,{r1,r2,r3}
  00098d72: ldr r0,[r4,#0x44]
  00098d74: blx 0x000709b4
  00098d78: b 0x00098da6
  00098d7a: ldr r1,[0x00098f34]
  00098d7c: movs r2,#0x0
  00098d7e: movs r3,#0x0
  00098d80: add r1,pc
  00098d82: ldr r5,[r1,#0x0]
  00098d84: movs r1,#0x0
  00098d86: blx r5
  00098d88: ldr r0,[r4,#0x64]
  00098d8a: movs r1,#0x0
  00098d8c: movs r2,#0x0
  00098d8e: movs r3,#0x0
  00098d90: blx r5
  00098d92: ldr r0,[r4,#0x6c]
  00098d94: movs r1,#0x0
  00098d96: movs r2,#0x0
  00098d98: movs r3,#0x0
  00098d9a: blx r5
  00098d9c: add.w r3,r6,#0x33c
  00098da0: ldmia r3,{r1,r2,r3}
  00098da2: ldr r0,[r4,#0x44]
  00098da4: blx r5
  00098da6: movs r0,#0x0
  00098da8: strb r0,[r4,#0x9]
  00098daa: ldr r0,[r4,#0x20]
  00098dac: cmp r0,#0x0
  00098dae: blt 0x00098db4
  00098db0: blx 0x00070a5c
  00098db4: ldr r0,[r4,#0x24]
  00098db6: cmp r0,#0x0
  00098db8: blt 0x00098dbe
  00098dba: blx 0x00070a5c
  00098dbe: ldr r0,[r4,#0x28]
  00098dc0: cmp r0,#0x0
  00098dc2: blt 0x00098dc8
  00098dc4: blx 0x00070a5c
  00098dc8: ldr r0,[r4,#0x2c]
  00098dca: cmp r0,#0x0
  00098dcc: blt 0x00098dd2
  00098dce: blx 0x00070a5c
  00098dd2: ldr r0,[r4,#0x30]
  00098dd4: cmp r0,#0x0
  00098dd6: blt 0x00098ddc
  00098dd8: blx 0x00070a5c
  00098ddc: ldrb.w r0,[r8,#0x5c]
  00098de0: cmp r0,#0x0
  00098de2: beq 0x00098e7c
  00098de4: ldr r0,[0x00098f38]
  00098de6: ldr.w r1,[r8,#0x60]
  00098dea: add r0,pc
  00098dec: ldr r6,[r0,#0x0]
  00098dee: movw r0,#0x8892
  00098df2: blx r6
  00098df4: ldr r1,[0x00098f3c]
  00098df6: mov.w r10,#0x0
  00098dfa: ldr r0,[r4,#0x20]
  00098dfc: movw r2,#0x1406
  00098e00: add r1,pc
  00098e02: movs r3,#0x0
  00098e04: strd r10,r10,[sp,#0x0]
  00098e08: ldr r5,[r1,#0x0]
  00098e0a: movs r1,#0x3
  00098e0c: blx r5
  00098e0e: ldr.w r1,[r8,#0x68]
  00098e12: movw r0,#0x8892
  00098e16: blx r6
  00098e18: ldr r0,[r4,#0x24]
  00098e1a: movs r1,#0x2
  00098e1c: movw r2,#0x1406
  00098e20: movs r3,#0x0
  00098e22: strd r10,r10,[sp,#0x0]
  00098e26: blx r5
  00098e28: ldr.w r1,[r8,#0x6c]
  00098e2c: movw r0,#0x8892
  00098e30: blx r6
  00098e32: ldr r0,[r4,#0x28]
  00098e34: movs r1,#0x3
  00098e36: movw r2,#0x1406
  00098e3a: movs r3,#0x0
  00098e3c: strd r10,r10,[sp,#0x0]
  00098e40: blx r5
  00098e42: ldr.w r1,[r8,#0x70]
  00098e46: movw r0,#0x8892
  00098e4a: blx r6
  00098e4c: ldr r0,[r4,#0x2c]
  00098e4e: movs r1,#0x3
  00098e50: movw r2,#0x1406
  00098e54: movs r3,#0x0
  00098e56: strd r10,r10,[sp,#0x0]
  00098e5a: blx r5
  00098e5c: ldr.w r1,[r8,#0x74]
  00098e60: movw r0,#0x8892
  00098e64: blx r6
  00098e66: ldr r0,[r4,#0x30]
  00098e68: movs r1,#0x3
  00098e6a: movw r2,#0x1406
  00098e6e: movs r3,#0x0
  00098e70: strd r10,r10,[sp,#0x0]
  00098e74: blx r5
  00098e76: b 0x00098f08
  00098e7c: ldr r0,[r4,#0x20]
  00098e7e: cmp r0,#0x0
  00098e80: blt 0x00098e98
  00098e82: ldr.w r1,[r8,#0x4]
  00098e86: movs r2,#0x0
  00098e88: movs r3,#0x0
  00098e8a: strd r2,r1,[sp,#0x0]
  00098e8e: movs r1,#0x3
  00098e90: movw r2,#0x1406
  00098e94: blx 0x000709c0
  00098e98: ldr r0,[r4,#0x24]
  00098e9a: cmp r0,#0x0
  00098e9c: blt 0x00098eb4
  00098e9e: ldr.w r1,[r8,#0x8]
  00098ea2: movs r2,#0x0
  00098ea4: movs r3,#0x0
  00098ea6: strd r2,r1,[sp,#0x0]
  00098eaa: movs r1,#0x2
  00098eac: movw r2,#0x1406
  00098eb0: blx 0x000709c0
  00098eb4: ldr r0,[r4,#0x28]
  00098eb6: cmp r0,#0x0
  00098eb8: blt 0x00098ed0
  00098eba: ldr.w r1,[r8,#0x10]
  00098ebe: movs r2,#0x0
  00098ec0: movs r3,#0x0
  00098ec2: strd r2,r1,[sp,#0x0]
  00098ec6: movs r1,#0x3
  00098ec8: movw r2,#0x1406
  00098ecc: blx 0x000709c0
  00098ed0: ldr r0,[r4,#0x2c]
  00098ed2: cmp r0,#0x0
  00098ed4: blt 0x00098eec
  00098ed6: ldr.w r1,[r8,#0x14]
  00098eda: movs r2,#0x0
  00098edc: movs r3,#0x0
  00098ede: strd r2,r1,[sp,#0x0]
  00098ee2: movs r1,#0x3
  00098ee4: movw r2,#0x1406
  00098ee8: blx 0x000709c0
  00098eec: ldr r0,[r4,#0x30]
  00098eee: cmp r0,#0x0
  00098ef0: blt 0x00098f08
  00098ef2: ldr.w r1,[r8,#0x18]
  00098ef6: movs r2,#0x0
  00098ef8: movs r3,#0x0
  00098efa: strd r2,r1,[sp,#0x0]
  00098efe: movs r1,#0x3
  00098f00: movw r2,#0x1406
  00098f04: blx 0x000709c0
  00098f08: ldr r0,[sp,#0x14]
  00098f0a: ldr.w r1,[r9,#0x0]
  00098f0e: subs r0,r1,r0
  00098f10: ittt eq
  00098f12: add.eq sp,#0x18
  00098f14: pop.eq.w {r8,r9,r10}
  00098f18: pop.eq {r4,r5,r6,r7,pc}
  00098f1a: blx 0x0006e824
```
