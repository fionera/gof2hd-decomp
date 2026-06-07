# NoTexShader::UpdateMeshData
elf 0x906b6 body 132
Sig: undefined __thiscall UpdateMeshData(NoTexShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::NoTexShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::NoTexShader::UpdateMeshData(NoTexShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x24),1,0,param_2 + 0x104);
  if (this[9] != (NoTexShader)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x28),1,param_2 + 0xd0);
    this[9] = (NoTexShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x20));
  if (param_1 == (Mesh *)0x0) {
    uVar2 = *(undefined4 *)(param_2 + 0x348);
    uVar1 = *(undefined4 *)(this + 0x20);
    uVar3 = 2;
  }
  else {
    if (param_1[0x5c] == (Mesh)0x0) {
      uVar1 = *(undefined4 *)(this + 0x20);
      uVar2 = *(undefined4 *)(param_1 + 4);
    }
    else {
      glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
      uVar2 = 0;
      uVar1 = *(undefined4 *)(this + 0x20);
    }
    uVar3 = 3;
  }
  glVertexAttribPointer(uVar1,uVar3,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  000a06b6: push {r4,r5,r6,r7,lr}
  000a06b8: add r7,sp,#0xc
  000a06ba: push.w r8
  000a06be: sub sp,#0x8
  000a06c0: mov r4,r0
  000a06c2: ldr r0,[r0,#0x24]
  000a06c4: add.w r3,r2,#0x104
  000a06c8: mov r6,r2
  000a06ca: mov r5,r1
  000a06cc: movs r1,#0x1
  000a06ce: movs r2,#0x0
  000a06d0: mov.w r8,#0x0
  000a06d4: blx 0x00070990
  000a06d8: ldrb r0,[r4,#0x9]
  000a06da: cbz r0,0x000a06ec
  000a06dc: ldr r0,[r4,#0x28]
  000a06de: add.w r2,r6,#0xd0
  000a06e2: movs r1,#0x1
  000a06e4: blx 0x00070984
  000a06e8: strb.w r8,[r4,#0x9]
  000a06ec: ldr r0,[r4,#0x20]
  000a06ee: blx 0x00070a5c
  000a06f2: cbz r5,0x000a070c
  000a06f4: ldrb.w r0,[r5,#0x5c]
  000a06f8: cbz r0,0x000a071c
  000a06fa: ldr r1,[r5,#0x60]
  000a06fc: movw r0,#0x8892
  000a0700: blx 0x0006ed04
  000a0704: movs r1,#0x0
  000a0706: ldr r0,[r4,#0x20]
  000a0708: str r1,[sp,#0x0]
  000a070a: b 0x000a0724
  000a070c: ldr.w r1,[r6,#0x348]
  000a0710: movs r2,#0x0
  000a0712: ldr r0,[r4,#0x20]
  000a0714: strd r2,r1,[sp,#0x0]
  000a0718: movs r1,#0x2
  000a071a: b 0x000a0728
  000a071c: ldr r0,[r4,#0x20]
  000a071e: movs r2,#0x0
  000a0720: ldr r1,[r5,#0x4]
  000a0722: str r2,[sp,#0x0]
  000a0724: str r1,[sp,#0x4]
  000a0726: movs r1,#0x3
  000a0728: movw r2,#0x1406
  000a072c: movs r3,#0x0
  000a072e: blx 0x000709c0
  000a0732: add sp,#0x8
  000a0734: pop.w r8
  000a0738: pop {r4,r5,r6,r7,pc}
```
