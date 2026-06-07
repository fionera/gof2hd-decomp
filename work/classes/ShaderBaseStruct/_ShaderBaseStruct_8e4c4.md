# ShaderBaseStruct::~ShaderBaseStruct
elf 0x8e4c4 body 30
Sig: undefined __thiscall ~ShaderBaseStruct(ShaderBaseStruct * this)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::~ShaderBaseStruct() */

ShaderBaseStruct * __thiscall
AbyssEngine::ShaderBaseStruct::~ShaderBaseStruct(ShaderBaseStruct *this)

{
  *(int *)this = *(int *)(DAT_0009e4e4 + 0x9e4d0) + 8;
  String::~String((String *)(this + 0xc));
  return this;
}

```

## target disasm
```
  0009e4c4: push {r4,r6,r7,lr}
  0009e4c6: add r7,sp,#0x8
  0009e4c8: mov r4,r0
  0009e4ca: ldr r0,[0x0009e4e4]
  0009e4cc: add r0,pc
  0009e4ce: ldr r0,[r0,#0x0]
  0009e4d0: add.w r1,r0,#0x8
  0009e4d4: mov r0,r4
  0009e4d6: str.w r1,[r0],#0xc
  0009e4da: blx 0x0006ee30
  0009e4de: mov r0,r4
  0009e4e0: pop {r4,r6,r7,pc}
```
