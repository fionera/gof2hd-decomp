# SimpleRefractionShader::SetInActive
elf 0x8c8a0 body 62
Sig: undefined __thiscall SetInActive(SimpleRefractionShader * this)

## decompile
```c

/* AbyssEngine::SimpleRefractionShader::SetInActive() */

void __thiscall AbyssEngine::SimpleRefractionShader::SetInActive(SimpleRefractionShader *this)

{
  if (-1 < *(int *)(this + 0x20)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x28)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x2c)) {
    glDisableVertexAttribArray();
  }
  if (*(int *)(this + 0x30) < 0) {
    return;
  }
  (*(code *)(DAT_001ab784 + 0x1ab788))();
  return;
}

```

## target disasm
```
  0009c8a0: push {r4,r6,r7,lr}
  0009c8a2: add r7,sp,#0x8
  0009c8a4: mov r4,r0
  0009c8a6: ldr r0,[r0,#0x20]
  0009c8a8: cmp r0,#0x0
  0009c8aa: blt 0x0009c8b0
  0009c8ac: blx 0x00070a44
  0009c8b0: ldr r0,[r4,#0x24]
  0009c8b2: cmp r0,#0x0
  0009c8b4: blt 0x0009c8ba
  0009c8b6: blx 0x00070a44
  0009c8ba: ldr r0,[r4,#0x28]
  0009c8bc: cmp r0,#0x0
  0009c8be: blt 0x0009c8c4
  0009c8c0: blx 0x00070a44
  0009c8c4: ldr r0,[r4,#0x2c]
  0009c8c6: cmp r0,#0x0
  0009c8c8: blt 0x0009c8ce
  0009c8ca: blx 0x00070a44
  0009c8ce: ldr r0,[r4,#0x30]
  0009c8d0: cmp r0,#0x0
  0009c8d2: blt 0x0009c8dc
  0009c8d4: pop.w {r4,r6,r7,lr}
  0009c8d8: b.w 0x001ab778
  0009c8dc: pop {r4,r6,r7,pc}
```
