# SimpleShader::UpdateMeshData
elf 0x8903e body 114
Sig: undefined __thiscall UpdateMeshData(SimpleShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::SimpleShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::SimpleShader::UpdateMeshData(SimpleShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x24),1,0,param_2 + 0x104);
  if (this[9] != (SimpleShader)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x28),1,param_2 + 0xd0);
    this[9] = (SimpleShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x20));
  if (param_1[0x5c] == (Mesh)0x0) {
    uVar1 = *(undefined4 *)(this + 0x20);
    uVar2 = *(undefined4 *)(param_1 + 4);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    uVar2 = 0;
    uVar1 = *(undefined4 *)(this + 0x20);
  }
  glVertexAttribPointer(uVar1,3,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  0009903e: push {r4,r5,r6,r7,lr}
  00099040: add r7,sp,#0xc
  00099042: push.w r8
  00099046: sub sp,#0x8
  00099048: mov r4,r0
  0009904a: ldr r0,[r0,#0x24]
  0009904c: add.w r3,r2,#0x104
  00099050: mov r6,r2
  00099052: mov r5,r1
  00099054: movs r1,#0x1
  00099056: movs r2,#0x0
  00099058: mov.w r8,#0x0
  0009905c: blx 0x00070990
  00099060: ldrb r0,[r4,#0x9]
  00099062: cbz r0,0x00099074
  00099064: ldr r0,[r4,#0x28]
  00099066: add.w r2,r6,#0xd0
  0009906a: movs r1,#0x1
  0009906c: blx 0x00070984
  00099070: strb.w r8,[r4,#0x9]
  00099074: ldr r0,[r4,#0x20]
  00099076: blx 0x00070a5c
  0009907a: ldrb.w r0,[r5,#0x5c]
  0009907e: cbz r0,0x00099092
  00099080: ldr r1,[r5,#0x60]
  00099082: movw r0,#0x8892
  00099086: blx 0x0006ed04
  0009908a: movs r1,#0x0
  0009908c: ldr r0,[r4,#0x20]
  0009908e: str r1,[sp,#0x0]
  00099090: b 0x0009909a
  00099092: ldr r0,[r4,#0x20]
  00099094: movs r2,#0x0
  00099096: ldr r1,[r5,#0x4]
  00099098: str r2,[sp,#0x0]
  0009909a: str r1,[sp,#0x4]
  0009909c: movs r1,#0x3
  0009909e: movw r2,#0x1406
  000990a2: movs r3,#0x0
  000990a4: blx 0x000709c0
  000990a8: add sp,#0x8
  000990aa: pop.w r8
  000990ae: pop {r4,r5,r6,r7,pc}
```
