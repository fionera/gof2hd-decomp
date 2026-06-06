# Route::getDockingTime

ELF vaddr 0x11be16, body 8 bytes
Signature: undefined __stdcall getDockingTime(void)

## Ghidra decompile
```c

/* Route::getDockingTime() */

void Route::getDockingTime(void)

{
  (*(code *)(DAT_001ac324 + 0x1ac328))();
  return;
}

```

## Target disassembly (must match)
```
  0012be16: ldr r1,[r0,#0x0]
  0012be18: b.w 0x001ac318
  001ac318: bx pc
```

Verify: tools/try.sh work/src/getDockingTime.cpp getDockingTime 11be16 32
