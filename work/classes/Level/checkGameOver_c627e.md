# Level::checkGameOver
elf 0xc627e body 14
Sig: undefined __stdcall checkGameOver(int param_1)

## decompile
```c

/* Level::checkGameOver(int) */

undefined4 Level::checkGameOver(int param_1)

{
  undefined4 uVar1;
  
  if (*(int *)(param_1 + 0x2c) == 0) {
    return 0;
  }
  uVar1 = (*(code *)(DAT_001ac014 + 0x1ac018))();
  return uVar1;
}

```
## target disasm
```
  000d627e: ldr r0,[r0,#0x2c]
  000d6280: cbz r0,0x000d6286
  000d6282: b.w 0x001ac008
  000d6286: movs r0,#0x0
  000d6288: bx lr
  001ac008: bx pc
```
