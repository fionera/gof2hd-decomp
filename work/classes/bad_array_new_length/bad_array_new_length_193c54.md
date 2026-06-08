# bad_array_new_length::bad_array_new_length
elf 0x193c54 body 12
Sig: undefined __thiscall bad_array_new_length(bad_array_new_length * this)

## decompile
```c

/* std::bad_array_new_length::bad_array_new_length() */

void __thiscall std::bad_array_new_length::bad_array_new_length(bad_array_new_length *this)

{
  *(int *)this = *(int *)(DAT_001a3c60 + 0x1a3c5a) + 8;
  return;
}

```

## target disasm
```
  001a3c54: ldr r1,[0x001a3c60]
  001a3c56: add r1,pc
  001a3c58: ldr r1,[r1,#0x0]
  001a3c5a: adds r1,#0x8
  001a3c5c: str r1,[r0,#0x0]
  001a3c5e: bx lr
```
