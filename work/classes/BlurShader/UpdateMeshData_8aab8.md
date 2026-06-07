# BlurShader::UpdateMeshData
elf 0x8aab8 body 306
Sig: undefined __thiscall UpdateMeshData(BlurShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BlurShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BlurShader::UpdateMeshData(BlurShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  int iVar2;
  uint in_fpscr;
  float fVar3;
  float fVar4;
  longlong lVar5;
  
  if (-1 < *(int *)(this + 0x2c)) {
    glUniformMatrix4fv(*(int *)(this + 0x2c),1,0,param_2 + 0x104);
  }
  iVar2 = *(int *)(this + 0x38);
  if (-1 < iVar2) {
    if (*(int *)(**(int **)(param_2 + 0x30) + 0x30) == 2) {
      uVar1 = Engine::GetDisplayWidth();
      fVar4 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      uVar1 = Engine::GetDisplayHeight();
    }
    else {
      uVar1 = Engine::GetDisplayHeight();
      fVar4 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      uVar1 = Engine::GetDisplayWidth();
    }
    fVar3 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    glUniform2f(iVar2,1.0 / fVar4,1.0 / fVar3);
  }
  if (-1 < *(int *)(this + 0x44)) {
    glUniform2f(*(int *)(this + 0x44),0x3f000000,DAT_0009abec);
  }
  if (-1 < *(int *)(this + 0x40)) {
    glUniform1f(*(int *)(this + 0x40),*(undefined4 *)(this + 0x58));
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniform1f(*(int *)(this + 0x3c),*(undefined4 *)(this + 0x5c));
  }
  if (this[9] != (BlurShader)0x0) {
    this[9] = (BlurShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x48));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x50));
  if (param_1[0x5c] == (Mesh)0x0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x48),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    uVar1 = *(undefined4 *)(this + 0x50);
    lVar5 = (ulonglong)*(uint *)(param_1 + 8) << 0x20;
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x48),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    uVar1 = *(undefined4 *)(this + 0x50);
    lVar5 = 0;
  }
  glVertexAttribPointer(uVar1,2,0x1406,0,lVar5);
  return;
}

```

## target disasm
```
  0009aab8: push {r4,r5,r6,r7,lr}
  0009aaba: add r7,sp,#0xc
  0009aabc: push.w r8
  0009aac0: vpush {d7,d8,d9}
  0009aac4: mov r4,r0
  0009aac6: ldr r0,[r0,#0x2c]
  0009aac8: mov r5,r2
  0009aaca: mov r8,r1
  0009aacc: cmp r0,#0x0
  0009aace: blt 0x0009aadc
  0009aad0: add.w r3,r5,#0x104
  0009aad4: movs r1,#0x1
  0009aad6: movs r2,#0x0
  0009aad8: blx 0x00070990
  0009aadc: ldr r6,[r4,#0x38]
  0009aade: cmp r6,#0x0
  0009aae0: blt 0x0009ab3c
  0009aae2: ldr r0,[r5,#0x30]
  0009aae4: ldr r0,[r0,#0x0]
  0009aae6: ldr r0,[r0,#0x30]
  0009aae8: cmp r0,#0x2
  0009aaea: bne 0x0009ab06
  0009aaec: mov r0,r5
  0009aaee: blx 0x0006e8cc
  0009aaf2: vmov s0,r0
  0009aaf6: mov r0,r5
  0009aaf8: vmov.f32 s16,0x3f800000
  0009aafc: vcvt.f32.s32 s18,s0
  0009ab00: blx 0x0006e8d8
  0009ab04: b 0x0009ab1e
  0009ab06: mov r0,r5
  0009ab08: blx 0x0006e8d8
  0009ab0c: vmov s0,r0
  0009ab10: mov r0,r5
  0009ab12: vmov.f32 s16,0x3f800000
  0009ab16: vcvt.f32.s32 s18,s0
  0009ab1a: blx 0x0006e8cc
  0009ab1e: vmov s0,r0
  0009ab22: mov r0,r6
  0009ab24: vdiv.f32 s2,s16,s18
  0009ab28: vcvt.f32.s32 s0,s0
  0009ab2c: vmov r1,s2
  0009ab30: vdiv.f32 s0,s16,s0
  0009ab34: vmov r2,s0
  0009ab38: blx 0x00070c48
  0009ab3c: ldr r0,[r4,#0x44]
  0009ab3e: cmp r0,#0x0
  0009ab40: blt 0x0009ab4c
  0009ab42: ldr r2,[0x0009abec]
  0009ab44: mov.w r1,#0x3f000000
  0009ab48: blx 0x00070c48
  0009ab4c: ldr r0,[r4,#0x40]
  0009ab4e: cmp r0,#0x0
  0009ab50: blt 0x0009ab58
  0009ab52: ldr r1,[r4,#0x58]
  0009ab54: blx 0x00070978
  0009ab58: ldr r0,[r4,#0x3c]
  0009ab5a: cmp r0,#0x0
  0009ab5c: blt 0x0009ab64
  0009ab5e: ldr r1,[r4,#0x5c]
  0009ab60: blx 0x00070978
  0009ab64: ldrb r0,[r4,#0x9]
  0009ab66: cbz r0,0x0009ab6c
  0009ab68: movs r0,#0x0
  0009ab6a: strb r0,[r4,#0x9]
  0009ab6c: ldr r0,[r4,#0x48]
  0009ab6e: blx 0x00070a5c
  0009ab72: ldr r0,[r4,#0x50]
  0009ab74: blx 0x00070a5c
  0009ab78: ldrb.w r0,[r8,#0x5c]
  0009ab7c: cbz r0,0x0009abb2
  0009ab7e: ldr.w r1,[r8,#0x60]
  0009ab82: movw r0,#0x8892
  0009ab86: blx 0x0006ed04
  0009ab8a: ldr r0,[r4,#0x48]
  0009ab8c: movs r5,#0x0
  0009ab8e: movs r1,#0x3
  0009ab90: movw r2,#0x1406
  0009ab94: movs r3,#0x0
  0009ab96: strd r5,r5,[sp,#0x0]
  0009ab9a: blx 0x000709c0
  0009ab9e: ldr.w r1,[r8,#0x68]
  0009aba2: movw r0,#0x8892
  0009aba6: blx 0x0006ed04
  0009abaa: ldr r0,[r4,#0x50]
  0009abac: strd r5,r5,[sp,#0x0]
  0009abb0: b 0x0009abd4
  0009abb2: ldr.w r1,[r8,#0x4]
  0009abb6: movs r5,#0x0
  0009abb8: ldr r0,[r4,#0x48]
  0009abba: movw r2,#0x1406
  0009abbe: movs r3,#0x0
  0009abc0: strd r5,r1,[sp,#0x0]
  0009abc4: movs r1,#0x3
  0009abc6: blx 0x000709c0
  0009abca: ldr r0,[r4,#0x50]
  0009abcc: ldr.w r1,[r8,#0x8]
  0009abd0: strd r5,r1,[sp,#0x0]
  0009abd4: movs r1,#0x2
  0009abd6: movw r2,#0x1406
  0009abda: movs r3,#0x0
  0009abdc: blx 0x000709c0
  0009abe0: vpop {d7,d8,d9}
  0009abe4: pop.w r8
  0009abe8: pop {r4,r5,r6,r7,pc}
```
