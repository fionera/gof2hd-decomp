# TextureAlphaTestShader::~TextureAlphaTestShader
elf 0x893cc body 16
Sig: undefined __thiscall ~TextureAlphaTestShader(TextureAlphaTestShader * this)

## decompile
```c

/* AbyssEngine::TextureAlphaTestShader::~TextureAlphaTestShader() */

void __thiscall
AbyssEngine::TextureAlphaTestShader::~TextureAlphaTestShader(TextureAlphaTestShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000993cc: push {r7,lr}
  000993ce: mov r7,sp
  000993d0: blx 0x0007093c
  000993d4: pop.w {r7,lr}
  000993d8: b.w 0x001ab098
```
