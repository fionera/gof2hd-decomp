# DNSShader::~DNSShader
elf 0x8f750 body 16
Sig: undefined __thiscall ~DNSShader(DNSShader * this)

## decompile
```c

/* AbyssEngine::DNSShader::~DNSShader() */

void __thiscall AbyssEngine::DNSShader::~DNSShader(DNSShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009f750: push {r7,lr}
  0009f752: mov r7,sp
  0009f754: blx 0x0007093c
  0009f758: pop.w {r7,lr}
  0009f75c: b.w 0x001ab098
```
