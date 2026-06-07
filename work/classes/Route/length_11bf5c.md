# Route::length
elf 0x11bf5c body 6
Sig: undefined __stdcall length(void)

## decompile
```c

/* Route::length() */

undefined4 Route::length(void)

{
  int in_r0;
  
  return **(undefined4 **)(in_r0 + 0xc);
}

```

## target disasm
```
  0012bf5c: ldr r0,[r0,#0xc]
  0012bf5e: ldr r0,[r0,#0x0]
  0012bf60: bx lr
```
