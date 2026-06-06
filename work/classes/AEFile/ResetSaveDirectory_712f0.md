# AEFile::ResetSaveDirectory
elf 0x712f0 body 18
Sig: undefined __stdcall ResetSaveDirectory(void)

## decompile
```c

/* AEFile::ResetSaveDirectory() */

void AEFile::ResetSaveDirectory(void)

{
  if ((int *)**(int **)(DAT_00081304 + 0x812f6) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x000812fe. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(int *)**(int **)(DAT_00081304 + 0x812f6) + 0x54))();
    return;
  }
  return;
}

```
## target disasm
```
  000812f0: ldr r0,[0x00081304]
  000812f2: add r0,pc
  000812f4: ldr r0,[r0,#0x0]
  000812f6: ldr r0,[r0,#0x0]
  000812f8: cbz r0,0x00081300
  000812fa: ldr r1,[r0,#0x0]
  000812fc: ldr r1,[r1,#0x54]
  000812fe: bx r1
  00081300: bx lr
```
