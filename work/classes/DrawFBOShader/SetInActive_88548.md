# DrawFBOShader::SetInActive
elf 0x88548 body 22
Sig: undefined __thiscall SetInActive(DrawFBOShader * this)

## decompile
```c

/* AbyssEngine::DrawFBOShader::SetInActive() */

void __thiscall AbyssEngine::DrawFBOShader::SetInActive(DrawFBOShader *this)

{
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x48));
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x50));
  return;
}

```

## target disasm
```
  00098548: push {r4,r6,r7,lr}
  0009854a: add r7,sp,#0x8
  0009854c: mov r4,r0
  0009854e: ldr r0,[r0,#0x48]
  00098550: blx 0x00070a44
  00098554: ldr r0,[r4,#0x50]
  00098556: pop.w {r4,r6,r7,lr}
  0009855a: b.w 0x001ab778
```
