# TextureVtxColorShader::UseShader
elf 0x8bda8 body 26
Sig: undefined __stdcall UseShader(bool param_1)

## decompile
```c

/* AbyssEngine::TextureVtxColorShader::UseShader(bool) */

void AbyssEngine::TextureVtxColorShader::UseShader(bool param_1)

{
  int iVar1;
  
  if ((**(char **)(DAT_0009bdc4 + 0x9bdae) == '\0') ||
     (iVar1 = *(int *)(param_1 + 0x20), iVar1 == 0)) {
    iVar1 = *(int *)(param_1 + 4);
  }
  (*(code *)(DAT_001ab7a4 + 0x1ab7a8))(iVar1);
  return;
}

```

## target disasm
```
  0009bda8: ldr r1,[0x0009bdc4]
  0009bdaa: add r1,pc
  0009bdac: ldr r1,[r1,#0x0]
  0009bdae: ldrb r1,[r1,#0x0]
  0009bdb0: cbz r1,0x0009bdbc
  0009bdb2: ldr r1,[r0,#0x20]
  0009bdb4: cbz r1,0x0009bdbc
  0009bdb6: mov r0,r1
  0009bdb8: b.w 0x001ab798
  0009bdbc: ldr r0,[r0,#0x4]
  0009bdbe: b.w 0x001ab798
```
