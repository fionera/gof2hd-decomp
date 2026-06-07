# BumpShaderV2::SetInActive
elf 0x8d888 body 62
Sig: undefined __thiscall SetInActive(BumpShaderV2 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV2::SetInActive() */

void __thiscall AbyssEngine::BumpShaderV2::SetInActive(BumpShaderV2 *this)

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
  0009d888: push {r4,r6,r7,lr}
  0009d88a: add r7,sp,#0x8
  0009d88c: mov r4,r0
  0009d88e: ldr r0,[r0,#0x20]
  0009d890: cmp r0,#0x0
  0009d892: blt 0x0009d898
  0009d894: blx 0x00070a44
  0009d898: ldr r0,[r4,#0x24]
  0009d89a: cmp r0,#0x0
  0009d89c: blt 0x0009d8a2
  0009d89e: blx 0x00070a44
  0009d8a2: ldr r0,[r4,#0x28]
  0009d8a4: cmp r0,#0x0
  0009d8a6: blt 0x0009d8ac
  0009d8a8: blx 0x00070a44
  0009d8ac: ldr r0,[r4,#0x2c]
  0009d8ae: cmp r0,#0x0
  0009d8b0: blt 0x0009d8b6
  0009d8b2: blx 0x00070a44
  0009d8b6: ldr r0,[r4,#0x30]
  0009d8b8: cmp r0,#0x0
  0009d8ba: blt 0x0009d8c4
  0009d8bc: pop.w {r4,r6,r7,lr}
  0009d8c0: b.w 0x001ab778
  0009d8c4: pop {r4,r6,r7,pc}
```
