# AEFile::Read
elf 0x70bea body 10
Sig: undefined __stdcall Read(uint * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(unsigned int&, unsigned int) */

void AEFile::Read(uint *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(4,param_1,param_2);
  return;
}

```
## target disasm
```
  00080bea: mov r2,r1
  00080bec: mov r1,r0
  00080bee: movs r0,#0x4
  00080bf0: b.w 0x001ab0d8
```
