# BumpShaderV4::SetInActive
elf 0x8b3e0 body 62
Sig: undefined __thiscall SetInActive(BumpShaderV4 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV4::SetInActive() */

void __thiscall AbyssEngine::BumpShaderV4::SetInActive(BumpShaderV4 *this)

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
  0009b3e0: push {r4,r6,r7,lr}
  0009b3e2: add r7,sp,#0x8
  0009b3e4: mov r4,r0
  0009b3e6: ldr r0,[r0,#0x20]
  0009b3e8: cmp r0,#0x0
  0009b3ea: blt 0x0009b3f0
  0009b3ec: blx 0x00070a44
  0009b3f0: ldr r0,[r4,#0x24]
  0009b3f2: cmp r0,#0x0
  0009b3f4: blt 0x0009b3fa
  0009b3f6: blx 0x00070a44
  0009b3fa: ldr r0,[r4,#0x28]
  0009b3fc: cmp r0,#0x0
  0009b3fe: blt 0x0009b404
  0009b400: blx 0x00070a44
  0009b404: ldr r0,[r4,#0x2c]
  0009b406: cmp r0,#0x0
  0009b408: blt 0x0009b40e
  0009b40a: blx 0x00070a44
  0009b40e: ldr r0,[r4,#0x30]
  0009b410: cmp r0,#0x0
  0009b412: blt 0x0009b41c
  0009b414: pop.w {r4,r6,r7,lr}
  0009b418: b.w 0x001ab778
  0009b41c: pop {r4,r6,r7,pc}
```
