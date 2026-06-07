# GenericShader1::SetInActive
elf 0x8ed4c body 62
Sig: undefined __thiscall SetInActive(GenericShader1 * this)

## decompile
```c

/* AbyssEngine::GenericShader1::SetInActive() */

void __thiscall AbyssEngine::GenericShader1::SetInActive(GenericShader1 *this)

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
  0009ed4c: push {r4,r6,r7,lr}
  0009ed4e: add r7,sp,#0x8
  0009ed50: mov r4,r0
  0009ed52: ldr r0,[r0,#0x20]
  0009ed54: cmp r0,#0x0
  0009ed56: blt 0x0009ed5c
  0009ed58: blx 0x00070a44
  0009ed5c: ldr r0,[r4,#0x24]
  0009ed5e: cmp r0,#0x0
  0009ed60: blt 0x0009ed66
  0009ed62: blx 0x00070a44
  0009ed66: ldr r0,[r4,#0x28]
  0009ed68: cmp r0,#0x0
  0009ed6a: blt 0x0009ed70
  0009ed6c: blx 0x00070a44
  0009ed70: ldr r0,[r4,#0x2c]
  0009ed72: cmp r0,#0x0
  0009ed74: blt 0x0009ed7a
  0009ed76: blx 0x00070a44
  0009ed7a: ldr r0,[r4,#0x30]
  0009ed7c: cmp r0,#0x0
  0009ed7e: blt 0x0009ed88
  0009ed80: pop.w {r4,r6,r7,lr}
  0009ed84: b.w 0x001ab778
  0009ed88: pop {r4,r6,r7,pc}
```
