# PlayerEgo::isMining
elf 0x9add8 body 12
Sig: undefined __stdcall isMining(void)

## decompile
```c

/* PlayerEgo::isMining() */

bool PlayerEgo::isMining(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x1e4) != 0;
}

```

## target disasm
```
  000aadd8: ldr.w r0,[r0,#0x1e4]
  000aaddc: cmp r0,#0x0
  000aadde: it ne
  000aade0: mov.ne r0,#0x1
  000aade2: bx lr
```
