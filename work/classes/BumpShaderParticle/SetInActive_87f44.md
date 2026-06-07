# BumpShaderParticle::SetInActive
elf 0x87f44 body 72
Sig: undefined __thiscall SetInActive(BumpShaderParticle * this)

## decompile
```c

/* AbyssEngine::BumpShaderParticle::SetInActive() */

void __thiscall AbyssEngine::BumpShaderParticle::SetInActive(BumpShaderParticle *this)

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
  if (-1 < *(int *)(this + 0x30)) {
    glDisableVertexAttribArray();
  }
  if (*(int *)(this + 0x34) < 0) {
    return;
  }
  (*(code *)(DAT_001ab784 + 0x1ab788))();
  return;
}

```

## target disasm
```
  00097f44: push {r4,r6,r7,lr}
  00097f46: add r7,sp,#0x8
  00097f48: mov r4,r0
  00097f4a: ldr r0,[r0,#0x20]
  00097f4c: cmp r0,#0x0
  00097f4e: blt 0x00097f54
  00097f50: blx 0x00070a44
  00097f54: ldr r0,[r4,#0x24]
  00097f56: cmp r0,#0x0
  00097f58: blt 0x00097f5e
  00097f5a: blx 0x00070a44
  00097f5e: ldr r0,[r4,#0x28]
  00097f60: cmp r0,#0x0
  00097f62: blt 0x00097f68
  00097f64: blx 0x00070a44
  00097f68: ldr r0,[r4,#0x2c]
  00097f6a: cmp r0,#0x0
  00097f6c: blt 0x00097f72
  00097f6e: blx 0x00070a44
  00097f72: ldr r0,[r4,#0x30]
  00097f74: cmp r0,#0x0
  00097f76: blt 0x00097f7c
  00097f78: blx 0x00070a44
  00097f7c: ldr r0,[r4,#0x34]
  00097f7e: cmp r0,#0x0
  00097f80: blt 0x00097f8a
  00097f82: pop.w {r4,r6,r7,lr}
  00097f86: b.w 0x001ab778
  00097f8a: pop {r4,r6,r7,pc}
```
