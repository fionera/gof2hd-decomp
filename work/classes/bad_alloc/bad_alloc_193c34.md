# bad_alloc::bad_alloc
elf 0x193c34 body 12
Sig: undefined __thiscall bad_alloc(bad_alloc * this)

## decompile
```c

/* std::bad_alloc::bad_alloc() */

void __thiscall std::bad_alloc::bad_alloc(bad_alloc *this)

{
  *(int *)this = *(int *)(DAT_001a3c40 + 0x1a3c3a) + 8;
  return;
}

```

## target disasm
```
  001a3c34: ldr r1,[0x001a3c40]
  001a3c36: add r1,pc
  001a3c38: ldr r1,[r1,#0x0]
  001a3c3a: adds r1,#0x8
  001a3c3c: str r1,[r0,#0x0]
  001a3c3e: bx lr
```
