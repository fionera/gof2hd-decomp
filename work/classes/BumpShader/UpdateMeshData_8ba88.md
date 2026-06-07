# BumpShader::UpdateMeshData
elf 0x8ba88 body 616
Sig: undefined __thiscall UpdateMeshData(BumpShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpShader::UpdateMeshData(BumpShader *this,Mesh *param_1,Engine *param_2)

{
  float *pfVar1;
  int iVar2;
  code *pcVar3;
  code *pcVar4;
  
  if (-1 < *(int *)(this + 0x2c)) {
    glUniformMatrix4fv(*(int *)(this + 0x2c),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x30)) {
    glUniformMatrix3fv(*(int *)(this + 0x30),1,0,param_2 + 0x204);
  }
  if (-1 < *(int *)(this + 0x6c)) {
    glUniform1f(*(int *)(this + 0x6c),**(undefined4 **)(DAT_0009bcf0 + 0x9bac6));
  }
  if (-1 < *(int *)(this + 0x70)) {
    glUniform1f(*(int *)(this + 0x70),**(undefined4 **)(DAT_0009bcf4 + 0x9bad8));
  }
  if (this[9] != (BumpShader)0x0) {
    glUniform3f(*(undefined4 *)(this + 0x34),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    if (-1 < *(int *)(this + 0x3c)) {
      glUniform3f(*(int *)(this + 0x3c),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x48)) {
      glUniform4fv(*(int *)(this + 0x48),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform3fv(*(int *)(this + 0x4c),1,param_2 + 0x2cc);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform3fv(*(int *)(this + 0x54),1,param_2 + 0x2fc);
    }
    if (-1 < *(int *)(this + 0x5c)) {
      glUniform3fv(*(int *)(this + 0x5c),1,param_2 + 0x2e4);
    }
    if (-1 < *(int *)(this + 100)) {
      glUniform1f(*(int *)(this + 100),*(undefined4 *)(param_2 + 0x2c8));
    }
    if (-1 < *(int *)(this + 0x68)) {
      glUniform3fv(*(int *)(this + 0x68),1,param_2 + 800);
    }
    iVar2 = *(int *)(this + 0x74);
    if (-1 < iVar2) {
      pfVar1 = AEMath::Vector::operator_cast_to_float_((Vector *)(param_2 + 0x3f0));
      glUniform3fv(iVar2,1,pfVar1);
    }
    if (-1 < *(int *)(this + 0x7c)) {
      glUniform1f(*(int *)(this + 0x7c),*(undefined4 *)(param_2 + 1000));
    }
    if (-1 < *(int *)(this + 0x78)) {
      glUniform1f(*(int *)(this + 0x78),*(undefined4 *)(param_2 + 0x3ec));
    }
    if (-1 < *(int *)(this + 0x80)) {
      glUniform1i(*(int *)(this + 0x80),**(undefined1 **)(DAT_0009bcf8 + 0x9bba2));
    }
    if (*(int *)(param_2 + 0x32c) < 2) {
      pcVar3 = *(code **)(DAT_0009bd00 + 0x9bbec);
      (*pcVar3)(*(undefined4 *)(this + 0x50),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x58),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x60),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x33c),
                *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    else {
      pcVar3 = *(code **)(DAT_0009bcfc + 0x9bbbc);
      (*pcVar3)(*(undefined4 *)(this + 0x50),1,param_2 + 0x2d8);
      (*pcVar3)(*(undefined4 *)(this + 0x58),1,param_2 + 0x308);
      (*pcVar3)(*(undefined4 *)(this + 0x60),1,param_2 + 0x2f0);
      glUniform3f(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x33c),
                  *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    this[9] = (BumpShader)0x0;
  }
  if (-1 < *(int *)(this + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x28)) {
    glEnableVertexAttribArray();
  }
  if (param_1[0x5c] == (Mesh)0x0) {
    if (-1 < *(int *)(this + 0x20)) {
      glVertexAttribPointer(*(int *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    }
    if (-1 < *(int *)(this + 0x24)) {
      glVertexAttribPointer(*(int *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
    }
    if (-1 < *(int *)(this + 0x28)) {
      glVertexAttribPointer(*(int *)(this + 0x28),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
    }
  }
  else {
    pcVar4 = *(code **)(DAT_0009bd04 + 0x9bc40);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_0009bd08 + 0x9bc56);
    (*pcVar3)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar3)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar3)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  0009ba88: push {r4,r5,r6,r7,lr}
  0009ba8a: add r7,sp,#0xc
  0009ba8c: push {r6,r7,r8,r9,r11}
  0009ba90: mov r4,r0
  0009ba92: ldr r0,[r0,#0x2c]
  0009ba94: mov r6,r2
  0009ba96: mov r8,r1
  0009ba98: cmp r0,#0x0
  0009ba9a: blt 0x0009baa8
  0009ba9c: add.w r3,r6,#0x104
  0009baa0: movs r1,#0x1
  0009baa2: movs r2,#0x0
  0009baa4: blx 0x00070990
  0009baa8: ldr r0,[r4,#0x30]
  0009baaa: cmp r0,#0x0
  0009baac: blt 0x0009baba
  0009baae: add.w r3,r6,#0x204
  0009bab2: movs r1,#0x1
  0009bab4: movs r2,#0x0
  0009bab6: blx 0x0007099c
  0009baba: ldr r0,[r4,#0x6c]
  0009babc: cmp r0,#0x0
  0009babe: blt 0x0009bacc
  0009bac0: ldr r1,[0x0009bcf0]
  0009bac2: add r1,pc
  0009bac4: ldr r1,[r1,#0x0]
  0009bac6: ldr r1,[r1,#0x0]
  0009bac8: blx 0x00070978
  0009bacc: ldr r0,[r4,#0x70]
  0009bace: cmp r0,#0x0
  0009bad0: blt 0x0009bade
  0009bad2: ldr r1,[0x0009bcf4]
  0009bad4: add r1,pc
  0009bad6: ldr r1,[r1,#0x0]
  0009bad8: ldr r1,[r1,#0x0]
  0009bada: blx 0x00070978
  0009bade: ldrb r0,[r4,#0x9]
  0009bae0: cmp r0,#0x0
  0009bae2: beq.w 0x0009bc12
  0009bae6: add.w r3,r6,#0x330
  0009baea: ldmia r3,{r1,r2,r3}
  0009baec: ldr r0,[r4,#0x34]
  0009baee: blx 0x000709b4
  0009baf2: ldr r0,[r4,#0x3c]
  0009baf4: cmp r0,#0x0
  0009baf6: blt 0x0009bb02
  0009baf8: add.w r3,r6,#0x34c
  0009bafc: ldmia r3,{r1,r2,r3}
  0009bafe: blx 0x000709b4
  0009bb02: ldr r0,[r4,#0x48]
  0009bb04: cmp r0,#0x0
  0009bb06: blt 0x0009bb12
  0009bb08: add.w r2,r6,#0xd0
  0009bb0c: movs r1,#0x1
  0009bb0e: blx 0x00070984
  0009bb12: ldr r0,[r4,#0x4c]
  0009bb14: cmp r0,#0x0
  0009bb16: blt 0x0009bb22
  0009bb18: add.w r2,r6,#0x2cc
  0009bb1c: movs r1,#0x1
  0009bb1e: blx 0x00070a50
  0009bb22: ldr r0,[r4,#0x54]
  0009bb24: cmp r0,#0x0
  0009bb26: blt 0x0009bb32
  0009bb28: add.w r2,r6,#0x2fc
  0009bb2c: movs r1,#0x1
  0009bb2e: blx 0x00070a50
  0009bb32: ldr r0,[r4,#0x5c]
  0009bb34: cmp r0,#0x0
  0009bb36: blt 0x0009bb42
  0009bb38: add.w r2,r6,#0x2e4
  0009bb3c: movs r1,#0x1
  0009bb3e: blx 0x00070a50
  0009bb42: ldr r0,[r4,#0x64]
  0009bb44: cmp r0,#0x0
  0009bb46: blt 0x0009bb50
  0009bb48: ldr.w r1,[r6,#0x2c8]
  0009bb4c: blx 0x00070978
  0009bb50: ldr r0,[r4,#0x68]
  0009bb52: cmp r0,#0x0
  0009bb54: blt 0x0009bb60
  0009bb56: add.w r2,r6,#0x320
  0009bb5a: movs r1,#0x1
  0009bb5c: blx 0x00070a50
  0009bb60: ldr r5,[r4,#0x74]
  0009bb62: cmp r5,#0x0
  0009bb64: blt 0x0009bb78
  0009bb66: add.w r0,r6,#0x3f0
  0009bb6a: blx 0x00070b28
  0009bb6e: mov r2,r0
  0009bb70: mov r0,r5
  0009bb72: movs r1,#0x1
  0009bb74: blx 0x00070a50
  0009bb78: ldr r0,[r4,#0x7c]
  0009bb7a: cmp r0,#0x0
  0009bb7c: blt 0x0009bb86
  0009bb7e: ldr.w r1,[r6,#0x3e8]
  0009bb82: blx 0x00070978
  0009bb86: ldr r0,[r4,#0x78]
  0009bb88: cmp r0,#0x0
  0009bb8a: blt 0x0009bb94
  0009bb8c: ldr.w r1,[r6,#0x3ec]
  0009bb90: blx 0x00070978
  0009bb94: ldr.w r0,[r4,#0x80]
  0009bb98: cmp r0,#0x0
  0009bb9a: blt 0x0009bba8
  0009bb9c: ldr r1,[0x0009bcf8]
  0009bb9e: add r1,pc
  0009bba0: ldr r1,[r1,#0x0]
  0009bba2: ldrb r1,[r1,#0x0]
  0009bba4: blx 0x0007096c
  0009bba8: ldr.w r1,[r6,#0x32c]
  0009bbac: ldr r0,[r4,#0x50]
  0009bbae: cmp r1,#0x2
  0009bbb0: blt 0x0009bbe2
  0009bbb2: ldr r1,[0x0009bcfc]
  0009bbb4: add.w r2,r6,#0x2d8
  0009bbb8: add r1,pc
  0009bbba: ldr r5,[r1,#0x0]
  0009bbbc: movs r1,#0x1
  0009bbbe: blx r5
  0009bbc0: ldr r0,[r4,#0x58]
  0009bbc2: add.w r2,r6,#0x308
  0009bbc6: movs r1,#0x1
  0009bbc8: blx r5
  0009bbca: ldr r0,[r4,#0x60]
  0009bbcc: add.w r2,r6,#0x2f0
  0009bbd0: movs r1,#0x1
  0009bbd2: blx r5
  0009bbd4: add.w r3,r6,#0x33c
  0009bbd8: ldmia r3,{r1,r2,r3}
  0009bbda: ldr r0,[r4,#0x38]
  0009bbdc: blx 0x000709b4
  0009bbe0: b 0x0009bc0e
  0009bbe2: ldr r1,[0x0009bd00]
  0009bbe4: movs r2,#0x0
  0009bbe6: movs r3,#0x0
  0009bbe8: add r1,pc
  0009bbea: ldr r5,[r1,#0x0]
  0009bbec: movs r1,#0x0
  0009bbee: blx r5
  0009bbf0: ldr r0,[r4,#0x58]
  0009bbf2: movs r1,#0x0
  0009bbf4: movs r2,#0x0
  0009bbf6: movs r3,#0x0
  0009bbf8: blx r5
  0009bbfa: ldr r0,[r4,#0x60]
  0009bbfc: movs r1,#0x0
  0009bbfe: movs r2,#0x0
  0009bc00: movs r3,#0x0
  0009bc02: blx r5
  0009bc04: add.w r3,r6,#0x33c
  0009bc08: ldmia r3,{r1,r2,r3}
  0009bc0a: ldr r0,[r4,#0x38]
  0009bc0c: blx r5
  0009bc0e: movs r0,#0x0
  0009bc10: strb r0,[r4,#0x9]
  0009bc12: ldr r0,[r4,#0x20]
  0009bc14: cmp r0,#0x0
  0009bc16: blt 0x0009bc1c
  0009bc18: blx 0x00070a5c
  0009bc1c: ldr r0,[r4,#0x24]
  0009bc1e: cmp r0,#0x0
  0009bc20: blt 0x0009bc26
  0009bc22: blx 0x00070a5c
  0009bc26: ldr r0,[r4,#0x28]
  0009bc28: cmp r0,#0x0
  0009bc2a: blt 0x0009bc30
  0009bc2c: blx 0x00070a5c
  0009bc30: ldrb.w r0,[r8,#0x5c]
  0009bc34: cbz r0,0x0009bc96
  0009bc36: ldr r0,[0x0009bd04]
  0009bc38: ldr.w r1,[r8,#0x60]
  0009bc3c: add r0,pc
  0009bc3e: ldr r6,[r0,#0x0]
  0009bc40: movw r0,#0x8892
  0009bc44: blx r6
  0009bc46: ldr r1,[0x0009bd08]
  0009bc48: mov.w r9,#0x0
  0009bc4c: ldr r0,[r4,#0x20]
  0009bc4e: movw r2,#0x1406
  0009bc52: add r1,pc
  0009bc54: movs r3,#0x0
  0009bc56: strd r9,r9,[sp,#0x0]
  0009bc5a: ldr r5,[r1,#0x0]
  0009bc5c: movs r1,#0x3
  0009bc5e: blx r5
  0009bc60: ldr.w r1,[r8,#0x68]
  0009bc64: movw r0,#0x8892
  0009bc68: blx r6
  0009bc6a: ldr r0,[r4,#0x24]
  0009bc6c: movs r1,#0x2
  0009bc6e: movw r2,#0x1406
  0009bc72: movs r3,#0x0
  0009bc74: strd r9,r9,[sp,#0x0]
  0009bc78: blx r5
  0009bc7a: ldr.w r1,[r8,#0x6c]
  0009bc7e: movw r0,#0x8892
  0009bc82: blx r6
  0009bc84: ldr r0,[r4,#0x28]
  0009bc86: movs r1,#0x3
  0009bc88: movw r2,#0x1406
  0009bc8c: movs r3,#0x0
  0009bc8e: strd r9,r9,[sp,#0x0]
  0009bc92: blx r5
  0009bc94: b 0x0009bcea
  0009bc96: ldr r0,[r4,#0x20]
  0009bc98: cmp r0,#0x0
  0009bc9a: blt 0x0009bcb2
  0009bc9c: ldr.w r1,[r8,#0x4]
  0009bca0: movs r2,#0x0
  0009bca2: movs r3,#0x0
  0009bca4: strd r2,r1,[sp,#0x0]
  0009bca8: movs r1,#0x3
  0009bcaa: movw r2,#0x1406
  0009bcae: blx 0x000709c0
  0009bcb2: ldr r0,[r4,#0x24]
  0009bcb4: cmp r0,#0x0
  0009bcb6: blt 0x0009bcce
  0009bcb8: ldr.w r1,[r8,#0x8]
  0009bcbc: movs r2,#0x0
  0009bcbe: movs r3,#0x0
  0009bcc0: strd r2,r1,[sp,#0x0]
  0009bcc4: movs r1,#0x2
  0009bcc6: movw r2,#0x1406
  0009bcca: blx 0x000709c0
  0009bcce: ldr r0,[r4,#0x28]
  0009bcd0: cmp r0,#0x0
  0009bcd2: blt 0x0009bcea
  0009bcd4: ldr.w r1,[r8,#0x10]
  0009bcd8: movs r2,#0x0
  0009bcda: movs r3,#0x0
  0009bcdc: strd r2,r1,[sp,#0x0]
  0009bce0: movs r1,#0x3
  0009bce2: movw r2,#0x1406
  0009bce6: blx 0x000709c0
  0009bcea: pop.w {r2,r3,r8,r9,r11}
  0009bcee: pop {r4,r5,r6,r7,pc}
```
