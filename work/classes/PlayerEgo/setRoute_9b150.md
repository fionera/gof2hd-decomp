# PlayerEgo::setRoute
elf 0x9b150 body 6
Sig: undefined __stdcall setRoute(Route * param_1)

## decompile
```c

/* PlayerEgo::setRoute(Route*) */

void PlayerEgo::setRoute(Route *param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 0xfc) = in_r1;
  return;
}

```

## target disasm
```
  000ab150: str.w r1,[r0,#0xfc]
  000ab154: bx lr
```
