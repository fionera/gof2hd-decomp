# bad_array_length::bad_array_length
elf 0x193c74 body 12
Sig: undefined __thiscall bad_array_length(bad_array_length * this)

## decompile
```c

/* std::bad_array_length::bad_array_length() */

void __thiscall std::bad_array_length::bad_array_length(bad_array_length *this)

{
  *(int *)this = *(int *)(DAT_001a3c80 + 0x1a3c7a) + 8;
  return;
}

```

## target disasm
```
  001a3c74: ldr r1,[0x001a3c80]
  001a3c76: add r1,pc
  001a3c78: ldr r1,[r1,#0x0]
  001a3c7a: adds r1,#0x8
  001a3c7c: str r1,[r0,#0x0]
  001a3c7e: bx lr
```
