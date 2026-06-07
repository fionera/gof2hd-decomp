# VertexColorAlphaTextureShader::SetInActive
elf 0x91c9c body 72
Sig: undefined __thiscall SetInActive(VertexColorAlphaTextureShader * this)

## decompile
```c

/* AbyssEngine::VertexColorAlphaTextureShader::SetInActive() */

void __thiscall
AbyssEngine::VertexColorAlphaTextureShader::SetInActive(VertexColorAlphaTextureShader *this)

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
  000a1c9c: push {r4,r6,r7,lr}
  000a1c9e: add r7,sp,#0x8
  000a1ca0: mov r4,r0
  000a1ca2: ldr r0,[r0,#0x20]
  000a1ca4: cmp r0,#0x0
  000a1ca6: blt 0x000a1cac
  000a1ca8: blx 0x00070a44
  000a1cac: ldr r0,[r4,#0x24]
  000a1cae: cmp r0,#0x0
  000a1cb0: blt 0x000a1cb6
  000a1cb2: blx 0x00070a44
  000a1cb6: ldr r0,[r4,#0x28]
  000a1cb8: cmp r0,#0x0
  000a1cba: blt 0x000a1cc0
  000a1cbc: blx 0x00070a44
  000a1cc0: ldr r0,[r4,#0x2c]
  000a1cc2: cmp r0,#0x0
  000a1cc4: blt 0x000a1cca
  000a1cc6: blx 0x00070a44
  000a1cca: ldr r0,[r4,#0x30]
  000a1ccc: cmp r0,#0x0
  000a1cce: blt 0x000a1cd4
  000a1cd0: blx 0x00070a44
  000a1cd4: ldr r0,[r4,#0x34]
  000a1cd6: cmp r0,#0x0
  000a1cd8: blt 0x000a1ce2
  000a1cda: pop.w {r4,r6,r7,lr}
  000a1cde: b.w 0x001ab778
  000a1ce2: pop {r4,r6,r7,pc}
```
