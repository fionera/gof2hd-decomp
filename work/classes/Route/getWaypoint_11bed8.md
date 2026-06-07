# Route::getWaypoint
elf 0x11bed8 body 6
Sig: undefined __stdcall getWaypoint(void)

## decompile
```c

/* Route::getWaypoint() */

void Route::getWaypoint(void)

{
  (*(code *)(DAT_001ac334 + 0x1ac338))();
  return;
}

```

## target disasm
```
  0012bed8: ldr r1,[r0,#0x0]
  0012beda: b.w 0x001ac328
```
