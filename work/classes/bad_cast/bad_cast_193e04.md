# bad_cast::bad_cast
elf 0x193e04 body 12
Sig: undefined __thiscall bad_cast(bad_cast * this)

## decompile
```c

/* std::bad_cast::bad_cast() */

void __thiscall std::bad_cast::bad_cast(bad_cast *this)

{
  *(int *)this = *(int *)(DAT_001a3e10 + 0x1a3e0a) + 8;
  return;
}

```

## target disasm
```
  001a3e04: ldr r1,[0x001a3e10]
  001a3e06: add r1,pc
  001a3e08: ldr r1,[r1,#0x0]
  001a3e0a: adds r1,#0x8
  001a3e0c: str r1,[r0,#0x0]
  001a3e0e: bx lr
```
