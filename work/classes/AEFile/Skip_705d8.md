# AEFile::Skip
elf 0x705d8 body 46
Sig: undefined __stdcall Skip(uint param_1, uint param_2)

## decompile
```c

/* AEFile::Skip(unsigned int, unsigned int) */

undefined4 AEFile::Skip(uint param_1,uint param_2)

{
  int *piVar1;
  undefined4 uVar2;
  
  if (((**(int **)(DAT_00080608 + 0x805e0) != 0) &&
      (param_2 < *(uint *)**(int **)(DAT_0008060c + 0x805ea))) &&
     (piVar1 = *(int **)(((uint *)**(int **)(DAT_0008060c + 0x805ea))[1] + param_2 * 4),
     piVar1 != (int *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x00080600. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(*piVar1 + 0x10))(piVar1,param_1);
    return uVar2;
  }
  return 0;
}

```
## target disasm
```
  000805d8: mov r2,r0
  000805da: ldr r0,[0x00080608]
  000805dc: add r0,pc
  000805de: ldr r0,[r0,#0x0]
  000805e0: ldr r0,[r0,#0x0]
  000805e2: cbz r0,0x00080602
  000805e4: ldr r0,[0x0008060c]
  000805e6: add r0,pc
  000805e8: ldr r0,[r0,#0x0]
  000805ea: ldr r0,[r0,#0x0]
  000805ec: ldr r3,[r0,#0x0]
  000805ee: cmp r3,r1
  000805f0: bls 0x00080602
  000805f2: ldr r0,[r0,#0x4]
  000805f4: ldr.w r0,[r0,r1,lsl #0x2]
  000805f8: cbz r0,0x00080602
  000805fa: ldr r1,[r0,#0x0]
  000805fc: ldr r3,[r1,#0x10]
  000805fe: mov r1,r2
  00080600: bx r3
  00080602: movs r0,#0x0
  00080604: bx lr
```
