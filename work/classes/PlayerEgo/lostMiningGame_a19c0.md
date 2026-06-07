# PlayerEgo::lostMiningGame
elf 0xa19c0 body 6
Sig: undefined __stdcall lostMiningGame(void)

## decompile
```c

/* PlayerEgo::lostMiningGame() */

undefined1 PlayerEgo::lostMiningGame(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x39b);
}

```

## target disasm
```
  000b19c0: ldrb.w r0,[r0,#0x39b]
  000b19c4: bx lr
```
