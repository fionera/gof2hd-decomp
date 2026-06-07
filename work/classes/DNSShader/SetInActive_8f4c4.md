# DNSShader::SetInActive
elf 0x8f4c4 body 62
Sig: undefined __thiscall SetInActive(DNSShader * this)

## decompile
```c

/* AbyssEngine::DNSShader::SetInActive() */

void __thiscall AbyssEngine::DNSShader::SetInActive(DNSShader *this)

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
  0009f4c4: push {r4,r6,r7,lr}
  0009f4c6: add r7,sp,#0x8
  0009f4c8: mov r4,r0
  0009f4ca: ldr r0,[r0,#0x20]
  0009f4cc: cmp r0,#0x0
  0009f4ce: blt 0x0009f4d4
  0009f4d0: blx 0x00070a44
  0009f4d4: ldr r0,[r4,#0x24]
  0009f4d6: cmp r0,#0x0
  0009f4d8: blt 0x0009f4de
  0009f4da: blx 0x00070a44
  0009f4de: ldr r0,[r4,#0x28]
  0009f4e0: cmp r0,#0x0
  0009f4e2: blt 0x0009f4e8
  0009f4e4: blx 0x00070a44
  0009f4e8: ldr r0,[r4,#0x2c]
  0009f4ea: cmp r0,#0x0
  0009f4ec: blt 0x0009f4f2
  0009f4ee: blx 0x00070a44
  0009f4f2: ldr r0,[r4,#0x30]
  0009f4f4: cmp r0,#0x0
  0009f4f6: blt 0x0009f500
  0009f4f8: pop.w {r4,r6,r7,lr}
  0009f4fc: b.w 0x001ab778
  0009f500: pop {r4,r6,r7,pc}
```
