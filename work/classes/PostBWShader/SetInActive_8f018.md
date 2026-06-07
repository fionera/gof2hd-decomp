# PostBWShader::SetInActive
elf 0x8f018 body 22
Sig: undefined __thiscall SetInActive(PostBWShader * this)

## decompile
```c

/* AbyssEngine::PostBWShader::SetInActive() */

void __thiscall AbyssEngine::PostBWShader::SetInActive(PostBWShader *this)

{
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x20));
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x28));
  return;
}

```

## target disasm
```
  0009f018: push {r4,r6,r7,lr}
  0009f01a: add r7,sp,#0x8
  0009f01c: mov r4,r0
  0009f01e: ldr r0,[r0,#0x20]
  0009f020: blx 0x00070a44
  0009f024: ldr r0,[r4,#0x28]
  0009f026: pop.w {r4,r6,r7,lr}
  0009f02a: b.w 0x001ab778
```
