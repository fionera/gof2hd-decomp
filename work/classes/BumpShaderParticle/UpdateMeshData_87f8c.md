# BumpShaderParticle::UpdateMeshData
elf 0x87f8c body 616
Sig: undefined __thiscall UpdateMeshData(BumpShaderParticle * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpShaderParticle::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpShaderParticle::UpdateMeshData
          (BumpShaderParticle *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix4fv(*(int *)(this + 0x38),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniformMatrix3fv(*(int *)(this + 0x3c),1,0,param_2 + 0x204);
  }
  if (-1 < *(int *)(this + 100)) {
    glUniform1f(*(int *)(this + 100),**(undefined4 **)(DAT_000981f4 + 0x97fca));
  }
  if (-1 < *(int *)(this + 0x68)) {
    glUniform1f(*(int *)(this + 0x68),**(undefined4 **)(DAT_000981f8 + 0x97fdc));
  }
  if (this[9] != (BumpShaderParticle)0x0) {
    glUniform3f(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    if (-1 < *(int *)(this + 0x44)) {
      glUniform3f(*(int *)(this + 0x44),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform3fv(*(int *)(this + 0x54),1,param_2 + 0x314);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform3fv(*(int *)(this + 0x58),1,param_2 + 0x2fc);
    }
    if (-1 < *(int *)(this + 0x5c)) {
      glUniform3fv(*(int *)(this + 0x5c),1,param_2 + 0x2e4);
    }
    if (-1 < *(int *)(this + 0x60)) {
      glUniform1f(*(int *)(this + 0x60),*(undefined4 *)(param_2 + 0x2c8));
    }
    this[9] = (BumpShaderParticle)0x0;
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
  if (-1 < *(int *)(this + 0x34)) {
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
    if (-1 < *(int *)(this + 0x34)) {
      glVertexAttribPointer(*(int *)(this + 0x34),4,0x1406,0,0,*(undefined4 *)(param_1 + 0xc));
    }
  }
  else {
    pcVar1 = *(code **)(DAT_000981fc + 0x980a2);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_00098200 + 0x980b8);
    (*pcVar2)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar2)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar2)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x78));
    (*pcVar2)(*(undefined4 *)(this + 0x34),4,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  00097f8c: push {r4,r5,r6,r7,lr}
  00097f8e: add r7,sp,#0xc
  00097f90: push {r6,r7,r8,r9,r11}
  00097f94: mov r4,r0
  00097f96: ldr r0,[r0,#0x38]
  00097f98: mov r6,r2
  00097f9a: mov r8,r1
  00097f9c: cmp r0,#0x0
  00097f9e: blt 0x00097fac
  00097fa0: add.w r3,r6,#0x104
  00097fa4: movs r1,#0x1
  00097fa6: movs r2,#0x0
  00097fa8: blx 0x00070990
  00097fac: ldr r0,[r4,#0x3c]
  00097fae: cmp r0,#0x0
  00097fb0: blt 0x00097fbe
  00097fb2: add.w r3,r6,#0x204
  00097fb6: movs r1,#0x1
  00097fb8: movs r2,#0x0
  00097fba: blx 0x0007099c
  00097fbe: ldr r0,[r4,#0x64]
  00097fc0: cmp r0,#0x0
  00097fc2: blt 0x00097fd0
  00097fc4: ldr r1,[0x000981f4]
  00097fc6: add r1,pc
  00097fc8: ldr r1,[r1,#0x0]
  00097fca: ldr r1,[r1,#0x0]
  00097fcc: blx 0x00070978
  00097fd0: ldr r0,[r4,#0x68]
  00097fd2: cmp r0,#0x0
  00097fd4: blt 0x00097fe2
  00097fd6: ldr r1,[0x000981f8]
  00097fd8: add r1,pc
  00097fda: ldr r1,[r1,#0x0]
  00097fdc: ldr r1,[r1,#0x0]
  00097fde: blx 0x00070978
  00097fe2: ldrb r0,[r4,#0x9]
  00097fe4: cbz r0,0x00098054
  00097fe6: add.w r3,r6,#0x330
  00097fea: ldmia r3,{r1,r2,r3}
  00097fec: ldr r0,[r4,#0x40]
  00097fee: blx 0x000709b4
  00097ff2: ldr r0,[r4,#0x44]
  00097ff4: cmp r0,#0x0
  00097ff6: blt 0x00098002
  00097ff8: add.w r3,r6,#0x34c
  00097ffc: ldmia r3,{r1,r2,r3}
  00097ffe: blx 0x000709b4
  00098002: ldr r0,[r4,#0x50]
  00098004: cmp r0,#0x0
  00098006: blt 0x00098012
  00098008: add.w r2,r6,#0xd0
  0009800c: movs r1,#0x1
  0009800e: blx 0x00070984
  00098012: ldr r0,[r4,#0x54]
  00098014: cmp r0,#0x0
  00098016: blt 0x00098022
  00098018: add.w r2,r6,#0x314
  0009801c: movs r1,#0x1
  0009801e: blx 0x00070a50
  00098022: ldr r0,[r4,#0x58]
  00098024: cmp r0,#0x0
  00098026: blt 0x00098032
  00098028: add.w r2,r6,#0x2fc
  0009802c: movs r1,#0x1
  0009802e: blx 0x00070a50
  00098032: ldr r0,[r4,#0x5c]
  00098034: cmp r0,#0x0
  00098036: blt 0x00098042
  00098038: add.w r2,r6,#0x2e4
  0009803c: movs r1,#0x1
  0009803e: blx 0x00070a50
  00098042: ldr r0,[r4,#0x60]
  00098044: cmp r0,#0x0
  00098046: blt 0x00098050
  00098048: ldr.w r1,[r6,#0x2c8]
  0009804c: blx 0x00070978
  00098050: movs r0,#0x0
  00098052: strb r0,[r4,#0x9]
  00098054: ldr r0,[r4,#0x20]
  00098056: cmp r0,#0x0
  00098058: blt 0x0009805e
  0009805a: blx 0x00070a5c
  0009805e: ldr r0,[r4,#0x24]
  00098060: cmp r0,#0x0
  00098062: blt 0x00098068
  00098064: blx 0x00070a5c
  00098068: ldr r0,[r4,#0x28]
  0009806a: cmp r0,#0x0
  0009806c: blt 0x00098072
  0009806e: blx 0x00070a5c
  00098072: ldr r0,[r4,#0x2c]
  00098074: cmp r0,#0x0
  00098076: blt 0x0009807c
  00098078: blx 0x00070a5c
  0009807c: ldr r0,[r4,#0x30]
  0009807e: cmp r0,#0x0
  00098080: blt 0x00098086
  00098082: blx 0x00070a5c
  00098086: ldr r0,[r4,#0x34]
  00098088: cmp r0,#0x0
  0009808a: blt 0x00098090
  0009808c: blx 0x00070a5c
  00098090: ldrb.w r0,[r8,#0x5c]
  00098094: cmp r0,#0x0
  00098096: beq 0x00098146
  00098098: ldr r0,[0x000981fc]
  0009809a: ldr.w r1,[r8,#0x60]
  0009809e: add r0,pc
  000980a0: ldr r5,[r0,#0x0]
  000980a2: movw r0,#0x8892
  000980a6: blx r5
  000980a8: ldr r1,[0x00098200]
  000980aa: mov.w r9,#0x0
  000980ae: ldr r0,[r4,#0x20]
  000980b0: movw r2,#0x1406
  000980b4: add r1,pc
  000980b6: movs r3,#0x0
  000980b8: strd r9,r9,[sp,#0x0]
  000980bc: ldr r6,[r1,#0x0]
  000980be: movs r1,#0x3
  000980c0: blx r6
  000980c2: ldr.w r1,[r8,#0x68]
  000980c6: movw r0,#0x8892
  000980ca: blx r5
  000980cc: ldr r0,[r4,#0x24]
  000980ce: movs r1,#0x2
  000980d0: movw r2,#0x1406
  000980d4: movs r3,#0x0
  000980d6: strd r9,r9,[sp,#0x0]
  000980da: blx r6
  000980dc: ldr.w r1,[r8,#0x6c]
  000980e0: movw r0,#0x8892
  000980e4: blx r5
  000980e6: ldr r0,[r4,#0x28]
  000980e8: movs r1,#0x3
  000980ea: movw r2,#0x1406
  000980ee: movs r3,#0x0
  000980f0: strd r9,r9,[sp,#0x0]
  000980f4: blx r6
  000980f6: ldr.w r1,[r8,#0x70]
  000980fa: movw r0,#0x8892
  000980fe: blx r5
  00098100: ldr r0,[r4,#0x2c]
  00098102: movs r1,#0x3
  00098104: movw r2,#0x1406
  00098108: movs r3,#0x0
  0009810a: strd r9,r9,[sp,#0x0]
  0009810e: blx r6
  00098110: ldr.w r1,[r8,#0x74]
  00098114: movw r0,#0x8892
  00098118: blx r5
  0009811a: ldr r0,[r4,#0x30]
  0009811c: movs r1,#0x3
  0009811e: movw r2,#0x1406
  00098122: movs r3,#0x0
  00098124: strd r9,r9,[sp,#0x0]
  00098128: blx r6
  0009812a: ldr.w r1,[r8,#0x78]
  0009812e: movw r0,#0x8892
  00098132: blx r5
  00098134: ldr r0,[r4,#0x34]
  00098136: movs r1,#0x4
  00098138: movw r2,#0x1406
  0009813c: movs r3,#0x0
  0009813e: strd r9,r9,[sp,#0x0]
  00098142: blx r6
  00098144: b 0x000981ee
  00098146: ldr r0,[r4,#0x20]
  00098148: cmp r0,#0x0
  0009814a: blt 0x00098162
  0009814c: ldr.w r1,[r8,#0x4]
  00098150: movs r2,#0x0
  00098152: movs r3,#0x0
  00098154: strd r2,r1,[sp,#0x0]
  00098158: movs r1,#0x3
  0009815a: movw r2,#0x1406
  0009815e: blx 0x000709c0
  00098162: ldr r0,[r4,#0x24]
  00098164: cmp r0,#0x0
  00098166: blt 0x0009817e
  00098168: ldr.w r1,[r8,#0x8]
  0009816c: movs r2,#0x0
  0009816e: movs r3,#0x0
  00098170: strd r2,r1,[sp,#0x0]
  00098174: movs r1,#0x2
  00098176: movw r2,#0x1406
  0009817a: blx 0x000709c0
  0009817e: ldr r0,[r4,#0x28]
  00098180: cmp r0,#0x0
  00098182: blt 0x0009819a
  00098184: ldr.w r1,[r8,#0x10]
  00098188: movs r2,#0x0
  0009818a: movs r3,#0x0
  0009818c: strd r2,r1,[sp,#0x0]
  00098190: movs r1,#0x3
  00098192: movw r2,#0x1406
  00098196: blx 0x000709c0
  0009819a: ldr r0,[r4,#0x2c]
  0009819c: cmp r0,#0x0
  0009819e: blt 0x000981b6
  000981a0: ldr.w r1,[r8,#0x14]
  000981a4: movs r2,#0x0
  000981a6: movs r3,#0x0
  000981a8: strd r2,r1,[sp,#0x0]
  000981ac: movs r1,#0x3
  000981ae: movw r2,#0x1406
  000981b2: blx 0x000709c0
  000981b6: ldr r0,[r4,#0x30]
  000981b8: cmp r0,#0x0
  000981ba: blt 0x000981d2
  000981bc: ldr.w r1,[r8,#0x18]
  000981c0: movs r2,#0x0
  000981c2: movs r3,#0x0
  000981c4: strd r2,r1,[sp,#0x0]
  000981c8: movs r1,#0x3
  000981ca: movw r2,#0x1406
  000981ce: blx 0x000709c0
  000981d2: ldr r0,[r4,#0x34]
  000981d4: cmp r0,#0x0
  000981d6: blt 0x000981ee
  000981d8: ldr.w r1,[r8,#0xc]
  000981dc: movs r2,#0x0
  000981de: movs r3,#0x0
  000981e0: strd r2,r1,[sp,#0x0]
  000981e4: movs r1,#0x4
  000981e6: movw r2,#0x1406
  000981ea: blx 0x000709c0
  000981ee: pop.w {r2,r3,r8,r9,r11}
  000981f2: pop {r4,r5,r6,r7,pc}
```
