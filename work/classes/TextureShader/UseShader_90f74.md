# TextureShader::UseShader
elf 0x90f74 body 36
Sig: undefined __stdcall UseShader(bool param_1)

## decompile
```c

/* AbyssEngine::TextureShader::UseShader(bool) */

void AbyssEngine::TextureShader::UseShader(bool param_1)

{
  int iVar1;
  
  if ((**(char **)(DAT_000a0f90 + 0xa0f7a) == '\0') ||
     (iVar1 = *(int *)(param_1 + 0x20), iVar1 == 0)) {
    iVar1 = *(int *)(param_1 + 4);
  }
  (*(code *)(DAT_001ab7a4 + 0x1ab7a8))(iVar1);
  return;
}

```

## target disasm
```
  000a0f74: ldr r1,[0x000a0f90]
  000a0f76: add r1,pc
  000a0f78: ldr r1,[r1,#0x0]
  000a0f7a: ldrb r1,[r1,#0x0]
  000a0f7c: cbz r1,0x000a0f88
  000a0f7e: ldr r1,[r0,#0x20]
  000a0f80: cbz r1,0x000a0f88
  000a0f82: mov r0,r1
  000a0f84: b.w 0x001ab798
  000a0f88: ldr r0,[r0,#0x4]
  000a0f8a: b.w 0x001ab798
  001ab798: bx pc
  001ab79c: ldr r12,[0x1ab7a4]
  001ab7a0: add pc,r12,pc
```
