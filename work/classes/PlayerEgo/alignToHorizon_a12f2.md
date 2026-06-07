# PlayerEgo::alignToHorizon
elf 0xa12f2 body 8
Sig: undefined __stdcall alignToHorizon(void)

## decompile
```c

/* PlayerEgo::alignToHorizon() */

void PlayerEgo::alignToHorizon(void)

{
  int in_r0;
  
  *(undefined1 *)(in_r0 + 0x2f4) = 1;
  return;
}

```

## target disasm
```
  000b12f2: movs r1,#0x1
  000b12f4: strb.w r1,[r0,#0x2f4]
  000b12f8: bx lr
```
