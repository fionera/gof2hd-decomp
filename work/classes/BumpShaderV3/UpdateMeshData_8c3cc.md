# BumpShaderV3::UpdateMeshData
elf 0x8c3cc body 712
Sig: undefined __thiscall UpdateMeshData(BumpShaderV3 * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpShaderV3::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpShaderV3::UpdateMeshData(BumpShaderV3 *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  undefined4 uVar3;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (-1 < *(int *)(this + 0x78)) {
    glUniform1f(*(int *)(this + 0x78),**(undefined4 **)(DAT_0009c698 + 0x9c40a));
  }
  if (-1 < *(int *)(this + 0x7c)) {
    glUniform1f(*(int *)(this + 0x7c),**(undefined4 **)(DAT_0009c69c + 0x9c41c));
  }
  if (this[9] != (BumpShaderV3)0x0) {
    glUniform3f(*(undefined4 *)(this + 0x3c),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    if (-1 < *(int *)(this + 0x44)) {
      glUniform3f(*(int *)(this + 0x44),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform3fv(*(int *)(this + 0x58),1,param_2 + 0x2cc);
    }
    if (-1 < *(int *)(this + 0x60)) {
      glUniform3fv(*(int *)(this + 0x60),1,param_2 + 0x2fc);
    }
    if (-1 < *(int *)(this + 0x68)) {
      glUniform3fv(*(int *)(this + 0x68),1,param_2 + 0x2e4);
    }
    if (-1 < *(int *)(this + 0x70)) {
      glUniform1f(*(int *)(this + 0x70),*(undefined4 *)(param_2 + 0x2c8));
    }
    if (-1 < *(int *)(this + 0x74)) {
      glUniform3fv(*(int *)(this + 0x74),1,param_2 + 800);
    }
    if (-1 < *(int *)(this + 0x80)) {
      uVar3 = DAT_0009c694;
      if (*(int *)(*(int *)(param_1 + 0x30) + 0x24) != 0) {
        uVar3 = 0x3f800000;
      }
      glUniform1f(*(int *)(this + 0x80),uVar3);
    }
    if (*(int *)(param_2 + 0x32c) < 2) {
      pcVar1 = *(code **)(DAT_0009c6a4 + 0x9c50e);
      (*pcVar1)(*(undefined4 *)(this + 0x5c),0,0,0);
      (*pcVar1)(*(undefined4 *)(this + 100),0,0,0);
      (*pcVar1)(*(undefined4 *)(this + 0x6c),0,0,0);
      (*pcVar1)(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x33c),
                *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    else {
      pcVar1 = *(code **)(DAT_0009c6a0 + 0x9c4de);
      (*pcVar1)(*(undefined4 *)(this + 0x5c),1,param_2 + 0x2d8);
      (*pcVar1)(*(undefined4 *)(this + 100),1,param_2 + 0x308);
      (*pcVar1)(*(undefined4 *)(this + 0x6c),1,param_2 + 0x2f0);
      glUniform3f(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x33c),
                  *(undefined4 *)(param_2 + 0x340),*(undefined4 *)(param_2 + 0x344));
    }
    this[9] = (BumpShaderV3)0x0;
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
    pcVar1 = *(code **)(DAT_0009c6a8 + 0x9c578);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_0009c6ac + 0x9c58e);
    (*pcVar2)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar2)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar2)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  0009c3cc: push {r4,r5,r6,r7,lr}
  0009c3ce: add r7,sp,#0xc
  0009c3d0: push {r6,r7,r8,r9,r11}
  0009c3d4: mov r4,r0
  0009c3d6: ldr r0,[r0,#0x34]
  0009c3d8: mov r6,r2
  0009c3da: mov r8,r1
  0009c3dc: cmp r0,#0x0
  0009c3de: blt 0x0009c3ec
  0009c3e0: add.w r3,r6,#0x104
  0009c3e4: movs r1,#0x1
  0009c3e6: movs r2,#0x0
  0009c3e8: blx 0x00070990
  0009c3ec: ldr r0,[r4,#0x38]
  0009c3ee: cmp r0,#0x0
  0009c3f0: blt 0x0009c3fe
  0009c3f2: add.w r3,r6,#0x204
  0009c3f6: movs r1,#0x1
  0009c3f8: movs r2,#0x0
  0009c3fa: blx 0x0007099c
  0009c3fe: ldr r0,[r4,#0x78]
  0009c400: cmp r0,#0x0
  0009c402: blt 0x0009c410
  0009c404: ldr r1,[0x0009c698]
  0009c406: add r1,pc
  0009c408: ldr r1,[r1,#0x0]
  0009c40a: ldr r1,[r1,#0x0]
  0009c40c: blx 0x00070978
  0009c410: ldr r0,[r4,#0x7c]
  0009c412: cmp r0,#0x0
  0009c414: blt 0x0009c422
  0009c416: ldr r1,[0x0009c69c]
  0009c418: add r1,pc
  0009c41a: ldr r1,[r1,#0x0]
  0009c41c: ldr r1,[r1,#0x0]
  0009c41e: blx 0x00070978
  0009c422: ldrb r0,[r4,#0x9]
  0009c424: cmp r0,#0x0
  0009c426: beq.w 0x0009c534
  0009c42a: add.w r3,r6,#0x330
  0009c42e: ldmia r3,{r1,r2,r3}
  0009c430: ldr r0,[r4,#0x3c]
  0009c432: blx 0x000709b4
  0009c436: ldr r0,[r4,#0x44]
  0009c438: cmp r0,#0x0
  0009c43a: blt 0x0009c446
  0009c43c: add.w r3,r6,#0x34c
  0009c440: ldmia r3,{r1,r2,r3}
  0009c442: blx 0x000709b4
  0009c446: ldr r0,[r4,#0x54]
  0009c448: cmp r0,#0x0
  0009c44a: blt 0x0009c456
  0009c44c: add.w r2,r6,#0xd0
  0009c450: movs r1,#0x1
  0009c452: blx 0x00070984
  0009c456: ldr r0,[r4,#0x58]
  0009c458: cmp r0,#0x0
  0009c45a: blt 0x0009c466
  0009c45c: add.w r2,r6,#0x2cc
  0009c460: movs r1,#0x1
  0009c462: blx 0x00070a50
  0009c466: ldr r0,[r4,#0x60]
  0009c468: cmp r0,#0x0
  0009c46a: blt 0x0009c476
  0009c46c: add.w r2,r6,#0x2fc
  0009c470: movs r1,#0x1
  0009c472: blx 0x00070a50
  0009c476: ldr r0,[r4,#0x68]
  0009c478: cmp r0,#0x0
  0009c47a: blt 0x0009c486
  0009c47c: add.w r2,r6,#0x2e4
  0009c480: movs r1,#0x1
  0009c482: blx 0x00070a50
  0009c486: ldr r0,[r4,#0x70]
  0009c488: cmp r0,#0x0
  0009c48a: blt 0x0009c494
  0009c48c: ldr.w r1,[r6,#0x2c8]
  0009c490: blx 0x00070978
  0009c494: ldr r0,[r4,#0x74]
  0009c496: cmp r0,#0x0
  0009c498: blt 0x0009c4a4
  0009c49a: add.w r2,r6,#0x320
  0009c49e: movs r1,#0x1
  0009c4a0: blx 0x00070a50
  0009c4a4: ldr.w r0,[r4,#0x80]
  0009c4a8: cmp r0,#0x0
  0009c4aa: blt 0x0009c4ca
  0009c4ac: vmov.f32 s2,0x3f800000
  0009c4b0: ldr.w r1,[r8,#0x30]
  0009c4b4: vldr.32 s0,[pc,#0x1dc]
  0009c4b8: ldr r1,[r1,#0x24]
  0009c4ba: cmp r1,#0x0
  0009c4bc: it ne
  0009c4be: vmov.ne.f32 s0,s2
  0009c4c2: vmov r1,s0
  0009c4c6: blx 0x00070978
  0009c4ca: ldr.w r1,[r6,#0x32c]
  0009c4ce: ldr r0,[r4,#0x5c]
  0009c4d0: cmp r1,#0x2
  0009c4d2: blt 0x0009c504
  0009c4d4: ldr r1,[0x0009c6a0]
  0009c4d6: add.w r2,r6,#0x2d8
  0009c4da: add r1,pc
  0009c4dc: ldr r5,[r1,#0x0]
  0009c4de: movs r1,#0x1
  0009c4e0: blx r5
  0009c4e2: ldr r0,[r4,#0x64]
  0009c4e4: add.w r2,r6,#0x308
  0009c4e8: movs r1,#0x1
  0009c4ea: blx r5
  0009c4ec: ldr r0,[r4,#0x6c]
  0009c4ee: add.w r2,r6,#0x2f0
  0009c4f2: movs r1,#0x1
  0009c4f4: blx r5
  0009c4f6: add.w r3,r6,#0x33c
  0009c4fa: ldmia r3,{r1,r2,r3}
  0009c4fc: ldr r0,[r4,#0x40]
  0009c4fe: blx 0x000709b4
  0009c502: b 0x0009c530
  0009c504: ldr r1,[0x0009c6a4]
  0009c506: movs r2,#0x0
  0009c508: movs r3,#0x0
  0009c50a: add r1,pc
  0009c50c: ldr r5,[r1,#0x0]
  0009c50e: movs r1,#0x0
  0009c510: blx r5
  0009c512: ldr r0,[r4,#0x64]
  0009c514: movs r1,#0x0
  0009c516: movs r2,#0x0
  0009c518: movs r3,#0x0
  0009c51a: blx r5
  0009c51c: ldr r0,[r4,#0x6c]
  0009c51e: movs r1,#0x0
  0009c520: movs r2,#0x0
  0009c522: movs r3,#0x0
  0009c524: blx r5
  0009c526: add.w r3,r6,#0x33c
  0009c52a: ldmia r3,{r1,r2,r3}
  0009c52c: ldr r0,[r4,#0x40]
  0009c52e: blx r5
  0009c530: movs r0,#0x0
  0009c532: strb r0,[r4,#0x9]
  0009c534: ldr r0,[r4,#0x20]
  0009c536: cmp r0,#0x0
  0009c538: blt 0x0009c53e
  0009c53a: blx 0x00070a5c
  0009c53e: ldr r0,[r4,#0x24]
  0009c540: cmp r0,#0x0
  0009c542: blt 0x0009c548
  0009c544: blx 0x00070a5c
  0009c548: ldr r0,[r4,#0x28]
  0009c54a: cmp r0,#0x0
  0009c54c: blt 0x0009c552
  0009c54e: blx 0x00070a5c
  0009c552: ldr r0,[r4,#0x2c]
  0009c554: cmp r0,#0x0
  0009c556: blt 0x0009c55c
  0009c558: blx 0x00070a5c
  0009c55c: ldr r0,[r4,#0x30]
  0009c55e: cmp r0,#0x0
  0009c560: blt 0x0009c566
  0009c562: blx 0x00070a5c
  0009c566: ldrb.w r0,[r8,#0x5c]
  0009c56a: cmp r0,#0x0
  0009c56c: beq 0x0009c602
  0009c56e: ldr r0,[0x0009c6a8]
  0009c570: ldr.w r1,[r8,#0x60]
  0009c574: add r0,pc
  0009c576: ldr r5,[r0,#0x0]
  0009c578: movw r0,#0x8892
  0009c57c: blx r5
  0009c57e: ldr r1,[0x0009c6ac]
  0009c580: mov.w r9,#0x0
  0009c584: ldr r0,[r4,#0x20]
  0009c586: movw r2,#0x1406
  0009c58a: add r1,pc
  0009c58c: movs r3,#0x0
  0009c58e: strd r9,r9,[sp,#0x0]
  0009c592: ldr r6,[r1,#0x0]
  0009c594: movs r1,#0x3
  0009c596: blx r6
  0009c598: ldr.w r1,[r8,#0x68]
  0009c59c: movw r0,#0x8892
  0009c5a0: blx r5
  0009c5a2: ldr r0,[r4,#0x24]
  0009c5a4: movs r1,#0x2
  0009c5a6: movw r2,#0x1406
  0009c5aa: movs r3,#0x0
  0009c5ac: strd r9,r9,[sp,#0x0]
  0009c5b0: blx r6
  0009c5b2: ldr.w r1,[r8,#0x6c]
  0009c5b6: movw r0,#0x8892
  0009c5ba: blx r5
  0009c5bc: ldr r0,[r4,#0x28]
  0009c5be: movs r1,#0x3
  0009c5c0: movw r2,#0x1406
  0009c5c4: movs r3,#0x0
  0009c5c6: strd r9,r9,[sp,#0x0]
  0009c5ca: blx r6
  0009c5cc: ldr.w r1,[r8,#0x70]
  0009c5d0: movw r0,#0x8892
  0009c5d4: blx r5
  0009c5d6: ldr r0,[r4,#0x2c]
  0009c5d8: movs r1,#0x3
  0009c5da: movw r2,#0x1406
  0009c5de: movs r3,#0x0
  0009c5e0: strd r9,r9,[sp,#0x0]
  0009c5e4: blx r6
  0009c5e6: ldr.w r1,[r8,#0x74]
  0009c5ea: movw r0,#0x8892
  0009c5ee: blx r5
  0009c5f0: ldr r0,[r4,#0x30]
  0009c5f2: movs r1,#0x3
  0009c5f4: movw r2,#0x1406
  0009c5f8: movs r3,#0x0
  0009c5fa: strd r9,r9,[sp,#0x0]
  0009c5fe: blx r6
  0009c600: b 0x0009c68e
  0009c602: ldr r0,[r4,#0x20]
  0009c604: cmp r0,#0x0
  0009c606: blt 0x0009c61e
  0009c608: ldr.w r1,[r8,#0x4]
  0009c60c: movs r2,#0x0
  0009c60e: movs r3,#0x0
  0009c610: strd r2,r1,[sp,#0x0]
  0009c614: movs r1,#0x3
  0009c616: movw r2,#0x1406
  0009c61a: blx 0x000709c0
  0009c61e: ldr r0,[r4,#0x24]
  0009c620: cmp r0,#0x0
  0009c622: blt 0x0009c63a
  0009c624: ldr.w r1,[r8,#0x8]
  0009c628: movs r2,#0x0
  0009c62a: movs r3,#0x0
  0009c62c: strd r2,r1,[sp,#0x0]
  0009c630: movs r1,#0x2
  0009c632: movw r2,#0x1406
  0009c636: blx 0x000709c0
  0009c63a: ldr r0,[r4,#0x28]
  0009c63c: cmp r0,#0x0
  0009c63e: blt 0x0009c656
  0009c640: ldr.w r1,[r8,#0x10]
  0009c644: movs r2,#0x0
  0009c646: movs r3,#0x0
  0009c648: strd r2,r1,[sp,#0x0]
  0009c64c: movs r1,#0x3
  0009c64e: movw r2,#0x1406
  0009c652: blx 0x000709c0
  0009c656: ldr r0,[r4,#0x2c]
  0009c658: cmp r0,#0x0
  0009c65a: blt 0x0009c672
  0009c65c: ldr.w r1,[r8,#0x14]
  0009c660: movs r2,#0x0
  0009c662: movs r3,#0x0
  0009c664: strd r2,r1,[sp,#0x0]
  0009c668: movs r1,#0x3
  0009c66a: movw r2,#0x1406
  0009c66e: blx 0x000709c0
  0009c672: ldr r0,[r4,#0x30]
  0009c674: cmp r0,#0x0
  0009c676: blt 0x0009c68e
  0009c678: ldr.w r1,[r8,#0x18]
  0009c67c: movs r2,#0x0
  0009c67e: movs r3,#0x0
  0009c680: strd r2,r1,[sp,#0x0]
  0009c684: movs r1,#0x3
  0009c686: movw r2,#0x1406
  0009c68a: blx 0x000709c0
  0009c68e: pop.w {r2,r3,r8,r9,r11}
  0009c692: pop {r4,r5,r6,r7,pc}
```
