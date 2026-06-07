# TextureLightShader::UpdateMeshData
elf 0x8dd14 body 494
Sig: undefined __thiscall UpdateMeshData(TextureLightShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::TextureLightShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::TextureLightShader::UpdateMeshData
          (TextureLightShader *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x2c),1,0,param_2 + 0x104);
  if (-1 < *(int *)(this + 0x68)) {
    glUniformMatrix4fv(*(int *)(this + 0x68),1,0,param_2 + 0x1c4);
  }
  glUniformMatrix3fv(*(undefined4 *)(this + 0x34),1,0,param_2 + 0x204);
  if (-1 < *(int *)(this + 0x30)) {
    glUniformMatrix4fv(*(int *)(this + 0x30),1,0,param_2 + 0x144);
  }
  if (this[9] != (TextureLightShader)0x0) {
    if (-1 < *(int *)(this + 0x6c)) {
      glUniform1i(*(int *)(this + 0x6c),param_1[0x85]);
    }
    glUniform4fv(*(undefined4 *)(this + 0x40),1,param_2 + 0xd0);
    pcVar1 = *(code **)(DAT_0009df04 + 0x9dd8e);
    (*pcVar1)(*(undefined4 *)(this + 0x48),1,param_2 + 0x2cc);
    (*pcVar1)(*(undefined4 *)(this + 0x50),1,param_2 + 0x2fc);
    (*pcVar1)(*(undefined4 *)(this + 0x58),1,param_2 + 0x2e4);
    glUniform4f(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338),
                *(undefined4 *)(param_2 + 0x378));
    if (*(int *)(param_2 + 0x32c) < 2) {
      pcVar1 = *(code **)(DAT_0009df08 + 0x9ddec);
      (*pcVar1)(*(undefined4 *)(this + 0x4c),0,0,0);
      (*pcVar1)(*(undefined4 *)(this + 0x54),0,0,0);
      (*pcVar1)(*(undefined4 *)(this + 0x5c),0,0,0);
    }
    else {
      (*pcVar1)(*(undefined4 *)(this + 0x4c),1,param_2 + 0x2d8);
      (*pcVar1)(*(undefined4 *)(this + 0x54),1,param_2 + 0x308);
      (*pcVar1)(*(undefined4 *)(this + 0x5c),1,param_2 + 0x2f0);
    }
    glUniform4f(*(undefined4 *)(this + 0x3c),*(undefined4 *)(param_2 + 0x33c),
                *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344),
                *(undefined4 *)(param_2 + 0x37c));
    glUniform1f(*(undefined4 *)(this + 0x60),*(undefined4 *)(param_2 + 0x2c8));
    if (-1 < *(int *)(this + 100)) {
      glUniform3f(*(int *)(this + 100),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    this[9] = (TextureLightShader)0x0;
  }
  pcVar1 = *(code **)(DAT_0009df0c + 0x9de44);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  if (param_1[0x5c] != (Mesh)0x0) {
    pcVar2 = *(code **)(DAT_0009df10 + 0x9de5e);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar1 = *(code **)(DAT_0009df14 + 0x9de74);
    (*pcVar1)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar1)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar1)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    return;
  }
  pcVar1 = *(code **)(DAT_0009df18 + 0x9dec8);
  (*pcVar1)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
  (*pcVar1)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
  (*pcVar1)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
  return;
}

```

## target disasm
```
  0009dd14: push {r4,r5,r6,r7,lr}
  0009dd16: add r7,sp,#0xc
  0009dd18: push {r6,r7,r8,r9,r11}
  0009dd1c: mov r4,r0
  0009dd1e: ldr r0,[r0,#0x2c]
  0009dd20: add.w r3,r2,#0x104
  0009dd24: mov r6,r2
  0009dd26: mov r8,r1
  0009dd28: movs r1,#0x1
  0009dd2a: movs r2,#0x0
  0009dd2c: blx 0x00070990
  0009dd30: ldr r0,[r4,#0x68]
  0009dd32: cmp r0,#0x0
  0009dd34: blt 0x0009dd42
  0009dd36: add.w r3,r6,#0x1c4
  0009dd3a: movs r1,#0x1
  0009dd3c: movs r2,#0x0
  0009dd3e: blx 0x00070990
  0009dd42: ldr r0,[r4,#0x34]
  0009dd44: add.w r3,r6,#0x204
  0009dd48: movs r1,#0x1
  0009dd4a: movs r2,#0x0
  0009dd4c: blx 0x0007099c
  0009dd50: ldr r0,[r4,#0x30]
  0009dd52: cmp r0,#0x0
  0009dd54: blt 0x0009dd62
  0009dd56: add.w r3,r6,#0x144
  0009dd5a: movs r1,#0x1
  0009dd5c: movs r2,#0x0
  0009dd5e: blx 0x00070990
  0009dd62: ldrb r0,[r4,#0x9]
  0009dd64: cmp r0,#0x0
  0009dd66: beq 0x0009de3c
  0009dd68: ldr r0,[r4,#0x6c]
  0009dd6a: cmp r0,#0x0
  0009dd6c: blt 0x0009dd76
  0009dd6e: ldrb.w r1,[r8,#0x85]
  0009dd72: blx 0x0007096c
  0009dd76: ldr r0,[r4,#0x40]
  0009dd78: add.w r2,r6,#0xd0
  0009dd7c: movs r1,#0x1
  0009dd7e: blx 0x00070984
  0009dd82: ldr r1,[0x0009df04]
  0009dd84: add.w r2,r6,#0x2cc
  0009dd88: ldr r0,[r4,#0x48]
  0009dd8a: add r1,pc
  0009dd8c: ldr r5,[r1,#0x0]
  0009dd8e: movs r1,#0x1
  0009dd90: blx r5
  0009dd92: ldr r0,[r4,#0x50]
  0009dd94: add.w r2,r6,#0x2fc
  0009dd98: movs r1,#0x1
  0009dd9a: blx r5
  0009dd9c: ldr r0,[r4,#0x58]
  0009dd9e: add.w r2,r6,#0x2e4
  0009dda2: movs r1,#0x1
  0009dda4: blx r5
  0009dda6: add.w r3,r6,#0x330
  0009ddaa: ldmia r3,{r1,r2,r3}
  0009ddac: ldr r0,[r4,#0x38]
  0009ddae: vldr.32 s0,[r6,#0x378]
  0009ddb2: vstr.32 s0,[sp]
  0009ddb6: blx 0x000709a8
  0009ddba: ldr.w r1,[r6,#0x32c]
  0009ddbe: ldr r0,[r4,#0x4c]
  0009ddc0: cmp r1,#0x2
  0009ddc2: blt 0x0009dde2
  0009ddc4: add.w r2,r6,#0x2d8
  0009ddc8: movs r1,#0x1
  0009ddca: blx r5
  0009ddcc: ldr r0,[r4,#0x54]
  0009ddce: add.w r2,r6,#0x308
  0009ddd2: movs r1,#0x1
  0009ddd4: blx r5
  0009ddd6: ldr r0,[r4,#0x5c]
  0009ddd8: add.w r2,r6,#0x2f0
  0009dddc: movs r1,#0x1
  0009ddde: blx r5
  0009dde0: b 0x0009de04
  0009dde2: ldr r1,[0x0009df08]
  0009dde4: movs r2,#0x0
  0009dde6: movs r3,#0x0
  0009dde8: add r1,pc
  0009ddea: ldr r5,[r1,#0x0]
  0009ddec: movs r1,#0x0
  0009ddee: blx r5
  0009ddf0: ldr r0,[r4,#0x54]
  0009ddf2: movs r1,#0x0
  0009ddf4: movs r2,#0x0
  0009ddf6: movs r3,#0x0
  0009ddf8: blx r5
  0009ddfa: ldr r0,[r4,#0x5c]
  0009ddfc: movs r1,#0x0
  0009ddfe: movs r2,#0x0
  0009de00: movs r3,#0x0
  0009de02: blx r5
  0009de04: ldr.w r1,[r6,#0x33c]
  0009de08: ldr.w r2,[r6,#0x340]
  0009de0c: ldr.w r3,[r6,#0x344]
  0009de10: ldr r0,[r4,#0x3c]
  0009de12: vldr.32 s0,[r6,#0x37c]
  0009de16: vstr.32 s0,[sp]
  0009de1a: blx 0x000709a8
  0009de1e: ldr.w r1,[r6,#0x2c8]
  0009de22: ldr r0,[r4,#0x60]
  0009de24: blx 0x00070978
  0009de28: ldr r0,[r4,#0x64]
  0009de2a: cmp r0,#0x0
  0009de2c: blt 0x0009de38
  0009de2e: add.w r3,r6,#0x34c
  0009de32: ldmia r3,{r1,r2,r3}
  0009de34: blx 0x000709b4
  0009de38: movs r0,#0x0
  0009de3a: strb r0,[r4,#0x9]
  0009de3c: ldr r1,[0x0009df0c]
  0009de3e: ldr r0,[r4,#0x20]
  0009de40: add r1,pc
  0009de42: ldr r5,[r1,#0x0]
  0009de44: blx r5
  0009de46: ldr r0,[r4,#0x24]
  0009de48: blx r5
  0009de4a: ldr r0,[r4,#0x28]
  0009de4c: blx r5
  0009de4e: ldrb.w r0,[r8,#0x5c]
  0009de52: cbz r0,0x0009deb8
  0009de54: ldr r0,[0x0009df10]
  0009de56: ldr.w r1,[r8,#0x60]
  0009de5a: add r0,pc
  0009de5c: ldr r6,[r0,#0x0]
  0009de5e: movw r0,#0x8892
  0009de62: blx r6
  0009de64: ldr r1,[0x0009df14]
  0009de66: mov.w r9,#0x0
  0009de6a: ldr r0,[r4,#0x20]
  0009de6c: movw r2,#0x1406
  0009de70: add r1,pc
  0009de72: movs r3,#0x0
  0009de74: strd r9,r9,[sp,#0x0]
  0009de78: ldr r5,[r1,#0x0]
  0009de7a: movs r1,#0x3
  0009de7c: blx r5
  0009de7e: ldr.w r1,[r8,#0x68]
  0009de82: movw r0,#0x8892
  0009de86: blx r6
  0009de88: ldr r0,[r4,#0x24]
  0009de8a: movs r1,#0x2
  0009de8c: movw r2,#0x1406
  0009de90: movs r3,#0x0
  0009de92: strd r9,r9,[sp,#0x0]
  0009de96: blx r5
  0009de98: ldr.w r1,[r8,#0x6c]
  0009de9c: movw r0,#0x8892
  0009dea0: blx r6
  0009dea2: ldr r0,[r4,#0x28]
  0009dea4: movs r1,#0x3
  0009dea6: movw r2,#0x1406
  0009deaa: movs r3,#0x0
  0009deac: strd r9,r9,[sp,#0x0]
  0009deb0: blx r5
  0009deb2: pop.w {r2,r3,r8,r9,r11}
  0009deb6: pop {r4,r5,r6,r7,pc}
  0009deb8: ldr r0,[r4,#0x20]
  0009deba: movs r5,#0x0
  0009debc: ldr.w r1,[r8,#0x4]
  0009dec0: movs r3,#0x0
  0009dec2: ldr r2,[0x0009df18]
  0009dec4: add r2,pc
  0009dec6: strd r5,r1,[sp,#0x0]
  0009deca: movs r1,#0x3
  0009decc: ldr r6,[r2,#0x0]
  0009dece: movw r2,#0x1406
  0009ded2: blx r6
  0009ded4: ldr.w r1,[r8,#0x8]
  0009ded8: movw r2,#0x1406
  0009dedc: ldr r0,[r4,#0x24]
  0009dede: movs r3,#0x0
  0009dee0: strd r5,r1,[sp,#0x0]
  0009dee4: movs r1,#0x2
  0009dee6: blx r6
  0009dee8: ldr.w r1,[r8,#0x10]
  0009deec: movw r2,#0x1406
  0009def0: ldr r0,[r4,#0x28]
  0009def2: movs r3,#0x0
  0009def4: strd r5,r1,[sp,#0x0]
  0009def8: movs r1,#0x3
  0009defa: blx r6
  0009defc: pop.w {r2,r3,r8,r9,r11}
  0009df00: pop {r4,r5,r6,r7,pc}
```
