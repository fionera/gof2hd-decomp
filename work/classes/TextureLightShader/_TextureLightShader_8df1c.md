# TextureLightShader::~TextureLightShader
elf 0x8df1c body 16
Sig: undefined __thiscall ~TextureLightShader(TextureLightShader * this)

## decompile
```c

/* AbyssEngine::TextureLightShader::~TextureLightShader() */

void __thiscall AbyssEngine::TextureLightShader::~TextureLightShader(TextureLightShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009df1c: push {r7,lr}
  0009df1e: mov r7,sp
  0009df20: blx 0x0007093c
  0009df24: pop.w {r7,lr}
  0009df28: b.w 0x001ab098
```
