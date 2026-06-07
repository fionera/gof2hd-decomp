# GenericShader::~GenericShader
elf 0x8eb90 body 16
Sig: undefined __thiscall ~GenericShader(GenericShader * this)

## decompile
```c

/* AbyssEngine::GenericShader::~GenericShader() */

void __thiscall AbyssEngine::GenericShader::~GenericShader(GenericShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009eb90: push {r7,lr}
  0009eb92: mov r7,sp
  0009eb94: blx 0x0007093c
  0009eb98: pop.w {r7,lr}
  0009eb9c: b.w 0x001ab098
```
