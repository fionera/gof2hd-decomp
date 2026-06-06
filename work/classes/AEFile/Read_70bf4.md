# AEFile::Read
elf 0x70bf4 body 10
Sig: undefined __stdcall Read(int * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(int&, unsigned int) */

void AEFile::Read(int *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(4,param_1,param_2);
  return;
}

```
## target disasm
```
  00080bf4: mov r2,r1
  00080bf6: mov r1,r0
  00080bf8: movs r0,#0x4
  00080bfa: b.w 0x001ab0d8
```
