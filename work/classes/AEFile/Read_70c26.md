# AEFile::Read
elf 0x70c26 body 10
Sig: undefined __stdcall Read(longlong * param_1, uint param_2)

## decompile
```c

/* AEFile::Read(long long&, unsigned int) */

void AEFile::Read(longlong *param_1,uint param_2)

{
  (*(code *)(DAT_001ab0e4 + 0x1ab0e8))(8,param_1,param_2);
  return;
}

```
## target disasm
```
  00080c26: mov r2,r1
  00080c28: mov r1,r0
  00080c2a: movs r0,#0x8
  00080c2c: b.w 0x001ab0d8
```
