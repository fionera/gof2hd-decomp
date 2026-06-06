# AEFile::OpenAppend
elf 0x70b8c body 10
Sig: undefined __stdcall OpenAppend(String * param_1, uint * param_2)

## decompile
```c

/* AEFile::OpenAppend(AbyssEngine::String&, unsigned int*) */

void AEFile::OpenAppend(String *param_1,uint *param_2)

{
  (*(code *)(DAT_001ab0c4 + 0x1ab0c8))(param_1,2,param_2);
  return;
}

```
## target disasm
```
  00080b8c: mov r2,r1
  00080b8e: movs r1,#0x2
  00080b90: b.w 0x001ab0b8
  001ab0b8: bx pc
```
