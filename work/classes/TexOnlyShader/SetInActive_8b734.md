# TexOnlyShader::SetInActive
elf 0x8b734 body 22
Sig: undefined __thiscall SetInActive(TexOnlyShader * this)

## decompile
```c

/* AbyssEngine::TexOnlyShader::SetInActive() */

void __thiscall AbyssEngine::TexOnlyShader::SetInActive(TexOnlyShader *this)

{
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x20));
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x24));
  return;
}

```

## target disasm
```
  0009b734: push {r4,r6,r7,lr}
  0009b736: add r7,sp,#0x8
  0009b738: mov r4,r0
  0009b73a: ldr r0,[r0,#0x20]
  0009b73c: blx 0x00070a44
  0009b740: ldr r0,[r4,#0x24]
  0009b742: pop.w {r4,r6,r7,lr}
  0009b746: b.w 0x001ab778
```
