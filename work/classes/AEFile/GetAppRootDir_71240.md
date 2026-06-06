# AEFile::GetAppRootDir
elf 0x71240 body 20
Sig: undefined __stdcall GetAppRootDir(void)

## decompile
```c

/* AEFile::GetAppRootDir() */

undefined4 AEFile::GetAppRootDir(void)

{
  undefined4 uVar1;
  
  if ((int *)**(int **)(DAT_00081254 + 0x81246) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0008124e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(*(int *)**(int **)(DAT_00081254 + 0x81246) + 0x30))();
    return uVar1;
  }
  return 0;
}

```
## target disasm
```
  00081240: ldr r0,[0x00081254]
  00081242: add r0,pc
  00081244: ldr r0,[r0,#0x0]
  00081246: ldr r0,[r0,#0x0]
  00081248: cbz r0,0x00081250
  0008124a: ldr r1,[r0,#0x0]
  0008124c: ldr r1,[r1,#0x30]
  0008124e: bx r1
  00081250: movs r0,#0x0
  00081252: bx lr
```
