# NoTexVtxColorShader::SetInActive
elf 0x8e024 body 22
Sig: undefined __thiscall SetInActive(NoTexVtxColorShader * this)

## decompile
```c

/* AbyssEngine::NoTexVtxColorShader::SetInActive() */

void __thiscall AbyssEngine::NoTexVtxColorShader::SetInActive(NoTexVtxColorShader *this)

{
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x20));
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x24));
  return;
}

```

## target disasm
```
  0009e024: push {r4,r6,r7,lr}
  0009e026: add r7,sp,#0x8
  0009e028: mov r4,r0
  0009e02a: ldr r0,[r0,#0x20]
  0009e02c: blx 0x00070a44
  0009e030: ldr r0,[r4,#0x24]
  0009e032: pop.w {r4,r6,r7,lr}
  0009e036: b.w 0x001ab778
```
