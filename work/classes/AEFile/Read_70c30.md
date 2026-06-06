# AEFile::Read
elf 0x70c30 body 10
Sig: undefined __stdcall Read(bool * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(bool&, unsigned int) */

void AEFile::Read(bool *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(1,param_1,param_2);
  return;
}

```
## target disasm
```
  00080c30: mov r2,r1
  00080c32: mov r1,r0
  00080c34: movs r0,#0x1
  00080c36: b.w 0x001ab0d8
```
