# GenericShader2::SetInActive
elf 0x8e27c body 62
Sig: undefined __thiscall SetInActive(GenericShader2 * this)

## decompile
```c

/* AbyssEngine::GenericShader2::SetInActive() */

void __thiscall AbyssEngine::GenericShader2::SetInActive(GenericShader2 *this)

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
  0009e27c: push {r4,r6,r7,lr}
  0009e27e: add r7,sp,#0x8
  0009e280: mov r4,r0
  0009e282: ldr r0,[r0,#0x20]
  0009e284: cmp r0,#0x0
  0009e286: blt 0x0009e28c
  0009e288: blx 0x00070a44
  0009e28c: ldr r0,[r4,#0x24]
  0009e28e: cmp r0,#0x0
  0009e290: blt 0x0009e296
  0009e292: blx 0x00070a44
  0009e296: ldr r0,[r4,#0x28]
  0009e298: cmp r0,#0x0
  0009e29a: blt 0x0009e2a0
  0009e29c: blx 0x00070a44
  0009e2a0: ldr r0,[r4,#0x2c]
  0009e2a2: cmp r0,#0x0
  0009e2a4: blt 0x0009e2aa
  0009e2a6: blx 0x00070a44
  0009e2aa: ldr r0,[r4,#0x30]
  0009e2ac: cmp r0,#0x0
  0009e2ae: blt 0x0009e2b8
  0009e2b0: pop.w {r4,r6,r7,lr}
  0009e2b4: b.w 0x001ab778
  0009e2b8: pop {r4,r6,r7,pc}
```
