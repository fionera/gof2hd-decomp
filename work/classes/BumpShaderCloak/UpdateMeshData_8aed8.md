# BumpShaderCloak::UpdateMeshData
elf 0x8aed8 body 790
Sig: undefined __thiscall UpdateMeshData(BumpShaderCloak * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpShaderCloak::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpShaderCloak::UpdateMeshData(BumpShaderCloak *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  code *pcVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (this[9] != (BumpShaderCloak)0x0) {
    glUniform3f(*(undefined4 *)(this + 0x3c),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    if (-1 < *(int *)(this + 0x44)) {
      glUniform3f(*(int *)(this + 0x44),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform4fv(*(int *)(this + 0x58),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x5c)) {
      glUniform3fv(*(int *)(this + 0x5c),1,param_2 + 0x2cc);
    }
    if (-1 < *(int *)(this + 100)) {
      glUniform3fv(*(int *)(this + 100),1,param_2 + 0x2fc);
    }
    if (-1 < *(int *)(this + 0x6c)) {
      glUniform3fv(*(int *)(this + 0x6c),1,param_2 + 0x2e4);
    }
    if (-1 < *(int *)(this + 0x74)) {
      glUniform1f(*(int *)(this + 0x74),*(undefined4 *)(param_2 + 0x2c8));
    }
    if (-1 < *(int *)(this + 0x78)) {
      glUniform3fv(*(int *)(this + 0x78),1,param_2 + 800);
    }
    iVar2 = *(int *)(this + 0x8c);
    if (-1 < iVar2) {
      if (*(int *)(**(int **)(param_2 + 0x30) + 0x30) == 2) {
        uVar1 = Engine::GetDisplayWidth();
        fVar6 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
        uVar1 = Engine::GetDisplayHeight();
      }
      else {
        uVar1 = Engine::GetDisplayHeight();
        fVar6 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
        uVar1 = Engine::GetDisplayWidth();
      }
      fVar5 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      glUniform2f(iVar2,1.0 / fVar6,1.0 / fVar5);
    }
    glActiveTexture(0x84c7);
    Engine::ActivateRefractFBO();
    if (-1 < *(int *)(this + 0x80)) {
      glUniform1f(*(int *)(this + 0x80),*(undefined4 *)(param_1 + 0x1c));
    }
    if (-1 < *(int *)(this + 0x84)) {
      glUniform1f(*(int *)(this + 0x84),*(undefined4 *)(param_1 + 0x20));
    }
    if (*(int *)(param_2 + 0x32c) < 2) {
      pcVar3 = *(code **)(DAT_0009b1f4 + 0x9b064);
      (*pcVar3)(*(undefined4 *)(this + 0x60),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x68),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x70),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x33c),
                *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    else {
      pcVar3 = *(code **)(DAT_0009b1f0 + 0x9b034);
      (*pcVar3)(*(undefined4 *)(this + 0x60),1,param_2 + 0x2d8);
      (*pcVar3)(*(undefined4 *)(this + 0x68),1,param_2 + 0x308);
      (*pcVar3)(*(undefined4 *)(this + 0x70),1,param_2 + 0x2f0);
      glUniform3f(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x33c),
                  *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    this[9] = (BumpShaderCloak)0x0;
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
  if (-1 < *(int *)(this + 0x2c)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x30)) {
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
    if (-1 < *(int *)(this + 0x2c)) {
      glVertexAttribPointer(*(int *)(this + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x14));
    }
    if (-1 < *(int *)(this + 0x30)) {
      glVertexAttribPointer(*(int *)(this + 0x30),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x18));
    }
  }
  else {
    pcVar4 = *(code **)(DAT_0009b1f8 + 0x9b0ce);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_0009b1fc + 0x9b0e4);
    (*pcVar3)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar3)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar3)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar3)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar3)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  0009aed8: push {r4,r5,r6,r7,lr}
  0009aeda: add r7,sp,#0xc
  0009aedc: push {r8,r9,r11}
  0009aee0: vpush {d7,d8,d9}
  0009aee4: mov r4,r0
  0009aee6: ldr r0,[r0,#0x34]
  0009aee8: mov r6,r2
  0009aeea: mov r8,r1
  0009aeec: cmp r0,#0x0
  0009aeee: blt 0x0009aefc
  0009aef0: add.w r3,r6,#0x104
  0009aef4: movs r1,#0x1
  0009aef6: movs r2,#0x0
  0009aef8: blx 0x00070990
  0009aefc: ldr r0,[r4,#0x38]
  0009aefe: cmp r0,#0x0
  0009af00: blt 0x0009af0e
  0009af02: add.w r3,r6,#0x204
  0009af06: movs r1,#0x1
  0009af08: movs r2,#0x0
  0009af0a: blx 0x0007099c
  0009af0e: ldrb r0,[r4,#0x9]
  0009af10: cmp r0,#0x0
  0009af12: beq.w 0x0009b08a
  0009af16: add.w r3,r6,#0x330
  0009af1a: ldmia r3,{r1,r2,r3}
  0009af1c: ldr r0,[r4,#0x3c]
  0009af1e: blx 0x000709b4
  0009af22: ldr r0,[r4,#0x44]
  0009af24: cmp r0,#0x0
  0009af26: blt 0x0009af32
  0009af28: add.w r3,r6,#0x34c
  0009af2c: ldmia r3,{r1,r2,r3}
  0009af2e: blx 0x000709b4
  0009af32: ldr r0,[r4,#0x58]
  0009af34: cmp r0,#0x0
  0009af36: blt 0x0009af42
  0009af38: add.w r2,r6,#0xd0
  0009af3c: movs r1,#0x1
  0009af3e: blx 0x00070984
  0009af42: ldr r0,[r4,#0x5c]
  0009af44: cmp r0,#0x0
  0009af46: blt 0x0009af52
  0009af48: add.w r2,r6,#0x2cc
  0009af4c: movs r1,#0x1
  0009af4e: blx 0x00070a50
  0009af52: ldr r0,[r4,#0x64]
  0009af54: cmp r0,#0x0
  0009af56: blt 0x0009af62
  0009af58: add.w r2,r6,#0x2fc
  0009af5c: movs r1,#0x1
  0009af5e: blx 0x00070a50
  0009af62: ldr r0,[r4,#0x6c]
  0009af64: cmp r0,#0x0
  0009af66: blt 0x0009af72
  0009af68: add.w r2,r6,#0x2e4
  0009af6c: movs r1,#0x1
  0009af6e: blx 0x00070a50
  0009af72: ldr r0,[r4,#0x74]
  0009af74: cmp r0,#0x0
  0009af76: blt 0x0009af80
  0009af78: ldr.w r1,[r6,#0x2c8]
  0009af7c: blx 0x00070978
  0009af80: ldr r0,[r4,#0x78]
  0009af82: cmp r0,#0x0
  0009af84: blt 0x0009af90
  0009af86: add.w r2,r6,#0x320
  0009af8a: movs r1,#0x1
  0009af8c: blx 0x00070a50
  0009af90: ldr.w r5,[r4,#0x8c]
  0009af94: cmp r5,#0x0
  0009af96: blt 0x0009aff2
  0009af98: ldr r0,[r6,#0x30]
  0009af9a: ldr r0,[r0,#0x0]
  0009af9c: ldr r0,[r0,#0x30]
  0009af9e: cmp r0,#0x2
  0009afa0: bne 0x0009afbc
  0009afa2: mov r0,r6
  0009afa4: blx 0x0006e8cc
  0009afa8: vmov s0,r0
  0009afac: mov r0,r6
  0009afae: vmov.f32 s16,0x3f800000
  0009afb2: vcvt.f32.s32 s18,s0
  0009afb6: blx 0x0006e8d8
  0009afba: b 0x0009afd4
  0009afbc: mov r0,r6
  0009afbe: blx 0x0006e8d8
  0009afc2: vmov s0,r0
  0009afc6: mov r0,r6
  0009afc8: vmov.f32 s16,0x3f800000
  0009afcc: vcvt.f32.s32 s18,s0
  0009afd0: blx 0x0006e8cc
  0009afd4: vmov s0,r0
  0009afd8: mov r0,r5
  0009afda: vdiv.f32 s2,s16,s18
  0009afde: vcvt.f32.s32 s0,s0
  0009afe2: vmov r1,s2
  0009afe6: vdiv.f32 s0,s16,s0
  0009afea: vmov r2,s0
  0009afee: blx 0x00070c48
  0009aff2: movw r0,#0x84c7
  0009aff6: blx 0x0006fb8c
  0009affa: mov r0,r6
  0009affc: blx 0x00070c3c
  0009b000: ldr.w r0,[r4,#0x80]
  0009b004: cmp r0,#0x0
  0009b006: blt 0x0009b010
  0009b008: ldr.w r1,[r8,#0x1c]
  0009b00c: blx 0x00070978
  0009b010: ldr.w r0,[r4,#0x84]
  0009b014: cmp r0,#0x0
  0009b016: blt 0x0009b020
  0009b018: ldr.w r1,[r8,#0x20]
  0009b01c: blx 0x00070978
  0009b020: ldr.w r1,[r6,#0x32c]
  0009b024: ldr r0,[r4,#0x60]
  0009b026: cmp r1,#0x2
  0009b028: blt 0x0009b05a
  0009b02a: ldr r1,[0x0009b1f0]
  0009b02c: add.w r2,r6,#0x2d8
  0009b030: add r1,pc
  0009b032: ldr r5,[r1,#0x0]
  0009b034: movs r1,#0x1
  0009b036: blx r5
  0009b038: ldr r0,[r4,#0x68]
  0009b03a: add.w r2,r6,#0x308
  0009b03e: movs r1,#0x1
  0009b040: blx r5
  0009b042: ldr r0,[r4,#0x70]
  0009b044: add.w r2,r6,#0x2f0
  0009b048: movs r1,#0x1
  0009b04a: blx r5
  0009b04c: add.w r3,r6,#0x33c
  0009b050: ldmia r3,{r1,r2,r3}
  0009b052: ldr r0,[r4,#0x40]
  0009b054: blx 0x000709b4
  0009b058: b 0x0009b086
  0009b05a: ldr r1,[0x0009b1f4]
  0009b05c: movs r2,#0x0
  0009b05e: movs r3,#0x0
  0009b060: add r1,pc
  0009b062: ldr r5,[r1,#0x0]
  0009b064: movs r1,#0x0
  0009b066: blx r5
  0009b068: ldr r0,[r4,#0x68]
  0009b06a: movs r1,#0x0
  0009b06c: movs r2,#0x0
  0009b06e: movs r3,#0x0
  0009b070: blx r5
  0009b072: ldr r0,[r4,#0x70]
  0009b074: movs r1,#0x0
  0009b076: movs r2,#0x0
  0009b078: movs r3,#0x0
  0009b07a: blx r5
  0009b07c: add.w r3,r6,#0x33c
  0009b080: ldmia r3,{r1,r2,r3}
  0009b082: ldr r0,[r4,#0x40]
  0009b084: blx r5
  0009b086: movs r0,#0x0
  0009b088: strb r0,[r4,#0x9]
  0009b08a: ldr r0,[r4,#0x20]
  0009b08c: cmp r0,#0x0
  0009b08e: blt 0x0009b094
  0009b090: blx 0x00070a5c
  0009b094: ldr r0,[r4,#0x24]
  0009b096: cmp r0,#0x0
  0009b098: blt 0x0009b09e
  0009b09a: blx 0x00070a5c
  0009b09e: ldr r0,[r4,#0x28]
  0009b0a0: cmp r0,#0x0
  0009b0a2: blt 0x0009b0a8
  0009b0a4: blx 0x00070a5c
  0009b0a8: ldr r0,[r4,#0x2c]
  0009b0aa: cmp r0,#0x0
  0009b0ac: blt 0x0009b0b2
  0009b0ae: blx 0x00070a5c
  0009b0b2: ldr r0,[r4,#0x30]
  0009b0b4: cmp r0,#0x0
  0009b0b6: blt 0x0009b0bc
  0009b0b8: blx 0x00070a5c
  0009b0bc: ldrb.w r0,[r8,#0x5c]
  0009b0c0: cmp r0,#0x0
  0009b0c2: beq 0x0009b158
  0009b0c4: ldr r0,[0x0009b1f8]
  0009b0c6: ldr.w r1,[r8,#0x60]
  0009b0ca: add r0,pc
  0009b0cc: ldr r6,[r0,#0x0]
  0009b0ce: movw r0,#0x8892
  0009b0d2: blx r6
  0009b0d4: ldr r1,[0x0009b1fc]
  0009b0d6: mov.w r9,#0x0
  0009b0da: ldr r0,[r4,#0x20]
  0009b0dc: movw r2,#0x1406
  0009b0e0: add r1,pc
  0009b0e2: movs r3,#0x0
  0009b0e4: strd r9,r9,[sp,#0x0]
  0009b0e8: ldr r5,[r1,#0x0]
  0009b0ea: movs r1,#0x3
  0009b0ec: blx r5
  0009b0ee: ldr.w r1,[r8,#0x68]
  0009b0f2: movw r0,#0x8892
  0009b0f6: blx r6
  0009b0f8: ldr r0,[r4,#0x24]
  0009b0fa: movs r1,#0x2
  0009b0fc: movw r2,#0x1406
  0009b100: movs r3,#0x0
  0009b102: strd r9,r9,[sp,#0x0]
  0009b106: blx r5
  0009b108: ldr.w r1,[r8,#0x6c]
  0009b10c: movw r0,#0x8892
  0009b110: blx r6
  0009b112: ldr r0,[r4,#0x28]
  0009b114: movs r1,#0x3
  0009b116: movw r2,#0x1406
  0009b11a: movs r3,#0x0
  0009b11c: strd r9,r9,[sp,#0x0]
  0009b120: blx r5
  0009b122: ldr.w r1,[r8,#0x70]
  0009b126: movw r0,#0x8892
  0009b12a: blx r6
  0009b12c: ldr r0,[r4,#0x2c]
  0009b12e: movs r1,#0x3
  0009b130: movw r2,#0x1406
  0009b134: movs r3,#0x0
  0009b136: strd r9,r9,[sp,#0x0]
  0009b13a: blx r5
  0009b13c: ldr.w r1,[r8,#0x74]
  0009b140: movw r0,#0x8892
  0009b144: blx r6
  0009b146: ldr r0,[r4,#0x30]
  0009b148: movs r1,#0x3
  0009b14a: movw r2,#0x1406
  0009b14e: movs r3,#0x0
  0009b150: strd r9,r9,[sp,#0x0]
  0009b154: blx r5
  0009b156: b 0x0009b1e4
  0009b158: ldr r0,[r4,#0x20]
  0009b15a: cmp r0,#0x0
  0009b15c: blt 0x0009b174
  0009b15e: ldr.w r1,[r8,#0x4]
  0009b162: movs r2,#0x0
  0009b164: movs r3,#0x0
  0009b166: strd r2,r1,[sp,#0x0]
  0009b16a: movs r1,#0x3
  0009b16c: movw r2,#0x1406
  0009b170: blx 0x000709c0
  0009b174: ldr r0,[r4,#0x24]
  0009b176: cmp r0,#0x0
  0009b178: blt 0x0009b190
  0009b17a: ldr.w r1,[r8,#0x8]
  0009b17e: movs r2,#0x0
  0009b180: movs r3,#0x0
  0009b182: strd r2,r1,[sp,#0x0]
  0009b186: movs r1,#0x2
  0009b188: movw r2,#0x1406
  0009b18c: blx 0x000709c0
  0009b190: ldr r0,[r4,#0x28]
  0009b192: cmp r0,#0x0
  0009b194: blt 0x0009b1ac
  0009b196: ldr.w r1,[r8,#0x10]
  0009b19a: movs r2,#0x0
  0009b19c: movs r3,#0x0
  0009b19e: strd r2,r1,[sp,#0x0]
  0009b1a2: movs r1,#0x3
  0009b1a4: movw r2,#0x1406
  0009b1a8: blx 0x000709c0
  0009b1ac: ldr r0,[r4,#0x2c]
  0009b1ae: cmp r0,#0x0
  0009b1b0: blt 0x0009b1c8
  0009b1b2: ldr.w r1,[r8,#0x14]
  0009b1b6: movs r2,#0x0
  0009b1b8: movs r3,#0x0
  0009b1ba: strd r2,r1,[sp,#0x0]
  0009b1be: movs r1,#0x3
  0009b1c0: movw r2,#0x1406
  0009b1c4: blx 0x000709c0
  0009b1c8: ldr r0,[r4,#0x30]
  0009b1ca: cmp r0,#0x0
  0009b1cc: blt 0x0009b1e4
  0009b1ce: ldr.w r1,[r8,#0x18]
  0009b1d2: movs r2,#0x0
  0009b1d4: movs r3,#0x0
  0009b1d6: strd r2,r1,[sp,#0x0]
  0009b1da: movs r1,#0x3
  0009b1dc: movw r2,#0x1406
  0009b1e0: blx 0x000709c0
  0009b1e4: vpop {d7,d8,d9}
  0009b1e8: pop.w {r8,r9,r11}
  0009b1ec: pop {r4,r5,r6,r7,pc}
```
