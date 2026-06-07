# ShaderBaseStruct::Update
elf 0x8e74a body 6
Sig: undefined __thiscall Update(ShaderBaseStruct * this)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::Update() */

void __thiscall AbyssEngine::ShaderBaseStruct::Update(ShaderBaseStruct *this)

{
  this[9] = (ShaderBaseStruct)0x1;
  return;
}

```

## target disasm
```
  0009e74a: movs r1,#0x1
  0009e74c: strb r1,[r0,#0x9]
  0009e74e: bx lr
```
