# AENormalFile::GetFileSize
elf 0x7018c body 14
Sig: undefined __thiscall GetFileSize(AENormalFile * this)

## decompile
```c

/* AENormalFile::GetFileSize() */

undefined4 __thiscall AENormalFile::GetFileSize(AENormalFile *this)

{
  undefined4 uVar1;
  
  if (*(int **)(this + 4) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00080194. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(**(int **)(this + 4) + 0x20))();
    return uVar1;
  }
  return 0;
}

```

## target disasm
```
  0008018c: ldr r0,[r0,#0x4]
  0008018e: cbz r0,0x00080196
  00080190: ldr r1,[r0,#0x0]
  00080192: ldr r1,[r1,#0x20]
  00080194: bx r1
  00080196: movs r0,#0x0
  00080198: bx lr
```
