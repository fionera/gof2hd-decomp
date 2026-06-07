# BumpRimCubeShader_new::SetInActive
elf 0x88b18 body 62
Sig: undefined __thiscall SetInActive(BumpRimCubeShader_new * this)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader_new::SetInActive() */

void __thiscall AbyssEngine::BumpRimCubeShader_new::SetInActive(BumpRimCubeShader_new *this)

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
  00098b18: push {r4,r6,r7,lr}
  00098b1a: add r7,sp,#0x8
  00098b1c: mov r4,r0
  00098b1e: ldr r0,[r0,#0x20]
  00098b20: cmp r0,#0x0
  00098b22: blt 0x00098b28
  00098b24: blx 0x00070a44
  00098b28: ldr r0,[r4,#0x24]
  00098b2a: cmp r0,#0x0
  00098b2c: blt 0x00098b32
  00098b2e: blx 0x00070a44
  00098b32: ldr r0,[r4,#0x28]
  00098b34: cmp r0,#0x0
  00098b36: blt 0x00098b3c
  00098b38: blx 0x00070a44
  00098b3c: ldr r0,[r4,#0x2c]
  00098b3e: cmp r0,#0x0
  00098b40: blt 0x00098b46
  00098b42: blx 0x00070a44
  00098b46: ldr r0,[r4,#0x30]
  00098b48: cmp r0,#0x0
  00098b4a: blt 0x00098b54
  00098b4c: pop.w {r4,r6,r7,lr}
  00098b50: b.w 0x001ab778
  00098b54: pop {r4,r6,r7,pc}
```
