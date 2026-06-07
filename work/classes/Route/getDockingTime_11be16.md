# Route::getDockingTime
elf 0x11be16 body 8
Sig: undefined __stdcall getDockingTime(void)

## decompile
```c

/* Route::getDockingTime() */

void Route::getDockingTime(void)

{
  (*(code *)(DAT_001ac324 + 0x1ac328))();
  return;
}

```

## target disasm
```
  0012be16: ldr r1,[r0,#0x0]
  0012be18: b.w 0x001ac318
  001ac318: bx pc
```
