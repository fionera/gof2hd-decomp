# AEFile::SetZipDirectory
elf 0x71270 body 20
Sig: undefined __stdcall SetZipDirectory(void * param_1)

## decompile
```c

/* AEFile::SetZipDirectory(void*) */

void AEFile::SetZipDirectory(void *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)**(int **)(DAT_00081284 + 0x81278);
  if (piVar1 != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00081280. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*piVar1 + 0x38))(piVar1,param_1);
    return;
  }
  return;
}

```
## target disasm
```
  00081270: mov r1,r0
  00081272: ldr r0,[0x00081284]
  00081274: add r0,pc
  00081276: ldr r0,[r0,#0x0]
  00081278: ldr r0,[r0,#0x0]
  0008127a: cbz r0,0x00081282
  0008127c: ldr r2,[r0,#0x0]
  0008127e: ldr r2,[r2,#0x38]
  00081280: bx r2
  00081282: bx lr
```
