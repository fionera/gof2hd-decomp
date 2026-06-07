# BumpShader::SetInActive
elf 0x8ba5c body 42
Sig: undefined __thiscall SetInActive(BumpShader * this)

## decompile
```c

/* AbyssEngine::BumpShader::SetInActive() */

void __thiscall AbyssEngine::BumpShader::SetInActive(BumpShader *this)

{
  if (-1 < *(int *)(this + 0x20)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glDisableVertexAttribArray();
  }
  if (*(int *)(this + 0x28) < 0) {
    return;
  }
  (*(code *)(DAT_001ab784 + 0x1ab788))();
  return;
}

```

## target disasm
```
  0009ba5c: push {r4,r6,r7,lr}
  0009ba5e: add r7,sp,#0x8
  0009ba60: mov r4,r0
  0009ba62: ldr r0,[r0,#0x20]
  0009ba64: cmp r0,#0x0
  0009ba66: blt 0x0009ba6c
  0009ba68: blx 0x00070a44
  0009ba6c: ldr r0,[r4,#0x24]
  0009ba6e: cmp r0,#0x0
  0009ba70: blt 0x0009ba76
  0009ba72: blx 0x00070a44
  0009ba76: ldr r0,[r4,#0x28]
  0009ba78: cmp r0,#0x0
  0009ba7a: blt 0x0009ba84
  0009ba7c: pop.w {r4,r6,r7,lr}
  0009ba80: b.w 0x001ab778
  0009ba84: pop {r4,r6,r7,pc}
```
