# DNSShader::UpdateMeshData
elf 0x8f504 body 570
Sig: undefined __thiscall UpdateMeshData(DNSShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::DNSShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::DNSShader::UpdateMeshData(DNSShader *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniformMatrix4fv(*(int *)(this + 0x3c),1,0,param_2 + 0x144);
  }
  if (-1 < *(int *)(this + 0x5c)) {
    glUniform1f(*(int *)(this + 0x5c),**(undefined4 **)(DAT_0009f740 + 0x9f554));
  }
  if (-1 < *(int *)(this + 0x60)) {
    glUniform1f(*(int *)(this + 0x60),**(undefined4 **)(DAT_0009f744 + 0x9f566));
  }
  if (this[9] != (DNSShader)0x0) {
    glUniform3f(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x330),
                *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    if (-1 < *(int *)(this + 0x44)) {
      glUniform3f(*(int *)(this + 0x44),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x48)) {
      glUniform4fv(*(int *)(this + 0x48),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform3fv(*(int *)(this + 0x4c),1,param_2 + 0x2cc);
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform3fv(*(int *)(this + 0x50),1,param_2 + 0x2fc);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform3fv(*(int *)(this + 0x54),1,param_2 + 0x2e4);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform1f(*(int *)(this + 0x58),*(undefined4 *)(param_2 + 0x2c8));
    }
    this[9] = (DNSShader)0x0;
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
    pcVar1 = *(code **)(DAT_0009f748 + 0x9f622);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_0009f74c + 0x9f638);
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
  0009f504: push {r4,r5,r6,r7,lr}
  0009f506: add r7,sp,#0xc
  0009f508: push {r6,r7,r8,r9,r11}
  0009f50c: mov r4,r0
  0009f50e: ldr r0,[r0,#0x34]
  0009f510: mov r6,r2
  0009f512: mov r8,r1
  0009f514: cmp r0,#0x0
  0009f516: blt 0x0009f524
  0009f518: add.w r3,r6,#0x104
  0009f51c: movs r1,#0x1
  0009f51e: movs r2,#0x0
  0009f520: blx 0x00070990
  0009f524: ldr r0,[r4,#0x38]
  0009f526: cmp r0,#0x0
  0009f528: blt 0x0009f536
  0009f52a: add.w r3,r6,#0x204
  0009f52e: movs r1,#0x1
  0009f530: movs r2,#0x0
  0009f532: blx 0x0007099c
  0009f536: ldr r0,[r4,#0x3c]
  0009f538: cmp r0,#0x0
  0009f53a: blt 0x0009f548
  0009f53c: add.w r3,r6,#0x144
  0009f540: movs r1,#0x1
  0009f542: movs r2,#0x0
  0009f544: blx 0x00070990
  0009f548: ldr r0,[r4,#0x5c]
  0009f54a: cmp r0,#0x0
  0009f54c: blt 0x0009f55a
  0009f54e: ldr r1,[0x0009f740]
  0009f550: add r1,pc
  0009f552: ldr r1,[r1,#0x0]
  0009f554: ldr r1,[r1,#0x0]
  0009f556: blx 0x00070978
  0009f55a: ldr r0,[r4,#0x60]
  0009f55c: cmp r0,#0x0
  0009f55e: blt 0x0009f56c
  0009f560: ldr r1,[0x0009f744]
  0009f562: add r1,pc
  0009f564: ldr r1,[r1,#0x0]
  0009f566: ldr r1,[r1,#0x0]
  0009f568: blx 0x00070978
  0009f56c: ldrb r0,[r4,#0x9]
  0009f56e: cbz r0,0x0009f5de
  0009f570: add.w r3,r6,#0x330
  0009f574: ldmia r3,{r1,r2,r3}
  0009f576: ldr r0,[r4,#0x40]
  0009f578: blx 0x000709b4
  0009f57c: ldr r0,[r4,#0x44]
  0009f57e: cmp r0,#0x0
  0009f580: blt 0x0009f58c
  0009f582: add.w r3,r6,#0x34c
  0009f586: ldmia r3,{r1,r2,r3}
  0009f588: blx 0x000709b4
  0009f58c: ldr r0,[r4,#0x48]
  0009f58e: cmp r0,#0x0
  0009f590: blt 0x0009f59c
  0009f592: add.w r2,r6,#0xd0
  0009f596: movs r1,#0x1
  0009f598: blx 0x00070984
  0009f59c: ldr r0,[r4,#0x4c]
  0009f59e: cmp r0,#0x0
  0009f5a0: blt 0x0009f5ac
  0009f5a2: add.w r2,r6,#0x2cc
  0009f5a6: movs r1,#0x1
  0009f5a8: blx 0x00070a50
  0009f5ac: ldr r0,[r4,#0x50]
  0009f5ae: cmp r0,#0x0
  0009f5b0: blt 0x0009f5bc
  0009f5b2: add.w r2,r6,#0x2fc
  0009f5b6: movs r1,#0x1
  0009f5b8: blx 0x00070a50
  0009f5bc: ldr r0,[r4,#0x54]
  0009f5be: cmp r0,#0x0
  0009f5c0: blt 0x0009f5cc
  0009f5c2: add.w r2,r6,#0x2e4
  0009f5c6: movs r1,#0x1
  0009f5c8: blx 0x00070a50
  0009f5cc: ldr r0,[r4,#0x58]
  0009f5ce: cmp r0,#0x0
  0009f5d0: blt 0x0009f5da
  0009f5d2: ldr.w r1,[r6,#0x2c8]
  0009f5d6: blx 0x00070978
  0009f5da: movs r0,#0x0
  0009f5dc: strb r0,[r4,#0x9]
  0009f5de: ldr r0,[r4,#0x20]
  0009f5e0: cmp r0,#0x0
  0009f5e2: blt 0x0009f5e8
  0009f5e4: blx 0x00070a5c
  0009f5e8: ldr r0,[r4,#0x24]
  0009f5ea: cmp r0,#0x0
  0009f5ec: blt 0x0009f5f2
  0009f5ee: blx 0x00070a5c
  0009f5f2: ldr r0,[r4,#0x28]
  0009f5f4: cmp r0,#0x0
  0009f5f6: blt 0x0009f5fc
  0009f5f8: blx 0x00070a5c
  0009f5fc: ldr r0,[r4,#0x2c]
  0009f5fe: cmp r0,#0x0
  0009f600: blt 0x0009f606
  0009f602: blx 0x00070a5c
  0009f606: ldr r0,[r4,#0x30]
  0009f608: cmp r0,#0x0
  0009f60a: blt 0x0009f610
  0009f60c: blx 0x00070a5c
  0009f610: ldrb.w r0,[r8,#0x5c]
  0009f614: cmp r0,#0x0
  0009f616: beq 0x0009f6ac
  0009f618: ldr r0,[0x0009f748]
  0009f61a: ldr.w r1,[r8,#0x60]
  0009f61e: add r0,pc
  0009f620: ldr r5,[r0,#0x0]
  0009f622: movw r0,#0x8892
  0009f626: blx r5
  0009f628: ldr r1,[0x0009f74c]
  0009f62a: mov.w r9,#0x0
  0009f62e: ldr r0,[r4,#0x20]
  0009f630: movw r2,#0x1406
  0009f634: add r1,pc
  0009f636: movs r3,#0x0
  0009f638: strd r9,r9,[sp,#0x0]
  0009f63c: ldr r6,[r1,#0x0]
  0009f63e: movs r1,#0x3
  0009f640: blx r6
  0009f642: ldr.w r1,[r8,#0x68]
  0009f646: movw r0,#0x8892
  0009f64a: blx r5
  0009f64c: ldr r0,[r4,#0x24]
  0009f64e: movs r1,#0x2
  0009f650: movw r2,#0x1406
  0009f654: movs r3,#0x0
  0009f656: strd r9,r9,[sp,#0x0]
  0009f65a: blx r6
  0009f65c: ldr.w r1,[r8,#0x6c]
  0009f660: movw r0,#0x8892
  0009f664: blx r5
  0009f666: ldr r0,[r4,#0x28]
  0009f668: movs r1,#0x3
  0009f66a: movw r2,#0x1406
  0009f66e: movs r3,#0x0
  0009f670: strd r9,r9,[sp,#0x0]
  0009f674: blx r6
  0009f676: ldr.w r1,[r8,#0x70]
  0009f67a: movw r0,#0x8892
  0009f67e: blx r5
  0009f680: ldr r0,[r4,#0x2c]
  0009f682: movs r1,#0x3
  0009f684: movw r2,#0x1406
  0009f688: movs r3,#0x0
  0009f68a: strd r9,r9,[sp,#0x0]
  0009f68e: blx r6
  0009f690: ldr.w r1,[r8,#0x74]
  0009f694: movw r0,#0x8892
  0009f698: blx r5
  0009f69a: ldr r0,[r4,#0x30]
  0009f69c: movs r1,#0x3
  0009f69e: movw r2,#0x1406
  0009f6a2: movs r3,#0x0
  0009f6a4: strd r9,r9,[sp,#0x0]
  0009f6a8: blx r6
  0009f6aa: b 0x0009f738
  0009f6ac: ldr r0,[r4,#0x20]
  0009f6ae: cmp r0,#0x0
  0009f6b0: blt 0x0009f6c8
  0009f6b2: ldr.w r1,[r8,#0x4]
  0009f6b6: movs r2,#0x0
  0009f6b8: movs r3,#0x0
  0009f6ba: strd r2,r1,[sp,#0x0]
  0009f6be: movs r1,#0x3
  0009f6c0: movw r2,#0x1406
  0009f6c4: blx 0x000709c0
  0009f6c8: ldr r0,[r4,#0x24]
  0009f6ca: cmp r0,#0x0
  0009f6cc: blt 0x0009f6e4
  0009f6ce: ldr.w r1,[r8,#0x8]
  0009f6d2: movs r2,#0x0
  0009f6d4: movs r3,#0x0
  0009f6d6: strd r2,r1,[sp,#0x0]
  0009f6da: movs r1,#0x2
  0009f6dc: movw r2,#0x1406
  0009f6e0: blx 0x000709c0
  0009f6e4: ldr r0,[r4,#0x28]
  0009f6e6: cmp r0,#0x0
  0009f6e8: blt 0x0009f700
  0009f6ea: ldr.w r1,[r8,#0x10]
  0009f6ee: movs r2,#0x0
  0009f6f0: movs r3,#0x0
  0009f6f2: strd r2,r1,[sp,#0x0]
  0009f6f6: movs r1,#0x3
  0009f6f8: movw r2,#0x1406
  0009f6fc: blx 0x000709c0
  0009f700: ldr r0,[r4,#0x2c]
  0009f702: cmp r0,#0x0
  0009f704: blt 0x0009f71c
  0009f706: ldr.w r1,[r8,#0x14]
  0009f70a: movs r2,#0x0
  0009f70c: movs r3,#0x0
  0009f70e: strd r2,r1,[sp,#0x0]
  0009f712: movs r1,#0x3
  0009f714: movw r2,#0x1406
  0009f718: blx 0x000709c0
  0009f71c: ldr r0,[r4,#0x30]
  0009f71e: cmp r0,#0x0
  0009f720: blt 0x0009f738
  0009f722: ldr.w r1,[r8,#0x18]
  0009f726: movs r2,#0x0
  0009f728: movs r3,#0x0
  0009f72a: strd r2,r1,[sp,#0x0]
  0009f72e: movs r1,#0x3
  0009f730: movw r2,#0x1406
  0009f734: blx 0x000709c0
  0009f738: pop.w {r2,r3,r8,r9,r11}
  0009f73c: pop {r4,r5,r6,r7,pc}
```
