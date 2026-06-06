# AEFile::OpenWrite
elf 0x70b38 body 8
Sig: undefined __stdcall OpenWrite(String * param_1, uint * param_2)

## decompile
```c

/* AEFile::OpenWrite(AbyssEngine::String&, unsigned int*) */

void AEFile::OpenWrite(String *param_1,uint *param_2)

{
  (*(code *)(DAT_001ab0c4 + 0x1ab0c8))(param_1,0,param_2);
  return;
}

```
## target disasm
```
  00080b38: mov r2,r1
  00080b3a: movs r1,#0x0
  00080b3c: b.w 0x001ab0b8
```
