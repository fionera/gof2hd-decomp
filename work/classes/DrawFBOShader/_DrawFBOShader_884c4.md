# DrawFBOShader::~DrawFBOShader
elf 0x884c4 body 16
Sig: undefined __thiscall ~DrawFBOShader(DrawFBOShader * this)

## decompile
```c

/* AbyssEngine::DrawFBOShader::~DrawFBOShader() */

void __thiscall AbyssEngine::DrawFBOShader::~DrawFBOShader(DrawFBOShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000984c4: push {r7,lr}
  000984c6: mov r7,sp
  000984c8: blx 0x0007093c
  000984cc: pop.w {r7,lr}
  000984d0: b.w 0x001ab098
```
