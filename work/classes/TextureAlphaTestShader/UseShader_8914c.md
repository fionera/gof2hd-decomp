# TextureAlphaTestShader::UseShader
elf 0x8914c body 26
Sig: undefined __stdcall UseShader(bool param_1)

## decompile
```c

/* AbyssEngine::TextureAlphaTestShader::UseShader(bool) */

void AbyssEngine::TextureAlphaTestShader::UseShader(bool param_1)

{
  int iVar1;
  
  if ((**(char **)(DAT_00099168 + 0x99152) == '\0') ||
     (iVar1 = *(int *)(param_1 + 0x20), iVar1 == 0)) {
    iVar1 = *(int *)(param_1 + 4);
  }
  (*(code *)(DAT_001ab7a4 + 0x1ab7a8))(iVar1);
  return;
}

```

## target disasm
```
  0009914c: ldr r1,[0x00099168]
  0009914e: add r1,pc
  00099150: ldr r1,[r1,#0x0]
  00099152: ldrb r1,[r1,#0x0]
  00099154: cbz r1,0x00099160
  00099156: ldr r1,[r0,#0x20]
  00099158: cbz r1,0x00099160
  0009915a: mov r0,r1
  0009915c: b.w 0x001ab798
  00099160: ldr r0,[r0,#0x4]
  00099162: b.w 0x001ab798
```
