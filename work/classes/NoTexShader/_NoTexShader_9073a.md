# NoTexShader::~NoTexShader
elf 0x9073a body 16
Sig: undefined __thiscall ~NoTexShader(NoTexShader * this)

## decompile
```c

/* AbyssEngine::NoTexShader::~NoTexShader() */

void __thiscall AbyssEngine::NoTexShader::~NoTexShader(NoTexShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a073a: push {r7,lr}
  000a073c: mov r7,sp
  000a073e: blx 0x0007093c
  000a0742: pop.w {r7,lr}
  000a0746: b.w 0x001ab098
```
