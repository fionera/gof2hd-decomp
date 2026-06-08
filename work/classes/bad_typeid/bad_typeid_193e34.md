# bad_typeid::bad_typeid
elf 0x193e34 body 12
Sig: undefined __thiscall bad_typeid(bad_typeid * this)

## decompile
```c

/* std::bad_typeid::bad_typeid() */

void __thiscall std::bad_typeid::bad_typeid(bad_typeid *this)

{
  *(int *)this = *(int *)(DAT_001a3e40 + 0x1a3e3a) + 8;
  return;
}

```

## target disasm
```
  001a3e34: ldr r1,[0x001a3e40]
  001a3e36: add r1,pc
  001a3e38: ldr r1,[r1,#0x0]
  001a3e3a: adds r1,#0x8
  001a3e3c: str r1,[r0,#0x0]
  001a3e3e: bx lr
```
