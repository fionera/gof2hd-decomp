# SpecCubeMapping::~SpecCubeMapping
elf 0x92b60 body 16
Sig: undefined __thiscall ~SpecCubeMapping(SpecCubeMapping * this)

## decompile
```c

/* AbyssEngine::SpecCubeMapping::~SpecCubeMapping() */

void __thiscall AbyssEngine::SpecCubeMapping::~SpecCubeMapping(SpecCubeMapping *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  000a2b60: push {r7,lr}
  000a2b62: mov r7,sp
  000a2b64: blx 0x0007093c
  000a2b68: pop.w {r7,lr}
  000a2b6c: b.w 0x001ab098
```
