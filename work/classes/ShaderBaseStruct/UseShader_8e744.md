# ShaderBaseStruct::UseShader
elf 0x8e744 body 6
Sig: undefined __stdcall UseShader(bool param_1)

## decompile
```c

/* AbyssEngine::ShaderBaseStruct::UseShader(bool) */

void AbyssEngine::ShaderBaseStruct::UseShader(bool param_1)

{
  (*(code *)(DAT_001ab7a4 + 0x1ab7a8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  0009e744: ldr r0,[r0,#0x4]
  0009e746: b.w 0x001ab798
```
