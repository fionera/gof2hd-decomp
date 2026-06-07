# BumpShaderV3::SetInActive
elf 0x8c38c body 62
Sig: undefined __thiscall SetInActive(BumpShaderV3 * this)

## decompile
```c

/* AbyssEngine::BumpShaderV3::SetInActive() */

void __thiscall AbyssEngine::BumpShaderV3::SetInActive(BumpShaderV3 *this)

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
  0009c38c: push {r4,r6,r7,lr}
  0009c38e: add r7,sp,#0x8
  0009c390: mov r4,r0
  0009c392: ldr r0,[r0,#0x20]
  0009c394: cmp r0,#0x0
  0009c396: blt 0x0009c39c
  0009c398: blx 0x00070a44
  0009c39c: ldr r0,[r4,#0x24]
  0009c39e: cmp r0,#0x0
  0009c3a0: blt 0x0009c3a6
  0009c3a2: blx 0x00070a44
  0009c3a6: ldr r0,[r4,#0x28]
  0009c3a8: cmp r0,#0x0
  0009c3aa: blt 0x0009c3b0
  0009c3ac: blx 0x00070a44
  0009c3b0: ldr r0,[r4,#0x2c]
  0009c3b2: cmp r0,#0x0
  0009c3b4: blt 0x0009c3ba
  0009c3b6: blx 0x00070a44
  0009c3ba: ldr r0,[r4,#0x30]
  0009c3bc: cmp r0,#0x0
  0009c3be: blt 0x0009c3c8
  0009c3c0: pop.w {r4,r6,r7,lr}
  0009c3c4: b.w 0x001ab778
  0009c3c8: pop {r4,r6,r7,pc}
```
