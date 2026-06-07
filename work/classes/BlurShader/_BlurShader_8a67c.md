# BlurShader::~BlurShader
elf 0x8a67c body 16
Sig: undefined __thiscall ~BlurShader(BlurShader * this)

## decompile
```c

/* AbyssEngine::BlurShader::~BlurShader() */

void __thiscall AbyssEngine::BlurShader::~BlurShader(BlurShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009a67c: push {r7,lr}
  0009a67e: mov r7,sp
  0009a680: blx 0x0007093c
  0009a684: pop.w {r7,lr}
  0009a688: b.w 0x001ab098
```
