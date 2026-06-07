# GenericShader1::~GenericShader1
elf 0x8eee2 body 16
Sig: undefined __thiscall ~GenericShader1(GenericShader1 * this)

## decompile
```c

/* AbyssEngine::GenericShader1::~GenericShader1() */

void __thiscall AbyssEngine::GenericShader1::~GenericShader1(GenericShader1 *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009eee2: push {r7,lr}
  0009eee4: mov r7,sp
  0009eee6: blx 0x0007093c
  0009eeea: pop.w {r7,lr}
  0009eeee: b.w 0x001ab098
```
