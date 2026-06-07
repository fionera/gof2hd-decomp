# BumpRimCubeShader::UpdateMeshData
elf 0x8fe0c body 818
Sig: undefined __thiscall UpdateMeshData(BumpRimCubeShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpRimCubeShader::UpdateMeshData
          (BumpRimCubeShader *this,Mesh *param_1,Engine *param_2)

{
  float *pfVar1;
  int iVar2;
  code *pcVar3;
  code *pcVar4;
  undefined4 uVar5;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniformMatrix4fv(*(int *)(this + 0x3c),1,0,param_2 + 0x144);
  }
  if (-1 < *(int *)(this + 0x7c)) {
    glUniform1f(*(int *)(this + 0x7c),**(undefined4 **)(DAT_000a0144 + 0x9fe5c));
  }
  if (-1 < *(int *)(this + 0x80)) {
    glUniform1f(*(int *)(this + 0x80),**(undefined4 **)(DAT_000a0148 + 0x9fe70));
  }
  if (this[9] != (BumpRimCubeShader)0x0) {
    glUniform3f(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    if (-1 < *(int *)(this + 0x48)) {
      glUniform3f(*(int *)(this + 0x48),*(undefined4 *)(param_2 + 0x34c),
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
    iVar2 = *(int *)(this + 0x88);
    if (-1 < iVar2) {
      pfVar1 = AEMath::Vector::operator_cast_to_float_((Vector *)(param_2 + 0x3f0));
      glUniform3fv(iVar2,1,pfVar1);
    }
    if (-1 < *(int *)(this + 0x90)) {
      glUniform1f(*(int *)(this + 0x90),*(undefined4 *)(param_2 + 1000));
    }
    if (-1 < *(int *)(this + 0x8c)) {
      glUniform1f(*(int *)(this + 0x8c),*(undefined4 *)(param_2 + 0x3ec));
    }
    if (-1 < *(int *)(this + 0x94)) {
      glUniform1i(*(int *)(this + 0x94),**(undefined1 **)(DAT_000a014c + 0x9ff40));
    }
    if (-1 < *(int *)(this + 0x84)) {
      uVar5 = DAT_000a0140;
      if ((*(int *)(param_1 + 0x30) != 0) &&
         (uVar5 = 0x3f800000, *(int *)(*(int *)(param_1 + 0x30) + 0x24) == 0)) {
        uVar5 = DAT_000a0140;
      }
      glUniform1f(*(int *)(this + 0x84),uVar5);
    }
    if (*(int *)(param_2 + 0x32c) < 2) {
      pcVar3 = *(code **)(DAT_000a0154 + 0x9ffb8);
      (*pcVar3)(*(undefined4 *)(this + 0x60),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x68),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x70),0,0,0);
      (*pcVar3)(*(undefined4 *)(this + 0x44),*(undefined4 *)(param_2 + 0x33c),
                *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    else {
      pcVar3 = *(code **)(DAT_000a0150 + 0x9ff88);
      (*pcVar3)(*(undefined4 *)(this + 0x60),1,param_2 + 0x2d8);
      (*pcVar3)(*(undefined4 *)(this + 0x68),1,param_2 + 0x308);
      (*pcVar3)(*(undefined4 *)(this + 0x70),1,param_2 + 0x2f0);
      glUniform3f(*(undefined4 *)(this + 0x44),*(undefined4 *)(param_2 + 0x33c),
                  *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    this[9] = (BumpRimCubeShader)0x0;
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
    pcVar4 = *(code **)(DAT_000a0158 + 0xa0022);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_000a015c + 0xa0038);
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
  0009fe0c: push {r4,r5,r6,r7,lr}
  0009fe0e: add r7,sp,#0xc
  0009fe10: push {r6,r7,r8,r9,r11}
  0009fe14: mov r4,r0
  0009fe16: ldr r0,[r0,#0x34]
  0009fe18: mov r6,r2
  0009fe1a: mov r8,r1
  0009fe1c: cmp r0,#0x0
  0009fe1e: blt 0x0009fe2c
  0009fe20: add.w r3,r6,#0x104
  0009fe24: movs r1,#0x1
  0009fe26: movs r2,#0x0
  0009fe28: blx 0x00070990
  0009fe2c: ldr r0,[r4,#0x38]
  0009fe2e: cmp r0,#0x0
  0009fe30: blt 0x0009fe3e
  0009fe32: add.w r3,r6,#0x204
  0009fe36: movs r1,#0x1
  0009fe38: movs r2,#0x0
  0009fe3a: blx 0x0007099c
  0009fe3e: ldr r0,[r4,#0x3c]
  0009fe40: cmp r0,#0x0
  0009fe42: blt 0x0009fe50
  0009fe44: add.w r3,r6,#0x144
  0009fe48: movs r1,#0x1
  0009fe4a: movs r2,#0x0
  0009fe4c: blx 0x00070990
  0009fe50: ldr r0,[r4,#0x7c]
  0009fe52: cmp r0,#0x0
  0009fe54: blt 0x0009fe62
  0009fe56: ldr r1,[0x000a0144]
  0009fe58: add r1,pc
  0009fe5a: ldr r1,[r1,#0x0]
  0009fe5c: ldr r1,[r1,#0x0]
  0009fe5e: blx 0x00070978
  0009fe62: ldr.w r0,[r4,#0x80]
  0009fe66: cmp r0,#0x0
  0009fe68: blt 0x0009fe76
  0009fe6a: ldr r1,[0x000a0148]
  0009fe6c: add r1,pc
  0009fe6e: ldr r1,[r1,#0x0]
  0009fe70: ldr r1,[r1,#0x0]
  0009fe72: blx 0x00070978
  0009fe76: ldrb r0,[r4,#0x9]
  0009fe78: cmp r0,#0x0
  0009fe7a: beq.w 0x0009ffde
  0009fe7e: add.w r3,r6,#0x330
  0009fe82: ldmia r3,{r1,r2,r3}
  0009fe84: ldr r0,[r4,#0x40]
  0009fe86: blx 0x000709b4
  0009fe8a: ldr r0,[r4,#0x48]
  0009fe8c: cmp r0,#0x0
  0009fe8e: blt 0x0009fe9a
  0009fe90: add.w r3,r6,#0x34c
  0009fe94: ldmia r3,{r1,r2,r3}
  0009fe96: blx 0x000709b4
  0009fe9a: ldr r0,[r4,#0x58]
  0009fe9c: cmp r0,#0x0
  0009fe9e: blt 0x0009feaa
  0009fea0: add.w r2,r6,#0xd0
  0009fea4: movs r1,#0x1
  0009fea6: blx 0x00070984
  0009feaa: ldr r0,[r4,#0x5c]
  0009feac: cmp r0,#0x0
  0009feae: blt 0x0009feba
  0009feb0: add.w r2,r6,#0x2cc
  0009feb4: movs r1,#0x1
  0009feb6: blx 0x00070a50
  0009feba: ldr r0,[r4,#0x64]
  0009febc: cmp r0,#0x0
  0009febe: blt 0x0009feca
  0009fec0: add.w r2,r6,#0x2fc
  0009fec4: movs r1,#0x1
  0009fec6: blx 0x00070a50
  0009feca: ldr r0,[r4,#0x6c]
  0009fecc: cmp r0,#0x0
  0009fece: blt 0x0009feda
  0009fed0: add.w r2,r6,#0x2e4
  0009fed4: movs r1,#0x1
  0009fed6: blx 0x00070a50
  0009feda: ldr r0,[r4,#0x74]
  0009fedc: cmp r0,#0x0
  0009fede: blt 0x0009fee8
  0009fee0: ldr.w r1,[r6,#0x2c8]
  0009fee4: blx 0x00070978
  0009fee8: ldr r0,[r4,#0x78]
  0009feea: cmp r0,#0x0
  0009feec: blt 0x0009fef8
  0009feee: add.w r2,r6,#0x320
  0009fef2: movs r1,#0x1
  0009fef4: blx 0x00070a50
  0009fef8: ldr.w r5,[r4,#0x88]
  0009fefc: cmp r5,#0x0
  0009fefe: blt 0x0009ff12
  0009ff00: add.w r0,r6,#0x3f0
  0009ff04: blx 0x00070b28
  0009ff08: mov r2,r0
  0009ff0a: mov r0,r5
  0009ff0c: movs r1,#0x1
  0009ff0e: blx 0x00070a50
  0009ff12: ldr.w r0,[r4,#0x90]
  0009ff16: cmp r0,#0x0
  0009ff18: blt 0x0009ff22
  0009ff1a: ldr.w r1,[r6,#0x3e8]
  0009ff1e: blx 0x00070978
  0009ff22: ldr.w r0,[r4,#0x8c]
  0009ff26: cmp r0,#0x0
  0009ff28: blt 0x0009ff32
  0009ff2a: ldr.w r1,[r6,#0x3ec]
  0009ff2e: blx 0x00070978
  0009ff32: ldr.w r0,[r4,#0x94]
  0009ff36: cmp r0,#0x0
  0009ff38: blt 0x0009ff46
  0009ff3a: ldr r1,[0x000a014c]
  0009ff3c: add r1,pc
  0009ff3e: ldr r1,[r1,#0x0]
  0009ff40: ldrb r1,[r1,#0x0]
  0009ff42: blx 0x0007096c
  0009ff46: ldr.w r0,[r4,#0x84]
  0009ff4a: cmp r0,#0x0
  0009ff4c: blt 0x0009ff74
  0009ff4e: ldr.w r1,[r8,#0x30]
  0009ff52: cbz r1,0x0009ff68
  0009ff54: ldr r1,[r1,#0x24]
  0009ff56: vmov.f32 s0,0x3f800000
  0009ff5a: vldr.32 s2,[pc,#0x1e4]
  0009ff5e: cmp r1,#0x0
  0009ff60: it eq
  0009ff62: vmov.eq.f32 s0,s2
  0009ff66: b 0x0009ff6c
  0009ff68: vldr.32 s0,[pc,#0x1d4]
  0009ff6c: vmov r1,s0
  0009ff70: blx 0x00070978
  0009ff74: ldr.w r1,[r6,#0x32c]
  0009ff78: ldr r0,[r4,#0x60]
  0009ff7a: cmp r1,#0x2
  0009ff7c: blt 0x0009ffae
  0009ff7e: ldr r1,[0x000a0150]
  0009ff80: add.w r2,r6,#0x2d8
  0009ff84: add r1,pc
  0009ff86: ldr r5,[r1,#0x0]
  0009ff88: movs r1,#0x1
  0009ff8a: blx r5
  0009ff8c: ldr r0,[r4,#0x68]
  0009ff8e: add.w r2,r6,#0x308
  0009ff92: movs r1,#0x1
  0009ff94: blx r5
  0009ff96: ldr r0,[r4,#0x70]
  0009ff98: add.w r2,r6,#0x2f0
  0009ff9c: movs r1,#0x1
  0009ff9e: blx r5
  0009ffa0: add.w r3,r6,#0x33c
  0009ffa4: ldmia r3,{r1,r2,r3}
  0009ffa6: ldr r0,[r4,#0x44]
  0009ffa8: blx 0x000709b4
  0009ffac: b 0x0009ffda
  0009ffae: ldr r1,[0x000a0154]
  0009ffb0: movs r2,#0x0
  0009ffb2: movs r3,#0x0
  0009ffb4: add r1,pc
  0009ffb6: ldr r5,[r1,#0x0]
  0009ffb8: movs r1,#0x0
  0009ffba: blx r5
  0009ffbc: ldr r0,[r4,#0x68]
  0009ffbe: movs r1,#0x0
  0009ffc0: movs r2,#0x0
  0009ffc2: movs r3,#0x0
  0009ffc4: blx r5
  0009ffc6: ldr r0,[r4,#0x70]
  0009ffc8: movs r1,#0x0
  0009ffca: movs r2,#0x0
  0009ffcc: movs r3,#0x0
  0009ffce: blx r5
  0009ffd0: add.w r3,r6,#0x33c
  0009ffd4: ldmia r3,{r1,r2,r3}
  0009ffd6: ldr r0,[r4,#0x44]
  0009ffd8: blx r5
  0009ffda: movs r0,#0x0
  0009ffdc: strb r0,[r4,#0x9]
  0009ffde: ldr r0,[r4,#0x20]
  0009ffe0: cmp r0,#0x0
  0009ffe2: blt 0x0009ffe8
  0009ffe4: blx 0x00070a5c
  0009ffe8: ldr r0,[r4,#0x24]
  0009ffea: cmp r0,#0x0
  0009ffec: blt 0x0009fff2
  0009ffee: blx 0x00070a5c
  0009fff2: ldr r0,[r4,#0x28]
  0009fff4: cmp r0,#0x0
  0009fff6: blt 0x0009fffc
  0009fff8: blx 0x00070a5c
  0009fffc: ldr r0,[r4,#0x2c]
  0009fffe: cmp r0,#0x0
  000a0000: blt 0x000a0006
  000a0002: blx 0x00070a5c
  000a0006: ldr r0,[r4,#0x30]
  000a0008: cmp r0,#0x0
  000a000a: blt 0x000a0010
  000a000c: blx 0x00070a5c
  000a0010: ldrb.w r0,[r8,#0x5c]
  000a0014: cmp r0,#0x0
  000a0016: beq 0x000a00ac
  000a0018: ldr r0,[0x000a0158]
  000a001a: ldr.w r1,[r8,#0x60]
  000a001e: add r0,pc
  000a0020: ldr r6,[r0,#0x0]
  000a0022: movw r0,#0x8892
  000a0026: blx r6
  000a0028: ldr r1,[0x000a015c]
  000a002a: mov.w r9,#0x0
  000a002e: ldr r0,[r4,#0x20]
  000a0030: movw r2,#0x1406
  000a0034: add r1,pc
  000a0036: movs r3,#0x0
  000a0038: strd r9,r9,[sp,#0x0]
  000a003c: ldr r5,[r1,#0x0]
  000a003e: movs r1,#0x3
  000a0040: blx r5
  000a0042: ldr.w r1,[r8,#0x68]
  000a0046: movw r0,#0x8892
  000a004a: blx r6
  000a004c: ldr r0,[r4,#0x24]
  000a004e: movs r1,#0x2
  000a0050: movw r2,#0x1406
  000a0054: movs r3,#0x0
  000a0056: strd r9,r9,[sp,#0x0]
  000a005a: blx r5
  000a005c: ldr.w r1,[r8,#0x6c]
  000a0060: movw r0,#0x8892
  000a0064: blx r6
  000a0066: ldr r0,[r4,#0x28]
  000a0068: movs r1,#0x3
  000a006a: movw r2,#0x1406
  000a006e: movs r3,#0x0
  000a0070: strd r9,r9,[sp,#0x0]
  000a0074: blx r5
  000a0076: ldr.w r1,[r8,#0x70]
  000a007a: movw r0,#0x8892
  000a007e: blx r6
  000a0080: ldr r0,[r4,#0x2c]
  000a0082: movs r1,#0x3
  000a0084: movw r2,#0x1406
  000a0088: movs r3,#0x0
  000a008a: strd r9,r9,[sp,#0x0]
  000a008e: blx r5
  000a0090: ldr.w r1,[r8,#0x74]
  000a0094: movw r0,#0x8892
  000a0098: blx r6
  000a009a: ldr r0,[r4,#0x30]
  000a009c: movs r1,#0x3
  000a009e: movw r2,#0x1406
  000a00a2: movs r3,#0x0
  000a00a4: strd r9,r9,[sp,#0x0]
  000a00a8: blx r5
  000a00aa: b 0x000a0138
  000a00ac: ldr r0,[r4,#0x20]
  000a00ae: cmp r0,#0x0
  000a00b0: blt 0x000a00c8
  000a00b2: ldr.w r1,[r8,#0x4]
  000a00b6: movs r2,#0x0
  000a00b8: movs r3,#0x0
  000a00ba: strd r2,r1,[sp,#0x0]
  000a00be: movs r1,#0x3
  000a00c0: movw r2,#0x1406
  000a00c4: blx 0x000709c0
  000a00c8: ldr r0,[r4,#0x24]
  000a00ca: cmp r0,#0x0
  000a00cc: blt 0x000a00e4
  000a00ce: ldr.w r1,[r8,#0x8]
  000a00d2: movs r2,#0x0
  000a00d4: movs r3,#0x0
  000a00d6: strd r2,r1,[sp,#0x0]
  000a00da: movs r1,#0x2
  000a00dc: movw r2,#0x1406
  000a00e0: blx 0x000709c0
  000a00e4: ldr r0,[r4,#0x28]
  000a00e6: cmp r0,#0x0
  000a00e8: blt 0x000a0100
  000a00ea: ldr.w r1,[r8,#0x10]
  000a00ee: movs r2,#0x0
  000a00f0: movs r3,#0x0
  000a00f2: strd r2,r1,[sp,#0x0]
  000a00f6: movs r1,#0x3
  000a00f8: movw r2,#0x1406
  000a00fc: blx 0x000709c0
  000a0100: ldr r0,[r4,#0x2c]
  000a0102: cmp r0,#0x0
  000a0104: blt 0x000a011c
  000a0106: ldr.w r1,[r8,#0x14]
  000a010a: movs r2,#0x0
  000a010c: movs r3,#0x0
  000a010e: strd r2,r1,[sp,#0x0]
  000a0112: movs r1,#0x3
  000a0114: movw r2,#0x1406
  000a0118: blx 0x000709c0
  000a011c: ldr r0,[r4,#0x30]
  000a011e: cmp r0,#0x0
  000a0120: blt 0x000a0138
  000a0122: ldr.w r1,[r8,#0x18]
  000a0126: movs r2,#0x0
  000a0128: movs r3,#0x0
  000a012a: strd r2,r1,[sp,#0x0]
  000a012e: movs r1,#0x3
  000a0130: movw r2,#0x1406
  000a0134: blx 0x000709c0
  000a0138: pop.w {r2,r3,r8,r9,r11}
  000a013c: pop {r4,r5,r6,r7,pc}
```
