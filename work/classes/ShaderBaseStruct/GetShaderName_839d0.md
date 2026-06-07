# ShaderBaseStruct::GetShaderName
elf 0x839d0 body 14
Sig: undefined __stdcall GetShaderName(void)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::GetShaderName() */

void AbyssEngine::ShaderBaseStruct::GetShaderName(void)

{
  String *in_r0;
  int in_r1;
  
  String::String(in_r0,(String *)(in_r1 + 0xc),false);
  return;
}

```

## target disasm
```
  000939d0: push {r7,lr}
  000939d2: mov r7,sp
  000939d4: adds r1,#0xc
  000939d6: movs r2,#0x0
  000939d8: blx 0x0006f028
  000939dc: pop {r7,pc}
```
