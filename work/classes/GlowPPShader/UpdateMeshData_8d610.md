# GlowPPShader::UpdateMeshData
elf 0x8d610 body 166
Sig: undefined __thiscall UpdateMeshData(GlowPPShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::GlowPPShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::GlowPPShader::UpdateMeshData(GlowPPShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x80),1,0,param_2 + 0x104);
  if (this[9] != (GlowPPShader)0x0) {
    this[9] = (GlowPPShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x90));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x98));
  if (param_1[0x5c] == (Mesh)0x0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x90),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    uVar1 = *(undefined4 *)(this + 0x98);
    uVar2 = *(undefined4 *)(param_1 + 8);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x90),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    uVar1 = *(undefined4 *)(this + 0x98);
    uVar2 = 0;
  }
  glVertexAttribPointer(uVar1,2,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  0009d610: push {r4,r5,r6,r7,lr}
  0009d612: add r7,sp,#0xc
  0009d614: push.w r11
  0009d618: sub sp,#0x8
  0009d61a: mov r4,r0
  0009d61c: ldr.w r0,[r0,#0x80]
  0009d620: add.w r3,r2,#0x104
  0009d624: mov r5,r1
  0009d626: movs r1,#0x1
  0009d628: movs r2,#0x0
  0009d62a: movs r6,#0x0
  0009d62c: blx 0x00070990
  0009d630: ldrb r0,[r4,#0x9]
  0009d632: cbz r0,0x0009d636
  0009d634: strb r6,[r4,#0x9]
  0009d636: ldr.w r0,[r4,#0x90]
  0009d63a: blx 0x00070a5c
  0009d63e: ldr.w r0,[r4,#0x98]
  0009d642: blx 0x00070a5c
  0009d646: ldrb.w r0,[r5,#0x5c]
  0009d64a: cbz r0,0x0009d680
  0009d64c: ldr r1,[r5,#0x60]
  0009d64e: movw r0,#0x8892
  0009d652: blx 0x0006ed04
  0009d656: ldr.w r0,[r4,#0x90]
  0009d65a: movs r6,#0x0
  0009d65c: movs r1,#0x3
  0009d65e: movw r2,#0x1406
  0009d662: movs r3,#0x0
  0009d664: strd r6,r6,[sp,#0x0]
  0009d668: blx 0x000709c0
  0009d66c: ldr r1,[r5,#0x68]
  0009d66e: movw r0,#0x8892
  0009d672: blx 0x0006ed04
  0009d676: ldr.w r0,[r4,#0x98]
  0009d67a: strd r6,r6,[sp,#0x0]
  0009d67e: b 0x0009d6a2
  0009d680: ldr r1,[r5,#0x4]
  0009d682: movs r6,#0x0
  0009d684: ldr.w r0,[r4,#0x90]
  0009d688: movw r2,#0x1406
  0009d68c: movs r3,#0x0
  0009d68e: strd r6,r1,[sp,#0x0]
  0009d692: movs r1,#0x3
  0009d694: blx 0x000709c0
  0009d698: ldr.w r0,[r4,#0x98]
  0009d69c: ldr r1,[r5,#0x8]
  0009d69e: strd r6,r1,[sp,#0x0]
  0009d6a2: movs r1,#0x2
  0009d6a4: movw r2,#0x1406
  0009d6a8: movs r3,#0x0
  0009d6aa: blx 0x000709c0
  0009d6ae: add sp,#0x8
  0009d6b0: pop.w r11
  0009d6b4: pop {r4,r5,r6,r7,pc}
```
