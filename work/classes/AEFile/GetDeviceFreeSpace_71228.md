# AEFile::GetDeviceFreeSpace
elf 0x71228 body 20
Sig: undefined __stdcall GetDeviceFreeSpace(void)

## decompile
```c

/* AEFile::GetDeviceFreeSpace() */

undefined4 AEFile::GetDeviceFreeSpace(void)

{
  undefined4 uVar1;
  
  if ((int *)**(int **)(DAT_0008123c + 0x8122e) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00081236. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(*(int *)**(int **)(DAT_0008123c + 0x8122e) + 0x2c))();
    return uVar1;
  }
  return 0;
}

```
## target disasm
```
  00081228: ldr r0,[0x0008123c]
  0008122a: add r0,pc
  0008122c: ldr r0,[r0,#0x0]
  0008122e: ldr r0,[r0,#0x0]
  00081230: cbz r0,0x00081238
  00081232: ldr r1,[r0,#0x0]
  00081234: ldr r1,[r1,#0x2c]
  00081236: bx r1
  00081238: movs r0,#0x0
  0008123a: bx lr
```
