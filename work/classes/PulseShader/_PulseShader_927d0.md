# PulseShader::~PulseShader
elf 0x927d0 body 16
Sig: undefined __thiscall ~PulseShader(PulseShader * this)

## decompile
```c

/* AbyssEngine::PulseShader::~PulseShader() */

void __thiscall AbyssEngine::PulseShader::~PulseShader(PulseShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a27d0: push {r7,lr}
  000a27d2: mov r7,sp
  000a27d4: blx 0x0007093c
  000a27d8: pop.w {r7,lr}
  000a27dc: b.w 0x001ab098
```
