# PostBWShader::~PostBWShader
elf 0x8ef80 body 24
Sig: undefined __thiscall ~PostBWShader(PostBWShader * this)

## decompile
```c

/* AbyssEngine::PostBWShader::~PostBWShader() */

void __thiscall AbyssEngine::PostBWShader::~PostBWShader(PostBWShader *this)

{
  *(int *)this = *(int *)(DAT_0009ef90 + 0x9ef86) + 8;
  (*(code *)(DAT_001ab794 + 0x1ab798))();
  return;
}

```

## target disasm
```
  0009ef80: ldr r1,[0x0009ef90]
  0009ef82: add r1,pc
  0009ef84: ldr r1,[r1,#0x0]
  0009ef86: adds r1,#0x8
  0009ef88: str r1,[r0,#0x0]
  0009ef8a: b.w 0x001ab788
  001ab788: bx pc
  001ab78c: ldr r12,[0x1ab794]
  001ab790: add pc,r12,pc
```
