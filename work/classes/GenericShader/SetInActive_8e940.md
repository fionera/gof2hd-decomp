# GenericShader::SetInActive
elf 0x8e940 body 62
Sig: undefined __thiscall SetInActive(GenericShader * this)

## decompile
```c

/* AbyssEngine::GenericShader::SetInActive() */

void __thiscall AbyssEngine::GenericShader::SetInActive(GenericShader *this)

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
  0009e940: push {r4,r6,r7,lr}
  0009e942: add r7,sp,#0x8
  0009e944: mov r4,r0
  0009e946: ldr r0,[r0,#0x20]
  0009e948: cmp r0,#0x0
  0009e94a: blt 0x0009e950
  0009e94c: blx 0x00070a44
  0009e950: ldr r0,[r4,#0x24]
  0009e952: cmp r0,#0x0
  0009e954: blt 0x0009e95a
  0009e956: blx 0x00070a44
  0009e95a: ldr r0,[r4,#0x28]
  0009e95c: cmp r0,#0x0
  0009e95e: blt 0x0009e964
  0009e960: blx 0x00070a44
  0009e964: ldr r0,[r4,#0x2c]
  0009e966: cmp r0,#0x0
  0009e968: blt 0x0009e96e
  0009e96a: blx 0x00070a44
  0009e96e: ldr r0,[r4,#0x30]
  0009e970: cmp r0,#0x0
  0009e972: blt 0x0009e97c
  0009e974: pop.w {r4,r6,r7,lr}
  0009e978: b.w 0x001ab778
  0009e97c: pop {r4,r6,r7,pc}
```
