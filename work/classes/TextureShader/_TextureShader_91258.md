# TextureShader::~TextureShader
elf 0x91258 body 16
Sig: undefined __thiscall ~TextureShader(TextureShader * this)

## decompile
```c

/* AbyssEngine::TextureShader::~TextureShader() */

void __thiscall AbyssEngine::TextureShader::~TextureShader(TextureShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a1258: push {r7,lr}
  000a125a: mov r7,sp
  000a125c: blx 0x0007093c
  000a1260: pop.w {r7,lr}
  000a1264: b.w 0x001ab098
```
