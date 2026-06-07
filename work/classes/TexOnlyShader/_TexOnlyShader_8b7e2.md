# TexOnlyShader::~TexOnlyShader
elf 0x8b7e2 body 16
Sig: undefined __thiscall ~TexOnlyShader(TexOnlyShader * this)

## decompile
```c

/* AbyssEngine::TexOnlyShader::~TexOnlyShader() */

void __thiscall AbyssEngine::TexOnlyShader::~TexOnlyShader(TexOnlyShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009b7e2: push {r7,lr}
  0009b7e4: mov r7,sp
  0009b7e6: blx 0x0007093c
  0009b7ea: pop.w {r7,lr}
  0009b7ee: b.w 0x001ab098
```
