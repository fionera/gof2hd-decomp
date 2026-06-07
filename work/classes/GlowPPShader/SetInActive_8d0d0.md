# GlowPPShader::SetInActive
elf 0x8d0d0 body 26
Sig: undefined __thiscall SetInActive(GlowPPShader * this)

## decompile
```c

/* AbyssEngine::GlowPPShader::SetInActive() */

void __thiscall AbyssEngine::GlowPPShader::SetInActive(GlowPPShader *this)

{
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x90));
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x98));
  return;
}

```

## target disasm
```
  0009d0d0: push {r4,r6,r7,lr}
  0009d0d2: add r7,sp,#0x8
  0009d0d4: mov r4,r0
  0009d0d6: ldr.w r0,[r0,#0x90]
  0009d0da: blx 0x00070a44
  0009d0de: ldr.w r0,[r4,#0x98]
  0009d0e2: pop.w {r4,r6,r7,lr}
  0009d0e6: b.w 0x001ab778
```
