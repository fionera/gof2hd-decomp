# Level::checkObjective
elf 0xc6250 body 12
Sig: undefined __stdcall checkObjective(int param_1)

## decompile
```c

/* Level::checkObjective(int) */

undefined4 Level::checkObjective(int param_1)

{
  undefined4 uVar1;
  
  if (*(int *)(param_1 + 0x28) == 0) {
    return 0;
  }
  uVar1 = (*(code *)(DAT_001ac014 + 0x1ac018))();
  return uVar1;
}

```
## target disasm
```
  000d6250: ldr r0,[r0,#0x28]
  000d6252: cbz r0,0x000d6258
  000d6254: b.w 0x001ac008
  000d6258: movs r0,#0x0
  000d625a: bx lr
```
