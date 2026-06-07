# PostBWShader::~PostBWShader
elf 0x8ef94 body 16
Sig: undefined __thiscall ~PostBWShader(PostBWShader * this)

## decompile
```c

/* AbyssEngine::PostBWShader::~PostBWShader() */

void __thiscall AbyssEngine::PostBWShader::~PostBWShader(PostBWShader *this)

{
  ~PostBWShader(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009ef94: push {r7,lr}
  0009ef96: mov r7,sp
  0009ef98: blx 0x000710c8
  0009ef9c: pop.w {r7,lr}
  0009efa0: b.w 0x001ab098
```
