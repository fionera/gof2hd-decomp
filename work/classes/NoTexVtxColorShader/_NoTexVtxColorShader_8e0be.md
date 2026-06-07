# NoTexVtxColorShader::~NoTexVtxColorShader
elf 0x8e0be body 16
Sig: undefined __thiscall ~NoTexVtxColorShader(NoTexVtxColorShader * this)

## decompile
```c

/* AbyssEngine::NoTexVtxColorShader::~NoTexVtxColorShader() */

void __thiscall AbyssEngine::NoTexVtxColorShader::~NoTexVtxColorShader(NoTexVtxColorShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009e0be: push {r7,lr}
  0009e0c0: mov r7,sp
  0009e0c2: blx 0x0007093c
  0009e0c6: pop.w {r7,lr}
  0009e0ca: b.w 0x001ab098
```
