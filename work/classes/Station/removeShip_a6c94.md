# Station::removeShip
elf 0xa6c94 body 16
Sig: undefined __stdcall removeShip(Ship * param_1)

## decompile
```c

/* Station::removeShip(Ship*) */

void Station::removeShip(Ship *param_1)

{
  if (*(int *)(param_1 + 0x2c) == 0) {
    return;
  }
  (*(code *)(DAT_001abeb4 + 0x1abeb8))();
  return;
}

```

## target disasm
```
  000b6c94: ldr r2,[r0,#0x2c]
  000b6c96: cbz r2,0x000b6ca0
  000b6c98: mov r0,r1
  000b6c9a: mov r1,r2
  000b6c9c: b.w 0x001abea8
  000b6ca0: bx lr
  001abea8: bx pc
```
