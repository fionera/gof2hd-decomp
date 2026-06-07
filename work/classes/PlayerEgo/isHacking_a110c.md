# PlayerEgo::isHacking
elf 0xa110c body 12
Sig: undefined __stdcall isHacking(void)

## decompile
```c

/* PlayerEgo::isHacking() */

bool PlayerEgo::isHacking(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x1e8) != 0;
}

```

## target disasm
```
  000b110c: ldr.w r0,[r0,#0x1e8]
  000b1110: cmp r0,#0x0
  000b1112: it ne
  000b1114: mov.ne r0,#0x1
  000b1116: bx lr
```
