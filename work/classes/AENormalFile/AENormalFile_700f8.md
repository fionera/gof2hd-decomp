# AENormalFile::AENormalFile
elf 0x700f8 body 14
Sig: undefined __thiscall AENormalFile(AENormalFile * this, FileInterface * param_1)

## decompile
```c

/* AENormalFile::AENormalFile(FileInterface*) */

void __thiscall AENormalFile::AENormalFile(AENormalFile *this,FileInterface *param_1)

{
  *(int *)this = *(int *)(DAT_00080108 + 0x800fe) + 8;
  *(FileInterface **)(this + 4) = param_1;
  return;
}

```

## target disasm
```
  000800f8: ldr r2,[0x00080108]
  000800fa: add r2,pc
  000800fc: ldr r2,[r2,#0x0]
  000800fe: adds r2,#0x8
  00080100: strd r2,r1,[r0,#0x0]
  00080104: bx lr
```
