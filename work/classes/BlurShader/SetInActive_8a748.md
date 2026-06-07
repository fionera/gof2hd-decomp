# BlurShader::SetInActive
elf 0x8a748 body 22
Sig: undefined __thiscall SetInActive(BlurShader * this)

## decompile
```c

/* AbyssEngine::BlurShader::SetInActive() */

void __thiscall AbyssEngine::BlurShader::SetInActive(BlurShader *this)

{
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x48));
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x50));
  return;
}

```

## target disasm
```
  0009a748: push {r4,r6,r7,lr}
  0009a74a: add r7,sp,#0x8
  0009a74c: mov r4,r0
  0009a74e: ldr r0,[r0,#0x48]
  0009a750: blx 0x00070a44
  0009a754: ldr r0,[r4,#0x50]
  0009a756: pop.w {r4,r6,r7,lr}
  0009a75a: b.w 0x001ab778
```
