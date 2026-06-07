# GreenShader::SetInActive
elf 0x883c0 body 62
Sig: undefined __thiscall SetInActive(GreenShader * this)

## decompile
```c

/* AbyssEngine::GreenShader::SetInActive() */

void __thiscall AbyssEngine::GreenShader::SetInActive(GreenShader *this)

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
  000983c0: push {r4,r6,r7,lr}
  000983c2: add r7,sp,#0x8
  000983c4: mov r4,r0
  000983c6: ldr r0,[r0,#0x20]
  000983c8: cmp r0,#0x0
  000983ca: blt 0x000983d0
  000983cc: blx 0x00070a44
  000983d0: ldr r0,[r4,#0x24]
  000983d2: cmp r0,#0x0
  000983d4: blt 0x000983da
  000983d6: blx 0x00070a44
  000983da: ldr r0,[r4,#0x28]
  000983dc: cmp r0,#0x0
  000983de: blt 0x000983e4
  000983e0: blx 0x00070a44
  000983e4: ldr r0,[r4,#0x2c]
  000983e6: cmp r0,#0x0
  000983e8: blt 0x000983ee
  000983ea: blx 0x00070a44
  000983ee: ldr r0,[r4,#0x30]
  000983f0: cmp r0,#0x0
  000983f2: blt 0x000983fc
  000983f4: pop.w {r4,r6,r7,lr}
  000983f8: b.w 0x001ab778
  000983fc: pop {r4,r6,r7,pc}
```
