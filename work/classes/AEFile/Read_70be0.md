# AEFile::Read
elf 0x70be0 body 10
Sig: undefined __stdcall Read(char * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(char&, unsigned int) */

void AEFile::Read(char *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(1,param_1,param_2);
  return;
}

```
## target disasm
```
  00080be0: mov r2,r1
  00080be2: mov r1,r0
  00080be4: movs r0,#0x1
  00080be6: b.w 0x001ab0d8
```
