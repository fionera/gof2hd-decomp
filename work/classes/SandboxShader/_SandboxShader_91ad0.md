# SandboxShader::~SandboxShader
elf 0x91ad0 body 16
Sig: undefined __thiscall ~SandboxShader(SandboxShader * this)

## decompile
```c

/* AbyssEngine::SandboxShader::~SandboxShader() */

void __thiscall AbyssEngine::SandboxShader::~SandboxShader(SandboxShader *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a1ad0: push {r7,lr}
  000a1ad2: mov r7,sp
  000a1ad4: blx 0x0007093c
  000a1ad8: pop.w {r7,lr}
  000a1adc: b.w 0x001ab098
```
