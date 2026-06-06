# AEFile::SetAppRootDir
elf 0x71258 body 20
Sig: undefined __stdcall SetAppRootDir(void * param_1)

## decompile
```c

/* AEFile::SetAppRootDir(void*) */

void AEFile::SetAppRootDir(void *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)**(int **)(DAT_0008126c + 0x81260);
  if (piVar1 != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00081268. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*piVar1 + 0x34))(piVar1,param_1);
    return;
  }
  return;
}

```
## target disasm
```
  00081258: mov r1,r0
  0008125a: ldr r0,[0x0008126c]
  0008125c: add r0,pc
  0008125e: ldr r0,[r0,#0x0]
  00081260: ldr r0,[r0,#0x0]
  00081262: cbz r0,0x0008126a
  00081264: ldr r2,[r0,#0x0]
  00081266: ldr r2,[r2,#0x34]
  00081268: bx r2
  0008126a: bx lr
```
