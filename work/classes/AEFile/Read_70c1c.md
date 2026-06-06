# AEFile::Read
elf 0x70c1c body 10
Sig: undefined __stdcall Read(signed * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(signed char&, unsigned int) */

void AEFile::Read(signed *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(1,param_1,param_2);
  return;
}

```
## target disasm
```
  00080c1c: mov r2,r1
  00080c1e: mov r1,r0
  00080c20: movs r0,#0x1
  00080c22: b.w 0x001ab0d8
```
