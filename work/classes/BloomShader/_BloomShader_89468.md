# BloomShader::~BloomShader
elf 0x89468 body 14
Sig: undefined __thiscall ~BloomShader(BloomShader * this)

## decompile
```c

/* AbyssEngine::BloomShader::~BloomShader() */

void __thiscall AbyssEngine::BloomShader::~BloomShader(BloomShader *this)

{
  *(int *)this = *(int *)(DAT_00099478 + 0x9946e) + 8;
  (*(code *)(DAT_001ab794 + 0x1ab798))();
  return;
}

```

## target disasm
```
  00099468: ldr r1,[0x00099478]
  0009946a: add r1,pc
  0009946c: ldr r1,[r1,#0x0]
  0009946e: adds r1,#0x8
  00099470: str r1,[r0,#0x0]
  00099472: b.w 0x001ab788
```
