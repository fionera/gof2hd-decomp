# GlowShader::SetInActive
elf 0x92050 body 32
Sig: undefined __thiscall SetInActive(GlowShader * this)

## decompile
```c

/* AbyssEngine::GlowShader::SetInActive() */

void __thiscall AbyssEngine::GlowShader::SetInActive(GlowShader *this)

{
  if (-1 < *(int *)(this + 0x20)) {
    glDisableVertexAttribArray();
  }
  if (*(int *)(this + 0x24) < 0) {
    return;
  }
  (*(code *)(DAT_001ab784 + 0x1ab788))();
  return;
}

```

## target disasm
```
  000a2050: push {r4,r6,r7,lr}
  000a2052: add r7,sp,#0x8
  000a2054: mov r4,r0
  000a2056: ldr r0,[r0,#0x20]
  000a2058: cmp r0,#0x0
  000a205a: blt 0x000a2060
  000a205c: blx 0x00070a44
  000a2060: ldr r0,[r4,#0x24]
  000a2062: cmp r0,#0x0
  000a2064: blt 0x000a206e
  000a2066: pop.w {r4,r6,r7,lr}
  000a206a: b.w 0x001ab778
  000a206e: pop {r4,r6,r7,pc}
```
