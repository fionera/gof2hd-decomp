# BloomShader::UpdateMeshData
elf 0x8a158 body 166
Sig: undefined __thiscall UpdateMeshData(BloomShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BloomShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BloomShader::UpdateMeshData(BloomShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x90),1,0,param_2 + 0x104);
  if (this[9] != (BloomShader)0x0) {
    this[9] = (BloomShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x8c));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x94));
  if (param_1[0x5c] == (Mesh)0x0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x8c),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    uVar1 = *(undefined4 *)(this + 0x94);
    uVar2 = *(undefined4 *)(param_1 + 8);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x8c),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    uVar1 = *(undefined4 *)(this + 0x94);
    uVar2 = 0;
  }
  glVertexAttribPointer(uVar1,2,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  0009a158: push {r4,r5,r6,r7,lr}
  0009a15a: add r7,sp,#0xc
  0009a15c: push.w r11
  0009a160: sub sp,#0x8
  0009a162: mov r4,r0
  0009a164: ldr.w r0,[r0,#0x90]
  0009a168: add.w r3,r2,#0x104
  0009a16c: mov r5,r1
  0009a16e: movs r1,#0x1
  0009a170: movs r2,#0x0
  0009a172: movs r6,#0x0
  0009a174: blx 0x00070990
  0009a178: ldrb r0,[r4,#0x9]
  0009a17a: cbz r0,0x0009a17e
  0009a17c: strb r6,[r4,#0x9]
  0009a17e: ldr.w r0,[r4,#0x8c]
  0009a182: blx 0x00070a5c
  0009a186: ldr.w r0,[r4,#0x94]
  0009a18a: blx 0x00070a5c
  0009a18e: ldrb.w r0,[r5,#0x5c]
  0009a192: cbz r0,0x0009a1c8
  0009a194: ldr r1,[r5,#0x60]
  0009a196: movw r0,#0x8892
  0009a19a: blx 0x0006ed04
  0009a19e: ldr.w r0,[r4,#0x8c]
  0009a1a2: movs r6,#0x0
  0009a1a4: movs r1,#0x3
  0009a1a6: movw r2,#0x1406
  0009a1aa: movs r3,#0x0
  0009a1ac: strd r6,r6,[sp,#0x0]
  0009a1b0: blx 0x000709c0
  0009a1b4: ldr r1,[r5,#0x68]
  0009a1b6: movw r0,#0x8892
  0009a1ba: blx 0x0006ed04
  0009a1be: ldr.w r0,[r4,#0x94]
  0009a1c2: strd r6,r6,[sp,#0x0]
  0009a1c6: b 0x0009a1ea
  0009a1c8: ldr r1,[r5,#0x4]
  0009a1ca: movs r6,#0x0
  0009a1cc: ldr.w r0,[r4,#0x8c]
  0009a1d0: movw r2,#0x1406
  0009a1d4: movs r3,#0x0
  0009a1d6: strd r6,r1,[sp,#0x0]
  0009a1da: movs r1,#0x3
  0009a1dc: blx 0x000709c0
  0009a1e0: ldr.w r0,[r4,#0x94]
  0009a1e4: ldr r1,[r5,#0x8]
  0009a1e6: strd r6,r1,[sp,#0x0]
  0009a1ea: movs r1,#0x2
  0009a1ec: movw r2,#0x1406
  0009a1f0: movs r3,#0x0
  0009a1f2: blx 0x000709c0
  0009a1f6: add sp,#0x8
  0009a1f8: pop.w r11
  0009a1fc: pop {r4,r5,r6,r7,pc}
```
