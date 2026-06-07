# SpecCubeAlphaMapping::~SpecCubeAlphaMapping
elf 0x87d1c body 16
Sig: undefined __thiscall ~SpecCubeAlphaMapping(SpecCubeAlphaMapping * this)

## decompile
```c

/* AbyssEngine::SpecCubeAlphaMapping::~SpecCubeAlphaMapping() */

void __thiscall AbyssEngine::SpecCubeAlphaMapping::~SpecCubeAlphaMapping(SpecCubeAlphaMapping *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00097d1c: push {r7,lr}
  00097d1e: mov r7,sp
  00097d20: blx 0x0007093c
  00097d24: pop.w {r7,lr}
  00097d28: b.w 0x001ab098
```
