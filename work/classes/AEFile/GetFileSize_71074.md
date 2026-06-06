# AEFile::GetFileSize
elf 0x71074 body 42
Sig: undefined __stdcall GetFileSize(uint param_1)

## decompile
```c

/* AEFile::GetFileSize(unsigned int) */

undefined4 AEFile::GetFileSize(uint param_1)

{
  int *piVar1;
  undefined4 uVar2;
  
  if (**(int **)(DAT_000810a0 + 0x8107a) != 0) {
    if ((param_1 < *(uint *)**(int **)(DAT_000810a4 + 0x81084)) &&
       (piVar1 = *(int **)(((uint *)**(int **)(DAT_000810a4 + 0x81084))[1] + param_1 * 4),
       piVar1 != (int *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x00081098. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar2 = (**(code **)(*piVar1 + 0x18))();
      return uVar2;
    }
  }
  return 0;
}

```
## target disasm
```
  00081074: ldr r1,[0x000810a0]
  00081076: add r1,pc
  00081078: ldr r1,[r1,#0x0]
  0008107a: ldr r1,[r1,#0x0]
  0008107c: cbz r1,0x0008109a
  0008107e: ldr r1,[0x000810a4]
  00081080: add r1,pc
  00081082: ldr r1,[r1,#0x0]
  00081084: ldr r1,[r1,#0x0]
  00081086: ldr r2,[r1,#0x0]
  00081088: cmp r2,r0
  0008108a: bls 0x0008109a
  0008108c: ldr r1,[r1,#0x4]
  0008108e: ldr.w r0,[r1,r0,lsl #0x2]
  00081092: cbz r0,0x0008109a
  00081094: ldr r1,[r0,#0x0]
  00081096: ldr r1,[r1,#0x18]
  00081098: bx r1
  0008109a: movs r0,#0x0
  0008109c: bx lr
```
